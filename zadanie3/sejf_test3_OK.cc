#include "sejf.h"
#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std;

static const char* ALARM_WLAMANIE = "ALARM: WLAMANIE\n";

/* Test sprawdza możliwość zamiany sejfów. */

int main()
{
	Sejf s1("aaa", 2);
	Sejf s2("bbb", 3);

	std::swap(s1, s2);

	assert(s2[0] == 'a');
	assert(s2[0] == 'a');
	assert(s2[0] == -1);

	assert(s1[0] == 'b');
	assert(s1[0] == 'b');
	assert(s1[0] == 'b');
	assert(s1[0] == -1);

	/* Sprawdzamy konstruktor przenoszący... */
	Sejf s3(Sejf("ab", 2));
	assert(s3[0] == 'a');
	assert(s3[0] == 'a');
	assert(s3[0] == -1);

	s3 = Sejf("bb", 2);
	assert(s3[0] == 'b');
	assert(s3[0] == 'b');
	assert(s3[0] == -1);


	/* Kontroler po swap() powinien korzystać z nowego stanu wewnętrznego. */
	Sejf a("A", 1), b("B", 1);
	stringstream ss;
	b += 1;
	auto k = b.kontroler();
	a[0]; a[0];
	swap(a, b);
	ss << k;
	assert(ss.str() == ALARM_WLAMANIE);
}
