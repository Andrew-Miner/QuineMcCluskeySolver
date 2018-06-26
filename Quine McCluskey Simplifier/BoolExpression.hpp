//
//  BoolExpression.hpp
//  Quine McCluskey Simplifier
//
//  Created by Andrew Miner on 6/25/18.
//  Copyright © 2018 Andrew Miner. All rights reserved.
//

#ifndef BoolExpression_hpp
#define BoolExpression_hpp

#include <stdio.h>
#include "PMFunctions.hpp"

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

#endif /* BoolExpression_hpp */
