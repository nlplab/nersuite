/*
*      dictionary library
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
#ifndef _DICTIONARY_COMPILER_H
#define _DICTIONARY_COMPILER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include "../cdbpp-1.1/include/cdbpp.h"
#include "nersuite_exception.h"
#include "ne.h"

typedef std::vector<std::string>		V1_STR;


namespace NER
{
	/** 
	* @ingroup NERsuite
	* String normalization type definitions
	* The members of this enum are used as bit flags,
	* combined with OR operator and produce an int value
	* (excepting the special value NormalizationUnknown).
	*/
	enum NormalizeType
	{
		/** 
		* Unknown normalization
		*/
		NormalizationUnknown = -1,
		/** 
		* No Normalization
		*/
		NormalizeNone = 0,
		/**
		* Case Normalization
		*/
		NormalizeCase = 0x01,
		/**
		* Number Normalization
		*/
		NormalizeNumber = 0x02,
		/**
		* Symbol Normalization
		*/
		NormalizeSymbol = 0x04,
		/**
		* Token Normalization
		*/
		NormalizeToken = 0x08,
	};

	/** 
	* @ingroup NERsuite
	* External Dictionary Manager Class
	*/
	class Dictionary
	{
	public:
		/** Path to the CDBPP Database File (Read only)
		*/
		const std::string	db_path;

	private:
		static const char *VERSION_STRING;

		int db_normalization_type;
		std::ifstream	db_ifs;
		cdbpp::cdbpp	db_reader;
		std::map< int, std::string > map_idx2name;

	public:
		/** Construct a Dictionary object.  The path to the CDBPP database file must be provided.
		*  @param[in] binary_dbname Path to the CDBPP database.
		*/
		Dictionary(const char* binary_dbname);

		/** Destroy a Dictionary object.
		*/
		virtual ~Dictionary();

		/** Open the Dictionary for reading.
		*/
		void open();

		/** Retrieve the normalization type applied when creating the Dictionary.
		*/
		int get_normalization_type() { return db_normalization_type; }

		/** Retrieve the array of Classes which match the provided key from the Database.
		* @pre The Dictionary must be open before calling this function.
		* @param[in] key Key string
		* @param[in] normalize_type Combination of all normalization which should be applied before key matching
		* @param[out] count Match count
		* @return Returns an array of all class indices which satisfy the given condition
		*/
		const int* get_classes(const std::string& key, int normalize_type, size_t* count) const;

		/** Retrieve the count of Classes this Dictionary contains.
		* @pre The Dictionary must be open before calling this function.
		* @return Size of the Class set.
		*/
		size_t get_class_count() const;

		/** Convert a Class index to its name.
		* @pre The Dictionary must be open before calling this function.
		* @param[in] class_index Index of a class (must be smaller than the value returned from @ref get_class_count)
		* @return Name of the class
		*/
		const std::string& get_class_name(int class_index) const;

		/** Build up a new CDBPP Dictionary from a text file.
		* @param[in] txt_dbname Path to an existing text file to be read as the Dictionary source.
		* @param[in] normalize_type A combination of normalization types which is applied to parse the surface of input Dictioanry entries.
		*/
		void build(const char* txt_dbname, int normalize_type);

	private:
		void normalize(const std::string& form, int normalize_type, V1_STR& normalized_tokens);

		void load_header_info();

		void load_index_mapping();
	};
}
#endif
