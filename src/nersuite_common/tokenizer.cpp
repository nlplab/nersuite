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
#include "tokenizer.h"

using namespace std;

namespace NER
{
	int Tokenizer::tokenize( const string &raw_sent, V2_STR &data, const size_t init_offset ) 
	{
		string	trimmed_sent = trim_ws( raw_sent );		

		if( trimmed_sent == "") {
			return 0;
		}else {
			int	n_tokens = splitter( trimmed_sent, data );
			mark_pos( raw_sent, data, init_offset );

			return	n_tokens;
		}
	}

	int Tokenizer::splitter( const string &trimmed_sent, V2_STR &data )
	{
		size_t	beg = 0, end = find_token_end( trimmed_sent, 0 ), n_tokens = 0;

		V1_STR	one_row;
		one_row.push_back( "token" );		

		while( end != string::npos ) {
			string	token = trimmed_sent.substr( beg, end - beg );

			if( token != " " ) {				// Put all tokens except a space token into the 'data' container
				one_row.back() = token;
				data.push_back( one_row );
				++n_tokens;
			}

			beg = end;
			end = find_token_end( trimmed_sent, beg );
		}

		return n_tokens;
	}

	void Tokenizer::mark_pos( const string &raw_sent, V2_STR &data, const size_t init_offset ) 
	{
		size_t	beg = 0, end = 0;
		char	chr_pos[128];

		for( V2_STR::iterator i_row = data.begin(); i_row != data.end(); ++i_row ) {
			beg = init_offset + raw_sent.find_first_of( *(i_row->end() - 1), end);
			end = beg + (i_row->end() - 1)->length();

			sprintf(chr_pos, "%d", beg);
			i_row->insert( i_row->end() - 1, chr_pos );

			sprintf(chr_pos, "%d", end);
			i_row->insert( i_row->end() - 1, chr_pos );
		}	
	}


	// Trim left and right hand side spaces, and squeeze all continuous spaces into a single space within a sentence
	string Tokenizer::trim_ws( const string &raw_sent )
	{
		string	trimmed_sent = raw_sent;
		int	idx = 0;

		// 1. Trim LHS spaces
		int	sent_len = (int) trimmed_sent.length();
		for(; idx < sent_len; ++idx) {
			if(trimmed_sent[ idx ] != ' ')
				break;
		}
		if(idx == trimmed_sent.length()) {			// If a string has only space(s)
			trimmed_sent = "";
		}else {
			trimmed_sent = trimmed_sent.substr(idx, string::npos);
		}

		// 2. Trim RHS spaces
		sent_len = (int) trimmed_sent.length();
		if(sent_len > 0) {
			for(idx = sent_len - 1; idx >= 0; --idx) {
				if(trimmed_sent[ idx ] != ' ')
					break;
			}
			trimmed_sent = trimmed_sent.substr(0, sent_len + 1);
		}

		return trimmed_sent;
	}

	// Find the past the end position for a next token
	size_t Tokenizer::find_token_end( const string &trimmed_sent, const size_t beg )
	{
		size_t	max = trimmed_sent.length();

		if( beg == max ) {										// Case 1. all characters are processed
			return string::npos;
		}else if( ( trimmed_sent[ beg ] == ' ' ) || ( isalnum( trimmed_sent[ beg ] ) == 0 ) ) {		// Case 2. the current character is a delimiter character
			return beg + 1;
		}else {												// Case 3. find the position of a next delimiter character
			for( size_t i = beg; i < max; ++i )
				if( ( trimmed_sent[ i ] == ' ' ) || ( isalnum( trimmed_sent[ i ] ) == 0 ) )
					return i;
			return max;
		}
	}

}

