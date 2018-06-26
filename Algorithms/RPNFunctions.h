#pragma once
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <iomanip>
#include <iostream>
#include <bitset>
#include "QMFunctions.h"

using namespace std;

const char OR_OP = '+';
const char AND_OP = '*';
const char NOT_OP = '\'';
const char NAND_OP = '@';
const char XOR_OP = '^';
const char NOR_OP = '%';
const char LESS_OP = '<';
const char GREATER_OP = '>';
const char SPACE = ' ';
const char EQUAL_OP = '=';
const char LEFT_PAR = '(';
const char RIGHT_PAR = ')';

const std::string OPERATORS = "\'@*^%+><";//"" + NOT_OP + NAND_OP + AND_OP + XOR_OP + NOR_OP + OR_OP + GREATER_OP + LESS_OP; //"~@&^%|><";
const int PRECEDENCE[10] = { -4,-3,-3,-2,-2,-2,-1,-0 };

bool parseExpression(vector<string>& exp, const string& src);
bool parseRPNQueue(queue<string>& outputQueue, const vector<string>& exp);

bool calculateExpression(queue<string> reversePolish, bitset<27>& variables);
vector<vector<string>> buildTable(vector<string>& expression, queue<string> reversePolish);
bool displayTable(const queue<string>& rPolish, const vector<string>& exp);

bool parseExpression(vector<string>& exp, const string& src)
{
	int i = 0;
	int length = src.length();
	char lastIndex = SPACE;
	bool wasChar = false;
	while (i < length)
	{
		switch (src[i])
		{
		case LESS_OP:
			if (!(i + 2 < length - 1))
				return false;
			if (src[i + 1] != EQUAL_OP || src[i + 2] != GREATER_OP)
				return false;
			exp.push_back(src.substr(i, 1));
			lastIndex = src[i];
			i++;
			break;
		case EQUAL_OP:
			if (!(i + 1 < length - 1))
				return false;
			if (i - 1 < 0)
				return false;
			if (src[i + 1] != GREATER_OP)
				return false;
			i++;
			break;
		case GREATER_OP:
			if (i + 1 >= length)
				return false;
			if (i - 2 < 0)
				return false;
			if (src[i - 2] == LESS_OP && i - 2 < 1)
				return false;
			if (src[i - 2] != LESS_OP)
			{
				exp.push_back(src.substr(i, 1));
				lastIndex = src[i];
			}
			i++;
			break;
		case SPACE:
			i++;
			break;
		default:
			if (isalpha(src[i]) && isalpha(lastIndex))
				return false;
			if (src[i] == RIGHT_PAR && lastIndex == LEFT_PAR)
				return false;
			if (src[i] == LEFT_PAR && lastIndex == RIGHT_PAR)
				return false;
			//if (src[i] != LEFT_PAR && src[i] != RIGHT_PAR)
			//	if (lastIndex != LEFT_PAR && lastIndex != RIGHT_PAR)
			//		if (src[i] != NOT_OP && lastIndex != NOT_OP)
			//			if (!isalpha(src[i]) && !isalpha(lastIndex))
			//				return false;
			//if (lastIndex == NOT_OP && (!isalpha(src[i]) && src[i] != LEFT_PAR) && src[i] != NOT_OP)
			//	return false;
			if ((!isalpha(src[i]) && src[i] != RIGHT_PAR && src[i] != NOT_OP) && i == length - 1)
				return false;
			if (!wasChar && isalpha(src[i]))
				wasChar = true;
			exp.push_back(src.substr(i, 1));
			lastIndex = src[i];
			i++;
			break;
		}

	}

	return wasChar;
}

