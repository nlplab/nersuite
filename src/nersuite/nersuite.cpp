/*
*      NERSuite main
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

#ifdef    HAVE_CONFIG_H
#include <config.h>
#endif/*HAVE_CONFIG_H*/

#include "nersuite.h"

using namespace std;

namespace NER
{
	Suite::Suite(int nargs, char** args)
	{
		opt_parser.parse(nargs, args);
	}

	int Suite::learn()
	{
		string opt_value;

		if (opt_parser.get_value("-f", opt_value)) {
			int retval;
			ifstream ifs(opt_value.c_str());
			if (! ifs) {
				cerr << "Cannot open an input file! " << opt_value << endl;
				return -2;
			} else {
				retval = learn_crfsuite(ifs);
			}
			ifs.close();
			return retval;
		} else {
			return learn_crfsuite(cin);
		}
	}

	int Suite::learn_crfsuite(istream& is)
	{
		set_column_info(MODE_LEARN);

		int   ret = 0; //, arg_used = 0;
		FILE*	fpo = stdout;
		FILE*	fpe = stderr;

		CRFSuite::Trainer2 trainer;

		trainer.select("lbfgs", "crf1d");
		CRFSuite::StringList list = trainer.params();
		// Initialize the default sigma value
		char	name[] = "c2";
		char	value[128];
		string    c2_value = "";
		if (opt_parser.get_value("-C", c2_value))
		{
			sprintf(value, "%f", atof(c2_value.c_str()));
		}
		else
		{
			sprintf(value, "%f", 1.0);
		}
		// Set the default C2 value.
		try
		{
			trainer.set(name, value);
		}
		catch (exception& ex)
		{
			cerr << ex.what() << "\n";
			//		return -1;
		}

		/* Log the start time. */
		time_t       ts;
		char         timestamp[80];
		time(&ts);
		strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&ts));
		fprintf(fpo, "Start time of the training: %s\n", timestamp);
		fprintf(fpo, "\n");

		/* Read the training data. */
		fprintf(fpo, "Reading the training data\n");
		clock_t		clk_begin = clock();
		read_data(is , COL_INFO, &trainer);
		clock_t		clk_current = clock();

		/* Report the statistics of the training data. */
		fprintf(fpo, "Number of instances: %d\n", (int) trainer.get_instance_num());
		fprintf(fpo, "Total number of items: %d\n", (int) trainer.get_item_num());
		fprintf(fpo, "Number of attributes: %d\n", (int) trainer.get_label_num());
		fprintf(fpo, "Number of labels: %d\n", (int) trainer.get_attribute_num());
		fprintf(fpo, "Seconds required: %.3f\n", (clk_current - clk_begin) / (double)CLOCKS_PER_SEC);
		fprintf(fpo, "\n");
		fflush(fpo);

		string            m_name = DEFAULT_MODEL_FILE;
		opt_parser.get_value("-m", m_name);

		/* Start training. */
		ret = trainer.train(m_name, -1);

		/* Log the end time. */
		time(&ts);
		strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&ts));
		fprintf(fpo, "End time of the training: %s\n", timestamp);
		fprintf(fpo, "\n");

		return ret;
	}

	int Suite::tag()
	{
		string            m_name = DEFAULT_MODEL_FILE;

		opt_parser.get_value("-m", m_name);
		CRFSuite::Tagger  tagger;
		if (!tagger.open(m_name.c_str())) {
			cerr << "Cannot create a model instance from " << m_name << endl;
			return 1;
		}

		string              opt_value;
		FeatureExtractor   FExtor(COL_INFO);

		if (opt_parser.get_value("-f", opt_value)) {
			ifstream ifs(opt_value.c_str());
			if (! ifs) {
				cerr << "Cannot open an input file! " << opt_value << endl;
				return -2;
			}else {
				run_tagging(ifs, cout, tagger, FExtor);
			}
			ifs.close();
		}else if (opt_parser.get_value("-l", opt_value)) {
			string    path = "./", fn = "";
			int       idx = opt_value.find_last_of('/');
			if (idx != string::npos)
				path =  opt_value.substr(0, idx+1);

			ifstream  ifs_lst(opt_value.c_str());
			if (! ifs_lst) {
				cerr << "Cannot open a list file! " << opt_value << endl;
				return -3;
			}

			while(getline(ifs_lst, fn)) {
				string target = path + fn, result = path + fn + ".ner";

				ifstream ifs_trg(target.c_str());
				if (! ifs_trg) {
					cerr << "Cannot open a target file! " << target << endl;
					return -4;
				}
				ofstream ofs_res(result.c_str());

				run_tagging(ifs_trg, ofs_res, tagger, FExtor);

				ifs_trg.close();
				ofs_res.close();
			}

			ifs_lst.close();
		}else {
			run_tagging(cin, cout, tagger, FExtor);
		}
		return 0;
	}

	/**
	* CRF tagging functions
	**/
	int Suite::run_tagging(
		istream             &is, 
		ostream             &os,
		CRFSuite::Tagger&   tagger,
		FeatureExtractor    &FExtor
		)
	{
		map<string, int>  term_idx;
		V2_STR            one_sent;
		V2_STR            sent_feats;

		set_column_info(MODE_TAG);

		// check multidoc mode
		string multidoc_separator = "";
		bool multidoc_mode = opt_parser.get_value("-multidoc", multidoc_separator); 
		bool separator_read;

		while (! is.eof() ) {
	    // 1. Read a sentence (or comments)
			get_sent(is, one_sent, multidoc_separator, separator_read);

			// 2. Pass the input to output, if the multidoc mode is on and the read sentence is lines of comments
	    if( multidoc_mode && separator_read ) {
				for( V2_STR::iterator irow = one_sent.begin(); irow != one_sent.end(); ++irow ) {
					os << irow->front() << endl;
				}
				os << endl;
				continue;
			}
			
			// 3. Extract features
			sent_feats.clear();

			pad_answer(string("tag"), one_sent, sent_feats);            // CRFsuite needs that first columns are answer or dummy tags (in both training and test)

			FExtor.ext_WORD_feats(one_sent, sent_feats);
			FExtor.ext_LEMMA_feats(one_sent, sent_feats);
			FExtor.ext_ORTHO_feats(one_sent, sent_feats);
			FExtor.ext_POS_feats(one_sent, sent_feats);
			FExtor.ext_LEMMA_POS_feats(one_sent, sent_feats);
			FExtor.ext_CHUNK_feats(one_sent, sent_feats);

			// 3.1. Use dictionaries as a default option (the last argument turn on lexicalized dictionary features)
			FExtor.ext_DIC_feats(one_sent, sent_feats, 0);      

			// 4. Tag named entities
			tag_crfsuite(one_sent, sent_feats, tagger, term_idx, os);
		}

		return 0;
	}

	int Suite::tag_crfsuite(
		V2_STR                     &one_sent, 
		V2_STR                     &sent_feat, 
		CRFSuite::Tagger&          tagger,
		map<string, int>           &term_idx, 
		ostream                    &os
		)
	{
		int                   ret = 0;
		int                   N = 0, L = 0, lid = -1;
		clock_t               clk0, clk1;
		CRFSuite::StringList  labels;

		// Obtain the dictionary interface representing the labels in the model.
		try
		{
			labels = tagger.labels();
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what();
			return ret;
		}

		// Read the input data and assign labels.
		clk0 = clock();

		CRFSuite::ItemSequence xseq;
		for(V2_STR::iterator i = sent_feat.begin(); i != sent_feat.end(); ++i) {
			CRFSuite::Item item;
			// Attribute part (second ~ last-1 column)
			for(vector<string>::iterator j = (i->begin() + 1); j != i->end(); ++j) {
				CRFSuite::Attribute attr;
				size_t pos = j->find_first_of(":");
				if(pos == string::npos) {
					attr.attr = *j;
					attr.value = 1.0;
				}else {
					attr.attr = j->substr(0, pos);
					attr.value = atof((j->substr(pos + 1, j->length() - pos - 1).c_str()));
				}
				item.push_back(attr);
			}
			xseq.push_back(item);
		}

		// Tag the instance.
		CRFSuite::StringList yseq;
		try {
			yseq = tagger.tag(xseq);
		}
		catch (std::exception ex)
		{
			std::cerr << ex.what();
			return 0;
		}
		clk1 = clock();

		// Output tagging result
		string    out_format = "conll";
		opt_parser.get_value("-o", out_format);
		if (out_format == "standoff")
		{
			output_result_standoff(os, yseq, one_sent, term_idx);
		}
		else if (out_format == "brat")
		{
			output_result_standoff(os, yseq, one_sent, term_idx, true);
		}
		else
		{
			output_result_conll(os, yseq, one_sent);
		}
		return 1;
	}

	/**
	* Internal output function
	*/
	void Suite::output_single_standoff(
		ostream      &os,
		const string &beg,
		const string &end,
		int cnt,
		const string &ne_class,
		const string &ne_text,
		bool brat_flavored
		)
	{
		if (!brat_flavored) {
			os << beg << "\t" << end << "\t" << "entity_name" << "\t" << "id=\"entity-" << cnt 
			   << "\" " << "type=\"" << ne_class << "\"" << endl;
		}else {
			int begi = atoi(beg.c_str());
			int endi = atoi(end.c_str());
			os << "T" << cnt << "\t" << ne_class << " " << sentence_base_offset+begi << " " << sentence_base_offset+endi << "\t" << ne_text << endl;
		}
	}

	/**
	* Output functions
	*/
	void Suite::output_result_standoff(
		ostream                  &os,
		CRFSuite::StringList&    yseq,
		vector<vector<string> >  &one_sent, 
		map<string, int>         &term_idx,
		bool brat_flavored
		)
	{
		static int										cnt = 1;     // cnt counts the entity index regardless of its semantic type
		string                        ne_term = "", ne_class = "", beg = "", end = "";
		map<string, int>::iterator    check;

		int i = 0;
		for (CRFSuite::StringList::const_iterator itr = yseq.begin(); itr != yseq.end(); ++itr)
		{
			string s_label = *itr;

			// print begin position, end position, word and NE information
			if (s_label[0] == 'S') {
				s_label.replace(0, 1, string("B"));
			}else if (s_label[0] == 'E') {
				s_label.replace(0, 1, string("I"));
			}

			if (s_label == "O") {
				if (ne_term != "") {
					output_single_standoff(os, beg, end, cnt, ne_class, ne_term, brat_flavored);
					++cnt;

					ne_term = "";
				}
			}else if (s_label.substr(0, 1) == "B") {
				if (ne_term != "") {
					output_single_standoff(os, beg, end, cnt, ne_class, ne_term, brat_flavored);
					++cnt;
				}

				ne_term = one_sent[i][COL_INFO.WORD];
				ne_class = s_label.substr(2, s_label.length() - 2);
				beg = one_sent[i][COL_INFO.BEG];
				end = one_sent[i][COL_INFO.END];
			}else if (s_label.substr(0, 1) == "I") {
				if (ne_term != "") {
					if ((i != 0) && (one_sent[i - 1][1] == one_sent[i][0])) {
						ne_term = ne_term + one_sent[i][COL_INFO.WORD];
					}else if (i != 0) {
						ne_term = ne_term + " " + one_sent[i][COL_INFO.WORD];
					}

					end = one_sent[i][COL_INFO.END].c_str();
				}else {                // ***Exception) NE begins with I- label.
					ne_term = one_sent[i][COL_INFO.WORD];
					ne_class = s_label.substr(2, s_label.length() - 2);
					beg = one_sent[i][COL_INFO.BEG];
					end = one_sent[i][COL_INFO.END];
				}
			}else {
				cerr << "Can not reach here: BIO-" << endl;
				exit(1);
			}

			i++;
		}

		if (ne_term != "") {      // If the last token is "B" or "I"
			output_single_standoff(os, beg, end, cnt, ne_class, ne_term, brat_flavored);
			++cnt;

			ne_term = "";
		}
	}


	void Suite::output_result_conll(
		ostream						&os,
		CRFSuite::StringList&			yseq,
		vector< vector<string> >  &one_sent
		) 
	{
		int i = 0;
		for (CRFSuite::StringList::const_iterator itr = yseq.begin(); itr != yseq.end(); ++itr)
		{
			const string label = *itr;

			os << one_sent[i][COL_INFO.BEG] << "\t" << one_sent[i][COL_INFO.END] << "\t"
				<< one_sent[i][COL_INFO.WORD] << "\t" << one_sent[i][COL_INFO.LEMMA] << "\t"
				<< one_sent[i][COL_INFO.POS] << "\t" << one_sent[i][COL_INFO.CHUNK] << "\t"
				<< label.c_str() << endl;
			i++;
		}
		os << endl;
	}


	void Suite::read_data(
		istream              &is,
		const COLUMN_INFO    &COL_INFO,
		CRFSuite::Trainer*   trainer
		)
	{
		int               k_sents = 0;
		FeatureExtractor  FExtor(COL_INFO);

		std::cerr << "Start feature extraction" << endl;

		V2_STR	one_sent;
		V2_STR	sent_feats;
		string	_attr = "";
		string _value = "";

		string multidoc_separator = "";
		bool multidoc_mode = opt_parser.get_value("-multidoc", multidoc_separator); 
		bool separator_read;

		while( ! is.eof() ) {
	    // 1. Read a sentence (or comments)
			get_sent(is, one_sent, multidoc_separator, separator_read);

			// 2. Ignore comment lines
	    if( multidoc_mode && separator_read ) {
				continue;
			}
		
			// 3. Extract features
			sent_feats.clear();

			pad_answer(string("learn"), one_sent, sent_feats);        // CRFsuite needs that first columns are answer or dummy tags (in both training and test)

			FExtor.ext_WORD_feats(one_sent, sent_feats);
			FExtor.ext_LEMMA_feats(one_sent, sent_feats);
			FExtor.ext_ORTHO_feats(one_sent, sent_feats);
			FExtor.ext_POS_feats(one_sent, sent_feats);
			FExtor.ext_LEMMA_POS_feats(one_sent, sent_feats);
			FExtor.ext_CHUNK_feats(one_sent, sent_feats);

			FExtor.ext_DIC_feats(one_sent, sent_feats, 0);      // Use dictionaries as a default option (the last argument turn on lexicalized dictionary features)

			// 4. Train a model
			CRFSuite::ItemSequence xseq;
			CRFSuite::StringList yseq;
			for (V2_STR::const_iterator i_row = sent_feats.begin(); i_row != sent_feats.end(); ++i_row) {
				// Get a label first (IWA_ITEM #1)
				string label = (i_row->front()).c_str();

				// Attribute fields (IWA_ITEM #2)
				CRFSuite::Item item;
				for (V1_STR::const_iterator i_attr = i_row->begin() + 1; i_attr != i_row->end(); ++i_attr) {
					CRFSuite::Attribute attr;
					size_t	_pos;
					// Divide an attribute name and its value
					// Any trailing ":value" expression is optional -- if presents, designates scaling value for the feature.
					// If absent, the scalig is 1 (default).
					if ((_pos = i_attr->find_first_of(":")) != string::npos) {
						attr.attr = i_attr->substr(0, _pos);
						attr.value = atof(i_attr->substr(_pos + 1, i_attr->length() - _pos - 1).c_str());
					} else {
						attr.attr = *i_attr;
						attr.value = 1.0;
					}
					item.push_back(attr);
				}
				xseq.push_back(item);
				yseq.push_back(label);
			}
			trainer->append(xseq, yseq, 0/*group*/);

			++k_sents;
			if ((k_sents % 1000) == 0) {
				std::cerr << ".";
			}
		}
		std::cerr << endl;
	}

	/**
	* ETC functions
	*/
	static char* mystrdup(const char *src)
	{
		char *dst = (char*)malloc(strlen(src)+1);
		if (dst != NULL) {
			strcpy(dst, src);
		}
		return dst;
	}

	void Suite::set_column_info(const string &mode)
	{
		int    begin = 0;

		if(mode == "learn")  
			begin = 1;

		COL_INFO.BEG  = begin;
		COL_INFO.END  = begin + 1;
		COL_INFO.WORD  = begin + 2;
		COL_INFO.LEMMA  = begin + 3;
		COL_INFO.POS  = begin + 4;
		COL_INFO.CHUNK  = begin + 5;
		COL_INFO.DIC  = begin + 6;  // NE result of GENIA tagger is removed (if it isn't, DIC needs to be + 6)
	}

	int Suite::pad_answer(const string &mode, const V2_STR &one_sent, V2_STR &sent_feats)
	{
		vector<string>      answer_tag(1, "DUMMY");

		if (mode == "tag") {      // no answer tag
			for(V2_STR_citr i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) {
				sent_feats.push_back(answer_tag);
			}
		}else if (mode == "learn") {
			for(V2_STR_citr i_row = one_sent.begin(); i_row != one_sent.end(); ++i_row) {
				answer_tag[ 0 ] = (*i_row)[ 0 ];
				sent_feats.push_back(answer_tag);
			}
		}else {
			cerr << "invalid mode option! " << endl;
			exit(1);
		}

		return 0;
	}

}
