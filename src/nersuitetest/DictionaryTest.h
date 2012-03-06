#ifndef _DICTIONARY_TEST
#define _DICTIONARY_TEST

#include <string>
#include <fstream>
#include "../nersuite_common/dictionary.h"

using namespace std;
using namespace NER;

static const char* DICTIONARY_TEST_TEXT_FILE = "dictionary_test.txt";
static const char* DICTIONARY_TEST_DB_FILE = "dictionary_test.cdbpp";

void PrepareDictionaryTestData(const char* filename)
{
	ofstream ofs(filename);
	ofs << "SampleEntry\tClass1\tClass2\tClass3" << endl;
	ofs << "S0a1m2p3l4e5E6n7t8r9y\tClass1\tClass4\tClass5" << endl;
	ofs << "S_a!m\"p#l$e%E&n\'t(r)y[S]a-m=p~l^e|E\\n@t[r]yS+a;m:p<l>e,E.n?t/r y\tClass6\tClass2\tClass7\n" << endl;
	ofs << "SampleEntry SampleEntry1 SampleEntry2\tClass8\tClass3\tClass9" << endl;
	ofs.close();
}

void TestDictionaryBuild_NoNormalization()
{
	PrepareDictionaryTestData(DICTIONARY_TEST_TEXT_FILE);
	Dictionary dict(DICTIONARY_TEST_DB_FILE);
	dict.build(DICTIONARY_TEST_TEXT_FILE, NormalizeNone);

	dict.open();
	size_t count;

	const int* classes = dict.get_classes("SampleEntry", NormalizeNone, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("S0a1m2p3l4e5E6n7t8r9y", NormalizeNone, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class4")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class5")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("S_a!m\"p#l$e%E&n\'t(r)y[S]a-m=p~l^e|E\\n@t[r]yS+a;m:p<l>e,E.n?t/r y", NormalizeNone, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class6")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class7")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("SampleEntry SampleEntry1 SampleEntry2", NormalizeNone, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
}

void TestDictionaryBuild_CaseNormalization()
{
	PrepareDictionaryTestData(DICTIONARY_TEST_TEXT_FILE);
	Dictionary dict(DICTIONARY_TEST_DB_FILE);
	dict.build(DICTIONARY_TEST_TEXT_FILE, NormalizeCase);

	dict.open();
	size_t count;

	const int* classes = dict.get_classes("sampleentry", NormalizeCase, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("s0a1m2p3l4e5e6n7t8r9y", NormalizeCase, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class4")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class5")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("s_a!m\"p#l$e%e&n\'t(r)y[s]a-m=p~l^e|e\\n@t[r]ys+a;m:p<l>e,e.n?t/r y", NormalizeCase, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class6")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class7")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("sampleentry sampleentry1 sampleentry2", NormalizeCase, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
}

void TestDictionaryBuild_CaseAndNumberNormalization()
{
	PrepareDictionaryTestData(DICTIONARY_TEST_TEXT_FILE);
	Dictionary dict(DICTIONARY_TEST_DB_FILE);
	dict.build(DICTIONARY_TEST_TEXT_FILE, NormalizeCase|NormalizeNumber);

	dict.open();
	size_t count;

	const int* classes = dict.get_classes("sampleentry", NormalizeCase|NormalizeNumber, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("s0a0m0p0l0e0e0n0t0r0y", NormalizeCase|NormalizeNumber, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class4")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class5")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("s_a!m\"p#l$e%e&n\'t(r)y[s]a-m=p~l^e|e\\n@t[r]ys+a;m:p<l>e,e.n?t/r y", NormalizeCase|NormalizeNumber, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class6")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class7")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("sampleentry sampleentry0 sampleentry0", NormalizeCase|NormalizeNumber, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
}

void TestDictionaryBuild_CaseAndNumberAndSymbolNormalization()
{
	PrepareDictionaryTestData(DICTIONARY_TEST_TEXT_FILE);
	Dictionary dict(DICTIONARY_TEST_DB_FILE);
	dict.build(DICTIONARY_TEST_TEXT_FILE, NormalizeCase|NormalizeNumber|NormalizeSymbol);

	dict.open();
	size_t count;

	const int* classes = dict.get_classes("sampleentry", NormalizeCase|NormalizeNumber|NormalizeSymbol, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("s0a0m0p0l0e0e0n0t0r0y", NormalizeCase|NormalizeNumber|NormalizeSymbol, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class4")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class5")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("s_a_m_p_l_e_e_n_t_r_y_s_a_m_p_l_e_e_n_t_r_ys_a_m_p_l_e_e_n_t_r_y", NormalizeCase|NormalizeNumber|NormalizeSymbol, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class6")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class7")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("sampleentry_sampleentry0_sampleentry0", NormalizeCase|NormalizeNumber|NormalizeSymbol, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);
}

void TestDictionaryBuild_TokenizerNormalization()
{
	PrepareDictionaryTestData(DICTIONARY_TEST_TEXT_FILE);
	Dictionary dict(DICTIONARY_TEST_DB_FILE);
	dict.build(DICTIONARY_TEST_TEXT_FILE, NormalizeToken);

	dict.open();
	size_t count;

	const int* classes = dict.get_classes("SampleEntry", NormalizeToken, &count);
	if (count != 5)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class1")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class2")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[3]) != "Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[4]) != "Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("SampleEntry1", NormalizeToken, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);

	classes = dict.get_classes("SampleEntry2", NormalizeToken, &count);
	if (count != 3)
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[0]) != "Class8")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[1]) != "Class3")
		throw new TestException("assert failed", __FILE__, __LINE__);
	if (dict.get_class_name(classes[2]) != "Class9")
		throw new TestException("assert failed", __FILE__, __LINE__);


}
#endif
