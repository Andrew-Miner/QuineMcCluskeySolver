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

	std::set<size_t> getMinTerms(const TruthTable& table);
	void displayTable(const TruthTable& table, const std::string& expression, const int& varCount);

	size_t getVariableCount(const std::vector<Token*>& expression);
	TruthTable buildTruthTable(std::vector<Token*>& rpnTokens);
	std::bitset<MAX_VARS> getVariables(const std::vector<Token*>& expression);
	bool calculateExpression(std::vector<Token*> rpnTokens, std::bitset<52/*MAX_VARS*/>& variables, const size_t varCount);
}
