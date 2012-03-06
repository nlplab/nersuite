// 
//
//


using namespace std;

#include <iostream>

#include "string_utils/string_utils.h"
#include "text_loader/text_loader.h"
#include "opt_parser/opt_parser.h"
#include "seg_rep_changer/seg_rep_changer.h"
#include "mp_corrector/mp_corrector.h"


typedef		vector< string >		V1_STR;
typedef		vector< V1_STR >		V2_STR;


void print_data( const V2_STR &one_sent );


int main( int argc, char* argv[] )
{
	// 1. Process command line arguments
	OPTION_PARSER	opt_parser;
	string			req_opt_codes[] = { "-TOK_COL", "-NE_COL" };
	int				n_req_opts = 2;

	if( (opt_parser.parse( argc - 1, &argv[1] ) < 0 ) || (opt_parser.required( req_opt_codes, n_req_opts ) == false) ) {
		cerr << "Usage: " << argv[0] << " -TOK_COL=[0-9]* -NE_COL=[0-9]*  <  input file" << endl;
		return -1;
	}
		
	
	// 2. Handle NEs having mismatched parentheses
	int			removed_NEs = 0;
	int			tok_col = atoi( opt_parser.get_opt("-TOK_COL").c_str() );
	int			ne_col = atoi( opt_parser.get_opt("-NE_COL").c_str() );
	V2_STR		one_sent;
	SEG_REP_CHANGER		sp_changer;
	MP_CORRECTOR		mp_corrector;

	while( get_sent(cin, one_sent) != 0 ) {								// Get a sentence
		if( (one_sent.front().front().length() >= 3) && (one_sent.front().front().substr(0, 3) == "###") ) {
			print_data( one_sent );											// 1.1. Print comment lines

		}else {
			sp_changer.IOB2_to_IOBES(one_sent, ne_col);										// 2.1. Convert the data from the IOB2 rep. to the IOBES rep.
			removed_NEs += mp_corrector.remove_mismatches(one_sent, tok_col, ne_col);		// 2.2. Remove NEs having mismatched parentheses
			sp_changer.IOBES_to_IOB2(one_sent, ne_col);										// 2.3. Recover to the IOB2 rep.

			print_data( one_sent );															// 2.4. Print the output
		}
	}

	cerr << endl;
	cerr << "The number of removed NEs: " << removed_NEs << endl;
	
	return 0;
}


void print_data( const V2_STR &one_sent )
{
	for( V2_STR::const_iterator i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row ) {
		for( V1_STR::const_iterator i_col = i_row->begin(); i_col != i_row->end(); ++i_col ) {
			cout << *(i_col);
			if( (i_col + 1) != i_row->end() )
				cout << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

