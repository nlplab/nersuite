//
//
//
//
//


#include "string_utils.h"


using namespace std;


// TEMPLATE FUNCTIONS in HEADER FILES
void trim_ws(string &str)
{
	int beg_pos = 0, end_pos = (int) str.length() - 1;
	int str_len = (int) str.length();

	for(; beg_pos != str_len; ++beg_pos) {
		if(str[beg_pos] != ' ')
			break;
	}

	for(; end_pos != -1;  --end_pos) {
		if(str[end_pos] != ' ') {
			break;
		}
	}

	// make new string
	str = str.substr(beg_pos, end_pos - beg_pos + 1);
}

string int2str(int i)
{
	stringstream ss;
	string s;
	ss << i;
	s = ss.str();

	return s;
}

string int2strIDX(int idx)
{
	stringstream ss;
	string str = "";

	if (idx == 0) {
		str = "C";
	}else if (idx < 0) {
		str = "L";
		ss << abs(idx);
		str += ss.str();
	}else {
		str = "R";
		ss << idx;
		str += ss.str();
	}

	return str;
}

void make_lowercase(string &str)
{
	string str_tmp = "";

	for (string::iterator s_itr = str.begin(); s_itr != str.end(); ++s_itr) {
		str_tmp += tolower(*s_itr);
	}

	str = str_tmp;
}

bool check_alphanum(const char ch)
{
    if ( (('0' <= ch) && (ch <= '9')) ||
		 (('A' <= ch) && (ch <= 'Z')) ||
		 (('a' <= ch) && (ch <= 'z')) ) {
		return true;
	}else {
		return false;
	}
}

// squeeze_nums ver.2: return the squeezed string as a new string variable
string squeeze_nums(string str)
{
	int new_last_pos = 0, len = (int) str.length();

	if (len == 0)
		return str;
	if (('0' <= str[0]) && (str[0] <= '9'))
		str[0] = '0';
	
	for (int search_pos = 1; search_pos < len; ++search_pos) {
		if ( (str[ search_pos ] < '0') || (str[ search_pos ] > '9') ) {		// In case of alphabet character
			str[ ++new_last_pos ] = str[ search_pos ];
		}else if (str[ new_last_pos ] != '0') {										// In case of number
			str[ ++new_last_pos ] = '0';
		}
	}

	str.resize(new_last_pos + 1);

	return str;
}

// squeeze_syms ver.2: 
string squeeze_syms(string str)
{
	int new_last_pos = 0, len = (int) str.length();

	if (len == 0)
		return str;
	if (check_alphanum(str[0]) == false)
		str[0] = '_';
	
	for (int search_pos = 1; search_pos < len; ++search_pos) {
		if (check_alphanum(str[ search_pos ]) == true) { 
			str[ ++new_last_pos ] = str[ search_pos ];
		}else if (str[ new_last_pos ] != '_') {
			str[ ++new_last_pos ] = '_';
		}
	}

	str.resize(new_last_pos + 1);

	return str;
}

string squeeze_ws(string str)
{
	int new_last_pos = 0, len = (int) str.length();

	if (len == 0)
		return str;
	if ((str[0] == ' ') || (str[0] == '\t'))
		str[0] = ' ';
	
	for (int search_pos = 1; search_pos < len; ++search_pos) {
		if ( (str[ search_pos ] != ' ') && (str[ search_pos ] != '\t') ) {		// In case of non-WS
			str[ ++new_last_pos ] = str[ search_pos ];
		}else if (str[ new_last_pos ] != ' ') {										// In case of WS and the last character of a new string is not WS
			str[ ++new_last_pos ] = ' ';
		}
	}

	str.resize(new_last_pos + 1);

	return str;
}



