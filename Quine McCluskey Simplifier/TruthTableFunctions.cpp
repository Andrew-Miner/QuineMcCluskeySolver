#include "TruthTableFunctions.h"
#include "Operator.h"
#include "Variable.h"
#include "Parenthesis.h"
#include "Integer.h"

namespace tbl
{
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

	size_t getVariableCount(const std::vector<Token*>& expression)
	{
		size_t varCount = 0;

		std::vector<Token*>::const_iterator it;
		for (it = expression.cbegin(); it != expression.cend(); ++it)
		{
			if ((*it)->getType() == Variable::VARIABLE_TYPE)
				if (reinterpret_cast<Variable*>(*it)->getVar() - 64 > varCount)
					varCount = reinterpret_cast<Variable*>(*it)->getVar() - 64;
		}

		return varCount;
	}

	std::bitset<MAX_VARS> getVariables(const std::vector<Token*>& expression)
	{
		std::bitset<MAX_VARS> vars;
		size_t varCount = getVariableCount(expression);

		assert(varCount <= MAX_VARS);

		std::vector<Token*>::const_iterator it;
		for (it = expression.cbegin(); it != expression.cend(); ++it)
		{
			if ((*it)->getType() == Variable::VARIABLE_TYPE)
				vars[varCount - 1 - (int)(reinterpret_cast<Variable*>(*it)->getVar() - 65)] = 1;
		}

		return vars;
	}

	TruthTable buildTruthTable(std::vector<Token*>& rpnTokens)
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

	bool calculateExpression(std::vector<Token*> rpnTokens, std::bitset<52/*MAX_VARS*/>& variables, const size_t varCount)
	{
		if (rpnTokens.size() == 1)
		{
			if (rpnTokens.front()->getType() != Variable::VARIABLE_TYPE)
				throw std::invalid_argument("Expression not valid");

			return variables[varCount - 1 - (reinterpret_cast<Variable*>(rpnTokens.front())->getVar() - 65)];
		}

		std::stack<Token*> varStack;
		std::vector<Token*>::iterator it;
		for (it = rpnTokens.begin(); it != rpnTokens.end(); ++it)
		{
			// Change to instanceof for more general function
			if ((*it)->getType() == Operator<bool>::OPERATOR_TYPE || (*it)->getType() == UnaryOperator<bool>::UNARY_TYPE || (*it)->getType() == BinaryOperator<bool>::BINARY_TYPE)
			{
				Operator<bool>* op = reinterpret_cast<Operator<bool>*>(*it);

				if (op->operandCount() > varStack.size())
					throw std::invalid_argument("Expression not valid");

				std::vector<bool> operands;
				for (int i = 0; i < op->operandCount(); ++i)
				{
					switch (varStack.top()->getType())
					{
					case Variable::VARIABLE_TYPE:
						operands.push_back(variables[varCount - 1 - (reinterpret_cast<Variable*>(varStack.top())->getVar() - 65)]);
						break;
					case Integer::INTEGER_TYPE:
						operands.push_back(reinterpret_cast<Integer*>(varStack.top())->getValue());
						delete varStack.top();
						break;
					default:
						throw std::invalid_argument("Expression not valid");
					}
					varStack.pop();
				}

				if(op->getType() == UnaryOperator<bool>::UNARY_TYPE)
					varStack.push(new Integer((*reinterpret_cast<UnaryOperator<bool>*>(op))(operands)));
				else
					varStack.push(new Integer((*op)(operands)));
			}
			else
				varStack.push(*it);
		}

		if (varStack.size() != 1)
			throw std::invalid_argument("Expression not valid");

		int result = reinterpret_cast<Integer*>(varStack.top())->getValue();
		delete varStack.top();
		return result;
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

	void printLine(int varCount, int expSize)
	{
		std::cout << std::string(numDigits(pow(2, varCount)), '-');
		for (int i = 0; i < varCount; ++i)
			std::cout << "|---";
		std::cout << "||" << std::string(expSize, '-') << "|";
	}

	void displayTable(const TruthTable& table, const std::string& expression, const int& varCount)
	{
		int width = numDigits(pow(2, varCount));
		std::cout << std::setw(width) << "#";

		for (int i = 0; i < varCount; ++i)
			std::cout << "| " << char(i + 65) << " ";
		std::cout << "||" << expression << "|" << std::endl;

		printLine(varCount, expression.size());
		std::cout << std::endl;

		TruthTable::const_reverse_iterator it;
		for (it = table.crbegin(); it != table.crend(); ++it)
		{
			std::cout << std::setw(width) << it->variables.to_ullong();

			for (int i = 0; i < varCount; ++i)
				std::cout << "| " << it->variables[varCount - 1 - i] << " ";
			std::cout << "|| " << std::setw(expression.size() - 1) << it->truthValue << "|" << std::endl;

			printLine(varCount, expression.size());
			std::cout << std::endl;
		}
	}

}