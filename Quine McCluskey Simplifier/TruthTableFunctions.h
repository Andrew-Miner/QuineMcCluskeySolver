#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <iomanip>
#include <iostream>
#include <bitset>
#include <cmath>
#include <set>
#include <assert.h>
#include "ParseFunctions.h"

namespace tbl
{
	const size_t MAX_VARS = 52;

	struct tableRow
	{
		bool truthValue;
		std::bitset<MAX_VARS> variables;
	};

	typedef std::vector<tableRow> TruthTable;

	size_t getVariableCount(const std::vector<char>& expression);
	TruthTable buildTruthTable(std::vector<char>& rpnTokens);
	std::bitset<MAX_VARS> getVariables(const std::vector<char>& expression);
	bool calculateExpression(std::vector<char> reversePolish, std::bitset<MAX_VARS>& variables, const size_t varCount);
	std::set<size_t> getMinTerms(const TruthTable& table);
	void displayTable(const TruthTable& table, const std::string& expression, const int& varCount);
}
