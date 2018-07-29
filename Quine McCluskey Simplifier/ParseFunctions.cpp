#include "ParseFunctions.h"
#include <algorithm>
#include <string>
#include <queue>
#include <stack>
#include <iostream>

std::vector<char> tokenizeString(const std::string & str)
{
	std::string tmp = str;
	tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());

	std::vector<char> tokens;
	std::string::iterator it;
	for (it = tmp.begin(); it != tmp.end(); ++it)
		tokens.push_back(*it);
	return tokens;
}

bool validateExpression(const Graph<expLabel>& g, const std::vector<char>& expression)
{
	assert(!expression.empty());

	std::vector<char>::const_iterator curChar = expression.cbegin();

	bool found = false;
	size_t startVertex;
	for (startVertex = 0; startVertex < g.size(); ++startVertex)
	{
		if (g[startVertex].labels.find(*curChar) != g[startVertex].labels.end())
		{
			found = true;
			break;
		}
	}

	if (!found || !g[startVertex].startVertex)
		return false;

	return recursiveValidate(g, startVertex, curChar, expression.end());
}

bool recursiveValidate(const Graph<expLabel>& g, const size_t& curVertex, std::vector<char>::const_iterator& start, std::vector<char>::const_iterator& end)
{
	if (g[curVertex].labels.find(*start) == g[curVertex].labels.end())
		return false;

	++start;
	if (start == end)
	{
		if (g[curVertex].successVertex)
			return true;
	}
	else
	{
		size_t nextVertex;
		std::set<size_t> connections = g.neighbors(curVertex);

		std::set<size_t>::iterator it;
		for (it = connections.begin(); it != connections.end(); ++it)
		{
			if (recursiveValidate(g, *it, start, end))
				return true;
		}
	}

	return false;
}

bool parseRPN(const std::vector<char>& exp, std::vector<char>& rpnTokens)
{
	std::stack<char> tempStack;

	for (char c : exp)
	{
		switch (c)
		{
		case LEFT_PAR:
			tempStack.push(c);
			break;
		case RIGHT_PAR:
			while (!tempStack.empty() && tempStack.top() != LEFT_PAR)
			{
				rpnTokens.push_back(tempStack.top());
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

			while (!tempStack.empty() && OPERATORS.find(tempStack.top()) != std::string::npos)
			{
				//if op1 is left-associative and it's precedence is <= o2 precedence
				if (PRECEDENCE[OPERATORS.find(c)] < 0 &&
					(abs(PRECEDENCE[OPERATORS.find(c)]) <= abs(PRECEDENCE[OPERATORS.find(tempStack.top())])))
				{
					rpnTokens.push_back(tempStack.top());
					tempStack.pop();
				}
				else if (PRECEDENCE[OPERATORS.find(c)] > 0 &&
					(abs(PRECEDENCE[OPERATORS.find(c)]) < abs(PRECEDENCE[OPERATORS.find(tempStack.top())])))
				{
					rpnTokens.push_back(tempStack.top());
					tempStack.pop();
				}
				else
					break;
			}
			tempStack.push(c);
			break;
		default:
			if (isalpha(c))
				rpnTokens.push_back(c);
			else
				return false;
			break;
		}
	}

	while (!tempStack.empty())
	{
		if (tempStack.top() == LEFT_PAR || tempStack.top() == RIGHT_PAR)
			return false;
		rpnTokens.push_back(tempStack.top());
		tempStack.pop();

	}
	return true;
}
