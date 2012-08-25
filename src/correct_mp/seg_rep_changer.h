//
//
//


#ifndef _H_IOB2_TO_IOBES_
#define _H_IOB2_TO_IOBES_


using namespace std;

#include <string>
#include <vector>


class SEG_REP_CHANGER {
private:
	typedef		vector< string >	V1_STR;
	typedef		vector< V1_STR >	V2_STR;

public:
	// To a finer-grained rep.
	bool IOB2_to_IOBES( V2_STR &sent, int NE_COL );		// Change the IOB2 representation to the IOBES
	
	// To a coarser-grained rep.
	bool IOBES_to_IOB2( V2_STR &sent, int NE_COL );		// Change the IOBES representation to the IOB2
	
};


#endif


