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
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include <algorithm>

#include "../cdbpp-1.1/include/cdbpp.h"
#include "dictionary.h"
#include "string_utils.h"
#include "nersuite_exception.h"
#include "tokenizer.h"

using namespace std;

bool build(char* txt_dbname, char* binary_dbname);

namespace NER
{
	const char *Dictionary::VERSION_STRING = "NERsuite dictionary (v1.1)";

	Dictionary::Dictionary(const char* binary_dbname)
		: db_path(binary_dbname)
	{
		db_normalization_type = NormalizationUnknown;
	}

	Dictionary::~Dictionary()
	{
		if (!db_reader.is_open())
		{
			db_reader.close();
		}
		db_ifs.close();
	}


	void Dictionary::build(const char* txt_dbname, int normalize_type)
	{
		ifstream ifs(txt_dbname);
		if (ifs.fail()) {
			throw nersuite_exception("ERROR: failed to open a text database file.");
		}

		std::ofstream ofs(db_path.c_str(), std::ios_base::binary);
		if (ofs.fail()) {
			throw nersuite_exception("ERROR: Failed to open a database file.");
		}

		try {
			// 0.0. Write versioning information.
			ofs.write(VERSION_STRING, strlen(VERSION_STRING));
			ofs.put(0);

			// 0.1. Write normalization type.
			db_normalization_type = normalize_type;
			ofs.write( reinterpret_cast<char *>( &normalize_type ), sizeof(int) );

			// 1. Get a list of semantic class names
			string		line = "";
			V1_STR		tokens;
			map<string, int>	map_name2idx;

			pair< string, int >	map_elem;
			pair< map<string, int>::iterator, bool > map_insert_ret;

			while( !ifs.eof() ) {
				getline( ifs, line );
				if( line == "")
					continue;

				tokens.clear();
				tokenize( tokens, line, "\t" );

				// 1) Make a map of semantic class names with temporary indices
				for( V1_STR::iterator itr = tokens.begin() + 1; itr != tokens.end(); ++itr ) {
					// Set an element for the indexing map
					map_elem.first = *itr;
					map_elem.second = -1;

					// Insert the element
					map_insert_ret = map_name2idx.insert( map_elem );
				}

				// 2) Finalize indices for semantic class names
				int		idx = 0;
				for( map< string, int >::iterator itr = map_name2idx.begin(); itr != map_name2idx.end(); ++itr ) {
					itr->second = (idx++);
				}
			}

			// 2.1. Write header information (# of semantic classes / name to index mapping)
			map< string, int >::size_type	max_idx = map_name2idx.size();
			ofs.write( reinterpret_cast<char *>( &max_idx ), sizeof( map< string, int >::size_type ) );				// 1) Write # of classes

			for( map< string, int >::iterator itr = map_name2idx.begin(); itr != map_name2idx.end(); ++itr ) {
				string				str_name = itr->first;
				string::size_type	len_name = (itr->first).size();
				int					str_idx = itr->second;

				ofs.write( reinterpret_cast<char *>( &len_name ), sizeof( string::size_type ) );			// 2.1) Write the size of a semantic class name
				// Writing a string variable causes malfunction
				ofs.write( str_name.c_str(), len_name + 1 );												// 2.2) Write the semantic class name
				ofs.write( reinterpret_cast<char *>( &str_idx ), sizeof( int ) );							// 2.3) Write the index of the semantic class name
			}


			// Create key/value pairs on the memory
			ifs.clear();				// Clear the eof flag
			ifs.seekg( 0, ios::beg );	// Set the file pointer as the beginning

			int			nth = 0;
			map< string, int >::iterator	map_find_ret;
			V1_STR		normalized_tokens;

			map< string, vector<int> > dictionary;
			vector<int>	classes;
			classes.reserve(max_idx);

			while (! ifs.eof()) {
				getline( ifs, line );
				if ( line == "" )
					continue;

				tokens.clear();
				tokenize( tokens, line, "\t" );
				normalize(tokens[0], normalize_type, normalized_tokens);
				int		n_classes = tokens.size() - 1;

				// Create mapping of Classes: class index (integer) --> class name (string)
				classes.clear();
				for (int i = 0; i < n_classes; ++i) {
					string	sem_class = tokens[ i+1 ];
					map_find_ret = map_name2idx.find( sem_class );
					if( map_find_ret != map_name2idx.end() )
					{
						classes.push_back(map_find_ret->second);
					}
					else
					{
						throw nersuite_exception("ERROR: index not found for a given key \"" + tokens[ i+1 ] + "\"");
					}
				}
				for (V1_STR::iterator it = normalized_tokens.begin(); it != normalized_tokens.end(); ++it)
				{
					string&	key = *it;
					vector<int>& current_value = dictionary[key];
					for (int i = 0; i < n_classes; ++i) {
						if (find(current_value.begin(), current_value.end(), classes[i]) == current_value.end())
						{
							current_value.push_back(classes[i]);
						}
					}
				}

				++nth;
				if ((nth % 10000) == 0 ) {
					cerr << ".";
				}
			}
			cerr << endl;

			// 2.2. Insert key/value pairs to the CDB++ writer.
			cdbpp::builder dbw(ofs);
			int*	classes_to_store = new int[max_idx];
			for (map< string, vector<int> >::iterator i = dictionary.begin(); i != dictionary.end(); ++i)
			{
				const string& key = (*i).first;
				vector<int>& value = (*i).second;
				for (size_t j = 0; j < value.size(); ++j)
				{
					classes_to_store[j] = value[j];
				}
				dbw.put(key.c_str(), key.length(), classes_to_store, sizeof(int) * value.size());
			}
			delete [] classes_to_store;
		}
		catch (const cdbpp::builder_exception& e) {
			// Abort if something went wrong...
			throw nersuite_exception(string("ERROR: ") + e.what());
		}
		ifs.close();
		ofs.close();
	}

