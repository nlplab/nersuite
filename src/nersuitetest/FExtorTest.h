#ifndef _FEXTOR_TEST_H
#define _FEXTOR_TEST_H

#include <string>
#include <sstream>
#include <stdio.h>
#include "TestUtil.h"
#include "../nersuite/FExtor.h"
#include "fext_helper.h"

using namespace std;
using namespace NER;

void TestFExtorWordFeature()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "2", "ABC", "ABC", "N", "O", "O"));
	sent.push_back(CreateWordHelper("3", "5", "DEF", "DEF", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_WORD_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 2)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"2gram=$A,2gram=AB,2gram=BC,2gram=C$,"
		"3gram=$$A,3gram=$AB,3gram=ABC,3gram=BC$,3gram=C$$,"
		"4gram=$$$A,4gram=$$AB,4gram=$ABC,4gram=ABC$,4gram=BC$$,4gram=C$$$,"
		"W_U_C=ABC,W_NU_C=ABC,"
		"W_U_R1=DEF,W_NU_R1=DEF,"
		"W_B_L1C=NULL/ABC,W_NB_L1C=NULL/ABC,"
		"W_B_CR1=ABC/DEF,W_NB_CR1=ABC/DEF,"
		"W_B_R1R2=DEF/NULL,W_NB_R1R2=DEF/NULL,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[1] != 
		"2gram=$D,2gram=DE,2gram=EF,2gram=F$,"
		"3gram=$$D,3gram=$DE,3gram=DEF,3gram=EF$,3gram=F$$,"
		"4gram=$$$D,4gram=$$DE,4gram=$DEF,4gram=DEF$,4gram=EF$$,4gram=F$$$,"
		"W_U_L1=ABC,W_NU_L1=ABC,"
		"W_U_C=DEF,W_NU_C=DEF,"
		"W_B_L2L1=NULL/ABC,W_NB_L2L1=NULL/ABC,"
		"W_B_L1C=ABC/DEF,W_NB_L1C=ABC/DEF,"
		"W_B_CR1=DEF/NULL,W_NB_CR1=DEF/NULL,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorLemmaFeature()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "2", "ABC", "ABC", "N", "O", "O"));
	sent.push_back(CreateWordHelper("3", "5", "DEF", "DEF", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_LEMMA_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 2)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"L_U_C=ABC,L_NU_C=ABC,"
		"L_U_R1=DEF,L_NU_R1=DEF,"
		"L_B_L1C=NULL/ABC,L_NB_L1C=NULL/ABC,"
		"L_B_CR1=ABC/DEF,L_NB_CR1=ABC/DEF,"
		"L_B_R1R2=DEF/NULL,L_NB_R1R2=DEF/NULL,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[1] != 
		"L_U_L1=ABC,L_NU_L1=ABC,"
		"L_U_C=DEF,L_NU_C=DEF,"
		"L_B_L2L1=NULL/ABC,L_NB_L2L1=NULL/ABC,"
		"L_B_L1C=ABC/DEF,L_NB_L1C=ABC/DEF,"
		"L_B_CR1=DEF/NULL,L_NB_CR1=DEF/NULL,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "2", "ABC", "ABC", "N", "O", "O"));
	sent.push_back(CreateWordHelper("3", "5", "123", "DEF", "N", "O", "O"));
	sent.push_back(CreateWordHelper("6", "8", "Abc", "Abc", "N", "O", "O"));
	sent.push_back(CreateWordHelper("9", "14", "abc012", "abc012", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 4)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,"
		"O_ONLY_CAPnDIGITS,"
		"O_NO_LOWER_LETTERS,"
		"O_ONLY_CAPS,"
		"O_IN_CAP,"
		"O_2_IN_CAPS,"
		"O_LEN=3,"
		"O_LEN_n_NO_LOW_LETTERS=3/O,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[1] != 
		"O_BEG_CAP=X,"
		"O_ONLY_DIGITS,"
		"O_SOME_DIGITS,"
		"O_ONLY_CAPnDIGITS,"
		"O_NO_LOWER_LETTERS,"
		"O_LEN=3,"
		"O_LEN_n_NO_LOW_LETTERS=3/O,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[2] !=
		"O_BEG_CAP=O,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[3] !=
		"O_BEG_CAP=X,O_SOME_DIGITS,O_ONLY_ALPHANUMERIC,O_LEN=6,O_LEN_n_NO_LOW_LETTERS=6/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Greek()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "5", "alpha", "alpha", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_GREEK_A2Z=alpha/,O_LEN=5,O_LEN_n_NO_LOW_LETTERS=5/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Comma()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a,b", "a,b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_COMMAS,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Period()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a.b", "a,b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_PERIODS,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Hyphon()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a-b", "a,b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_HYPHONS,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Slash()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a/b", "a/b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_SLASHES,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_OpenSB()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "[ab", "[ab", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_OpenSB,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_CloseSB()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "ab]", "ab]", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_CloseSB,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_OpenP()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "(ab", "(ab", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_OpenP,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_CloseP()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "ab)", "ab)", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_CloseP,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Colon()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a:b", "a:b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_COLONS,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Semicolon()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a;b", "a;b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_SemiCOLONS,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorOrthoFeature_Percentage()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a%b", "a%b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_PERCENTAGE,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}


void TestFExtorOrthoFeature_Apostrophe()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "3", "a'b", "a'b", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_ORTHO_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"O_BEG_CAP=B,O_APOSTROPHE,O_LEN=3,O_LEN_n_NO_LOW_LETTERS=3/X,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}



void TestFExtorPOSFeature()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "2", "ABC", "ABC", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_POS_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"P_U_C=N,P_B_L1C=NULL/N,P_B_CR1=N/NULL,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorLemmaPOSFeature()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "2", "ABC", "ABC", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_LEMMA_POS_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 1)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"LP_U_C=ABC_N,LP_B_L1C=NULL_NULL/ABC_N,LP_B_CR1=ABC_N/NULL_NULL,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

void TestFExtorChunkFeature()
{
	COLUMN_INFO cinfo;
	SetupColumnInfoHelper(cinfo);

	FeatureExtractor fex(cinfo);
	V2_STR sent;
	sent.push_back(CreateWordHelper("0", "2", "ABC", "ABC", "N", "B-NP", "O"));
	sent.push_back(CreateWordHelper("3", "5", "DEF", "DEF", "N", "O", "O"));

	V2_STR feats;
	vector<string> feat;
	for (int i = 0; i < sent.size(); i++)
	{
		feats.push_back(feat);
	}
	// Test target function
	fex.ext_CHUNK_feats(sent, feats);

	// Test result
	vector<string> feats_str;
	FeatureToStringHelper(feats, feats_str);
	if (feats_str.size() != 2)
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[0] !=
		"CH_TYPE=B-NP,CH_C_LW=ABC,CH_C_LL=ABC,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
	if (feats_str[1] !=
		"CH_TYPE=O,CH_C_LW=DEF,CH_C_LL=DEF,")
	{
		throw new TestException("assert failed", __FILE__, __LINE__);
	}
}

#endif
