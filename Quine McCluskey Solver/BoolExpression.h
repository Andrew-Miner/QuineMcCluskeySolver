#pragma once
#include <stdio.h>
#include "Algorithms/PMFunctions.h"

typedef std::vector<size_t> MinTerms;

class BoolExpression
{
public:

	// @throws std::invalid_argument
	BoolExpression(const MinTerms& minTerms, const MinTerms& dontCares);

	// @throws std::invalid_argument
	// @throws std::out_of_range
	BoolExpression(const std::string& expression);

	MinTerms getMinTerms() { return minTerms; }
	MinTerms getDontCares() { return dontCares; }
	SOP getPetrickSOP() { return petrickSOP; }
	int getVarCount() { return varCount; }
	QMVec getPrimeImplicants() { return primeImps; }

	bool setExpression(const MinTerms& minTerms, const MinTerms& dontCares);

	// @throws std::out_of_range
	bool setExpression(const std::string& expression);

private:
	int varCount;
	MinTerms minTerms, dontCares;
	SOP petrickSOP;
	QMVec primeImps;

	void reduce();
	bool validateTerms(const MinTerms& minTerms, const MinTerms& dontCares, int& duplicateTerm);
};