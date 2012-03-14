#include "sentence_tagger.h"
#include "../nersuite_common/string_utils.h"

using namespace std;

namespace NER
{
	size_t	SentenceTagger::max_ne_len = 10;
	bool	SentenceTagger::filter_NN = true;
	int		SentenceTagger::normalize_type = NormalizeNone;

	SentenceTagger::SentenceTagger()
	{
		v_ne.reserve(256);
		v_idx.reserve(128);
		document_separator_seen = false;
	}

	size_t SentenceTagger::read(istream &is, const string &multidoc_separator)
	{
		m_Content.clear();		// clear the container

		string				line = "";
		vector<string>		line_items;

		// reset document end marker on start
		document_separator_seen = false;

		while (true) 
		{
			getline(is, line);
			if (line.empty())
			{
				// break if a blank line appears
				break;
			}

			if (multidoc_separator != "" && line == multidoc_separator)
			{
				// mark doc end and break on document separator
				document_separator_seen = true;
				break;
			}

			tokenize(line_items, line, "\t");		// tokenize and
			m_Content.push_back(line_items);		// save it
		}
		return size();
	}

	void SentenceTagger::tag_nes(const Dictionary& dict)
	{
		int	ne_len = 0;
		NE	one_ne;

		v_ne.clear();
		v_idx.clear();

		for (size_t i_row = 0; i_row < size(); ++i_row)
		{
			if ((normalize_type&NormalizeToken) != 0)
			{
				// Token-base matching
				// --- try to match each single word of the sentence with single token in the dictionary
				ne_len = find_exact(i_row, one_ne, dict);
			}
			else
			{
				// Normal matching
				// --- try to find the longest sequence of words which matches a dictionary entry
				ne_len = find_longest(i_row, one_ne, dict);
			}
			if (ne_len > 0)
			{
				v_ne.push_back(one_ne);
			}
		}
		resolve_collision();
		mark_ne(dict);
	}

	// Exact matching, choose a NE that comes first
	void SentenceTagger::resolve_collision()
	{
		int	re_beg = -1;
		size_t	num_ne = v_ne.size();

		for (size_t idx = 0; idx < num_ne; ++idx)
		{
			if (v_ne[idx].begin > re_beg)
			{
				v_idx.push_back(idx);
				re_beg = v_ne[idx].end;
			}
		}
	}

	void SentenceTagger::mark_ne(const Dictionary& dict)
	{
		int ori_n_col = (int) m_Content.front().size();
		size_t nclasses = dict.get_class_count();

		// 1) Create dictionary check columns
		for (V2_STR::iterator i_row = m_Content.begin(); i_row != m_Content.end(); ++i_row)
		{
			for (int k = 0; k < nclasses; ++k)
			{
				i_row->push_back("O");
			}
		}

		// 2) Fill matching information
		for (vector<int>::const_iterator itr = v_idx.begin(); itr != v_idx.end(); ++itr) 
		{
			// Tag for all classes that this NE belongs to
			for (list<string>::const_iterator cls_itr = v_ne[ *itr ].classes.begin();
				cls_itr != v_ne[ *itr ].classes.end();
				++cls_itr)
			{
				// 1) Find a descriptive semantic class name
				const string&	sem_name = dict.get_class_name(atoi(cls_itr->c_str()));

				// 2) Label the data
				int		pos = v_ne[ *itr ].begin;
				m_Content[ pos ][ ori_n_col + atoi(cls_itr->c_str()) ] = "B-" + sem_name;

				for (pos = pos + 1; pos <= v_ne[ *itr ].end; ++pos)
				{
					m_Content[ pos ][ ori_n_col + atoi(cls_itr->c_str()) ] = "I-" + sem_name;
				}
			}
		}
	}

	int SentenceTagger::find_exact(size_t i_row, NE& ne, const Dictionary& dict) const
	{
		// Search dictionary
		string key = m_Content[i_row][RAW_TOKEN_COL];

		size_t count;
		const int *value = dict.get_classes(key, normalize_type, &count);
		if (value != NULL)
		{
			ne.begin = i_row;
			ne.end = i_row;
			ne.classes.clear();
			for (int i = 0; i < count; ++i)
			{
				ne.classes.push_back(int2str(value[i]));
			}
			ne.sim = 1.0;
			return 1;
		}
		return 0;
	}

	int SentenceTagger::find_longest(size_t i_row, NE& ne, const Dictionary& dict) const
	{
		size_t	key_min_len = 0;
		size_t	key_len = max_ne_len;

		// Find the minimum length of a key that includes NN-POS tag
		if (filter_NN && ((key_min_len = find_min_length(i_row)) == (size_t)(-1)))
		{
			return 0;
		}

		if ((i_row + max_ne_len) > size())
		{
			// Make it sure that key generation does not encounter vector end boundary error
			key_len = size() - i_row;
		}

		if ((size() == (i_row + key_len)) && (m_Content.back()[RAW_TOKEN_COL] == "."))
		{
			// Last period will not be searched
			--key_len;
		}

		// Search dictionary, longer candidate first
		string	key = "";
		for (; key_len > key_min_len; --key_len)
		{
			// 1) Make a key
			key = m_Content[i_row][RAW_TOKEN_COL];

			for (int idx = 1; idx < key_len; ++idx)
			{
				if (m_Content[i_row + idx][BEG_COL] != m_Content[i_row + idx - 1][END_COL])
				{
					// Space between two tokens
					key += " ";
				}
				key += m_Content[i_row + idx][RAW_TOKEN_COL];
			}

			// 2) Search Dictionary
			size_t count;
			const int *value = dict.get_classes(key, normalize_type, &count);
			if (value != NULL)
			{
				ne.begin = i_row;
				ne.end = i_row + key_len - 1;			// A range is [begin, end]
				ne.classes.clear();
				for (int i = 0; i < count; ++i)
				{
					ne.classes.push_back(int2str(value[i]));
				}
				ne.sim = 1.0;
				return key_len;						// Break when the longest is found
			}
		}
		return 0;
	}

	size_t SentenceTagger::find_min_length(size_t i_row) const
	{
		size_t end = i_row + max_ne_len;

		if (end >= size())
		{
			end = size();
		}

		for (size_t col = i_row ; col < end; ++col)
		{
			if (m_Content[col][POS_COL].substr(0, 2) == "NN") 
			{
				return col - i_row;
			}
		}
		return (size_t)(-1);
	}
}
