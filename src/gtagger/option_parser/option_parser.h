/**
 *  Option Parser:
 *    Parse an array of strings. It only consumes pairs of a parameter type and a parameter value.
 *	  A parameter type begins with '-' symbol and its corresponding parameter value is the next string.
**/


#ifndef		__OPTION_PARSER__
#define		__OPTION_PARSER__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Define the structure of a parameter
typedef struct {
	string	name;
	string	value;
} PARAM;
typedef		vector<PARAM>			V1_PARAM;


class OPTION_PARSER {
private:
	V1_PARAM	params;			// All parameters will be stored here.

public:
	// Parse input parameters.
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
			}else {										// 2. No more parameters.
				break;
			}
		}
		return consumed;
	}

	// Get the value of a given parameter name.
	bool get_value(const string &name, string &value) {
		bool found = false;
		for (V1_PARAM::const_iterator citr = params.begin(); citr != params.end(); ++citr) {
			if (citr->name == name) {
				value = citr->value;
				found = true;
			}
		}
		return found;
	}

	// Output parameters.
	void output_params(void) {
		for (V1_PARAM::const_iterator citr = params.begin(); citr != params.end(); ++citr) {
			if (citr->value == "")
				cout << citr->name << endl;
			else
				cout << citr->name << " : " << citr->value << endl;	
		}
	}

};


#endif

