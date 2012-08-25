//
//
//


using namespace std;


#include "seg_rep_changer.h"


bool SEG_REP_CHANGER::IOB2_to_IOBES( V2_STR &sent, int NE_COL)
{
	V2_STR::iterator	new_end = sent.end() - 1;
	string				cur_ne_label, next_ne_label;

	// 1. Handle NE labels before the last one
	for( V2_STR::iterator i_row = sent.begin(); i_row != new_end; ++i_row ) 
	{
		cur_ne_label = i_row->at(NE_COL), next_ne_label = (i_row + 1)->at(NE_COL);
		
		if( cur_ne_label[0] == 'B' ) {
			if( (next_ne_label[0] == 'B') || ( next_ne_label[0] == 'O' ) ) {
				i_row->at(NE_COL) = 'S';
				if( cur_ne_label.length() > 1 )
					i_row->at(NE_COL) += cur_ne_label.substr(1, string::npos);
			}
		}else if( cur_ne_label[0] == 'I' ) {
			if( (next_ne_label[0] == 'B') || ( next_ne_label[0] == 'O' ) ) {
				i_row->at(NE_COL) = 'E';
				if( cur_ne_label.length() > 1 )
					i_row->at(NE_COL) += cur_ne_label.substr(1, string::npos);
			}
		}else if( cur_ne_label[0] == 'O' ) {
			// nothing
		}else {
			return false;	// Invalid segment label; the conversion process failed!
		}
	}
	
	// 2. Handle the last NE label of a sentence
	cur_ne_label = sent.back().at(NE_COL);
	
	if( cur_ne_label[0] == 'B' ) {
		sent.back().at(NE_COL) = 'S';
		if( cur_ne_label.length() > 1 )
			sent.back().at(NE_COL) += cur_ne_label.substr(1, string::npos);
	}else if( cur_ne_label[0] == 'I' ) {
		sent.back().at(NE_COL) = 'E';
		if( cur_ne_label.length() > 1 )
			sent.back().at(NE_COL) += cur_ne_label.substr(1, string::npos);
	}else if( cur_ne_label[0] == 'O' ) {

	}else {
		return false;
	}
	
	return true;
}



bool SEG_REP_CHANGER::IOBES_to_IOB2( V2_STR &sent, int NE_COL)
{
	string	cur_ne_label;

	for( V2_STR::iterator i_row = sent.begin(); i_row != sent.end(); ++i_row ) 
	{
		cur_ne_label = i_row->at(NE_COL);

		if( cur_ne_label[0] == 'S' ) {
			i_row->at(NE_COL) = 'B';
			if( cur_ne_label.length() > 1 )
				i_row->at(NE_COL) += cur_ne_label.substr(1, string::npos);
		}else if( cur_ne_label[0] == 'E' ) {
			i_row->at(NE_COL) = 'I';
			if( cur_ne_label.length() > 1 )
				i_row->at(NE_COL) += cur_ne_label.substr(1, string::npos);
		}else if ( (cur_ne_label[0] == 'B' ) || (cur_ne_label[0] == 'I' ) || (cur_ne_label[0] == 'O' ) ) {
			// same segment label
		}else {
			return false;
		}
	}

	return true;
}

