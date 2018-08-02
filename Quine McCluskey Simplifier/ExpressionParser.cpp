#include "ExpressionParser.h"
#include <iostream>
using namespace eprs;

ExpressionParser::ExpressionParser(): parseGraph(buildParseGraph())
{
	valid = false;
}

ExpressionParser::ExpressionParser(const std::string & expression): parseGraph(buildParseGraph())
{
	setExpression(expression);
}

bool ExpressionParser::setExpression(const std::string & expression)
{
	if (expression.empty())
		return valid = false;

	rpnTokens.clear();
	postfix.clear();
	infix.clear();
	this->expression = expression;
	tokens = eprs::tokenizeString(this->expression);

	try
	{
		infix = eprs::tokenizeExpression(parseGraph, tokens);
		postfix = eprs::parseRPN<bool>(infix);
		valid = true;
	} 
	catch (const std::invalid_argument& e) { std::cout << e.what() << std::endl; valid = false; }

	//if (validateExpression(parseGraph, tokens))
	//	valid = parseRPN(tokens, rpnTokens);
	//else
	//	valid = false;

	return valid;
}
