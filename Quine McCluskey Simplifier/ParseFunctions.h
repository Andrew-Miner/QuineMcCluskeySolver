#pragma once
#include "Graph.h"
#include <vector>
#include <bitset>
#include "Token.h"
#include "Operator.h"
#include "Variable.h"
#include "Parenthesis.h"
#include "Integer.h"
#include <stack>

namespace eprs // Expression Parsing Name Space
{
	enum vertices
	{
		variables,
		operators,
		notOp,
		leftPar,
		rightPar
	};

	struct expLabel // Expression Vertex Label
	{
		bool startVertex, successVertex;
		std::set<char> labels;
	};

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
	const int PRECEDENCE[10] = { -4,-3,-3,-2,-2,-2,-1,-0 }; // Index cooresponds to index of operator in OPERATORS string

	Graph<expLabel> buildParseGraph();
	std::vector<char> tokenizeString(const std::string & str);

	template <typename T>
	std::vector<Token*> parseRPN(const std::vector<Token*>& expression);

	Token* buildOperatorToken(const std::string& op);
	std::vector<Token*> tokenizeExpression(const Graph<expLabel>& g, const std::vector<char>& expression);
	bool recursiveTokenize(const Graph<expLabel>& g, std::vector<Token*>& tokens, const size_t& curVertex, std::vector<char>::const_iterator start, std::vector<char>::const_iterator& end);
}


template <typename T>
std::vector<Token*> eprs::parseRPN(const std::vector<Token*>& expression)
{
	std::stack<Token*> opStack;
	std::vector<Token*> rpnTokens;
	std::vector<Token*>::const_iterator it;
	for (it = expression.cbegin(); it != expression.cend(); ++it)
	{
		Operator<T>* op;
		switch ((*it)->getType())
		{
		case Integer::INTEGER_TYPE:
		case Variable::VARIABLE_TYPE:
			rpnTokens.push_back(*it);
			break;
		case Parenthesis::PARENTHESIS_TYPE:
			if (reinterpret_cast<Parenthesis*>(*it)->isRight())
			{
				while (!opStack.empty() && (opStack.top()->getType() != Parenthesis::PARENTHESIS_TYPE || reinterpret_cast<Parenthesis*>(opStack.top())->isRight()))
				{
					rpnTokens.push_back(opStack.top());
					opStack.pop();

					if (opStack.empty())
						throw std::invalid_argument("mistmatched parenthesis");
				}
				opStack.pop();
			}
			else
				opStack.push(*it);
			break;
		case Operator<T>::OPERATOR_TYPE:
		case BinaryOperator<T>::BINARY_TYPE:
		case UnaryOperator<T>::UNARY_TYPE:
			op = reinterpret_cast<Operator<T>*>(*it);
			while (!opStack.empty() && opStack.top()->getType() != Parenthesis::PARENTHESIS_TYPE &&
				(op->getPrecedence() < reinterpret_cast<Operator<T>*>(opStack.top())->getPrecedence() ||
				(op->isLeftAssociative() && op->getPrecedence() == reinterpret_cast<Operator<T>*>(opStack.top())->getPrecedence())))
			{
				rpnTokens.push_back(opStack.top());
				opStack.pop();
			}
			opStack.push(op);
			break;
		default:
			throw std::invalid_argument("unknown token type");
		}
	}

	while (!opStack.empty())
	{
		if (opStack.top()->getType() == Parenthesis::PARENTHESIS_TYPE)
			throw std::invalid_argument("mistmatched parenthesis");
		rpnTokens.push_back(opStack.top());
		opStack.pop();
	}

	return rpnTokens;
}
