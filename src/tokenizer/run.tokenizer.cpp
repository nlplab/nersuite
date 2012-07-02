// 
// A tester for a sentence_tokenizer class
//

#include <iostream>
#include <string>
#include <vector>

#include "../nersuite_common/tokenizer.h"


using namespace std;
using namespace NER;


int main(int argc, char* argv[])
{
	if( argc >= 2 ) {
		for( int i=1; i<argc; ++i ) {
			string arg = argv[i];
			if( arg == "--help" ) {
				cerr << "Usage: " << argv[0] << " < a sentence-per-line file" << endl;
				return 0;
			}
		}
	}

	bool multidoc_mode = false;
	string multidoc_separator;
	if( argc >= 3 ) {
		for( int j=1; j<argc-1; ++j ) {
			string arg = argv[j];
			string val = argv[j+1];
			if( arg == "-multidoc" ) {
				multidoc_mode = true;
				multidoc_separator = val;
			}
		}
	}

	Tokenizer	tokenizer;

	string  line = "";
	V2_STR  data;
	int	    n_lines = 1;
	int     base_offset = 0;
	bool    prev_comment = false;

	while( getline( cin, line ) ) {
		int sent_len = line.length() + 1;       // Remember the length of the line 

		// 1. Get a line
		if( line.empty() ) {                    // Ignore blank lines
			continue;
		}else {
			if (line[line.size()-1] =='\r') {     // Remove the linefeed if it exists
				line.resize(line.size() - 1);
			}
		}
		
		// 2. Pass the input line to the output if it is a comment line beginning with the separator
		if( multidoc_mode && (line.compare(0, multidoc_separator.length(), multidoc_separator) == 0) ) {  
			cout << line << endl;
			cout.flush();
			base_offset = 0;
			prev_comment = true;

			continue;
		}

		// 3. Tokenize if it is a sentence
		data.clear();
		if( tokenizer.tokenize( line, data, base_offset ) == 0 )	{ // Ignore lines that only have spaces
			base_offset += sent_len;
			continue;
		}else {
			base_offset += sent_len;
		}

		// 4. Print a tokenized sentence
		if( prev_comment ) {
			cout << endl;
			prev_comment = false;
		}

		for( V2_STR::iterator i_row = data.begin(); i_row != data.end(); ++i_row) {
			for( V1_STR::iterator i_col = i_row->begin(); i_col != i_row->end(); ++i_col) {
				cout << *i_col;
				if( (i_col + 1) != i_row->end() )
					cout << "\t";
			}
			cout << endl;
		}
		cout << endl;

		++n_lines;
	}
	
	return n_lines;
}
