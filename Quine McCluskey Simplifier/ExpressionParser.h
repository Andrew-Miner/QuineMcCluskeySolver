#pragma once
#include "Graph.h"
#include "ParseFunctions.h"
#include "TruthTableFunctions.h"

class ExpressionParser
{
public:
	ExpressionParser();
	ExpressionParser(const std::string& expression);

	bool setExpression(const std::string& expression);
	std::string getExpression() const { return expression; }
	
	std::vector<char> getPostfixString() { return valid ? rpnTokens : std::vector<char>(); }
	std::vector<Token*> getPostfixTokens() { return valid ? postfix : std::vector<Token*>(); }

	std::vector<char> getTokens() { return valid ? tokens : std::vector<char>(); }
	std::vector<Token*> getInfixTokens() { return valid ? infix : std::vector<Token*>(); }
	size_t getVariableCount() { return tbl::getVariableCount(infix); }

	bool isValid() const { return valid; }

private:
	bool valid;
	std::string expression;
	std::vector<char> rpnTokens, tokens;
	std::vector<Token*> postfix;
	std::vector<Token*> infix;

	Graph<eprs::expLabel> parseGraph;
};