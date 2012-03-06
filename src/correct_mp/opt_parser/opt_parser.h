//
//
//


#ifndef _H_OPTION_PARSER_
#define _H_OPTION_PARSER_


using namespace std;

#include <iostream>
#include <string>
#include <map>


class OPTION_PARSER {
private:
	map<string, string>		opts;
	
public:	
	int parse( int argc, char* argv[] );		// parse an option array and store them in the opts map
	bool required(string req_opt_codes[], int n_opts);		// check whether the required options exist or not
	string get_opt(char* opt_code);		// return the opt_value corresponding to the opt_code
	
};

#endif

