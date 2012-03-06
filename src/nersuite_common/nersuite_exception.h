/*
*      NERSuite exception
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
#ifndef _NERSUITE_EXCEPTION_H
#define _NERSUITE_EXCEPTION_H

#include <string>
#include <exception>

namespace NER
{
	/** 
	* @ingroup NERsuite
	* NERsuite General Exception
	*/
	class nersuite_exception : public std::exception
	{
		std::string	message;

	public:
		/** Construct an NERsuite exception object with a message
		* @param[in] _what A message describing the exception content
		*/
		nersuite_exception(const char* _what)
		{
			message = _what;
		}

		/** Destroy an NERsuite exception object
		*/
		virtual ~nersuite_exception() throw ()
		{
		}

		/** Construct an NERsuite exception object with a message
		* @param[in] _what A message describing the exception content
		*/
		nersuite_exception(const std::string& _what)
		{
			message = _what;
		}

		/** Retrieve the exception message
		* @returns The message
		*/
		virtual const char* what() const throw()
		{
			return message.c_str();
		}
	};
}

#endif
