//
//  BoolExpression.cpp
//  Quine McCluskey Simplifier
//
//  Created by Andrew Miner on 6/25/18.
//  Copyright © 2018 Andrew Miner. All rights reserved.
//

#include "BoolExpression.hpp"
#include <iomanip>
#include <sstream>
#include <climits>

BoolExpression::BoolExpression(const MinTerms& minTerms, const MinTerms& dontCares) : varCount(0)
{
    int errorTerm = -2;
	if (!setExpression(minTerms, dontCares, errorTerm))
	{
		if (errorTerm == -1)
			throw std::invalid_argument("Cannot have 0 min terms");
		else if(errorTerm >= 0)
			throw std::invalid_argument(std::to_string(errorTerm) + " cannot be both a min term and a don't care");
		else
			throw std::invalid_argument("Expression not valid");
	}
}

BoolExpression::BoolExpression(const std::string& expression) : varCount(0), parser(expression)
{
	int errorTerm = -2;
	if (!setExpression(expression, errorTerm))
	{
		if (errorTerm == -1)
			throw std::invalid_argument("Cannot have 0 min terms");
		else if (errorTerm >= 0)
			throw std::invalid_argument(std::to_string(errorTerm) + " cannot be both a min term and a don't care");
		else
			throw std::invalid_argument("Expression not valid");
	}
}

bool BoolExpression::setExpression(const MinTerms& minTerms, const MinTerms& dontCares)
{
    int errorTerm;
	return setExpression(minTerms, dontCares);
}

bool BoolExpression::setExpression(const MinTerms& minTerms, const MinTerms& dontCares, int& errorCode)
{
	if (!validateTerms(minTerms, dontCares, errorCode))
		return false;

	this->minTerms = minTerms;
	this->dontCares = dontCares;

	varCount = QM::getVariableCount(minTerms, dontCares);
	reduce();

	return true;
}

bool BoolExpression::setExpression(const std::string & expression)
{
	int errorCode;
	return setExpression(expression, errorCode);
}

//TODO:
//		Convert minTerms to set
bool BoolExpression::setExpression(const std::string & expression, int& errorCode)
{
	parser.setExpression(expression);
	if (parser.isValid())
	{
		varCount = tbl::getVariableCount(parser.getPostfixTokens());
		table = tbl::buildTruthTable(parser.getPostfixTokens());
		std::set<size_t> minTerms = tbl::getMinTerms(table);
		this->minTerms = std::vector<size_t>(minTerms.begin(), minTerms.end());
	}
	else if (QM::parseString(minTerms, dontCares, expression))
	{
		if (!validateTerms(minTerms, dontCares, errorCode))
		{
			minTerms.clear();
			dontCares.clear();
			return false;
		}

		varCount = QM::getVariableCount(minTerms, dontCares);
	}
	else
	{
		errorCode = -2;
		return false;
	}

    reduce();
    return true;
}

std::string BoolExpression::toString()
{
	if (parser.isValid())
		return parser.getExpression();

	std::string str = "m(";
		
	std::vector<size_t>::iterator it;
	for (it = minTerms.begin(); it != minTerms.end(); ++it)
	{
		str += *it;
	}
	str += ")";

	if (dontCares.size() > 0)
	{
		str += "+d(";
		for (it = dontCares.begin(); it != dontCares.end(); ++it)
		{
			str += *it;
		}
		str += ")";
	}

	return str;
}

void BoolExpression::reduce()
{
    QMVec nonEssentialPrimeImps = QM::getPrimeImplicants(minTerms, dontCares);
    petrickSOP = PM::getPetrickSOP(nonEssentialPrimeImps, minTerms);
    primeImps = QM::getEssentialTerms(nonEssentialPrimeImps, minTerms);
}

bool BoolExpression::validateTerms(const MinTerms& minTerms, const MinTerms& dontCares, int& duplicateTerm)
{
    if (!minTerms.size())
    {
        duplicateTerm = -1;
        return false;
    }
    
    for (size_t minTerm : dontCares)
    {
        if (std::find(minTerms.begin(), minTerms.end(), minTerm) != minTerms.end())
        {
            duplicateTerm = minTerm;
            return false;
        }
    }
    
    return true;
}
