//
//
//
//
//


using namespace std;


#include "mp_corrector.h"


// IN: this module assumes that the NE labels are represented with the IOBES representation
int MP_CORRECTOR::remove_mismatches( V2_STR &sent, int TOK_COL, int NE_COL )
{
	int					removed_NEs = 0;
	V2_STR::iterator	beg, end;		// end is a past-the-end iterator
	
	for( V2_STR::iterator i_row = sent.begin(); i_row != sent.end(); ++i_row ) {
		string	ne_label = i_row->at(NE_COL);

		switch( ne_label[0] ) {
		case 'O':
			break;
		case 'B':
			beg = i_row;
			break;
		case 'I':
			break;
		case 'E':
			end = i_row + 1;
			removed_NEs += validate_to_remove( beg, end, TOK_COL, NE_COL );	// erase a current NE if it has a mismatched parenthesis
			break;
		case 'S':
			beg = i_row;
			end = i_row + 1;
			removed_NEs += validate_to_remove( beg, end, TOK_COL, NE_COL );
			break;
		}
	}

	return removed_NEs;
}

int MP_CORRECTOR::validate_to_remove(const V2_STR::iterator beg, const V2_STR::iterator end, int TOK_COL, int NE_COL )
{
	int		val = 0;
	
	for( V2_STR::iterator i_row = beg; i_row != end; ++i_row ) {
		string	tok = i_row->at(TOK_COL);

		if( (tok == "(") || (tok == "[") || (tok == "{") ) {
			++val;
		}else if( (tok == ")") || (tok == "]") || (tok == "}") ) {
			--val;
		}

		if( val < 0 )	// absolute error
			break;
	}

	if( val != 0 ) {
		for( V2_STR::iterator i_row = beg; i_row != end; ++i_row )
			(i_row->at(NE_COL)) = "O";
		
		return 1;
	}else {
		return 0;
	}
}





