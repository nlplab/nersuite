#ifndef _SENTENCE_TAGGER_TEST
#define _SENTENCE_TAGGER_TEST

#include <string>
#include <fstream>
#include "../dictionary_tagger/sentence_tagger.h"

using namespace std;
using namespace NER;

static const char* SENTENCE_TAGGER_TEST_TEXT_FILE = "sentence_tagger_test.txt";
static const char* SENTENCE_TAGGER_TEST_DB_FILE = "sentence_tagger_test.cdbpp";


Dictionary* PrepareDictionaryTestDB(int normalize_type, const char* filename, const char* dbname)
{
	ofstream ofs(filename);
	ofs << "SampleEntry\tClass1\tClass2\tClass3" << endl;
	ofs << "S0a1m2p3l4e5E6n7t8r9y\tClass1\tClass4\tClass5" << endl;
	ofs << "S_a!m\"p#l$e%E&n\'t(r)y[S]a-m=p~l^e|E\\n@t[r]yS+a;m:p<l>e,E.n?t/r y\tClass6\tClass2\tClass7\n" << endl;
	ofs << "SampleEntry SampleEntry1 SampleEntry2\tClass8\tClass3\tClass9" << endl;
	ofs.close();

	Dictionary* pdict = new Dictionary(dbname);
	pdict->build(filename, normalize_type);
	return pdict;
}

void TestSentenceTagger_NormalizeNone()
{
	Dictionary* pdict = PrepareDictionaryTestDB(NormalizeNone, SENTENCE_TAGGER_TEST_TEXT_FILE, SENTENCE_TAGGER_TEST_DB_FILE);
	pdict->open();

	SentenceTagger::set_normalize_type(NormalizeNone);
	vector<NE>		v_ne;
	vector<int>		v_idx;
	SentenceTagger	one_sent;

	stringstream ss;
	ss << "0\t11\tSampleEntry\tSampleEntry\tNN\t-" << endl;
	ss << "12\t23\tSampleEntry1\tSampleEntry1\tNN\t-" << endl;
	ss << "24\t35\tSampleEntry2\tSampleEntry2\tNN\t-" << endl;
	ss << endl;
	one_sent.read(ss);
	one_sent.tag_nes(*pdict);
	if (one_sent[0][8] != "B-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][13] != "B-Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][14] != "B-Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[1][8] != "I-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[1][13] != "I-Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[1][14] != "I-Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[2][8] != "I-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[2][13] != "I-Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[2][14] != "I-Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);

	delete pdict;
}

void TestSentenceTagger_NormalizeCase()
{
	Dictionary* pdict = PrepareDictionaryTestDB(NormalizeCase, SENTENCE_TAGGER_TEST_TEXT_FILE, SENTENCE_TAGGER_TEST_DB_FILE);
	pdict->open();

	SentenceTagger::set_normalize_type(NormalizeCase);
	vector<NE>		v_ne;
	vector<int>		v_idx;
	SentenceTagger	one_sent;

	stringstream ss;
	ss << "0\t11\tsampleentry\tsampleentry\tNN\t-" << endl;
	ss << endl;
	one_sent.read(ss);
	one_sent.tag_nes(*pdict);
	if (one_sent[0][6] != "B-Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][7] != "B-Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][8] != "B-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);


	delete pdict;
}

void TestSentenceTagger_NormalizeSymbol()
{
	Dictionary* pdict = PrepareDictionaryTestDB(NormalizeSymbol, SENTENCE_TAGGER_TEST_TEXT_FILE, SENTENCE_TAGGER_TEST_DB_FILE);
	pdict->open();

	SentenceTagger::set_normalize_type(NormalizeSymbol);
	vector<NE>		v_ne;
	vector<int>		v_idx;
	SentenceTagger	one_sent;

	stringstream ss;
	ss << "0\t11\tS!a\"m#p$l%e&E\'n(t)r[y]S-a=m~p^l|e\\E@n[t]r+yS;a:m<p>l,e.E?n/t r_y\tsampleentry\tNN\t-" << endl;
	ss << endl;
	one_sent.read(ss);
	one_sent.tag_nes(*pdict);
	if (one_sent[0][7] != "B-Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][11] != "B-Class6")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][12] != "B-Class7")
		throw new TestException("assert failed", __FILE__, __LINE__);

	delete pdict;
}

void TestSentenceTagger_NormalizeNumber()
{
	Dictionary* pdict = PrepareDictionaryTestDB(NormalizeNumber, SENTENCE_TAGGER_TEST_TEXT_FILE, SENTENCE_TAGGER_TEST_DB_FILE);
	pdict->open();

	SentenceTagger::set_normalize_type(NormalizeNumber);
	vector<NE>		v_ne;
	vector<int>		v_idx;
	SentenceTagger	one_sent;

	stringstream ss;
	ss << "0\t11\tSampleEntry\tSampleEntry\tNN\t-" << endl;
	ss << "12\t23\tSampleEntry9\tSampleEntry9\tNN\t-" << endl;
	ss << "24\t35\tSampleEntry9\tSampleEntry9\tNN\t-" << endl;
	ss << endl;
	one_sent.read(ss);
	one_sent.tag_nes(*pdict);
	if (one_sent[0][8] != "B-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][13] != "B-Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][14] != "B-Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[1][8] != "I-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[1][13] != "I-Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[1][14] != "I-Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[2][8] != "I-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[2][13] != "I-Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[2][14] != "I-Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);

	delete pdict;
}

void TestSentenceTagger_TokenBase()
{
	Dictionary* pdict = PrepareDictionaryTestDB(NormalizeToken, SENTENCE_TAGGER_TEST_TEXT_FILE, SENTENCE_TAGGER_TEST_DB_FILE);
	pdict->open();

	SentenceTagger::set_normalize_type(NormalizeToken);
	vector<NE>		v_ne;
	vector<int>		v_idx;
	SentenceTagger	one_sent;

	stringstream ss;
	ss << "0\t11\tSampleEntry\tSampleEntry\tNN\t-" << endl;
	ss << endl;
	one_sent.read(ss);
	one_sent.tag_nes(*pdict);
	if (one_sent[0][6] != "B-Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][7] != "B-Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (one_sent[0][8] != "B-Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);

	delete pdict;
}

#endif
