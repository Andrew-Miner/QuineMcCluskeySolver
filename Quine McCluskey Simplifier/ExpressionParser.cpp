#include "ExpressionParser.h"



ExpressionParser::ExpressionParser()
{
	initGraph();
	valid = false;
}

ExpressionParser::ExpressionParser(const std::string & expression)
{
	initGraph();
	setExpression(expression);
}

bool ExpressionParser::setExpression(const std::string & expression)
{
	rpnTokens.clear();
	this->expression = expression;
	tokens = tokenizeString(this->expression);

	if (validateExpression(parseGraph, tokens))
		valid = parseRPN(tokens, rpnTokens);
	else
		valid = false;

	return valid;
}

void ExpressionParser::initGraph()
{
	parseGraph = Graph<expLabel>();

	std::set<char> letters;
	for (int i = 0; i < 26; ++i)
	{
		letters.insert('A' + i);
		letters.insert('a' + i);
	}

	// Adding Vertices
	parseGraph.addVertex({ true, true, letters });										// Vertex 0: Variables
	parseGraph.addVertex({ false, false, {OR_OP, AND_OP, NAND_OP, XOR_OP, NOR_OP} });	// Vertex 1: Operators
	parseGraph.addVertex({ true, false, {NOT_OP} });									// Vertex 2: Not Operator
	parseGraph.addVertex({ true, false, {LEFT_PAR} });									// Vertex 3: Left Parentheses
	parseGraph.addVertex({ false, true, {RIGHT_PAR} });									// Vertex 4: Right Parentheses

	// Adding Edges
	parseGraph.addEdge(variables, operators);
	parseGraph.addEdge(variables, rightPar);

	parseGraph.addEdge(operators, variables);
	parseGraph.addEdge(operators, leftPar);
	parseGraph.addEdge(operators, notOp);

	parseGraph.addEdge(leftPar, variables);
	parseGraph.addEdge(leftPar, notOp);

	parseGraph.addEdge(notOp, leftPar);
	parseGraph.addEdge(notOp, variables);
	parseGraph.addEdge(notOp, notOp);

	parseGraph.addEdge(rightPar, operators);
}
