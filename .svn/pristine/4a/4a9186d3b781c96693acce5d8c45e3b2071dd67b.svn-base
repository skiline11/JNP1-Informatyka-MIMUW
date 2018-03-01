#include "sejf.h"
#include <cassert>
#include <sstream>

using namespace std;

/* Test sprawdza poprawność komunikatów z kontrolera. */

static const char* ALARM_WLAMANIE = "ALARM: WLAMANIE\n";
static const char* ALARM_ZMANIPULOWANY = "ALARM: ZMANIPULOWANY\n";
static const char* ALARM_OK = "OK\n";

int main()
{
	stringstream ss;

	Sejf s1("aaa", 2);
	ss << s1.kontroler();
	assert(ss.str() == ALARM_OK);
	ss.str("");

	assert(s1[0] == 'a');
	assert(s1[0] == 'a');
	assert(s1[0] == -1);
	/* Nastąpiło włamanie. */
	ss << s1.kontroler();
	assert(ss.str() == ALARM_WLAMANIE);
	ss.str("");

	s1 *= 2;
	/* Włamanie jest ważniejsze niż manipulacja. */
	ss << s1.kontroler();
	assert(ss.str() == ALARM_WLAMANIE);
	ss.str("");

	Sejf s2("aaa", 0);
	/* Odczyt poza zakresem nie zmienia stanu. */
	assert(s2[10] == -1);
	ss << s2.kontroler();
	assert(ss.str() == ALARM_OK);
	ss.str("");

	s2 += 1;
	assert(s2[0] == 'a');
	/* Wystąpiła manipulacja, */
	ss << s2.kontroler();
	assert(ss.str() == ALARM_ZMANIPULOWANY);
	ss.str("");
	/* a potem włamanie. */
	assert(s2[0] == -1);
	/* Nastąpiło włamanie. */
	ss << s2.kontroler();
	assert(ss.str() == ALARM_WLAMANIE);
	ss.str("");
}
