/*
*      dictionary_tagger main
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
#include <list>
#include <iostream>
#include <exception>
#include <algorithm>
#include "../nersuite_common/dictionary.h"
#include "../nersuite_common/option_parser.h"
#include "../nersuite_common/text_loader.h"
#include "../nersuite_common/string_utils.h"
#include "../nersuite_common/nersuite_exception.h"
#include "sentence_tagger.h"

using namespace std;

void print(vector<string> &item);

void print_usage()
{
	cerr <<
		"usage: nersuite_dic_tagger [options] CDB++_DB_filename < standard input \n"
		"    Options:\n"
		"    -n <type_of_normalization> : Normalization type for dictionary matching\n"
		"       can be \"none\" or any combination of \"c\", \"n\", \"s\", \"t\".\n"
		"       Defaults to normalization used when compiling DB (recommended).\n"
		"       none: No normalization\n"
		"       c: Case insensitive (convert all letters to lowercase)\n"
		"       cn: Case AND Number insensitive (convert all numbers to \'0\')\n"
		"       cns: Case AND Number AND Symbol insensitive (convert all symbols to \'_\')\n"
		"       t: Use token-base matching\n"
		"\n"
		"    -multidoc SEPARATOR : look for lines beginning with the separator string \n"
		"       SEPARATOR in input and echo the same on output.\n"
		"\n"
		"    Line format of the input file: \n"
		"         [1st col.] - the byte position of the first letter of a token. \n"
		"         [2nd col.] - the byte position one past the last letter of a token. \n"
		"         [3rd col.] - raw token\n"
		"         [4th col.] - lemma\n"
		"         [5th col.] - POS tag\n"
		"         [6th col.] - chunk tag\n"
		"         [7th ... ] - any attributes\n";
}

int main(int argc, char* argv[])
{
	NER::OPTION_PARSER opt_parser;
	opt_parser.parse(argc, argv);
	const vector<const char*>& args = opt_parser.get_args();
	if (args.size() < 2)
	{
		print_usage();
		exit(1);
	}
	int		normalize_type = NER::NormalizationUnknown;
	string	normalize_option;
	if (opt_parser.get_value("-n", normalize_option))
	{
		normalize_type = NER::NormalizeNone;

		if (normalize_option == "none")
		{
			// do nothing
		}
		else
		{
			if (normalize_option.find('c') != string::npos)
			{
				normalize_type |= NER::NormalizeCase;
			}
			if (normalize_option.find('n') != string::npos)
			{
				normalize_type |= NER::NormalizeNumber;
			}
			if (normalize_option.find('s') != string::npos)
			{
				normalize_type |= NER::NormalizeSymbol;
			}
			if (normalize_option.find('t') != string::npos)
			{
				normalize_type |= NER::NormalizeToken;
			}
		}
	}

	string	multidoc_separator = "";
	bool	multidoc_mode = opt_parser.get_value("-multidoc", multidoc_separator);

	try
	{
		NER::SentenceTagger::set_normalize_type(normalize_type);

		NER::Dictionary dict(args[1]);
		dict.open();

		// If no normalization specified, set according to DB
		if ( normalize_type == NER::NormalizationUnknown )
		{
			normalize_type = dict.get_normalization_type();
		}

		// Check for mismatch in normalization type
		if ( dict.get_normalization_type() != NER::NormalizationUnknown &&
		     dict.get_normalization_type() != normalize_type)
		{
			std::cerr << "Warning: given normalization (" << normalize_type << ") does not match DB normalization (" << dict.get_normalization_type() << "). Tagging performance may be decreased.\n";
		}
		

		// Tag input with a dictionary
		NER::SentenceTagger	one_sent;
		
		while (! cin.eof()) 
		{
			// 1. Skip if it is a blank line
			if( one_sent.read(cin, multidoc_separator) == 0 ) {
				continue;
			}
			
			// 2. Print comment lines
			if( multidoc_mode && one_sent.get_content_type() == 1 ) {
				for( V2_STR::iterator irow = one_sent.begin(); irow != one_sent.end(); ++ irow) {
					cout << irow->front() << endl;     // Comment is stored as a string at [0] position
				}
				cout << endl;
				
				continue;
			}
		  
			// 3. Tag a sentence
			one_sent.tag_nes(dict);     // Find the best NE candidate at the beginning of each word in a sentence

			// 4. Print the output
			if (!one_sent.empty())
			{
				for_each(one_sent.begin(), one_sent.end(), print);
				cout << endl;
			}
		}
	}
	catch (const cdbpp::cdbpp_exception& e)
	{
		// Abort if something went wrong...
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	catch (const NER::nersuite_exception& ex)
	{
		cerr << ex.what();
		return 1;
	}
	return 0;
}

void print(vector<string> &item)
{
	for(vector<string>::iterator itr1 = item.begin(); itr1 != item.end(); itr1++) {
		cout << *itr1;
		if((itr1 + 1) == item.end()) {
			cout << endl;
		}else {
			cout << "\t";
		}
	}
}
