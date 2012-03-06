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

#ifndef		_STRING_UTILS_H
#define		_STRING_UTILS_H


#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>

namespace NER
{
	/** 
	* @ingroup NERsuite
	*/
	/** Tokenize a string
	* param[out] V_STR List of string where output is pushed. (Cleared before processing)
	* param[in] one_line Input string representing a line of text
	* param[out] del A delimiter
	*/
	template<typename T1> 
	int tokenize(T1 &V_STR, std::string &one_line, std::string del)
	{
		V_STR.clear();

		int total_elem = 0;
		size_t beg = 0, end = one_line.find(del, 0);

		while(beg < one_line.length())
		{
			if ((end = one_line.find(del, beg)) == std::string::npos)
				end = one_line.length();

			V_STR.push_back(one_line.substr(beg, end - beg));

			beg = end + 1;
			++total_elem;
		}

		return total_elem;
	}

	extern void		trim_ws(std::string &str);
	extern std::string	int2str(int i);
	extern std::string	int2strIDX(int idx);
	extern bool		check_alphanum(const char ch);

	extern std::string	make_lowercase(const std::string& str);
	extern std::string	squeeze_nums(const std::string& str);
	extern std::string	squeeze_syms(const std::string& str);
	extern std::string	squeeze_ws(const std::string& str);

}
#endif

