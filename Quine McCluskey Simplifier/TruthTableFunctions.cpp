#include "TruthTableFunctions.h"

namespace tbl
{
	size_t getVariableCount(const std::vector<char>& expression)
	{
		size_t varCount = 0;

		for (char c : expression)
		{
			if (isalpha(c))
			{
				if (c - 64 > varCount)
					varCount = c - 64;
			}
		}

		return varCount;
	}

	std::set<size_t> getMinTerms(const TruthTable& table)
	{
		std::set<size_t> minTerms;

		TruthTable::const_iterator it;
		for (it = table.begin(); it != table.end(); ++it)
		{
			if (it->truthValue)
				minTerms.insert(it->variables.to_ullong());
		}

		return minTerms;
	}

	TruthTable buildTruthTable(std::vector<char>& rpnTokens)
	{
		TruthTable table;
		size_t varCount = getVariableCount(rpnTokens);
		std::bitset<MAX_VARS> variables = getVariables(rpnTokens);

		int rowCount = pow(2, varCount);
		for (int i = 0; i < rowCount; ++i)
		{
			std::bitset<MAX_VARS> variables(i), minTerm;

			// note to self, might still need minTerm for BoolExpression (might not too)
			for (int pos = 0; pos < varCount; pos++)
				minTerm[varCount - 1 - pos] = variables[pos];

			table.insert(table.begin(), { calculateExpression(rpnTokens, variables, varCount), variables });
		}

		return table;
	}

	std::bitset<MAX_VARS> getVariables(const std::vector<char>& expression)
	{
		std::bitset<MAX_VARS> vars;
		size_t varCount = getVariableCount(expression);

		assert(varCount <= MAX_VARS);

		for (char c : expression)
		{
			if (isalpha(c))
				vars[varCount - 1 - (int)(c - 65)] = 1;
		}

		return vars;
	}

	bool calculateExpression(std::vector<char> reversePolish, std::bitset<MAX_VARS>& variables, const size_t varCount)
	{
		std::stack<char> tempStack;
		bool resultant = 0;
		bool var1, var2;

		if (reversePolish.size() == 1)
			resultant = variables[varCount - 1 - (reversePolish.front() - 65)];
		else
		{
			while (!reversePolish.empty())
			{
				if (!isalpha(reversePolish.front()))
				{
					if (tempStack.empty())
						return false;
					if (isdigit(tempStack.top()))
						var1 = (tempStack.top() != '0');
					else
						var1 = variables[varCount - 1 - (tempStack.top() - 65)];
					tempStack.pop();

					if (reversePolish.front() != NOT_OP)
					{
						if (tempStack.empty())
							return false;
						if (isdigit(tempStack.top()))
							var2 = (tempStack.top() != '0');
						else
							var2 = variables[varCount - 1 - (tempStack.top() - 65)];
						tempStack.pop();
					}
				}

				switch (reversePolish.front())
				{
				case AND_OP:
					resultant = var1 & var2;
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				case OR_OP:
					resultant = var1 | var2;
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				case NOT_OP:
					resultant = !var1;
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				case NAND_OP:
					resultant = !(var1 & var2);
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				case NOR_OP:
					resultant = !(var1 | var2);
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				case XOR_OP:
					resultant = var1 ^ var2;
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				case GREATER_OP:
					if (var2)
						resultant = var1;
					else
						resultant = 1;
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				case LESS_OP:
					resultant = ((var1 & var2) | (!var1 & !var2));
					tempStack.push('0' + resultant);
					reversePolish.erase(reversePolish.begin());
					break;
				default:
					if (isalpha(reversePolish.front()))
					{
						tempStack.push(reversePolish.front());
						reversePolish.erase(reversePolish.begin());
					}
					break;
				}
			}

			if (tempStack.size() != 1)
				throw std::invalid_argument("Expression not valid");

			resultant = (tempStack.top() != '0');
		}

		return resultant;
	}

	int numDigits(int number)
	{
		int digits = 0;
		do 
		{
			++digits;
			number /= 10;
		} while (number);
		return digits;
	}

	void printLine(int varCount)
	{
		std::cout << std::string(numDigits(pow(2, varCount)), '-');
		for (int i = 0; i < varCount; ++i)
			std::cout << "|---";
		std::cout << "||---|";
	}

	void displayTable(const TruthTable& table, const std::string& expression, const int& varCount)
	{
		int width = numDigits(pow(2, varCount));
		std::cout << std::setw(width) << "#";

		for (int i = 0; i < varCount; ++i)
			std::cout << "| " << char(i + 65) << " ";
		std::cout << "||T/F|" << std::endl;

		printLine(varCount);
		std::cout << std::endl;

		TruthTable::const_reverse_iterator it;
		for (it = table.crbegin(); it != table.crend(); ++it)
		{
			std::cout << std::setw(width) << it->variables.to_ullong();

			for (int i = 0; i < varCount; ++i)
				std::cout << "| " << it->variables[varCount - 1 - i] << " ";
			std::cout << "|| " << it->truthValue << " |" << std::endl;

			printLine(varCount);
			std::cout << std::endl;
		}
	}

}