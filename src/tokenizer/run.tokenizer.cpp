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
	if( argc == 2 ) {
		string	arg2 = argv[1];

		if( arg2 == "--help" ) {
			cerr << "Usage: " << argv[0] << " < a sentence-per-line file" << endl;
			return 0;
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

