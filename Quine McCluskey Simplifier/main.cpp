//
//  main.cpp
//  Quine McCluskey Simplifier
//
//  Created by Andrew Miner on 6/25/18.
//  Copyright © 2018 Andrew Miner. All rights reserved.
//

#include "TruthTableFunctions.h"
#include "BoolExpression.hpp"
#include "ExpressionParser.h"

void stringToUpper(std::string& src);

int main(int argc, const char * argv[])
{
	//test();
	std::string exp;
	ExpressionParser parser;

	do
	{
		std::cout << "Enter New Expression: ";
		getline(std::cin, exp);
		fflush(stdin);
		stringToUpper(exp);

		if (exp.size() != 1 || exp[0] != 'X')
		{
			try
			{
				BoolExpression expression(exp);
				std::cout << "Var Count: " << expression.getVarCount() << std::endl;

				std::vector<size_t> minTerms = expression.getMinTerms();
				std::cout << "Min Terms: ";
				for (std::vector<size_t>::iterator it = minTerms.begin(); it != minTerms.end(); ++it)
					std::cout << *it << " ";
				std::cout << std::endl << std::endl;

				displayTable(expression.getTruthTable(), expression.toString(), expression.getVarCount());

				QMVec primeImps = expression.getPrimeImplicants();
				SOP petricks = expression.getPetrickSOP();
				int varCount = expression.getVarCount();

				std::cout << std::endl;
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
			}
			catch (std::invalid_argument exception) { std::cout << "Error: " << exception.what() << "!" << std::endl << std::endl; }
		}
	} while (exp.size() != 1 || exp[0] != 'X');
	return 0;
}

void stringToUpper(std::string& src)
{
	for (char& c : src)
		c = toupper(c);
}