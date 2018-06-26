//
//  main.cpp
//  Quine McCluskey Simplifier
//
//  Created by Andrew Miner on 6/25/18.
//  Copyright © 2018 Andrew Miner. All rights reserved.
//

#include "RPNFunctions.hpp"
#include "BoolExpression.hpp"

void stringToUpper(string& src)
{
    for (char& c : src)
        c = toupper(c);
}

int main(int argc, const char * argv[])
{
    string expression;
    
    do
    {
        cout << "Enter New Expression: ";
        getline(cin, expression);
        fflush(stdin);
        stringToUpper(expression);
        
        vector<string> originalExp;
        queue<string> reversePolish;
        
        if (parseExpression(originalExp, expression) && parseRPNQueue(reversePolish, originalExp))
        {
            displayTable(reversePolish, originalExp);
            
            vector<size_t> minTerms = calculateMinTerms(reversePolish, originalExp);
            cout << endl << "Min Terms: ";
            for (vector<size_t>::iterator it = minTerms.begin(); it != minTerms.end(); ++it)
                cout << *it << " ";
            cout << endl;
            
            BoolExpression exp(minTerms, MinTerms());
            
            QMVec primeImps = exp.getPrimeImplicants();
            SOP petricks = exp.getPetrickSOP();
            int varCount = exp.getVarCount();
            
            std::cout << "Prime Implicants: " << std::endl;
            std::cout << std::string(35 + varCount, '-') << std::endl;
            
            for (QM::QMTerm term : primeImps)
            {
                std::vector<size_t> minTerms = QM::extractMinTerms(term);
                
                std::string minStr = "[";
                
                for (size_t min : minTerms)
                    minStr += std::to_string(min) + "-";
                minStr[minStr.size() - 1] = ']';
                
                std::cout << minStr << std::string((minStr.length() < 35) ? 35 - minStr.length() : 1, ' ');
                
                QM::printQMTerm(std::cout, term, varCount) << std::endl;
            }
            
            std::cout << std::endl << std::endl;
            
            
            for (int i = 0; i < petricks.size(); i++)
            {
                std::cout << "Solution " + std::to_string(i + 1) + ": ";
                PM::printPretrickProduct(std::cout, petricks.at(i), varCount) << std::endl << std::endl << std::endl;
            }
            
            std::cout << std::endl;
        }
        else
            cout << "Error: Invalid Expression!" << endl;
        
        cout << endl << endl << endl;
        
    } while (expression.size()!= 1 || expression[0] != 'X');
    return 0;
}
