//
//  PMFunctions.hpp
//  Quine McCluskey Simplifier
//
//  Created by Andrew Miner on 6/25/18.
//  Copyright © 2018 Andrew Miner. All rights reserved.
//

#ifndef PMFunctions_hpp
#define PMFunctions_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <algorithm>

#include "QMFunctions.hpp"

#define QMVec	std::vector<QM::QMTerm>
#define Product QMVec
#define Sum		QMVec
#define SOP		std::vector<Product>
#define POS		std::vector<Sum>

namespace PM
{
    SOP getPetrickSOP(const QMVec& primeImps, const std::vector<size_t>& minTerms);
    
    std::map<size_t, Sum> groupPrimeImplicants(const QMVec& primeImps, const std::vector<size_t>& minTerms);
    SOP expandGroups(const std::map<size_t, Sum>& groups);
    
    SOP toSOP(const Sum& sum);
    void removeLargerTerms(SOP& expression);
    void removeSmallerDashes(SOP& expression);
    
    SOP foilSums(const SOP& firstSum, const SOP& secondSum);
    SOP simplifySOP(const SOP& sumOfP);
    
    bool lessThanQMVec(const QMVec& i, const QMVec& j);
    bool isQMVecEqual(const QMVec& first, const QMVec& second);
    bool isProductUnique(const SOP& sumOfP, const Product& product);
    bool isProductSubset(const Product& subset, const Product& superset);
    
    std::ostream& printPretrickProduct(std::ostream& out, const Product& product, int varCount);
}



#endif /* PMFunctions_hpp */
