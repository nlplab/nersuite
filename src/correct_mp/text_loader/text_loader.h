//
//
//
//
//

#ifndef		_TEXT_LOADER_
#define		_TEXT_LOADER_

#include <iostream>
#include <fstream>
#include <istream>

#include <string>
#include <vector>


using namespace std;


typedef		vector< vector<string> >		V2_STR;


int get_sent(istream &cin, V2_STR &one_sent);
int get_sent(ifstream &cin, V2_STR &one_sent);



#endif


