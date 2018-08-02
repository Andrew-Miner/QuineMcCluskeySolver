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

void test();
void stringToUpper(std::string& src);

int main(int argc, const char * argv[])
{
	test();
	return 0;
}

void stringToUpper(std::string& src)
{
	for (char& c : src)
		c = toupper(c);
}

void test()
{
	std::string exp;
	ExpressionParser parser;

	do
	{
		std::cout << "Enter New Expression: ";
		getline(std::cin, exp);
		fflush(stdin);
		stringToUpper(exp);

		parser.setExpression(exp);

		std::cout << "Valid: " << parser.isValid() << std::endl;
		std::cout << "Expression: " << parser.getExpression() << std::endl;
		
		std::cout << "Infix: ";
		std::vector<Token*> inf = parser.getInfixTokens();
		for (std::vector<Token*>::iterator it = inf.begin(); it != inf.end(); ++it)
			std::cout << (*it)->getToken();
		std::cout << " " << inf.size() << std::endl;

		std::cout << "Postfix: ";
		std::vector<Token*> rpn = parser.getPostfixTokens();
		for (std::vector<Token*>::iterator it = rpn.begin(); it != rpn.end(); ++it)
			std::cout << (*it)->getToken();
		std::cout << std::endl;

		if (!parser.isValid())
			std::cout << "Error: Expression is not valid!!" << std::endl;
		else if (exp.size() != 1 || exp[0] != 'X')
		{
			try
			{
				std::cout << "Var Count: " << tbl::getVariableCount(rpn) << std::endl;
				tbl::TruthTable table = tbl::buildTruthTable(rpn);
				std::set<size_t> minTerms = tbl::getMinTerms(table);

				std::cout << "Min Terms: ";
				for (std::set<size_t>::iterator it = minTerms.begin(); it != minTerms.end(); ++it)
					std::cout << *it << " ";
				std::cout << std::endl << std::endl;

				displayTable(table, parser.getExpression(), parser.getVariableCount());

				BoolExpression exp(std::vector<size_t>(minTerms.begin(), minTerms.end()), MinTerms());
				QMVec primeImps = exp.getPrimeImplicants();
				SOP petricks = exp.getPetrickSOP();
				int varCount = exp.getVarCount();
				std::cout << "Var Count 2: " << varCount << std::endl;

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

				std::cout << std::endl;
			}
			catch (std::invalid_argument exception) { std::cout << "Error: " << exception.what() << "!" << std::endl; }

			std::cout << std::endl << std::endl;
		}
	} while (exp.size() != 1 || exp[0] != 'X');
}