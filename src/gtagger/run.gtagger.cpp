/**
 * A modified GENIA tagger for NERSuite
 * The information on the license of using the GENIA tagger is
 * in in LICENSE and README files, and NERSuite follows it.
 *
 * History
 *  
 *  Modified at 2011/02/25
 *    1. Support tagging multiple files.
 *    2. Model loading time is reduced.
 *
**/

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>

#include <stdio.h>  
#include <stdlib.h>
#include <time.h>
#include "geniatagger-3.0.1/maxent.h"
#include "geniatagger-3.0.1/common.h"


#include "option_parser/option_parser.h"
#include <fstream>

using namespace std;

typedef    vector<string>    V1_STR;
typedef    vector< V1_STR >  V2_STR;


vector<ME_Model>    vme(16);    // genia pos models
vector<ME_Model>    vme_chunking(16);  // genia chunking models

// Original functions of the Genia tagger 3.0.1
int     genia_init( const string &genia_dir );
string  bidir_postag( const string & s, const vector<ME_Model> & vme, const vector<ME_Model> & cvme, bool dont_tokenize );
  int     run_tagging(istream &is, ostream &os, string multidoc_separator, bool dont_tokenize);
void    bidir_chunking( vector<Sentence> & vs, const vector<ME_Model> & vme );
void    init_morphdic( const string &path );

// New functions for this version
int     get_sent( istream &is, V2_STR &one_sent, string &multidoc_separator, bool &separator_read );
string  assemble_tok_sent( const V2_STR &one_sent );
int     tokenize( V1_STR &one_seg, string &one_line, const string &del );
void    output_result( V2_STR &one_sent, ostream &os );
void    output_usage(char *command);


int main(int argc, char* argv[])
{
  bool            dont_tokenize = true;
  string          opt_value = "";
  OPTION_PARSER   opt_parser;

  if (argc < 3) {
    output_usage(argv[0]);
    return -1;
  }else{
    opt_parser.parse(argc - 1, &argv[1]);
    if( !opt_parser.get_value("-d", opt_value) ) {
      cerr << "-d option is required." << endl;
      return -2;
    }
  }

  // 1. Load POS-tagging, lemmatization and chunking models
  opt_parser.get_value("-d", opt_value);
  if( opt_value[ opt_value.length() - 1 ] != '/' )
    opt_value += "/";
  genia_init(opt_value);

  // 2. Check multi-document mode
  string   multidoc_separator = "";
  bool     multidoc_mode = opt_parser.get_value("-multidoc", multidoc_separator);
  
  // 3. Run POS-tagging, Lemmatization and Chunking with Genia tagger ver. 3.0.1
  if (multidoc_mode) {
    run_tagging(cin, cout, multidoc_separator, dont_tokenize);

  }else if (argc == 3) {
    run_tagging(cin, cout, "", dont_tokenize);
  
  }else if (opt_parser.get_value("-f", opt_value)) {
    ifstream ifs(opt_value.c_str());
    run_tagging(ifs, cout, "", dont_tokenize);
    ifs.close();
  
  }else if (opt_parser.get_value("-l", opt_value)) {
    string  path = "./";
		int     idx = opt_value.find_last_of('/');
		if (idx != string::npos)
			path = opt_value.substr(0, idx+1);
    
    ifstream ifs_lst(opt_value.c_str());
    if (! ifs_lst) {
      cerr << "Cannot open a list file! " << opt_value << endl;
      return -3;
    }
    
    string  fn = "";
    while(getline(ifs_lst, fn)) {
      string target = path + fn, result = path + fn + ".gtag";
      
      ifstream ifs_trg(target.c_str());
      if (! ifs_trg) {
        cerr << "Cannot open a target file! " << target << endl;
        return -4;
      }      
      ofstream ofs_res(result.c_str());
      
      run_tagging(ifs_trg, ofs_res, "", dont_tokenize);
      ifs_trg.close();
      ofs_res.close();
    }
    ifs_lst.close();
  }else {
    cerr << "Input must be 1) STDIN, 2) an input file name, or 3) a file name of input files." << endl;
    return -3;
  }
  
  return 0;
}

