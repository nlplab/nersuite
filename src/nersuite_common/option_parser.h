/*
*      NERSuite
*      Option Parser utility
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

#ifndef		_OPTION_PARSER_H
#define		_OPTION_PARSER_H

#include <iostream>
#include <string>
#include <vector>

namespace NER
{

	// Define the structure of a parameter
	typedef struct {
		std::string	name;
		std::string	value;
	} PARAM;
	typedef		std::vector<PARAM>			V1_PARAM;

	/** 
	* @ingroup NERsuite
	*/
	/**
	*  Option Parser:
	*    Parse an array of strings. It only consumes pairs of a parameter type and a parameter value.
	*	  A parameter type begins with '-' symbol and its corresponding parameter value is the next string.
	**/
	class OPTION_PARSER {
	private:
		V1_PARAM	params;			// All parameters will be stored here.
		std::vector<const char*>	args;

	public:
		/**
		* Parse input parameters
		* @param[in] n Number of arguments
		* @param[in] items Array of arguments
		* @returns Count of parameters parsed
		*/
		int parse(int n, char* items[]) {
			int		consumed = 0;
			PARAM	param;

			for (int i = 0; i < n; ++i) {
				if (items[i][0] == '-') {
					// 1. Get parameter name.
					param.name = items[i];
					++consumed;

					// 2. Get parameter value
					if ((i+1) < n) {
						if(items[i+1][0] == '-') {
							param.value = "";
						}else {
							param.value = items[i+1];
							++i;
							++consumed;
						}
					}else {
						param.value = "";
					}

					// 3. Put a parameter in the parameter container
					params.push_back(param);
				} else {
					args.push_back(items[i]);
				}
			}
			return consumed;
		}

		/**
		* Get the value of a given parameter name
		* @param[in] name Name of parameter
		* @param[out] value Value for the parameter if found
		* @returns true if parameter found
		*/
		bool get_value(const std::string &name, std::string &value) {
			bool found = false;
			for (V1_PARAM::const_iterator citr = params.begin(); citr != params.end(); ++citr) {
				if (citr->name == name) {
					value = citr->value;
					found = true;
				}
			}
			return found;
		}

		/**
		* Output parameters to std::cout
		*/
		void output_params() {
			for (V1_PARAM::const_iterator citr = params.begin(); citr != params.end(); ++citr) {
				if (citr->value == "")
					std::cout << citr->name << std::endl;
				else
					std::cout << citr->name << " : " << citr->value << std::endl;	
			}
		}

		/**
		* Get parameters as an array (not including option parameters)
		* @returns Array of parameters
		*/
		const std::vector<const char*>& get_args() const
		{
			return args;
		}
	};
}
#endif

