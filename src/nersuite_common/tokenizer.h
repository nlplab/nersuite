/*
*      A sentence tokenizer class with a user-defined delimiters
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
#ifndef _NERSUITE_TOKENIZER_H
#define _NERSUITE_TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

namespace NER
{
	typedef	std::vector< std::string >	V1_STR;
	typedef	std::vector< V1_STR >		V2_STR;

	/** 
	* @ingroup NERsuite
	* Tokenizer Utility
	*/
	class Tokenizer
	{
	private:
		int splitter( const std::string &trimmed_sent, V2_STR &data );
		void mark_pos( const std::string &raw_sent, V2_STR &data, size_t init_offset = 0 );

		std::string trim_ws( const std::string &raw_sent );
		size_t find_token_end( const std::string &trimmed_sent, const size_t beg );

	public:
		/** Tokenize input text and generate the (begin_pos, end_pos, token) triplet for each token.
		* @param[in] raw_sent Input sentence
		* @param[in,out] data A vector to store the result
		* @param[in] init_offset The offset value added to all begin_pos
		* @returns the count of tokens generated.
		*/
		int tokenize( const std::string &raw_sent, V2_STR &data, size_t init_offset = 0 );

	};
}

#endif
