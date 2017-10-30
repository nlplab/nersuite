/*
*      NERSuite
*      Implementation of feature extractor
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

#include "FExtor.h"


using namespace std;


#define		LHS_CS		-2			// Left hand side context size
#define		RHS_CS		2


//
// Interface member functions
//

namespace NER
{
	void FeatureExtractor::ext_WORD_feats(const V2_STR &one_sent, V2_STR &sent_feats)
	{
		string					one_feat= "", feat_head = "";
		string					feat_value1 = "", feat_value2 = "";
		string					feat_nor_value1 = "", feat_nor_value2 = "";
		vector<string>			ngrams;
		V2_STR::iterator		i_row_feats = sent_feats.begin(), i_tmp_row;

		ngrams.reserve(100);


		for (V2_STR_citr i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) 
		{
			// Character n-gram features	
			feat_head = "2gram=";
			get_n_grams((*i_row)[ COL_INFO.WORD ], 2, ngrams);
			for (vector<string>::iterator itr = ngrams.begin(); itr != ngrams.end(); ++itr) {
				one_feat = feat_head + (*itr);
				(*i_row_feats).push_back(one_feat);
			}

			feat_head = "3gram=";
			get_n_grams((*i_row)[ COL_INFO.WORD ], 3, ngrams);
			for (vector<string>::iterator itr = ngrams.begin(); itr != ngrams.end(); ++itr) {
				one_feat = feat_head + (*itr);
				(*i_row_feats).push_back(one_feat);
			}

			feat_head = "4gram=";
			get_n_grams((*i_row)[ COL_INFO.WORD ], 4, ngrams);
			for (vector<string>::iterator itr = ngrams.begin(); itr != ngrams.end(); ++itr) {
				one_feat = feat_head + (*itr);
				(*i_row_feats).push_back(one_feat);
			}


			// Uni-gram features
			for (int rel_pos = LHS_CS; rel_pos <= RHS_CS; ++rel_pos) {

				// Unnormalized  uni-gram word features
				feat_value1 = get_item(one_sent, i_row, COL_INFO.WORD, rel_pos);		

				if (feat_value1 != "NULL" ) { 			
					feat_head = "W_U_" + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_value1;
					(*i_row_feats).push_back(one_feat);

					// Normalized  uni-gram word features
					feat_nor_value1 = squeeze_nums(feat_value1);

					feat_head = "W_NU_" + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_nor_value1;
					(*i_row_feats).push_back(one_feat);
				}
			}


			// Bi-gram features
			for (int rel_pos = LHS_CS + 1; rel_pos <= RHS_CS; ++rel_pos) {

				// Unnormalized  bi-gram word features
				feat_value1 = get_item(one_sent, i_row, COL_INFO.WORD, rel_pos - 1);
				feat_value2 = get_item(one_sent, i_row, COL_INFO.WORD, rel_pos);

				if ((feat_value1 != "NULL") || (feat_value2 != "NULL")) {
					feat_head = "W_B_" + int2strIDX(rel_pos - 1) + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_value1 + "/" + feat_value2;
					(*i_row_feats).push_back(one_feat);

					// Normalized  bi-gram word features
					feat_nor_value1 = squeeze_nums(feat_value1);							
					feat_nor_value2 = squeeze_nums(feat_value2);

					feat_head = "W_NB_" + int2strIDX(rel_pos - 1) + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_nor_value1 + "/" + feat_nor_value2;
					(*i_row_feats).push_back(one_feat);
				}
			}

			i_row_feats++;				// Increase row feature iterator
		}
	}


	void FeatureExtractor::ext_LEMMA_feats(const V2_STR &one_sent, V2_STR &sent_feats)
	{
		string					one_feat= "", feat_head = "";
		string					feat_value1 = "", feat_value2 = "";
		string					feat_nor_value1 = "", feat_nor_value2 = "";
		V2_STR::iterator		i_row_feats = sent_feats.begin(), i_tmp_row;


		for (V2_STR_citr i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) 
		{
			// Uni-gram features
			for (int rel_pos = LHS_CS; rel_pos <= RHS_CS; ++rel_pos) {

				// Unnormalized  uni-gram lemma features
				feat_value1 = get_item(one_sent, i_row, COL_INFO.LEMMA, rel_pos);

				if (feat_value1 != "NULL" )  {
					feat_head = "L_U_" + int2strIDX(rel_pos) + "=";						
					one_feat = feat_head + feat_value1;
					(*i_row_feats).push_back(one_feat);

					// Normalized  uni-gram lemma features
					feat_nor_value1 = squeeze_nums(feat_value1);

					feat_head = "L_NU_" + int2strIDX(rel_pos) + "=";					
					one_feat = feat_head + feat_nor_value1;
					(*i_row_feats).push_back(one_feat);
				}
			}


			// Bi-gram features
			for (int rel_pos = LHS_CS + 1; rel_pos <= RHS_CS; ++rel_pos) {

				// Unnormalized  uni-gram lemma features
				feat_value1 = get_item(one_sent, i_row, COL_INFO.LEMMA, rel_pos - 1);	
				feat_value2 = get_item(one_sent, i_row, COL_INFO.LEMMA, rel_pos);

				if ((feat_value1 != "NULL") || (feat_value2 != "NULL")) {			
					feat_head = "L_B_" + int2strIDX(rel_pos - 1) + int2strIDX(rel_pos) + "=";	
					one_feat = feat_head + feat_value1 + "/" + feat_value2;				
					(*i_row_feats).push_back(one_feat);

					// Normalized  uni-gram lemma features
					feat_nor_value1 = squeeze_nums(feat_value1);							
					feat_nor_value2 = squeeze_nums(feat_value2);

					feat_head = "L_NB_" + int2strIDX(rel_pos - 1) + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_nor_value1 + "/" + feat_nor_value2;
					(*i_row_feats).push_back(one_feat);
				}
			}

			i_row_feats++;				// Increase row feature iterator
		}
	}


	void FeatureExtractor::ext_ORTHO_feats(const V2_STR &one_sent, V2_STR &sent_feats)	
	{
		bool					decision = true;
		string					one_feat= "", cur_word = "", str_tmp = "", O_NO_LOWER_LETTERS = "";
		string::size_type		beg_pos = 0, cur_pos = 0;	
		vector<string>			v_segment;

		string::iterator				s_itr;
		V2_STR::iterator				i_row_feats = sent_feats.begin();
		set<string>::const_iterator		grk_itr;


		for (V2_STR::const_iterator i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row)			// Features based on Mr. Ki-Joong Lee(2003)'s paper (09/07/15)
		{
			cur_word = get_item(one_sent, i_row, COL_INFO.WORD, 0);			// Get the current word


			// Contain a capital letter as the first letter (except the first word of a sentence)
			if (i_row == one_sent.begin())
				one_feat = "O_BEG_CAP=B";
			else if (('A' <= cur_word[0]) && (cur_word[0] <= 'Z'))
				one_feat = "O_BEG_CAP=O";
			else
				one_feat = "O_BEG_CAP=X";
			(*i_row_feats).push_back(one_feat);

			// Contain only digits 
			decision = true;
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if (((*s_itr) < '0') || ((*s_itr) > '9')) {
					decision = false; 
					break; 
				}
			}
			if (decision) {
				one_feat = "O_ONLY_DIGITS";
				(*i_row_feats).push_back(one_feat);
			}

			// Contain digits 
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if (((*s_itr) >= '0') && ((*s_itr) <= '9')) {
					one_feat = "O_SOME_DIGITS";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain onlyalphanumeric character(s)
			bool alpha = false, numeric = false, others = false;
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ( ((*s_itr) >= '0') && ((*s_itr) <= '9') ) {
					numeric = true;
				}else if ( ( ((*s_itr) >= 'A') && ((*s_itr) <= 'Z') ) || ( ((*s_itr) >= 'a') && ((*s_itr) <= 'z') ) ) {
					alpha = true;
				}else {
					others = true;
				}
			}
			if ((numeric == true) && (alpha == true) && (others == false)) {
				one_feat = "O_ONLY_ALPHANUMERIC";
				(*i_row_feats).push_back(one_feat);
			}

			// Contain only capital letters and digits
			decision = true;
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ( (((*s_itr) < '0') || ((*s_itr) > '9')) && (((*s_itr) < 'A') || ((*s_itr) > 'Z')) ) {
					decision = false;
					break; 
				}
			}
			if (decision) {
				one_feat = "O_ONLY_CAPnDIGITS";
				(*i_row_feats).push_back(one_feat);
			}

			// Contain no lower letters
			decision = true;
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if (((*s_itr) >= 'a') && ((*s_itr) <= 'z')) {
					decision = false;
					break;
				}
			}
			if (decision) {
				one_feat = "O_NO_LOWER_LETTERS";
				(*i_row_feats).push_back(one_feat);
				O_NO_LOWER_LETTERS = "O";					// For a different feature
			}else {
				O_NO_LOWER_LETTERS = "X";
			}


			// Contain all capital letters
			decision = true;
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ( ((*s_itr) < 'A' ) || ((*s_itr) > 'Z' )) {
					decision = false;
					break; 
				}
			}
			if (decision) {
				one_feat = "O_ONLY_CAPS";
				(*i_row_feats).push_back(one_feat);
			}

			// Contain capital letter(s) which is not the first letter
			if (cur_word.length() > 1) {
				for (s_itr = cur_word.begin() + 1; s_itr != cur_word.end(); ++s_itr) {
					if ( ((*s_itr) >= 'A' ) && ((*s_itr) <= 'Z' )) {
						one_feat = "O_IN_CAP";
						(*i_row_feats).push_back(one_feat);
						break; 
					}
				}
			}

			// Contain two consecutive capital letters
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ( ((*s_itr) >= 'A' ) && ((*s_itr) <= 'Z' )) {
					if ( ((s_itr+1) != cur_word.end()) && (((*(s_itr+1)) >= 'A' ) && ((*(s_itr+1)) <= 'Z' )) ) {
						one_feat = "O_2_IN_CAPS";
						(*i_row_feats).push_back(one_feat);      
						break; 
					}
				}
			}

			// Contain a greek word as a substring
			one_feat = "O_GREEK_A2Z=";
			beg_pos = 0, cur_pos = 0;	
			str_tmp = "";
			v_segment.clear();

			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr)			// Change a word into a lower-case word
				str_tmp += tolower(*s_itr);

			for (; cur_pos < str_tmp.length(); ++cur_pos) {										// Segment the word based on non-alphabetic characters
				if ( (str_tmp[cur_pos] < 'a') || ('z' < str_tmp[cur_pos]) ) { 
					if (beg_pos == cur_pos) {
						++beg_pos;
					}else {
						v_segment.push_back(str_tmp.substr(beg_pos, cur_pos - beg_pos));
						beg_pos = cur_pos + 1;				
					}
				}
			}
			if (beg_pos != cur_pos) {
				v_segment.push_back(str_tmp.substr(beg_pos, cur_pos - beg_pos));
			}

			for (vector<string>::iterator v_itr = v_segment.begin(); v_itr != v_segment.end(); ++v_itr)		// search greek words
			{
				if ((grk_itr = greek_alphabets.find(*v_itr)) != greek_alphabets.end()) {							// TODO: combined them? or put them separately?
					one_feat += *grk_itr;
					one_feat += "/";
				}
			}
			if (one_feat.length() != 12) {
				(*i_row_feats).push_back(one_feat);
			}

			// Contain comma(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == ',') {
					one_feat = "O_COMMAS";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain period(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == '.') {
					one_feat = "O_PERIODS";
					(*i_row_feats).push_back(one_feat);    
					break; 
				}
			}

			// Contain hyphon(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == '-') {
					one_feat = "O_HYPHONS";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain slash(es)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == '/') {
					one_feat = "O_SLASHES";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain open square bracket(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == '[') {
					one_feat = "O_OpenSB";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}


			// Contain close square bracket(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == ']') {
					one_feat = "O_CloseSB";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain open parentheses
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == '(') {
					one_feat = "O_OpenP";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain close parentheses
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == ')') {
					one_feat = "O_CloseP";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain colon(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == ':') {
					one_feat = "O_COLONS";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain semi-colon(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == ';') {
					one_feat = "O_SemiCOLONS";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain percentage symbol(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == '%') {
					one_feat = "O_PERCENTAGE";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// Contain apostrophe(s)
			for (s_itr = cur_word.begin(); s_itr != cur_word.end(); ++s_itr) {
				if ((*s_itr) == '\'') {
					one_feat = "O_APOSTROPHE";
					(*i_row_feats).push_back(one_feat);
					break; 
				}
			}

			// The length of the current word
			one_feat = "O_LEN=" + int2str((int) cur_word.length());
			(*i_row_feats).push_back(one_feat);

			// A combined feature - the length of the word & O_CAPnDIGITS
			one_feat = "O_LEN_n_NO_LOW_LETTERS=" + int2str((int) cur_word.length()) + "/" + O_NO_LOWER_LETTERS;
			(*i_row_feats).push_back(one_feat);


			i_row_feats++;
		}

	}


	void FeatureExtractor::ext_POS_feats(const V2_STR &one_sent, V2_STR &sent_feats)
	{
		string					one_feat= "", feat_head = "";
		string					feat_value1 = "", feat_value2 = "";
		V2_STR::iterator		i_row_feats = sent_feats.begin(), i_tmp_row;


		for (V2_STR_citr i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) 
		{
			// Uni-gram features
			for (int rel_pos = LHS_CS; rel_pos <= RHS_CS; ++rel_pos) {
				feat_value1 = get_item(one_sent, i_row, COL_INFO.POS, rel_pos);

				if (feat_value1 != "NULL") {
					feat_head = "P_U_" + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_value1;

					(*i_row_feats).push_back(one_feat);
				}
			}


			// Bi-gram features
			for (int rel_pos = LHS_CS + 1; rel_pos <= RHS_CS; ++rel_pos) {
				feat_value1 = get_item(one_sent, i_row, COL_INFO.POS, rel_pos - 1);
				feat_value2 = get_item(one_sent, i_row, COL_INFO.POS, rel_pos);

				if ((feat_value1 != "NULL") || (feat_value2 != "NULL")) {
					feat_head = "P_B_" + int2strIDX(rel_pos - 1) + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_value1 + "/" + feat_value2;

					(*i_row_feats).push_back(one_feat);
				}
			}

			i_row_feats++;					// Increase row feature iterator
		}
	}


	void FeatureExtractor::ext_LEMMA_POS_feats(const V2_STR &one_sent, V2_STR &sent_feats)
	{
		string					one_feat= "", feat_head = "";
		string					feat_lemma_value1 = "", feat_lemma_value2 = "", feat_POS_value1 = "", feat_POS_value2 = "";
		V2_STR::iterator		i_row_feats = sent_feats.begin(), i_tmp_row;


		for (V2_STR_citr i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) 
		{
			// Uni-gram features
			for (int rel_pos = LHS_CS; rel_pos <= RHS_CS; ++rel_pos) {
				feat_lemma_value1 = get_item(one_sent, i_row, COL_INFO.LEMMA, rel_pos);

				if (feat_lemma_value1 != "NULL") {
					feat_POS_value1 = get_item(one_sent, i_row, COL_INFO.POS, rel_pos);

					feat_head = "LP_U_" + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_lemma_value1 + "_" + feat_POS_value1;

					(*i_row_feats).push_back(one_feat);
				}
			}


			// Bi-gram features
			for (int rel_pos = LHS_CS + 1; rel_pos <= RHS_CS; ++rel_pos) {
				feat_lemma_value1 = get_item(one_sent, i_row, COL_INFO.LEMMA, rel_pos - 1);
				feat_lemma_value2 = get_item(one_sent, i_row, COL_INFO.LEMMA, rel_pos);

				if ((feat_lemma_value1 != "NULL") || (feat_lemma_value2 != "NULL")) {
					feat_POS_value1 = get_item(one_sent, i_row, COL_INFO.POS, rel_pos - 1);
					feat_POS_value2 = get_item(one_sent, i_row, COL_INFO.POS, rel_pos);

					feat_head = "LP_B_" + int2strIDX(rel_pos - 1) + int2strIDX(rel_pos) + "=";
					one_feat = feat_head + feat_lemma_value1 + "_" + feat_POS_value1 + "/" + feat_lemma_value2 + "_" + feat_POS_value2;

					(*i_row_feats).push_back(one_feat);
				}
			}


			i_row_feats++;				// Increase row feature iterator
		}
	}


	void FeatureExtractor::ext_CHUNK_feats(const V2_STR &one_sent, V2_STR &sent_feats)
	{
		string								one_feat= "", last_word ="", last_lemma = "", tmp_str = "";
		V2_STR::iterator					i_row_feats = sent_feats.begin();
		pair<V2_STR_citr, V2_STR_citr>		chk_range1, chk_range2;


		for (V2_STR::const_iterator i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) 
		{
			one_feat = "CH_TYPE=";							// Chunk type of a current word
			one_feat += (*i_row)[ COL_INFO.CHUNK ];
			(*i_row_feats).push_back(one_feat);

			one_feat = "CH_C_LW=";							// The last word of the chunk where a current belongs
			last_word = (*i_row)[ COL_INFO.WORD ];							// Last word
			last_lemma = (*i_row)[ COL_INFO.LEMMA ];						// Last lemma
			if ((*i_row)[ COL_INFO.CHUNK ] != "O") {
				for (int idx = 1; (i_row + idx) != one_sent.end(); idx++)
				{
					if ((*(i_row + idx))[ COL_INFO.CHUNK ][0] != 'I') {
						break;
					}else {
						last_word = (*(i_row + idx))[ COL_INFO.WORD ];
						last_lemma= (*(i_row + idx))[ COL_INFO.LEMMA ];
					}
				}
			}
			one_feat += last_word;
			(*i_row_feats).push_back(one_feat);

			one_feat = "CH_C_LL=";
			one_feat += last_lemma;
			(*i_row_feats).push_back(one_feat);

			/* Lower the performance	
			one_feat = "CH_L1_LW=";							// Last word/lemma of the left chunk
			find_chunk_range(one_sent, i_row, COL_INFO, chk_range1);
			if (chk_range1.first != one_sent.begin()) {
			find_chunk_range(one_sent, chk_range1.first - 1, COL_INFO, chk_range2);
			one_feat += (*(chk_range2.second - 1))[ COL_INFO.WORD ];
			(*i_row_feats).push_back(one_feat);

			one_feat = "CH_L1_LL=" + (*(chk_range2.second - 1))[ COL_INFO.LEMMA ];
			(*i_row_feats).push_back(one_feat);
			}

			one_feat = "CH_R1_LW=";							// Last word/lemma of the right chunk
			find_chunk_range(one_sent, i_row, COL_INFO, chk_range1);
			if (chk_range1.second != one_sent.end()) {
			find_chunk_range(one_sent, chk_range1.second, COL_INFO, chk_range2);
			one_feat += (*(chk_range2.second - 1))[ COL_INFO.WORD ];
			(*i_row_feats).push_back(one_feat);

			one_feat = "CH_R1_LL=" + (*(chk_range2.second - 1))[ COL_INFO.LEMMA ];
			(*i_row_feats).push_back(one_feat);
			}
			*/


			// Exact word features
			find_chunk_range(one_sent, i_row, chk_range1);				// 1) Word "the" in the left context

			for (V2_STR_citr itr = chk_range1.first; itr < i_row; ++itr) {				// Search the left context only
				tmp_str = (*itr)[ COL_INFO.WORD ];
				make_lowercase(tmp_str);

				if (tmp_str == "the") {
					one_feat = "CH_EXACT=THE_in_LHS";
					(*i_row_feats).push_back(one_feat);

					break;
				}
			}


			i_row_feats++;
		}
	}


	void FeatureExtractor::ext_DIC_feats(const V2_STR &one_sent, V2_STR &sent_feats, int opt_dic)
	{
		int						end_COL_DIC = 0, k_dics = 0;
		string					one_feat= "", feat_value1 = "", feat_value2 = "", feat_word_value1 = "", feat_word_value2 = "";
		V2_STR::iterator		i_row_feats = sent_feats.begin(), beg, end;


		for (V2_STR::const_iterator i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) 
		{
			// The number of dictionaries that includes the current token
			end_COL_DIC = ((*i_row)).size();		
			k_dics = 0;
			for (int kth_COL_DIC = COL_INFO.DIC; kth_COL_DIC < end_COL_DIC; ++kth_COL_DIC) {
				if ( (*i_row)[ kth_COL_DIC ] != "O") {
					k_dics++;
				}			 
			}
			if( COL_INFO.DIC < end_COL_DIC ) {
				one_feat = "K_DICS=" + int2str(k_dics);
				(*i_row_feats).push_back(one_feat);
			}

			// Uni/Bi-gram dictionary matching features for all dictionaries
			for (int kth_COL_DIC = COL_INFO.DIC; kth_COL_DIC < end_COL_DIC; ++kth_COL_DIC) {
				// Uni-gram features
				for (int rel_pos = LHS_CS; rel_pos <= RHS_CS; ++rel_pos) {
					feat_value1 = get_item(one_sent, i_row, kth_COL_DIC, rel_pos);
					feat_word_value1 = get_item(one_sent, i_row, COL_INFO.WORD, rel_pos); 

					if ((feat_value1 != "O" ) && (feat_value1 != "NULL")) {
						// Dictionary hit uni-gram feature
						one_feat = "DIC" + int2str((int) kth_COL_DIC - COL_INFO.DIC) + "_U_" + int2strIDX(rel_pos) + "=" + feat_value1;
						(*i_row_feats).push_back(one_feat);

						// Lexicalized dictionary hit feature
						if (opt_dic == 0) {
							one_feat = "DIC" + int2str((int) kth_COL_DIC - COL_INFO.DIC) + "_LU_" + int2strIDX(rel_pos) + "="
								+ feat_value1 + "_" + feat_word_value1;
							(*i_row_feats).push_back(one_feat);
						}
					}
				}

				// Bi-gram features
				for (int rel_pos = LHS_CS + 1; rel_pos <= RHS_CS; ++rel_pos) {
					feat_value1 = get_item(one_sent, i_row, kth_COL_DIC, rel_pos - 1);
					feat_value2 = get_item(one_sent, i_row, kth_COL_DIC, rel_pos);
					feat_word_value1 = get_item(one_sent, i_row, COL_INFO.WORD, rel_pos - 1);
					feat_word_value2 = get_item(one_sent, i_row, COL_INFO.WORD, rel_pos);

					if ( ((feat_value1 != "O") && (feat_value1 != "NULL")) || 
						((feat_value2 != "O") && (feat_value2 != "NULL")) ) {					
							// Dictionary hit bi-gram feature
							one_feat = "DIC" + int2str((int) kth_COL_DIC - COL_INFO.DIC) + "_B_" + int2strIDX(rel_pos - 1) 
								+ int2strIDX(rel_pos) + "=" + feat_value1 + "/" + feat_value2;
							(*i_row_feats).push_back(one_feat);

							// Lexicalized dictionary hit feautre
							if (opt_dic == 0) {
								one_feat = "DIC" + int2str((int) kth_COL_DIC - COL_INFO.DIC) + "_LB_" + int2strIDX(rel_pos - 1) + int2strIDX(rel_pos) 
									+ "=" + feat_value1 + "_" + feat_word_value1 + "/" + feat_value2 + "_" + feat_word_value2;
								(*i_row_feats).push_back(one_feat);
							}
					}
				}
			}


			i_row_feats++;
		}
	}



	//
	// Private member functions 
	//

	void FeatureExtractor::get_n_grams(const string &token, const int n, vector<string> &ngrams)
	{
		ngrams.clear();

		string						seq = "";
		vector<string>::iterator	new_end;


		/** Symbols in an input sentence will be tokenized as one-byte-per-one-token
		*   Therefore, any kinds of symbols can be used as beg/end indicators
		*/
		for (int i = 1; i < n; ++i)					
			seq += "$";
		seq += token;
		for (int i = 1; i < n; ++i)					
			seq += "$";

		int end_pos = (int) seq.length() - (n - 1);

		for (int pos = 0; pos < end_pos; ++pos)
			ngrams.push_back(seq.substr(pos, n));

		// Make them unique
		sort(ngrams.begin(), ngrams.end());

		new_end= unique(ngrams.begin(), ngrams.end());
		ngrams.resize(new_end - ngrams.begin());
	}

	string FeatureExtractor::get_item(
		const V2_STR		&one_sent, 
		const V2_STR_citr	&i_row,						// Current token
		const int			col,						// Column number (from 0) for word, lemma, POS, chunk, ...
		const int			rel_pos						// Relative position from i_row
		)
	{
		string	ret_word = "NULL";

		if (rel_pos == 0) {
			ret_word = (*i_row)[ col ];
		}else if (rel_pos < 0) {
			if (distance(i_row, one_sent.begin()) <= rel_pos)		// begin(), B, (cur), D, E, ... => distance(cur, begin()) = -2
				ret_word = (*(i_row + rel_pos))[ col ];
		}else {
			if (distance(i_row, one_sent.end()) > rel_pos  )		// D, E, (cur), G, H, end() => distance(cur, end()) = 3, but only rel_pos = 1 and 2 is accessible
				ret_word = (*(i_row + rel_pos))[ col ];
		}

		return ret_word;
	}

	void FeatureExtractor::find_chunk_range(
		const V2_STR					&one_sent,
		const V2_STR_citr				&i_row, 
		pair<V2_STR_citr, V2_STR_citr>	&chk_range)
	{
		V2_STR_citr itr;
		string	chk_bio = "";

		if ((*i_row)[COL_INFO.CHUNK] == "O") {
			chk_range.first = i_row;
			chk_range.second = i_row + 1;
			return;
		}

		for (itr = i_row; itr >= one_sent.begin(); --itr) {
			chk_bio = (*itr)[COL_INFO.CHUNK].substr(0, 1);

			if (chk_bio == "B" ) {			// O-tag word will be handled above 
				chk_range.first = itr;
				break;
			}
		}

		chk_range.second = one_sent.end();							// Set the initial value since the current token may belong to the last chunk
		for (itr = i_row + 1; itr != one_sent.end(); ++itr) {
			chk_bio = (*itr)[COL_INFO.CHUNK].substr(0, 1);

			if ( (chk_bio == "B") || (chk_bio == "O") )  {
				chk_range.second = itr;
				break;
			}
		}
	}
}
