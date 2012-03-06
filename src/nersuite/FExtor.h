/*
*      NERSuite
*      Feature extraction functionalities
*
* Copyright (c) 
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the names of the authors nor the names of its contributors
*       may be used to endorse or promote products derived from this
*       software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
* OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef	_FEXTOR_H
#define	_FEXTOR_H


#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>

#include "typedefs.h"
#include "../nersuite_common/string_utils.h"


namespace NER
{
	/** 
	* @ingroup NERsuite
	* Feature Extractor
	*/
	class FeatureExtractor
	{
	public:
		/** Constructs a Feature Extractor
		* @param[in] col_info Indicates how to interpret each input column
		*/
		FeatureExtractor(const COLUMN_INFO& col_info);

		/** Extracts Word Features from a sentence
		* @param[in] one_sent input sentence (List of token) in the POS tagged format
		* @param[in,out] sent_feats Resulting feature list for each input token
		*/
		void ext_WORD_feats(const V2_STR &one_sent, V2_STR &sent_feats);

		/** Extracts Lemma Features from a sentence
		* @param[in] one_sent input sentence (List of token) in the POS tagged format
		* @param[in,out] sent_feats Resulting feature list for each input token
		*/
		void ext_LEMMA_feats(const V2_STR &one_sent, V2_STR &sent_feats);

		/** Extracts Orthogonal Features from a sentence
		* @param[in] one_sent input sentence (List of token) in the POS tagged format
		* @param[in,out] sent_feats Resulting feature list for each input token
		*/
		void ext_ORTHO_feats(const V2_STR &one_sent, V2_STR &sent_feats);

		/** Extracts POS Features from a sentence
		* @param[in] one_sent input sentence (List of token) in the POS tagged format
		* @param[in,out] sent_feats Resulting feature list for each input token
		*/
		void ext_POS_feats(const V2_STR &one_sent, V2_STR &sent_feats);

		/** Extracts Lemma's POS Features from a sentence
		* @param[in] one_sent input sentence (List of token) in the POS tagged format
		* @param[in,out] sent_feats Resulting feature list for each input token
		*/
		void ext_LEMMA_POS_feats(const V2_STR &one_sent, V2_STR &sent_feats);

		/** Extracts Chunk Features from a sentence
		* @param[in] one_sent input sentence (List of token) in the POS tagged format
		* @param[in,out] sent_feats Resulting feature list for each input token
		*/
		void ext_CHUNK_feats(const V2_STR &one_sent, V2_STR &sent_feats);

		/** Extracts Dictionary Features from a sentence
		* @param[in] one_sent input sentence (List of token) in the POS tagged format
		* @param[in,out] sent_feats Resulting feature list for each input token
		* @param[in] opt_dic If 0, enables Lexicalized dictionary hit feature
		*/
		void ext_DIC_feats(const V2_STR &one_sent, V2_STR &sent_feats, int opt_dic);

	private:
		void get_n_grams(const std::string &token, const int n, std::vector<std::string> &ngrams);
		std::string get_item(const V2_STR &one_sent, const V2_STR_citr &i_row, const int col, const int rel_pos);
		void find_chunk_range(const V2_STR &one_sent, const V2_STR_citr &i_row, std::pair<V2_STR_citr, V2_STR_citr> &chk_range);

		std::set<std::string> greek_alphabets;
		const COLUMN_INFO&	COL_INFO;
	};

	inline FeatureExtractor::FeatureExtractor(const COLUMN_INFO& col_info)
		: COL_INFO(col_info)
	{
		std::string g_alphabets[] = { "alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta", "iota", "kappa", "lambda", 
			"mu", "nu", "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "chi", "psi", "omega" };
		greek_alphabets.insert(g_alphabets, g_alphabets + 24);
	}
}

#endif



