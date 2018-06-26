//
//  QMFunctions.cpp
//  Quine McCluskey Simplifier
//
//  Created by Andrew Miner on 6/25/18.
//  Copyright © 2018 Andrew Miner. All rights reserved.
//

#include "QMFunctions.hpp"
#include <algorithm>
#include <math.h>
#include <regex>
#include <iomanip>

std::vector<QM::QMTerm> QM::getPrimeImplicants(const std::vector<size_t>& minTerms, const std::vector<size_t>& dontCares, const int& varCount)
{
    std::vector<size_t> allInts = minTerms;
    allInts.insert(allInts.end(), dontCares.begin(), dontCares.end());
    
    std::vector<QMTerm> terms = toQMTerms(allInts);
    formatQMTerms(terms);
    
    std::vector<QMTerm> resultant;
    reduceTerms(terms, resultant, varCount);
    
    removeDuplicateTerms(resultant);
    
    return resultant;
}

std::ostream & QM::printQMTerm(std::ostream & out, const QMTerm & term, int variables)
{
    int varLimit = (variables == 0) ? MAX_TERMS - 1 : variables - 1;
    
    for (int i = varLimit; i >= 0; i--)
    {
        if ((i + 1) % 4 == 0)
            std::cout << " ";
        
        if (term.dashMask[i])
            std::cout << "-";
        else
            std::cout << term.term[i];
    }
    
    return out;
}

std::vector<QM::QMTerm> QM::toQMTerms(std::vector<size_t>& minTerms)
{
    std::vector<QM::QMTerm> terms;
    
    for (std::vector<size_t>::iterator i = minTerms.begin(); i != minTerms.end(); ++i)
        terms.push_back({ 0, *i, 0 });
    
    return terms;
}

void QM::formatQMTerms(std::vector<QMTerm>& terms)
{
    std::sort(terms.begin(), terms.end(), lessThan);
}

std::vector<size_t> QM::extractMinTerms(const QMTerm & qmTerm)
{
    std::bitset<MAX_TERMS> minTerm = qmTerm.term;
    
    std::vector<size_t> resultant;
    resultant.push_back(minTerm.to_ulong());
    
    int curCount = 1;
    int dashPos = -1;
    bool forward = true;
    int minTermCount = pow(2, qmTerm.dashMask.count());
    
    while (curCount < minTermCount)
    {
        int nextPos = -1;
        
        if (forward)
            nextPos = nextDash(qmTerm.dashMask, dashPos);
        else
            nextPos = prevDash(qmTerm.dashMask, dashPos);
        
        if (nextPos == -1)
        {
            forward = !forward;
            continue;
        }
        
        dashPos = nextPos;
        minTerm[dashPos] = ~minTerm[dashPos];
        resultant.push_back(minTerm.to_ulong());
        curCount++;
    }
    
    return resultant;
}

void QM::reduceTerms(const std::vector<QMTerm>& terms, std::vector<QMTerm>& resultant, int varCount)
{
    std::vector<QMTerm> tmpTerms = terms,
    reducedTerms;
    
    for (std::vector<QMTerm>::iterator it = tmpTerms.begin(); it != tmpTerms.end();)
    {
        for (std::vector<QMTerm>::iterator r = it + 1; r != tmpTerms.end(); ++r)
        {
            if (it->dashMask == r->dashMask)
            {
                // Core of QM method
                QMTerm resultant = { 0, it->term & r->term, it->term ^ r->term };
                
                // Real resultant will always have 1 more dash mark than the original terms
                if (resultant.dashMask.count() != 1)
                    continue;
                
                resultant.dashMask |= it->dashMask;
                
                reducedTerms.push_back(resultant);
                
                it->used = true;
                r->used = true;
            }
        }
        
        // Remove Used Terms
        if (it->used)
            it = tmpTerms.erase(it);
        else
            ++it;
    }
    
    // Remove Any Remaining Used Terms
    std::vector<QMTerm>::iterator iter = tmpTerms.begin();
    while (iter != tmpTerms.end())
    {
        if (iter->used)
            iter = tmpTerms.erase(iter);
        else
            ++iter;
    }
    
    // Save Unused Terms
    resultant.insert(resultant.end(), tmpTerms.begin(), tmpTerms.end());
    
    // If We're Not Done Reducing
    if (reducedTerms.size() != 0)
    {
        removeDuplicateTerms(reducedTerms);
        reduceTerms(reducedTerms, resultant, varCount);
    }
}

