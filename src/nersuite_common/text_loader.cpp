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

#include "text_loader.h"
#include "string_utils.h"

using namespace std;

namespace NER
{
	// retrieve one sentence from input stream.
	// Each line consists of tab-separated columns (label + feature list).
	//   First column is the label assigned for the feature list.
	//   The following columns are the feature list.
	// Sentence must be terminated with an empty line.
	int get_sent(istream &cin, V2_STR &one_sent)
	{
		one_sent.clear();		// clear the container

		int					n_lines = 0;
		string				line = "";
		vector<string>		line_items;

		while(true) 
		{
			getline(cin, line);
			if(line.empty())						// break if a blank line appears
				break;

			tokenize(line_items, line, "\t");		// tokenize and
			one_sent.push_back(line_items);			// save it

			n_lines++;
		}

		return n_lines;
	}

	int get_sent(istream &cin, V2_STR &one_sent, const string &multidoc_separator, bool &separator_read)
	{
		one_sent.clear();		// clear the container
		separator_read = false;

		int					n_lines = 0;
		string				line = "";
		vector<string>		line_items;

		while(true) 
		{
			getline(cin, line);
			if(line.empty())						// break if a blank line appears
				break;

			if(line == multidoc_separator)                  // break if a document separator appears
			{
				separator_read = true;
				break;
			}

			tokenize(line_items, line, "\t");		// tokenize and
			one_sent.push_back(line_items);			// save it

			n_lines++;
		}

		return n_lines;
	}

	int get_sent(ifstream &ifs, V2_STR &one_sent)
	{
		one_sent.clear();		// clear the container

		int					n_lines = 0;
		string				line = "";
		vector<string>		line_items;

		while(! ifs.eof()) 
		{
			getline(ifs, line);
			if(line.empty())						// break if a blank line appears
				break;

			tokenize(line_items, line, "\t");		// tokenize and
			one_sent.push_back(line_items);			// save it

			n_lines++;
		}

		return n_lines;
	}
}
