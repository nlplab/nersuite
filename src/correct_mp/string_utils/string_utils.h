//
//
//
//
//


#ifndef		_STRING_UTILS_
#define		_STRING_UTILS_


#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;


//
// BEGIN TEMPLATES FUNCTIONS
//

template<typename T1> 
int tokenize(T1 &V_STR, string &one_line, string del)
{
	V_STR.clear();

	int total_elem = 0;
	size_t beg = 0, end = one_line.find(del, 0);

	while(beg < one_line.length())
	{
		if ((end = one_line.find(del, beg)) == string::npos)
			end = one_line.length();

		V_STR.push_back(one_line.substr(beg, end - beg));

		beg = end + 1;
		++total_elem;
	}

	return total_elem;
}


// END: TEMPLATES FUNCTIONS


void trim_ws(string &str);
string	int2str(int i);
string	int2strIDX(int idx);
void	make_lowercase(string &str);
bool	check_alphanum(const char ch);

string	squeeze_nums(string str);
string	squeeze_syms(string str);
string	squeeze_ws(string str);


#endif