void QM::removeDuplicateTerms(std::vector<QMTerm>& terms)
{
    for (std::vector<QMTerm>::iterator it = terms.begin(); it != terms.end(); ++it)
    {
        std::vector<QMTerm>::iterator dup = it + 1;
        while (dup != terms.end())
        {
            if (it->term == dup->term && it->dashMask == dup->dashMask)
                dup = terms.erase(dup);
            else
                ++dup;
        }
    }
}

bool QM::lessThan(QMTerm i, QMTerm j)
{
    if (i.term.count() > j.term.count())
        return false;
    
    if (i.term.count() == j.term.count())
        return i.term.to_ulong() < j.term.to_ulong();
    
    return true;
}

int QM::prevDash(const std::bitset<MAX_TERMS>& dashMask, int pos)
{
    for (int i = (pos == -1) ? static_cast<int>(dashMask.size()) - 1 : pos - 1; i >= 0; i--)
    {
        if (dashMask[i])
            return i;
    }
    
    return -1;
}

int QM::nextDash(const std::bitset<MAX_TERMS>& dashMask, int pos)
{
    for (int i = (pos == -1) ? 0 : pos + 1; i < dashMask.size(); i++)
    {
        if (dashMask[i])
            return i;
    }
    
    return -1;
}

bool QM::parseString(std::vector<size_t>& parsedMin, std::vector<size_t>& parsedDonts, const std::string & line)
{
    std::regex fullExpression("^(" + REG_TERMS + ")(" + REG_DONTS + ")?$");
    std::regex regTerms(REG_TERMS);
    std::regex regDonts(REG_DONTS);
    std::regex regNumbs("\\d+");
    
    // m(1,2,3,4) + d(5,6,7,8)
    if (!std::regex_match(line, fullExpression))
        return false;
    
    
    // m(1,2,3,4,5)
    std::smatch mMatch;
    std::regex_search(line, mMatch, regTerms);
    std::string minTerms = mMatch[0];
    
    for (std::sregex_iterator it(minTerms.cbegin(), minTerms.cend(), regNumbs); it != std::sregex_iterator(); ++it)
    {
        try { parsedMin.push_back(std::stoul(it->str(0))); }
        catch (const std::out_of_range& oor) { throw std::out_of_range(it->str(0) + " is not a valid min term"); }
    }
    
    
    // +d(6,7,8,9,10)
    std::smatch dMatch;
    std::regex_search(line, dMatch, regDonts);
    
    if (dMatch.size())
    {
        std::string dontTerms = dMatch[0];
        
        for (std::sregex_iterator it(dontTerms.cbegin(), dontTerms.cend(), regNumbs); it != std::sregex_iterator(); ++it)
        {
            try { parsedDonts.push_back(std::stoul(it->str(0))); }
            catch (const std::out_of_range& oor) { throw std::out_of_range(it->str(0) + " is not a valid min term"); }
        }
    }
    
    return true;
}

int QM::getVariableCount(const std::vector<size_t>& minTerms, const std::vector<size_t>& dontCares)
{
    size_t highest = 0;
    
    for (size_t term : minTerms)
    {
        if (term > highest)
            highest = term;
        else if (!highest && !term)
            highest = 1;
    }
    
    for (size_t term : dontCares)
    {
        if (term > highest)
            highest = term;
    }
    
    int bits = 0;
    while (highest > 0)
    {
        highest >>= 1;
        bits++;
    }
    
    return bits;
}

std::vector<QM::QMTerm> QM::getEssentialTerms(const std::vector<QMTerm>& terms, const std::vector<size_t> minTerms)
{
    std::vector<QMTerm> essentialTerms;
    
    for (QMTerm term : terms)
    {
        std::vector<size_t> mins = extractMinTerms(term);
        
        for (size_t min : mins)
        {
            if (std::find(minTerms.begin(), minTerms.end(), min) != minTerms.end())
            {
                essentialTerms.push_back(term);
                break;
            }
        }
    }
    
    return essentialTerms;
}
