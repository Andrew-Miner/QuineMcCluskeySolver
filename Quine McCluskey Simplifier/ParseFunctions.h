#pragma once
#include "Graph.h"
#include <vector>
#include <bitset>


const char OR_OP = '+';
const char AND_OP = '*';
const char NOT_OP = '~';
const char NAND_OP = '@';
const char XOR_OP = '^';
const char NOR_OP = '%';
const char LESS_OP = '<';
const char GREATER_OP = '>';
const char SPACE = ' ';
const char EQUAL_OP = '=';
const char LEFT_PAR = '(';
const char RIGHT_PAR = ')';

const std::string OPERATORS = "~@*^%+><";//"" + NOT_OP + NAND_OP + AND_OP + XOR_OP + NOR_OP + OR_OP + GREATER_OP + LESS_OP; //"~@&^%|><";
const int PRECEDENCE[10] = { -4,-3,-3,-2,-2,-2,-1,-0 };

struct expLabel // Expression Vertex Label
{
	bool startVertex, successVertex;
	std::set<char> labels;
};

std::vector<char> tokenizeString(const std::string & str);
bool validateExpression(const Graph<expLabel>& g, const std::vector<char>& expression);
bool recursiveValidate(const Graph<expLabel>& g, const size_t& curVertex, std::vector<char>::const_iterator& start, std::vector<char>::const_iterator& end);
bool parseRPN(const std::vector<char>& exp, std::vector<char>& rpnTokens);