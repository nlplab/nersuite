#include <string>
#include "../nersuite/CRFSuite2.h"

using namespace std;
using namespace CRFSuite;

string CRFSuiteDummyLog;

Trainer::Trainer()
{
	CRFSuiteDummyLog.append("Trainer::Trainer;");
}

Trainer::~Trainer()
{
	CRFSuiteDummyLog.append("Trainer::~Trainer;");
}

void Trainer::set(const std::string& name, const std::string& value)
{
	CRFSuiteDummyLog.append("Trainer::set(" + name + "," + value + ");");
}

StringList Trainer::params()
{
	CRFSuiteDummyLog.append("Trainer::params;");
	static StringList result;
	return result;
}

int Trainer::train(const std::string& model, int holdout)
{
	static char msg[1024];
	sprintf(msg, "Trainer::train(%s,%d);", model.c_str(), holdout);
	CRFSuiteDummyLog.append(msg);
	return 0;
}

bool Trainer::select(const std::string& algorithm, const std::string& type)
{
	CRFSuiteDummyLog.append("Trainer::select(" + algorithm + "," + type + ");");
	return true;
}

void Trainer::append(const ItemSequence& xseq, const StringList& yseq, int group)
{
	CRFSuiteDummyLog.append("Trainer::append;");
}

void Trainer::message(const std::string& msg)
{
	CRFSuiteDummyLog.append("Trainer::message(" + msg + ");");
}

size_t	Trainer2::get_attribute_num() const
{
	CRFSuiteDummyLog.append("Trainer2::get_attribute_num;");
	return 0;
}

size_t	Trainer2::get_label_num() const
{
	CRFSuiteDummyLog.append("Trainer2::get_label_num;");
	return 0;
}

size_t	Trainer2::get_instance_num() const
{
	CRFSuiteDummyLog.append("Trainer2::get_instance_num;");
	return 0;
}

size_t	Trainer2::get_item_num() const
{
	CRFSuiteDummyLog.append("Trainer2::get_item_num;");
	return 0;
}

void Trainer2::message(const std::string& msg)
{
	CRFSuiteDummyLog.append("Trainer2::message(" + msg + ");");
}

Tagger::Tagger()
{
	CRFSuiteDummyLog.append("Tagger::Tagger;");
}

Tagger::~Tagger()
{
	CRFSuiteDummyLog.append("Tagger::~Tagger;");
}

bool Tagger::open(const std::string& name)
{
	CRFSuiteDummyLog.append("Tagger::open(" + name + ");");
	return true;
}

StringList Tagger::labels()
{
	CRFSuiteDummyLog.append("Tagger::labels;");
	static StringList dummy;
	return dummy;
}

StringList Tagger::tag(const ItemSequence& xseq)
{
	CRFSuiteDummyLog.append("Tagger::tag;");
	static StringList dummy;
	return dummy;
}

