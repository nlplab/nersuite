/*
*      NERSuite
*      string utility
*
* Copyright (c) 
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the names of the authors nor the names of its contributors
*       may be used to endorse or promote products derived from this
*       software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
* OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "string_utils.h"


using namespace std;

namespace NER
{

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

	string make_lowercase(const string &str)
	{
		string str_tmp = "";

		for (string::const_iterator s_itr = str.begin(); s_itr != str.end(); ++s_itr) {
			str_tmp += tolower(*s_itr);
		}

		return str_tmp;
	}

	// squeeze_nums ver.2: return the squeezed string as a new string variable
	string squeeze_nums(const string& str)
	{
		string result = str;
		int new_last_pos = 0, len = (int) str.length();

		if (len == 0)
			return result;
		if (('0' <= str[0]) && (str[0] <= '9'))
			result[0] = '0';

		for (int search_pos = 1; search_pos < len; ++search_pos) {
			if ( (str[ search_pos ] < '0') || (str[ search_pos ] > '9') ) {		// In case of alphabet character
				result[ ++new_last_pos ] = str[ search_pos ];
			}else if (str[ new_last_pos ] != '0') {										// In case of number
				result[ ++new_last_pos ] = '0';
			}
		}
		result.resize(new_last_pos + 1);
		return result;
	}

	// squeeze_syms ver.2: 
	string squeeze_syms(const string& str)
	{
		string result = str;
		int new_last_pos = 0, len = (int) str.length();

		if (len == 0)
			return result;
		if (check_alphanum(str[0]) == false)
			result[0] = '_';

		for (int search_pos = 1; search_pos < len; ++search_pos) {
			if (check_alphanum(str[ search_pos ]) == true) { 
				result[ ++new_last_pos ] = str[ search_pos ];
			}else if (str[ new_last_pos ] != '_') {
				result[ ++new_last_pos ] = '_';
			}
		}
		result.resize(new_last_pos + 1);
		return result;
	}

	string squeeze_ws(const string& str)
	{
		string result = str;
		int new_last_pos = 0, len = (int) str.length();

		if (len == 0)
			return result;
		if ((str[0] == ' ') || (str[0] == '\t'))
			result[0] = ' ';

		for (int search_pos = 1; search_pos < len; ++search_pos) {
			if ( (str[ search_pos ] != ' ') && (str[ search_pos ] != '\t') ) {		// In case of non-WS
				result[ ++new_last_pos ] = str[ search_pos ];
			}else if (str[ new_last_pos ] != ' ') {										// In case of WS and the last character of a new string is not WS
				result[ ++new_last_pos ] = ' ';
			}
		}
		result.resize(new_last_pos + 1);
		return result;
	}

}


