#pragma once
#include "Graph.h"
#include "ParseFunctions.h"
#include "TruthTableFunctions.h"


class ExpressionParser
{
public:
	enum vertices
	{
		variables,
		operators,
		notOp,
		leftPar,
		rightPar
	};

	ExpressionParser();
	ExpressionParser(const std::string& expression);

	bool setExpression(const std::string& expression);
	std::string getExpression() const { return expression; }
	
	std::vector<char> getRPNTokens() { return valid ? rpnTokens : std::vector<char>(); }
	std::vector<char> getTokens() { return valid ? tokens : std::vector<char>(); }
	size_t getVariableCount() { return tbl::getVariableCount(tokens); }

	bool isValid() const { return valid; }

private:
	bool valid;
	std::string expression;
	std::vector<char> rpnTokens, tokens;

	Graph<expLabel> parseGraph;

	void initGraph();
};

