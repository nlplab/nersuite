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

	string	line = "";
	V2_STR	data;
	int	n_lines = 1;

	while( getline(cin, line ) ) {
		data.clear();
		if( line.empty() ) {		              // Ignore blank lines
			continue;
		}else {
			if (line[line.size()-1] =='\r') {     // Remove the linefeed if it exists
				line.resize(line.size() - 1);
			}
		}
		
		if( multidoc_mode && line == multidoc_separator) {  // Check the end of document with separator
			cout << multidoc_separator << "\n";
			cout.flush();
			continue;
		}

		if( tokenizer.tokenize( line, data, 0 ) == 0 )	// Ignore lines that only have spaces
			continue;

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
