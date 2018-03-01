#include "sejf.h"
#include <cassert>
#include <sstream>

using namespace std;

/* Test z treści zadania */

int main()
{
	stringstream ss;
	Sejf s1("aaa", 2);
	s1[2];
	auto k1 = s1.kontroler();
	ss << k1 << "test\n";
	s1[2];
	s1[3];
	s1[4];
	ss << k1;
	s1[3];
	ss << k1;
	s1[2];
	ss << k1;

	assert(ss.str() == "OK\ntest\nOK\nOK\nALARM: WLAMANIE\n");
}
