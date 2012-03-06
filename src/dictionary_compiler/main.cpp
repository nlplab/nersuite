/*
*      dictionary_compiler main
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
#include <iostream>
#include <exception>
#include <cstdlib>
#include "../nersuite_common/dictionary.h"
#include "../nersuite_common/option_parser.h"

using namespace std;

void print_usage()
{
	cerr <<
		"usage: nersuite_dic_compiler [options] <Text_dictionary_filename> <CDB++_DB_filename>\n"
		"  Text dictionary should be a text file containing dictionary entries,\n"
		"  one per line with the following tab-separated format:\n"
		"    Named_Entity <tab> Class1 <tab> Class2 <tab> Class3 ...\n"
		"  Options:\n"
		"    -n <type_of_normalization> : Normalization type for dictionary matching\n"
		"       can be \"none\" or any combination of \"c\", \"n\", \"s\", \"t\"\n"
		"       none (default): No normalization\n"
		"       c: Case insensitive (convert all letters to lowercase)\n"
		"       cn: Case AND Number insensitive (convert all numbers to \'0\')\n"
		"       cns: Case AND Number AND Symbol insensitive (convert all symbols to \'_\')\n"
		"       t: Use token-base matching\n";
}

int main(int argc, char *argv[])
{
	NER::OPTION_PARSER opt_parser;
	opt_parser.parse(argc, argv);
	const vector<const char*>& args = opt_parser.get_args();
	if (args.size() != 3)
	{
		print_usage();
		exit(1);
	}
	int	normalize_type = NER::NormalizeNone;
	string normalize_option;
	if (opt_parser.get_value("-n", normalize_option))
	{
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

	try
	{
		NER::Dictionary dc(args[2]);
		dc.build(args[1], normalize_type);
	}
	catch (const exception& ex)
	{
		cerr << ex.what();
	}

	return 0;
}