bool parseRPNQueue(queue<string>& outputQueue, const vector<string>& exp)
{
	stack<string> tempStack;
	for (string s : exp)
	{
		switch (s[0])
		{
		case LEFT_PAR:
			tempStack.push(s);
			break;
		case RIGHT_PAR:
			while (!tempStack.empty() && tempStack.top()[0] != LEFT_PAR)
			{
				outputQueue.push(tempStack.top());
				tempStack.pop();
			}

			if (tempStack.empty())
				return false;

			tempStack.pop();
			break;
		case AND_OP:
		case OR_OP:
		case NOT_OP:
		case NAND_OP:
		case NOR_OP:
		case XOR_OP:
		case GREATER_OP:
		case LESS_OP:

			while (!tempStack.empty() && OPERATORS.find(tempStack.top()) != string::npos)
			{
				//if op1 is left-associative and it's precedence is <= o2 precedence
				if (PRECEDENCE[OPERATORS.find(s)] < 0 &&
					(abs(PRECEDENCE[OPERATORS.find(s)]) <= abs(PRECEDENCE[OPERATORS.find(tempStack.top())])))
				{
					outputQueue.push(tempStack.top());
					tempStack.pop();
				}
				else if (PRECEDENCE[OPERATORS.find(s)] > 0 &&
					(abs(PRECEDENCE[OPERATORS.find(s)]) < abs(PRECEDENCE[OPERATORS.find(tempStack.top())])))
				{
					outputQueue.push(tempStack.top());
					tempStack.pop();
				}
				else
					break;
			}
			tempStack.push(s);
			break;
		default:
			if (isalpha(s[0]))
				outputQueue.push(s);
			else
				return false;
			break;
		}
	}

	while (!tempStack.empty())
	{
		if (tempStack.top()[0] == LEFT_PAR || tempStack.top()[0] == RIGHT_PAR)
			return false;
		outputQueue.push(tempStack.top());
		tempStack.pop();

	}
	return true;
}

vector<size_t> calculateMinTerms(const queue<string>& rpn, const vector<string>& exp)
{
	int varCount = 0;

	for (string s : exp)
	{
		if (isalpha(s[0]))
		{
			if (s[0] - 64 > varCount)
				varCount = s[0] - 64;

			//variables[(int)(s[0] - 65)] = 1;
		}
	}

	vector<size_t> minTerms;
	int rowCount = pow(2, varCount);


	for (int i = rowCount - 1; i >= 0; i--)
	{
		bitset<27/*QM::MAX_TERMS + 1*/> variables(i), minTerm;
		
		for (int pos = 0; pos < varCount; pos++)
		{
			minTerm[varCount - 1 - pos] = variables[pos];
		}
		//cout << "Vars: " << variables << endl;
		calculateExpression(rpn, variables);

		if (variables[26])
			minTerms.push_back(minTerm.to_ullong());
	}

	return minTerms;
}

bool displayTable(const queue<string>& rPolish, const vector<string>& exp)
{
	queue<string> reversePolish = rPolish;
	vector<string> expression = exp;
	vector<vector<string>> truthTable;
	int lastLineLength = 0;

	truthTable = buildTable(expression, reversePolish);

	cout << endl;
	for (vector<string>::size_type i = 0; i<truthTable.size(); i++)
	{
		cout << '|';
		for (string::size_type j = 0; j<truthTable[i].size(); j++)
		{
			if (i == 0 && j != truthTable[i].size() - 1)
				lastLineLength += 3;
			if (j == truthTable[i].size() - 1)
				cout << setw(truthTable[0].back().length());
			else
				cout << setw(2);
			cout << truthTable[i][j] << '|';
		}
		cout << endl;
	}
	lastLineLength += truthTable[0].back().length() + 2;
	for (int i = 0; i<lastLineLength; i++)
		cout << "-";
	cout << endl;
	return true;
}

