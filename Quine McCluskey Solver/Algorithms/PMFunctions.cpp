#include "PMFunctions.h"
#include <iomanip>

namespace PM
{
	bool lessThanQMVec(const QMVec& i, const QMVec& j)
	{
		return i.size() < j.size();
	}

	SOP getPetrickSOP(const QMVec & primeImps, const std::vector<size_t>& minTerms)
	{
		std::map<size_t, Sum> groups = PM::groupPrimeImplicants(primeImps, minTerms);

		SOP finalSolution = PM::expandGroups(groups);
		PM::removeLargerTerms(finalSolution);
		PM::removeSmallerDashes(finalSolution);

		return finalSolution;
	}

	std::map<size_t, Sum> groupPrimeImplicants(const QMVec& primeImps, const std::vector<size_t>& minTerms)
	{
		std::map<size_t, Sum> groupings;

		// Initialize Groupings
		for (size_t minTerm : minTerms)
			groupings.insert(std::pair<size_t, Sum>(minTerm, Sum()));


		// Group QMTerms
		for (QM::QMTerm primeImp : primeImps)
		{
			// Calculate min terms using dash positions
			std::vector<size_t> piMinTerms = QM::extractMinTerms(primeImp);

			for (size_t minTerm : piMinTerms)
			{
				// If extracted min term is not a don't care, insert it into the map
				std::map<size_t, Sum>::iterator group = groupings.find(minTerm);

				if (group != groupings.end())
					group->second.push_back(primeImp);
			}
		}

		return groupings;
	}

	SOP expandGroups(const std::map<size_t, Sum>& groups)
	{
		if (!groups.size())
			return SOP();

		std::map<size_t, std::vector<QM::QMTerm>>::const_iterator it = groups.begin();

		SOP workingSOP = toSOP(it->second);
		++it;

		while (it != groups.end())
		{
			if (it->second.size())
			{
				SOP nextProd = toSOP(it->second);
				workingSOP = foilSums(workingSOP, nextProd);

				std::sort(workingSOP.begin(), workingSOP.end(), lessThanQMVec);
				workingSOP = simplifySOP(workingSOP);
			}

			++it;
		}

		return workingSOP;
	}

	void removeLargerTerms(SOP& expression)
	{
		if (!expression.size())
			return;

		// Find Smallest Term Size and remove those larger
		unsigned long smallestSize = expression.at(0).size();

		for (SOP::iterator it = expression.begin() + 1; it < expression.end();)
		{
			if (it->size() > smallestSize)
				it = expression.erase(it);
			else
			{
				if (it->size() < smallestSize)
					smallestSize = it->size();

				++it;
			}
		}

		// Iterate one more time to remove remaning large terms
		for (SOP::iterator it = expression.begin(); it < expression.end();)
		{
			if (it->size() > smallestSize)
				it = expression.erase(it);
			else
				++it;
		}
	}

	void removeSmallerDashes(SOP & expression)
	{
		if (!expression.size())
			return;

		// Find largest dash count and remove those smaller
		unsigned long largestDash = 0;
		int term = 1;

		for (SOP::iterator it = expression.begin(); it < expression.end();)
		{
			int dashCount = 0;

			for (QM::QMTerm term : *it)
				dashCount += term.dashMask.count();

			if (dashCount < largestDash)
				it = expression.erase(it);
			else
			{
				largestDash = dashCount;
				++it;
			}

		}

		// Iterate one more time to remove remaning small (larger) terms
		for (SOP::iterator it = expression.begin(); it < expression.end();)
		{
			int dashCount = 0;

			for (QM::QMTerm term : *it)
				dashCount += term.dashMask.count();

			if (dashCount < largestDash)
				it = expression.erase(it);
			else
				++it;
		}
	}

	SOP toSOP(const Sum& sum)
	{
		SOP result;

		for (QM::QMTerm term : sum)
		{
			Product pr;
			pr.push_back(term);
			result.push_back(pr);
		}

		return result;
	}

	SOP foilSums(const SOP& firstSum, const SOP& secondSum)
	{
		SOP resultant;

		for (SOP::const_iterator multTerm = firstSum.begin(); multTerm != firstSum.end(); ++multTerm)
		{
			for (SOP::const_iterator loopTerm = secondSum.begin(); loopTerm != secondSum.end(); ++loopTerm)
			{
				for (Product::const_iterator var = loopTerm->begin(); var != loopTerm->end(); ++var)
				{
					Product product;
					product.insert(product.end(), multTerm->begin(), multTerm->end());

					if (std::find(product.begin(), product.end(), *var) == product.end())
						product.push_back(*var);

					resultant.push_back(product);
				}
			}
		}

		return resultant;
	}

	// Precondition: SOP is sorted by var count per term
	SOP simplifySOP(const SOP& sumOfP)
	{
		SOP simplified(sumOfP);

		for (SOP::iterator it = simplified.begin(); it != simplified.end(); ++it)
		{
			for (SOP::iterator check = it + 1; check != simplified.end();)
			{
				if (isProductSubset(*it, *check))
					check = simplified.erase(check);
				else
					++check;
			}
		}

		return simplified;
	}

	bool isProductUnique(const SOP& sumOfP, const Product& product)
	{
		for (SOP::const_iterator prod = sumOfP.begin(); prod != sumOfP.end(); ++prod)
		{
			if (isQMVecEqual(*prod, product))
				return false;
		}

		return true;
	}

	bool isQMVecEqual(const QMVec& first, const QMVec& second)
	{
		// TODO try first == second

		if (first.size() != second.size())
			return false;

		for (QM::QMTerm term : first)
		{
			if (std::find(second.begin(), second.end(), term) == second.end())
				return false;
		}

		return true;
	}

	bool isProductSubset(const Product& subset, const Product& superset)
	{
		if (subset.size() > superset.size())
			return false;

		for (QM::QMTerm term : subset)
		{
			if (std::find(superset.begin(), superset.end(), term) == superset.end())
				return false;
		}

		return true;
	}

	std::ostream& printPretrickProduct(std::ostream& out, const Product& product, int varCount)
	{
		int varLimit = (varCount == 0) ? QM::MAX_TERMS - 1 : varCount - 1;

		for (Product::const_iterator term = product.begin(); term != product.end(); ++term)
		{
			for (int i = varLimit; i >= 0; i--)
			{
				if (!term->dashMask[i])
				{
					out << char('A' + (varLimit - i));

					if (!term->term[i])
						out << "'";
				}
			}

			if (term + 1 != product.end())
				out << " + ";
		}

		return out;
	}
}
