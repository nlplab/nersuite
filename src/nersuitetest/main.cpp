#include <map>
#include <exception>
#include <iostream>

#include "NERSuiteTest.h"
#include "FExtorTest.h"
#include "DictionaryTest.h"
#include "SentenceTaggerTest.h"

using namespace std;

typedef void (*TestFunc)();
#define REGISTER_TESTFUNC(table, funcname)	table[#funcname] = funcname

int main()
{
	// Register Tests
	map<const char*, TestFunc>	TestFuncTable;

	REGISTER_TESTFUNC(TestFuncTable, TestNERSuite_LearnArgs);
	REGISTER_TESTFUNC(TestFuncTable, TestNERSuite_TagArgs);
	REGISTER_TESTFUNC(TestFuncTable, TestNERSuite_Tag);
	REGISTER_TESTFUNC(TestFuncTable, TestNERSuite_Learn);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorWordFeature);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorLemmaFeature);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Greek);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Comma);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Period);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Hyphon);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Slash);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_OpenSB);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_CloseSB);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_OpenP);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_CloseP);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Colon);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Semicolon);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Percentage);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorOrthoFeature_Apostrophe);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorPOSFeature);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorLemmaPOSFeature);
	REGISTER_TESTFUNC(TestFuncTable, TestFExtorChunkFeature);
	REGISTER_TESTFUNC(TestFuncTable, TestDictionaryBuild_NoNormalization);
	REGISTER_TESTFUNC(TestFuncTable, TestDictionaryBuild_CaseNormalization);
	REGISTER_TESTFUNC(TestFuncTable, TestDictionaryBuild_CaseAndNumberNormalization);
	REGISTER_TESTFUNC(TestFuncTable, TestDictionaryBuild_CaseAndNumberAndSymbolNormalization);
	REGISTER_TESTFUNC(TestFuncTable, TestDictionaryBuild_TokenizerNormalization);
	REGISTER_TESTFUNC(TestFuncTable, TestSentenceTagger_NormalizeNone);
	REGISTER_TESTFUNC(TestFuncTable, TestSentenceTagger_NormalizeCase);
	REGISTER_TESTFUNC(TestFuncTable, TestSentenceTagger_NormalizeNumber);
	REGISTER_TESTFUNC(TestFuncTable, TestSentenceTagger_NormalizeSymbol);
	REGISTER_TESTFUNC(TestFuncTable, TestSentenceTagger_TokenBase);
	REGISTER_TESTFUNC(TestFuncTable, TestSentenceTagger_TokenBase);

	// Run the Test List
	size_t ntests = TestFuncTable.size();
	size_t n = 0;
	size_t success_count = 0;
	for (map<const char*, TestFunc>::iterator i = TestFuncTable.begin(); i != TestFuncTable.end(); ++i)
	{
		n ++;
		try
		{
			(*(i->second))();
		}
		catch (exception* ex)
		{
			cerr << "[" << n << "/" << ntests << "] Failed in test: " << (i->first) << endl;
			const char* p = ex->what();
			cerr << ex->what() << endl;
			delete ex;
			continue;
		}
		cerr << "[" << n << "/" << ntests << "] Passed : " << (i->first) << endl;
		success_count ++;
	}

	cerr << endl << "Test Completed.  Total: " << n << ", Success: " << success_count << endl;
	return 0;
}