vector<vector<string>> buildTable(vector<string>& expression, queue<string> reversePolish)
{
	int rowCount;
	int numOfTrue;
	string alphaCol;
	bool TF;
	string exp = "";
	vector<string> row;
	vector<vector<string>> table;
	queue<string> row1;
	bitset<27> alpha;
	vector<bitset<27>> tableVariables;

	//Find All Variables In Expression
	//Set Variable Values In Bitset To 1
	for (string s : expression)
	{
		if (s[0] == LESS_OP)
			exp += s + EQUAL_OP + GREATER_OP;
		else if (s[0] == GREATER_OP)
			exp += EQUAL_OP + s;
		else
			exp += s;

		if (isalpha(s[0]))
		{
			if (alpha[(int)(s[0] - 65)] == 0)
			{
				row1.push(s);
				alpha[(int)(s[0] - 65)] = 1;
			}
		}
	}
	row1.push(exp);


	//Allocates Size For Vector Strings and Vector Bitsets
	rowCount = pow(2, alpha.count());
	rowCount++;

	for (int i = 0; i<rowCount; i++)
	{
		table.push_back(row);
		if (i>0)
			tableVariables.push_back(alpha);
	}

	//Create Every Combination Of True False Values Of All Variables
	//Loads Combinations into 2D String Array and Vector of bitsets
	for (vector<bitset<27>>::size_type i = 0; i<alpha.count(); i++)
	{
		alphaCol = row1.front();
		row1.pop();
		table[0].push_back(alphaCol);
		numOfTrue = (rowCount - 1) / (pow(2, i + 1));
		TF = true;
		for (int j = 1; j<rowCount; j++)
		{
			tableVariables[j - 1][alphaCol[0] - 65] = TF;
			if (TF)
				table[j].push_back("T");
			else
				table[j].push_back("F");
			if (j%numOfTrue == 0)
				TF = !TF;
		}
	}

	//Add Expression To End Of Row 1
	table[0].push_back(row1.front());

	for (int j = 0; j<rowCount - 1; j++)
	{
		calculateExpression(reversePolish, tableVariables[j]);
		
		if (tableVariables[j][26])
			table[j + 1].push_back("T");
		else
			table[j + 1].push_back("F");
	}
	return table;
}

bool calculateExpression(queue<string> reversePolish, bitset<27>& variables)
{
	stack<string> tempStack;
	bool v1, v2;
	if (reversePolish.size() == 1)
		variables[26] = variables[reversePolish.front()[0] - 65];
	else
	{
		while (!reversePolish.empty())
		{
			if (!isalpha(reversePolish.front()[0]))
			{
				if (tempStack.empty())
					return false;
				if (isdigit(tempStack.top()[0]))
					v1 = (tempStack.top()[0] != '0');
				else
					v1 = variables[tempStack.top()[0] - 65];
				tempStack.pop();

				if (reversePolish.front()[0] != NOT_OP)
				{
					if (tempStack.empty())
						return false;
					if (isdigit(tempStack.top()[0]))
						v2 = (tempStack.top()[0] != '0');
					else
						v2 = variables[tempStack.top()[0] - 65];
					tempStack.pop();
				}
			}

			switch (reversePolish.front()[0])
			{
			case AND_OP:
				variables[26] = v1 & v2;
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			case OR_OP:
				variables[26] = v1 | v2;
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			case NOT_OP:
				variables[26] = !v1;
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			case NAND_OP:
				variables[26] = v1 & v2;
				variables[26] = !variables[26];
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			case NOR_OP:
				variables[26] = v1 | v2;
				variables[26] = !variables[26];
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			case XOR_OP:
				variables[26] = v1 ^ v2;
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			case GREATER_OP:
				if (v2)
					variables[26] = v1;
				else
					variables[26] = 1;
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			case LESS_OP:
				variables[26] = ((v1 & v2) | (!v1 & !v2));
				tempStack.push(to_string(variables[26]));
				reversePolish.pop();
				break;
			default:
				if (isalpha(reversePolish.front()[0]))
				{
					tempStack.push(reversePolish.front());
					reversePolish.pop();
				}
				break;

			}
		}

		if (tempStack.size() > 1)
			return false;
		
		variables[26] = (tempStack.top()[0] != '0');
	}

	return true;
}