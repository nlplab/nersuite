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
	// Retrieve one sentence from input stream.
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

	// Functionality: 
	//     - Retrieve one sentence from input stream, or comment sentences which begin with a specified 
	//     prefix, multidoc_separator.
	// Return value:  
	//     - # of lines read regardless input type (sentence or comment)
	// Input:
	//     - Lines of comments must be separated from a previous (and next) sentence with a blank line
	int get_sent(istream &cin, V2_STR &one_sent, const string &multidoc_separator, bool &separator_read)
	{
		one_sent.clear();		// clear the container
		separator_read = false;

		int              n_lines = 0;
		string           line = "";
		vector<string>   line_items;
		int              mode = 0;    // 0: initialized, 1: sentence, 2: comment
		int              nSep = multidoc_separator.length();
		bool             multidoc_mode = multidoc_separator != "";

		while(true) 
		{
			getline(cin, line);
			if(line.empty())						// break if a blank line appears
				break;

			if( multidoc_mode && (line.compare(0, nSep, multidoc_separator) == 0) )
			{
				if( mode == 1 ) {
					cerr << "Error: Input data format: multidoc comment lines must be separated from sentences by a blank line" << endl;
					exit(1);
				}else {
					mode = 2;
				}

				line_items.clear();
				line_items.push_back( line );
				one_sent.push_back( line_items );   // Add a comment line as it is
			}else {
				if( mode == 2 ) {
					cerr << "Error: Input data format: sentence part must be separated from comment lines by a blank" << endl;
					exit(1);
				}else {
					mode = 1;
				}

				tokenize(line_items, line, "\t");		// tokenize and
				one_sent.push_back(line_items);			// save it
			}
		
			n_lines++;
		}

		if( mode == 2 ) {
			separator_read = true;
		}

		return n_lines;
	}

}



