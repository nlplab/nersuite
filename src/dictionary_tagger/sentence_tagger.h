/*
*      SentenceTagger matcher
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
#ifndef _SENTENCE_TAGGER_H
#define _SENTENCE_TAGGER_H

#include <vector>
#include <string>
#include <iostream>
#include "../nersuite_common/dictionary.h"
#include "../nersuite_common/ne.h"

// Predefined column info.
#define		BEG_COL			0
#define		END_COL			1
#define		RAW_TOKEN_COL	2
#define		POS_COL			4

namespace NER
{
	/** 
	* @ingroup NERsuite
	*/

	/**
	* Sentence Tagger - internal class used to represent the sentence-token-feature array structure
	*
	* This class has two main functionarities: reading a sentence to construct the structure,
	* and appending Dictionary-class features to the structure.
	* 
	* This class reads a sentence block from a stream and creates a list of tokens. 
	* Each token consists of a list of features.
	* After reading a sentence, the function @ref tag_nes() will be used to append
	* extra features which are retrieved from the given Dictionary.
	*/
	class SentenceTagger
	{
	private:
		typedef std::vector< std::vector<std::string> >	V2_STR;

		static size_t	max_ne_len;
		static bool		filter_NN;
		static int		normalize_type;

		// Sentence Data (Tokenized array)
		V2_STR	m_Content;

		std::vector<NE>		v_ne;

		std::vector<int>	v_idx;

		bool		document_separator_seen;

	public:
		/**
		* Constructs a SentenceTagger object
		*/
		SentenceTagger();

		/**
		* Destroys a SentenceTagger object
		*/
		virtual ~SentenceTagger() {}

		/**
		* Set Normalization Type used while a SentenceTagger queries the dictionary
		* @param[in] nt A combination of Normalization Types (OR of NormalizeType).
		*/
		static void set_normalize_type(int nt) { normalize_type = nt; }

		/**
		* Get the size of sentence which this object is currentry processing.
		* @return Returns the size of sentence (Count of tokens).
		*/
		size_t	size() const { return m_Content.size(); }

		/**
		* Test if the token list is empty.
		* This function can be used to test whether the input read was an empty line.
		* @return Returns true if empty.
		*/
		bool	empty() const { return m_Content.empty(); }

		/**
		* Test if a document separator was seen on input.
		* @return Returns true if a document separator was seen.
		*/
		bool	doc_end() const { return document_separator_seen; }

		/**
		* The begin() iterator for the internal token list
		*/
		V2_STR::iterator	begin() { return m_Content.begin(); }

		/**
		* The end() iterator for the internal token list
		*/
		V2_STR::iterator	end() { return m_Content.end(); }

		/**
		* The array indexer operator for the internal token list
		*/
		V1_STR&	operator[](size_t index) { return m_Content[index]; }

		/**
		* Read a sentence from the given stream and create the internal token list.
		* @param[in] multidoc_separator String marking document break (if non-empty)
		* @return Size of tokens read
		*/
		size_t	read(std::istream &ifs, const std::string &multidoc_separator="");

		/**
		* Append dictionary-class features to the internal token list.
		* @param[in] dict The Dictionary used to search for feature classes.
		*/
		void	tag_nes(const Dictionary& dict);

	private:
		void	resolve_collision();

		void	mark_ne(const Dictionary& dict);

		int		find_longest(size_t i_row, NE& ne, const Dictionary& dict) const;

		int		find_exact(size_t i_row, NE& ne, const Dictionary& dict) const;

		size_t	find_min_length(size_t i_row) const;
	};
}

#endif
