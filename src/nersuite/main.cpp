/*
*      NERSuite main
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

#ifdef    HAVE_CONFIG_H
#include <config.h>
#endif/*HAVE_CONFIG_H*/

#include "nersuite.h"

#define DEFAULT_MODEL_FILE	"model.m"

using namespace std;

void print_usage(char exe_name[]);

int main(int argc, char* argv[])
{
	if (argc < 4) {
		print_usage(argv[0]);
		return 1;
	}

	string  ner_dir = argv[0];
	ner_dir = ner_dir.substr(0, ner_dir.find_last_of("/"));

	string	mode = argv[1];

	NER::Suite nersuite(argc-2, &argv[2]);

	// Train or Tag
	if (mode == MODE_LEARN) {
		nersuite.learn();
	} else if (mode == MODE_TAG) {            // Tag each sentence
		nersuite.tag();
	} else {
		cerr << " The first argument must be either \"" << MODE_LEARN << "\" or \"" << MODE_TAG << "\"" << endl;
		return -1;
	}

	return 0;
}

void print_usage(char exe_name[])
{
	cerr << "Usage: " << exe_name << " <mode> <-m model_filename> [-C C2_value] [-o output_format] [-persistent] [file(s)]" << endl;
	cerr << "  1. mode " << endl;
	cerr << "    - 'learn' or 'tag' " << endl;
	cerr << endl;
	cerr << "  2. model_filename " << endl;
	cerr << "    - A model file name for storing a trained model in 'train' mode, or for loading a model in 'tag' mode" << endl;
	cerr << endl;
	cerr << "  3. C2 value  (for learn mode) " << endl;
	cerr << "    - The C2 value of the CRFSuite Learning parameter " << endl;
	cerr << "    - Default value is 1.0 " << endl;
	cerr << endl;
	cerr << "  4. output_format  (for tag mode) " << endl;
	cerr << "    - 'conll' or 'standoff' format " << endl;
	cerr << "    - Default value is 'conll' format " << endl;
	cerr << endl;
	cerr << "  5. persistent mode (for tag mode) " << endl;
	cerr << "     - if -persistent option is given, the program goes into the infinite loop "   << endl;
	cerr << "      and it prints the EOT ('0x04') for the end of stream (e.g. end of an input " << endl; 
	cerr << "      file stream) instead of the EOF. (Please do not use -f nor -l option)" << endl;
	cerr << endl;
	cerr << "  6. input_file " << endl;
	cerr << "    - Input file(s) can be delivered in three ways." << endl;
	cerr << "      1. re-directed standard input." << endl;
	cerr << "      2. an input file name with -f option. (e.g. -f input.txt)" << endl;
	cerr << "      3. a file of input file names with -l option. (e.g. -l list.txt)" << endl;
	cerr << "        a file in the list file needs to have a relative path from the directory in which the list file stored." << endl;
	cerr << "    - An input file consists of columns as follows." << endl;
	cerr << "        [1st col.] - the byte position of the first letter of a token. " << endl;
	cerr << "        [2nd col.] - the byte position one past the last letter of a token. " << endl;
	cerr << "        [3rd col.] - raw token (word)" << endl;
	cerr << "        [4th col.] - lemma" << endl;
	cerr << "        [5th col.] - POS tag" << endl;
	cerr << "        [6th col.] - chunk tag" << endl;
	cerr << "        [7th ... ] - any attributes" << endl;
	cerr << "          P.s. With learn option, 1st column is a correct named entity label for each line and other columns will be placed one column after then the original position" << endl;
} 