	void Dictionary::open()
	{
		db_ifs.open(db_path.c_str(), ios_base::binary );
		if (db_ifs.fail())
		{
			throw nersuite_exception("ERROR: failed to open input filestream.");
		}

		load_header_info();
		load_index_mapping();

		db_reader.open(db_ifs);
		if (!db_reader.is_open())
		{
			throw nersuite_exception("ERROR: failed to open CDBPP for input.");
		}
	}

	void Dictionary::load_header_info()
	{
		// 0.0. Read and confirm version information.

		string db_version_string = "";
		while (db_ifs.good())
		{
			char read = db_ifs.get();
			if ( read == 0 )
			{
				break;
			} else {
				db_version_string += read;
			}
		}
		if (db_version_string != VERSION_STRING)
		{
			throw nersuite_exception("ERROR: DB version mismatch.");
		}

		// 0.1. Read normalization type.
		db_ifs.read( reinterpret_cast<char *>( &db_normalization_type ), sizeof(int) );
	}

	void Dictionary::load_index_mapping()
	{
		map< int, string >::size_type	map_size;
		db_ifs.read( reinterpret_cast<char *>( &map_size ), sizeof( map< int, string >::size_type ) );	// 1. Read # of semantic class names

		pair< int, string >		pair_idx2name;
		for (map< string, int >::size_type i = 0; i < map_size; ++i)
		{
			char				*p_sem_class = 0;
			string::size_type	len;

			db_ifs.read(reinterpret_cast<char *>(&len), sizeof(string::size_type));			// 2.1. Read the size of a semantic class name string
			p_sem_class = new char[len + 1];												// 2.2. Read the semantic class name string
			db_ifs.read( p_sem_class, len + 1);
			pair_idx2name.second = p_sem_class;
			delete[] p_sem_class;		
			db_ifs.read(reinterpret_cast<char *>(&pair_idx2name.first), sizeof(int));		// 2.3. Read the index of the semantic class name string

			map_idx2name.insert(pair_idx2name);
		}
	}

	const int* Dictionary::get_classes(const string& key, int normalize_type, size_t* count) const
	{
		if (!db_reader.is_open())
		{
			throw nersuite_exception("ERROR: failed to open CDBPP for input.");
		}

		string nkey = key;
		if ((normalize_type & NER::NormalizeNumber) != 0)
		{
			nkey = squeeze_nums(nkey);
		}
		if ((normalize_type & NER::NormalizeSymbol) != 0)
		{
			nkey = squeeze_syms(nkey);
		}
		if ((normalize_type & NER::NormalizeCase) != 0)
		{
			// lowercase search
			nkey = make_lowercase(nkey);
		}

		const int* result = (const int*)db_reader.get(nkey.c_str(), nkey.length(), count);
		if (count != NULL)
		{
			*count /= sizeof(int);
		}
		return result;
	}

	size_t Dictionary::get_class_count() const
	{
		return map_idx2name.size();
	}

	const string& Dictionary::get_class_name(int class_index) const
	{
		return map_idx2name.find(class_index)->second;
	}

	void Dictionary::normalize(const string& form, int normalize_type, V1_STR& normalized_tokens)
	{
		normalized_tokens.clear();
		string result = form;
		if ((normalize_type & NER::NormalizeCase) != 0)
		{
			result = make_lowercase(result);
		}
		if ((normalize_type & NER::NormalizeNumber) != 0)
		{
			result = squeeze_nums(result);
		}
		if ((normalize_type & NER::NormalizeSymbol) != 0)
		{
			result = squeeze_syms(result);
		}
		if ((normalize_type & NER::NormalizeToken) != 0)
		{
			Tokenizer tokenizer;
			V2_STR	tokens;
			tokenizer.tokenize(result, tokens);
			for (V2_STR::iterator i = tokens.begin(); i != tokens.end(); ++i)
			{
				V1_STR& token_attrs = *i;
				if (token_attrs.size() < 3)
				{
					throw nersuite_exception("ERROR: unexpected result found while tokenizing: " + result);
				}
				normalized_tokens.push_back(token_attrs[2]);
			}
		}
		else 
		{
			normalized_tokens.push_back(result);
		}
	}
}
