/*
*      NERSuite class header
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


#ifndef    _NERSUITE_H
#define    _NERSUITE_H


#include <iostream>
#include <map>
#include <list>
#include <stdio.h>
#include <string.h>

// BEGIN: utils
#include "../nersuite_common/string_utils.h"
#include "../nersuite_common/text_loader.h"
#include "../nersuite_common/option_parser.h"
// END: utils

// BEGIN: feature extractor
#include "typedefs.h"
#include "FExtor.h"
// END: feature extractor

// BEGIN: CRFSuite wrapping
#include <stdlib.h>
#include <string>
#include <time.h>
#include "crfsuite2.h"

typedef         double						floatval_t;
typedef         std::vector<std::string>	V1_STR;
typedef         std::vector< V1_STR >		V2_STR;
typedef         std::vector< V2_STR >		V3_STR;

#define MODE_LEARN	"learn"
#define MODE_TAG	"tag"
#define DEFAULT_MODEL_FILE	"model.m"


namespace NER
{
	/** 
	* @defgroup NERsuite
	* @brief This is the main module of NERsuite
	* 
	* The following diagram shows the overall structure of NERsuite.
	* The components "nersuite", "nersuite_dic_compiler", "nersuite_dic_tagger",
	* and "nersuite_tokenizer" are command executables. These executables require
	* the library "nersuite_common".
	* 
	* "nersuite" requires the external library
	* "CRFsuite" (and "cqdb" which is then required by "CRFsuite") as a classifier.
	* 
	* "nersuite_common" requires the internal library "cdbpp".
	* 
	* @image html nersuite.png "Component Diagram of NERsuite"
	*/

	/**
	* @ingroup NERsuite
	* NERsuite Main Functionarities
	*/
	class Suite
	{
	private:
		OPTION_PARSER opt_parser;
		COLUMN_INFO	COL_INFO;

	public:
		/** Construct an NER Suite object
		*/
		Suite(int nargs, char** args);

		/** Perform the Learning Process
		* @returns 0 if success
		*/
		int learn();

		/** Perform the Tagging Process
		* @returns 0 if success
		*/
		int tag();

	private:
		int learn_crfsuite(std::istream &is);
		void read_data(std::istream &is, const COLUMN_INFO &COL_INFO, CRFSuite::Trainer* trainer);

		int run_tagging(std::istream &is, std::ostream &os, CRFSuite::Tagger& tagger, FeatureExtractor &FExtor);
		int tag_crfsuite(V2_STR &one_sent, V2_STR &sent_feat, CRFSuite::Tagger& tagger, std::map<std::string, int> &term_idx, std::ostream &os);
		void output_result_standoff(std::ostream &os, CRFSuite::StringList& yseq, std::vector<std::vector<std::string> > &one_sent, std::map<std::string, int> &term_idx);
		void output_result_conll(std::ostream &os, CRFSuite::StringList& yseq, std::vector<std::vector<std::string> > &one_sent);

		void set_column_info(const std::string &mode);
		int pad_answer(const std::string &mode, const V2_STR &one_sent, V2_STR &sent_feats);
	};
}

/**
@mainpage NERsuite - Named Entity Recognition Toolkit

NERsuite is a set of programs used for Named Entity recognition based on the CRF (Conditional Random Fields) algorithm.
This manual explains its implementation details.
<br />
<br />
To start with, please visit the Main Module page: @ref NERsuite.
<br />
<br />
For usage and downloads of the program, please refer to the following page: 
<br />
<a href="www-tsujii.is.s.u-tokyo.ac.jp/nersuite/">NERsuite - a Named Entity Recognition toolkit - Tsujii Laboratory</a>
*/
#endif


