/*
*      NERSuite
*      Text loader
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

#ifndef		_TEXT_LOADER_H
#define		_TEXT_LOADER_H

#include <iostream>
#include <fstream>
#include <istream>

#include <string>
#include <vector>

typedef		std::vector< std::vector<std::string> >		V2_STR;

namespace NER
{
	/** 
	* @ingroup NERsuite
	*/
	/**
	* Read a sentence from a stream and construct Token List for the sentence.
	*
	* Each line consists of tab-separated columns (label + feature list).
	* First column is the label assigned for the feature list.
	* The following columns are the feature list.
	*
	* @param[in] cin Input stream (Each sentence must be separated with an empty line.)
	* @param[in,out] one_sent Token List for a sentence.
	*  Any existing contents are cleared before processing.
	* @returns count of tokens in the sentence
	* 
	*/
	extern int get_sent(std::istream &cin, V2_STR &one_sent);

	/** 
	* @ingroup NERsuite
	*/
	/**
	* Read a sentence from a stream and construct Token List for the sentence.
	*
	* Each line consists of tab-separated columns (label + feature list).
	* First column is the label assigned for the feature list.
	* The following columns are the feature list.
	* Accepts also lines containing only the given document break marker.
	*
	* @param[in] cin Input stream (Each sentence must be separated with an empty line.)
	* @param[in,out] one_sent Token List for a sentence.
	*  Any existing contents are cleared before processing.
	* @param[in] multidoc_separator String marking document break.
	* @param[out] separator_read Flag marking whether a document break was read.
	* @returns count of tokens in the sentence
	* 
	*/
	extern int get_sent(std::istream &cin, V2_STR &one_sent, const std::string &multidoc_separator, bool &separator_read);

	/** 
	* @ingroup NERsuite
	*/
	/**
	* Read a sentence from a file stream and construct Token List for the sentence.
	*
	* Each line consists of tab-separated columns (label + feature list).
	* First column is the label assigned for the feature list.
	* The following columns are the feature list.
	*
	* @param[in] cin Input stream (Each sentence must be separated with an empty line.)
	* @param[in,out] one_sent Token List for a sentence.
	*  Any existing contents are cleared before processing.
	* @returns count of tokens in the sentence
	* 
	*/
	extern int get_sent(std::ifstream &cin, V2_STR &one_sent);
}

#endif


