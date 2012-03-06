#ifndef _NERSUITE_TEST_H
#define _NERSUITE_TEST_H

#include <string>
#include <sstream>
#include <stdio.h>
#include "TestUtil.h"
#include "../nersuite/nersuite.h"
#include "fext_helper.h"

using namespace std;
using namespace NER;

void TestNERSuite_LearnArgs()
{
	char* args[] = { "-m", "model.m" };
	Suite nersuite(2, args);
}

void TestNERSuite_TagArgs()
{
	char* args[] = { "-m", "model.m" };
	Suite nersuite(2, args);
}

void TestNERSuite_Learn()
{
	FILE* fp = fopen("tmp_learn.txt", "wt");
	fputs("B-TestTag	0	2	ABC	ABC	NN	O	O	O	O	O	O	O	O	O\n", fp);
	fputs("B-TestTag	3	5	DEF	DEF	NN	O	O	O	O	O	O	O	O	O\n", fp);
	fputs("B-TestTag	6	8	GHI	GHI	NN	O	O	O	O	O	O	O	O	O\n", fp);
	fclose(fp);

	extern string CRFSuiteDummyLog;
	CRFSuiteDummyLog.clear();

	char* args[] = { "-m", "model.m", "-f", "tmp_learn.txt" };
	Suite nersuite(4, args);
	if (nersuite.learn() != 0)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}

	extern string CRFSuiteDummyLog;
	if (CRFSuiteDummyLog !=
		"Trainer::Trainer;"
		"Trainer::select(lbfgs,crf1d);"
		"Trainer::params;"
		"Trainer::set(c2,1.000000);"
		"Trainer::append;"
		"Trainer2::get_instance_num;"
		"Trainer2::get_item_num;"
		"Trainer2::get_label_num;"
		"Trainer2::get_attribute_num;"
		"Trainer::train(model.m,-1);"
		"Trainer::~Trainer;")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestNERSuite_Tag()
{
	FILE* fp = fopen("tmp_tag.txt", "wt");
	fputs("0	2	ABC	ABC	NN	O	O	O	O	O	O	O	O	O\n", fp);
	fputs("3	5	DEF	DEF	NN	O	O	O	O	O	O	O	O	O\n", fp);
	fputs("6	8	GHI	GHI	NN	O	O	O	O	O	O	O	O	O\n", fp);
	fclose(fp);

	extern string CRFSuiteDummyLog;
	CRFSuiteDummyLog.clear();

	char* args[] = { "-m", "model.m", "-f", "tmp_tag.txt" };
	Suite nersuite(4, args);
	if (nersuite.tag() != 0)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}

	if (CRFSuiteDummyLog !=
		"Tagger::Tagger;"
		"Tagger::open(model.m);"
		"Tagger::labels;"
		"Tagger::tag;"
		"Tagger::~Tagger;"
		)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

#endif