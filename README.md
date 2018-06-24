# Quine McCluskey Simplifier - Outdated

NOTE: This version is outdated as the most current version is located locally on a seperate computer. Expect an update soon.

This simplifier utilizes both Quine McCluskey's and Petrick's methods to simplify boolean algebra expressions.

I initially wrote one recursive function to perform the basics of the QM method. I was so pleased with this one function that it inspired me to write the rest.

The core QM method function:

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