// Tag input from the is stream, and output its result to the os stream.
int run_tagging(istream &is, ostream &os, string multidoc_separator, bool dont_tokenize) {
  int       n = 1;
  V2_STR    one_sent;
  
  bool multidoc_mode = multidoc_separator != "";
  bool separator_read;

  while (true) {
    int n_words = get_sent(is, one_sent, multidoc_separator, separator_read);
    if (n_words == 0) {
      if ( multidoc_mode ) {          // if the nersuite tag mode is running with the multi-document option, process doc end
	if ( separator_read ) {       // if the multi-document separator was seen, echo it on the output
          cout << multidoc_separator << endl;
	  cout.flush();
	}
	if ( is.eof() ) {             // only quit the program on EOF
	  break;
	} else {
	  continue;
	}
      }else {                         // quit the program if no input detected
        break;
      }
    }

    string    tok_sent = assemble_tok_sent( one_sent );
    if( tok_sent.size() > 1024 )
      cerr << "warning: the SentenceTagger seems to be too long at line " << n << endl;

    // 2.1. Run tagging
    string    tagged = bidir_postag( tok_sent, vme, vme_chunking, dont_tokenize );

    // 2.2. Split the result and push them into the data container
    V2_STR::iterator  i_row_sent = one_sent.begin();

    V1_STR    line_items;
    tokenize( line_items, tagged, "\n" );
    for( V1_STR::iterator i_row_tagged = line_items.begin(); i_row_tagged != line_items.end(); ++i_row_tagged) {
      V1_STR    token_items;
      tokenize( token_items, *i_row_tagged, "\t" );
      
      for( V1_STR::iterator i_col_tagged = token_items.begin() + 1; i_col_tagged != token_items.end(); ++i_col_tagged )
        i_row_sent->push_back( *i_col_tagged );
  
      ++i_row_sent;
    }

    // 2.3. Output the result
    output_result( one_sent, os );
    ++n;
  }
  
  return 0;
}


// Load models of the GENIA tagger 3.0.1
int genia_init( const string &genia_dir )
{
  init_morphdic( genia_dir );
  
  cerr << "Loading pos_models";
  for ( int i = 0; i < 16; i++ ) {
    char    buf[8];
    string    fn_model = genia_dir + "pos.model.bidir.";
    sprintf(buf, "%d", i);
    fn_model += buf;
    vme[i].load_from_file( fn_model.c_str() );
    cerr << ".";
  }
  cerr << "done." << endl;

  cerr << "Loading chunk_models";
  
  for ( int i = 0; i < 8; i +=2 ) {
    char    buf[8];
    string    fn_model = genia_dir + "chunk.model.bidir.";
    sprintf( buf, "%d", i );
    fn_model += buf;
    vme_chunking[i].load_from_file(fn_model.c_str());
    cerr << ".";
  }
  cerr << "done." << endl;

  return 0;
}


// Get a SentenceTagger into a 2D string vector container
int get_sent( istream &is, V2_STR &one_sent, string &multidoc_separator, bool &separator_read )
{
  int  n_lines = 0;
  string  line = "";
  V1_STR  one_row;

  one_sent.clear();
  separator_read = false;

  while( getline( is, line ) ) {
    if( line.empty() )    // Stop when a blank line (the end of a SentenceTagger) appears
      break;

    if( line == multidoc_separator ) {  // Stop when a line containing only a document separator appears
      separator_read = true;
      break;
    }

    tokenize( one_row, line, "\t" );
    one_sent.push_back( one_row ); 

    ++n_lines;
  }

  return n_lines;
}


// Assemble a tokens into a string
string assemble_tok_sent( const V2_STR &one_sent )
{
  string  tok_sent = "";

  for( V2_STR::const_iterator i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row ) {
    tok_sent += i_row->back();
    if( (i_row + 1) != one_sent.end() )
      tok_sent += " ";
  }

  return tok_sent;
}


// Tokenize a string with given delimiters
int tokenize( V1_STR &one_seg, string &one_line, const string &del )
{
  int  total_elem = 0;
  size_t  beg = 0, end = one_line.find(del, 0);

  one_seg.clear();

  while( beg < one_line.length() )
  {
    if( (end = one_line.find(del, beg)) == string::npos )
      end = one_line.length();

    one_seg.push_back( one_line.substr(beg, end - beg) );

    beg = end + 1;
    ++total_elem;
  }

  return total_elem;
}


// Print results
void output_result( V2_STR &one_sent, ostream &os )
{
  for( V2_STR::iterator i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row ) {
    for( V1_STR::iterator i_col = i_row->begin(); i_col != i_row->end(); ++i_col ) {
      os << *i_col;
      if( (i_col + 1) != i_row->end() )
        os << "\t";
    }
    os << endl;
  }
  os <<  endl;
}


void output_usage(char *command)
{
    cerr << "Usage: " << command << " -d  <path/to/the/model/directory/>  [file(s)]" << endl;
    cerr << "  1. <path/to/the/model/directory/>" << endl;
    cerr << "    - Path to the directory in which the GENIA tagger models are stored " << endl;
	cerr << endl;
    cerr << "  2. [file(s)]" << endl;
    cerr << "    - Input file(s) can be delivered in three ways." << endl;
    cerr << "      1. re-directed standard input." << endl; 
    cerr << "      2. an input file name with -f option. (e.g. -f input.txt)" << endl;
    cerr << "      3. a file of input file names with -l option. (e.g. -l list.txt)" << endl;
    cerr << "        a file in the list file needs to have a relative path from the directory in which the list file stored." << endl;
    cerr << "    - A file consists of a beginning position, a past-the-end position and a token columns." << endl;
    cerr << "    - Each column is tab-separated." << endl;
    cerr << endl;
    cerr << "  3. multi-document mode (for tag mode) " << endl;
    cerr << "     - if -multidoc SEP option is given, looks for lines containing only the"   << endl;
    cerr << "       separator SEP in the input and echoes the same on output." << endl; 
    cerr << "       (Please do not use -f nor -l option)" << endl;

}


