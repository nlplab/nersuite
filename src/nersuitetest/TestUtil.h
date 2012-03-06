#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H

#include <exception>
#include <string>
#include <sstream>

using namespace std;

class TestException : public exception
{
private:
	string m_What;

public:
	TestException(const char* msg, const char* file, int line)
	{
		ostringstream str;
		str << file << "(" << line << ") : " << msg;
		m_What = str.str();
	}

	virtual const char* what() const
	{
		return m_What.c_str();
	}
};

#endif