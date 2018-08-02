#include "ParseFunctions.h"
#include <algorithm>
#include <string>
#include <queue>
#include <stack>
#include <iostream>
#include <functional>
#include "OperationFunctors.h"

namespace eprs // Expression Parsing Name Space
{
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

	Token* buildOperatorToken(const std::string& op)
	{
		if (op.compare(std::string(1, OR_OP)) == 0)
			return new Operator<bool>(op, std::bit_or<bool>(), std::abs(PRECEDENCE[OPERATORS.find(OR_OP)]), PRECEDENCE[OPERATORS.find(OR_OP)] < 0);
		else if (op.compare(std::string(1, XOR_OP)) == 0)
			return new Operator<bool>(op, std::bit_xor<bool>(), std::abs(PRECEDENCE[OPERATORS.find(XOR_OP)]), PRECEDENCE[OPERATORS.find(XOR_OP)] < 0);
		else if (op.compare(std::string(1, NOR_OP)) == 0)
			return new Operator<bool>(op, NorOperation<bool>(), std::abs(PRECEDENCE[OPERATORS.find(NOR_OP)]), PRECEDENCE[OPERATORS.find(NOR_OP)] < 0);
		else if (op.compare(std::string(1, AND_OP)) == 0)
			return new Operator<bool>(op, std::logical_and<bool>(), std::abs(PRECEDENCE[OPERATORS.find(AND_OP)]), PRECEDENCE[OPERATORS.find(AND_OP)] < 0);
		else if (op.compare(std::string(1, NAND_OP)) == 0)
			return new Operator<bool>(op, NandOperation<bool>(), std::abs(PRECEDENCE[OPERATORS.find(NAND_OP)]), PRECEDENCE[OPERATORS.find(NAND_OP)] < 0);
		else if (op.compare(std::string(1, NOT_OP)) == 0)
			return reinterpret_cast<Token*>(new UnaryOperator<bool>(op, std::logical_not<bool>(), std::abs(PRECEDENCE[OPERATORS.find(NOT_OP)]), PRECEDENCE[OPERATORS.find(NOT_OP)] < 0));
	}

	// Returns:
	//		- bool: true/false if parsing has finished on a success/fail vertex 
	// Throws:
	//		- invalid_argument: all vertices have been traversed without success 
	bool recursiveTokenize(const Graph<expLabel>& g, std::vector<Token*>& tokens, const size_t& curVertex, std::vector<char>::const_iterator start, std::vector<char>::const_iterator& end)
	{
		if (g[curVertex].labels.find(*start) == g[curVertex].labels.end())
			return false;

		switch (curVertex)
		{
		case variables:
			tokens.push_back(new Variable(std::string(1, *start)));
			break;
		case operators:
			tokens.push_back(buildOperatorToken(std::string(1, *start)));
			break;
		case notOp:
			tokens.push_back(new UnaryOperator<bool>(std::string(1, *start), std::logical_not<bool>(), std::abs(PRECEDENCE[OPERATORS.find(NOT_OP)]), PRECEDENCE[OPERATORS.find(NOT_OP)] < 0));
			break;
		case leftPar:
			tokens.push_back(new Parenthesis(std::string(1, *start), false));
			break;
		case rightPar:
			tokens.push_back(new Parenthesis(std::string(1, *start), true));
			break;
		default:
			tokens.push_back(new UnknownToken(std::string(1, *start)));
			break;
		}

		++start;
		if (start == end)
			return g[curVertex].successVertex;

		size_t nextVertex;
		std::set<size_t> connections = g.neighbors(curVertex);

		std::set<size_t>::iterator it;
		for (it = connections.begin(); it != connections.end(); ++it)
		{
			if (g[*it].labels.find(*start) != g[*it].labels.end())
				return recursiveTokenize(g, tokens, *it, start, end);
		}

		return false;
	}

	std::vector<Token*> tokenizeExpression(const Graph<expLabel>& g, const std::vector<char>& expression)
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
			throw std::invalid_argument("Expression not valid");

		std::vector<Token*> tokens;

		bool success = false;
		try
		{
			success = recursiveTokenize(g, tokens, startVertex, curChar, expression.end());
		}
		catch (const std::invalid_argument& e)
		{
			throw(std::invalid_argument("Expression invalid: " + std::string(e.what())));
		}

		if (!success)
			throw std::invalid_argument("Expression not valid");

		return tokens;
	}

	Graph<expLabel> buildParseGraph()
	{
		Graph<expLabel> parseGraph = Graph<expLabel>();

		std::set<char> letters;
		for (int i = 0; i < 26; ++i)
		{
			letters.insert('A' + i);
			letters.insert('a' + i);
		}

		// Adding Vertices
		parseGraph.addVertex({ true, true, letters });										// Vertex 0: Variables
		parseGraph.addVertex({ false, false,{ OR_OP, AND_OP, NAND_OP, XOR_OP, NOR_OP } });	// Vertex 1: Operators

																							// Vertex 2: Not Operator
		//parseGraph.addVertex({ true, false, {NOT_OP} });									// Right-Associative
		parseGraph.addVertex({ false, true,{ NOT_OP } });									// Left-Associative

		parseGraph.addVertex({ true, false,{ LEFT_PAR } });									// Vertex 3: Left Parentheses
		parseGraph.addVertex({ false, true,{ RIGHT_PAR } });								// Vertex 4: Right Parentheses

																							// Adding Edges
		parseGraph.addEdge(variables, rightPar);
		parseGraph.addEdge(leftPar, variables);

		parseGraph.addEdge(operators, variables);
		parseGraph.addEdge(variables, operators);

		parseGraph.addEdge(notOp, notOp);
		parseGraph.addEdge(operators, leftPar);
		parseGraph.addEdge(rightPar, operators);

		// Right-Associative Unary Not Operator
		//parseGraph.addEdge(notOp, leftPar);
		//parseGraph.addEdge(leftPar, notOp);
		//parseGraph.addEdge(notOp, variables);
		//parseGraph.addEdge(operators, notOp);

		// Left-Associateive Unary Not Operator
		parseGraph.addEdge(notOp, rightPar);
		parseGraph.addEdge(rightPar, notOp);
		parseGraph.addEdge(variables, notOp);
		parseGraph.addEdge(notOp, operators);

		return parseGraph;
	}
}