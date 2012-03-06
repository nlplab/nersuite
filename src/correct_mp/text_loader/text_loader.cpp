//
//
//
//
//


#include "text_loader.h"
#include "../string_utils/string_utils.h"


int get_sent(istream &cin, V2_STR &one_sent)
{
	one_sent.clear();		// clear the container

	int					n_lines = 0;
	string				line = "";
	vector<string>		line_items;

	while(true) 
	{
		getline(cin, line);
		if(line.empty())						// break if a blank line appears
			break;

		tokenize(line_items, line, "\t");		// tokenize and
		one_sent.push_back(line_items);			// save it
		
		n_lines++;
	}

	return n_lines;
}

int get_sent(ifstream &ifs, V2_STR &one_sent)
{
	one_sent.clear();		// clear the container

	int					n_lines = 0;
	string				line = "";
	vector<string>		line_items;

	while(! ifs.eof()) 
	{
		getline(ifs, line);
		if(line.empty())						// break if a blank line appears
			break;

		tokenize(line_items, line, "\t");		// tokenize and
		one_sent.push_back(line_items);			// save it
		
		n_lines++;
	}

	return n_lines;
}
