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
    int errorTerm;
    if (!validateTerms(minTerms, dontCares, errorTerm))
	{
		if (errorTerm == -1)
			throw std::invalid_argument("Cannot have 0 min terms");
		else
			throw std::invalid_argument(std::to_string(errorTerm) + " cannot be both a min term and a don't care");
	}
    
    this->minTerms = minTerms;
    this->dontCares = dontCares;
    
    varCount = QM::getVariableCount(minTerms, dontCares);
    reduce();
}

BoolExpression::BoolExpression(const std::string& expression) : varCount(0)
{
    
    if (!QM::parseString(minTerms, dontCares, expression))
        throw std::invalid_argument("Expression not valid");
    
    int errorTerm;
	if (!validateTerms(minTerms, dontCares, errorTerm))
	{
		if (errorTerm == -1)
			throw std::invalid_argument("Cannot have 0 min terms");
		else
			throw std::invalid_argument(std::to_string(errorTerm) + " cannot be both a min term and a don't care");
	}
    
    varCount = QM::getVariableCount(minTerms, dontCares);
    reduce();
}

bool BoolExpression::setExpression(const MinTerms& minTerms, const MinTerms& dontCares)
{
    int errorTerm;
    if (!validateTerms(minTerms, dontCares, errorTerm))
        return false;
    
    this->minTerms = minTerms;
    this->dontCares = dontCares;
    
    varCount = QM::getVariableCount(minTerms, dontCares);
    reduce();
    
    return true;
}

bool BoolExpression::setExpression(const std::string & expression)
{
    if (!QM::parseString(minTerms, dontCares, expression))
        return false;
    
    int errorTerm;
    if (!validateTerms(minTerms, dontCares, errorTerm))
    {
        minTerms.clear();
        dontCares.clear();
        
        return false;
    }
    
    varCount = QM::getVariableCount(minTerms, dontCares);
    reduce();
    
    return true;
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
