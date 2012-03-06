#ifndef _FEXT_HELPER_H
#define _FEXT_HELPER_H

#include <vector>
#include <string>
#include "../nersuite/FExtor.h"

namespace NER
{
	void FeatureToStringHelper(const V2_STR& feats, std::vector<std::string>& feats_str);
	void SetupColumnInfoHelper(COLUMN_INFO& cinfo);
	std::vector<std::string> CreateWordHelper(const char* beg, const char* end, const char* word, const char* lemma, const char* pos, const char* chunk, const char* dic);
}

#endif
