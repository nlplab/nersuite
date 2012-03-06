/*
*      NERSuite
*      Typedefs for Feature Extractor 
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

#ifndef	_TYPEDEFS_H
#define	_TYPEDEFS_H

#include <string>
#include <vector>

typedef		std::vector< std::vector<std::string> >		V2_STR;
typedef		V2_STR::const_iterator						V2_STR_citr;

namespace NER
{
	/** 
	* @ingroup NERsuite
	* List of Feature Column Positions in the input/output file
	*
	*   Positions vary according to the NERsuite's mode.
	*   (BEG=1 in Learning mode, otherwise, BEG=0)
	*/
	struct COLUMN_INFO
	{
		/**
		* Beginning Position Feature Column
		*/
		int BEG;
		/**  
		* End Position Feature Column
		*/
		int END;
		/**  
		* Word (Surface Form) Feature Column
		*/
		int WORD;
		/**  
		* Word (Lemma Form) Feature Column
		*/
		int LEMMA;
		/**  
		* Part of Speech Feature Column
		*/
		int POS;
		/**  
		* Chunk Feature Column
		*/
		int CHUNK;
		/**  
		* Extra Dictionary Feature Column(s)
		* Indicates the first column of extra features
		*/
		int DIC;				// The first column of dictionary(ies)
	};
}
#endif
