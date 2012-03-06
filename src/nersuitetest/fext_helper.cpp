#include <vector>
#include <string>
#include "fext_helper.h"

using namespace std;

namespace NER
{
	void SetupColumnInfoHelper(COLUMN_INFO& cinfo)
	{
		cinfo.BEG	= 0;
		cinfo.END	= 1;
		cinfo.WORD	= 2;
		cinfo.LEMMA	= 3;
		cinfo.POS	= 4;
		cinfo.CHUNK	= 5;
		cinfo.DIC	= 6;
	}

	vector<string> CreateWordHelper(const char* beg, const char* end, const char* surface, const char* lemma, const char* pos, const char* chunk, const char* dic)
	{
		vector<string> word;
		word.push_back(beg);
		word.push_back(end);
		word.push_back(surface);
		word.push_back(lemma);
		word.push_back(pos);
		word.push_back(chunk);
		word.push_back(dic);
		return word;
	}

	void FeatureToStringHelper(const V2_STR& feats, vector<string>& feats_str)
	{
		for (V2_STR_citr i = feats.begin(); i != feats.end(); ++i)
		{
			ostringstream str;
			for (vector<string>::const_iterator j = (*i).begin(); j != (*i).end(); ++j)
			{
				str << *j;
				str << ",";
			}
			feats_str.push_back(str.str());
		}
	}
}