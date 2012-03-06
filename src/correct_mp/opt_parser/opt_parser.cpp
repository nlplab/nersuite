//
//
//


using namespace std;


#include "opt_parser.h"


int OPTION_PARSER::parse( int argc, char* argv[] )
{
	opts.clear();

	for( int i = 0; i < argc; ++i ) {
		pair<string, string>	one_opt;
		string					opt_code, opt_value;
		
		if( argv[i][0] == '-' ) {
			string	str_opt = argv[i];
			int		sep_pos = str_opt.find_first_of("=");
			
			if( sep_pos == string::npos ) {
				opt_code = str_opt;
				opt_value = "no_value";
			}else {
				opt_code = str_opt.substr(0, sep_pos);
				opt_value = str_opt.substr(sep_pos + 1, string::npos);
			}
			one_opt.first = opt_code;
			one_opt.second = opt_value;
			opts.insert(one_opt);		
		}else {
			cerr << "Unrecognized option code: " << opt_code << endl;
			return -1;
		}
	}

	return (int) opts.size();
}

bool OPTION_PARSER::required(string req_opt_codes[], int n_opts)
{
	for( int i = 0; i < n_opts; ++i ) {
		if( opts.find( req_opt_codes[i] ) == opts.end() )
			return false;
	}

	return true;
}

string OPTION_PARSER::get_opt( char* chr_opt_code )
{
	string							str_opt_code = chr_opt_code;
	map<string, string>::iterator	ret;

	if( (ret = opts.find( str_opt_code )) != opts.end() ) {
		return ret->second;
	}else {
		return NULL;
	}
}

