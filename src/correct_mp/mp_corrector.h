//
//
//


#ifndef _H_MISMATCHED_PARENTHESES_CORRECTOR_
#define _H_MISMATCHED_PARENTHESES_CORRECTOR_


using namespace std;

#include <string>
#include <vector>


// Correct NEs having mismatched parentheses in a given sentence
class MP_CORRECTOR {
private:
	typedef		vector< string >	V1_STR;
	typedef		vector< V1_STR >	V2_STR;
	
	int validate_to_remove( const V2_STR::iterator beg, const V2_STR::iterator end, int TOK_COL, int NE_COL );

public:
	int remove_mismatches(V2_STR &sent, int TOK_COL, int NE_COL);
	
	//TODO: implement different types of handling functions
	// extend_mismatch();
	// split_mismatch();	
};


#endif 


