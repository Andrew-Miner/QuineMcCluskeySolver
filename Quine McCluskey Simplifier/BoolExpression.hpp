//
//  BoolExpression.hpp
//  Quine McCluskey Simplifier
//
//  Created by Andrew Miner on 6/25/18.
//  Copyright © 2018 Andrew Miner. All rights reserved.
//

// TODO: Turn this into a usable class
//      * Regular Expressions Constructor: ~A + B * (C*D)
//      * Equal Operators
//      * Operator +/+=
//      * Operator */*=
//      * Operator !/!=
//      * to_string
//      * Make reduce public and return this
//      * Maybe A + B + d(dont cares)
//      * Maybe Contructor with varCount

// Wishlist:
//      * Replace Basic Operations with their
//        Secondary counterparts
//        On second thought maybe not
//        because circuit designers might
//        find use in seeing the Basic Operators


#ifndef BoolExpression_hpp
#define BoolExpression_hpp

#include <stdio.h>
#include "PMFunctions.hpp"
#include "ExpressionParser.h"

typedef std::vector<size_t> MinTerms;

class BoolExpression
{
public:
    
    // @throws std::invalid_argument
    BoolExpression(const MinTerms& minTerms, const MinTerms& dontCares);
    
    // @throws std::invalid_argument
    // @throws std::out_of_range
    BoolExpression(const std::string& expression);

	// check if expression valid
    MinTerms getMinTerms() { return isValid() ? minTerms : MinTerms(); }
    MinTerms getDontCares() { return isValid() ? dontCares : MinTerms(); }
    SOP getPetrickSOP() { return isValid() ? petrickSOP : SOP(); }
    int getVarCount() { return varCount; }
    QMVec getPrimeImplicants() { return isValid() ? primeImps : QMVec(); }

	bool isValid() { return parser.isValid() || varCount > 0; }

	// make table for minTerm constructor
	// check if expression valid
	tbl::TruthTable getTruthTable() { return table; }

    bool setExpression(const MinTerms& minTerms, const MinTerms& dontCares);
	bool setExpression(const MinTerms& minTerms, const MinTerms& dontCares, int& errorCode);
    
    // @throws std::out_of_range
    bool setExpression(const std::string& expression);
	bool setExpression(const std::string & expression, int& errorCode);

	// check if expression valid
	std::string toString();
    
private:
    int varCount;
    MinTerms minTerms, dontCares;
    SOP petrickSOP;
    QMVec primeImps;

	tbl::TruthTable table;
	ExpressionParser parser;
    
    void reduce();
    bool validateTerms(const MinTerms& minTerms, const MinTerms& dontCares, int& duplicateTerm);
};

#endif /* BoolExpression_hpp */
