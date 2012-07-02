// 
//
//


using namespace std;

#include <iostream>

#include "../nersuite_common/string_utils.h"
#include "../nersuite_common/text_loader.h"
#include "../nersuite_common/option_parser.h"
#include "seg_rep_changer.h"
#include "mp_corrector.h"


typedef		vector< string >		V1_STR;
typedef		vector< V1_STR >		V2_STR;


void print_data( const V2_STR &one_sent );
void print_usage( char* argv[] );



int main( int argc, char* argv[] )
{

	// 1. Parse command line arguments
	NER::OPTION_PARSER	opt_parser;
	opt_parser.parse( argc-1, &argv[1] );

	// 1.1. Get mandatory arguments 
	string   str_tokCol = "", str_neCol = "";
	if( (opt_parser.get_value( "-TOK_COL", str_tokCol ) == false) || 
	    (opt_parser.get_value( "-NE_COL", str_neCol ) == false) ) 
	{
		print_usage( argv );
		return -1;
	}
	int      tok_col = atoi( str_tokCol.c_str() ), ne_col = atoi( str_neCol.c_str() );

	// 1.2. Get optional arguments
	string   multidoc_separator = "";
	bool     multidoc_mode = opt_parser.get_value("-multidoc", multidoc_separator);
	

	// 2. Handle NEs having mismatched parentheses
	int   removed_NEs = 0;
	bool  separator_read = false;

	V2_STR            one_sent;
	MP_CORRECTOR      mp_corrector;
	SEG_REP_CHANGER   sp_changer;

	while( !cin.eof() ) {
		// 0. Read a sentence
		int   sent_len = NER::get_sent( cin, one_sent, multidoc_separator, separator_read );

		// 1. Skip blank lines
		if( sent_len == 0 ) {
			continue;
		}

		// 2. Pass it through the output stream if it is comment
		if( separator_read == true ) {
			print_data( one_sent );
			continue;
		}

		// 3. Remove entities having mismatched parenthesis, and print them otherwise
		// 3.1. Convert the data from the IOB2 rep. to the IOBES rep.
		sp_changer.IOB2_to_IOBES(one_sent, ne_col);									
		// 3.2. Remove NEs having mismatched parentheses
		removed_NEs += mp_corrector.remove_mismatches(one_sent, tok_col, ne_col);		
		// 3.3. Recover to the IOB2 rep.
		sp_changer.IOBES_to_IOB2(one_sent, ne_col);										
		// 3.4. Print it
		print_data( one_sent );
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



void print_usage( char* argv[] ) 
{
	cerr << "Usage: " << argv[0] << " -TOK_COL [#column] -NE_COL [#column]  -multidoc [SEP] <  input file" << endl;
	cerr << "   Mandatory arguments. " << endl;
	cerr << "        -TOK_COL #column : the column index in which tokens appear (begins at 0)" <<endl; 
	cerr << "        -NE_COL #column  : the column index in which named entities appear (begins at 0)" <<endl; 
	cerr << "   Optional arguments. " << endl;
	cerr << "        -multidoc [SEP]  : lines beginning with SEP will be passed to the output without any" << endl;
	cerr << "                             modification if given" << endl;
}	
