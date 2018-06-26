#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <bitset>


namespace QM
{
	const unsigned int MAX_TERMS = sizeof(size_t) * 8;
	const std::string REG_TERMS = " *m *\\( *(\\d+)(?:, *\\d+)* *\\)";
	const std::string REG_DONTS = " *\\+ *d *\\( *(\\d+)(?:, *\\d+)* *\\)";


	struct QMTerm
	{
		bool used;
		std::bitset<MAX_TERMS> term;
		std::bitset<MAX_TERMS> dashMask;

		bool operator==(const QMTerm& other) const
		{
			return term == other.term && dashMask == other.dashMask;
		}
	};

	std::vector<QM::QMTerm> getPrimeImplicants(const std::vector<size_t>& minTerms, const std::vector<size_t>& dontCares, const int& varCount);

	std::ostream& printQMTerm(std::ostream& out, const QMTerm& term, int variables = 0);
	std::vector<size_t> extractMinTerms(const QMTerm& qmTerm);

	void reduceTerms(const std::vector<QMTerm>& terms, std::vector<QMTerm>& resultant, int varCount);
	void removeDuplicateTerms(std::vector<QMTerm>& terms);

	int prevDash(const std::bitset<MAX_TERMS>& dashMask, int pos);
	int nextDash(const std::bitset<MAX_TERMS>& dashMask, int pos);


	bool lessThan(QMTerm i, QMTerm j);
	std::vector<QMTerm> toQMTerms(std::vector<size_t>& minTerms);
	void formatQMTerms(std::vector<QMTerm>& terms);

	bool parseString(std::vector<size_t>& parsedMin, std::vector<size_t>& parsedDonts, const std::string& line);
	int getVariableCount(const std::vector<size_t>& minTerms, const std::vector<size_t>& dontCares);

	std::vector<QMTerm> getEssentialTerms(const std::vector<QMTerm>& terms, const std::vector<size_t> minTerms);
}