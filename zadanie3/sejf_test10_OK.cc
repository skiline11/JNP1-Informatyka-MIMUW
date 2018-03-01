#include "sejf.h"
#include <cassert>
#include <sstream>

using namespace std;

static const char* ALARM_ZMANIPULOWANY = "ALARM: ZMANIPULOWANY\n";

/* Manipulacja o 0 też jest manipulacją (bo kończy się sukcesem). */

int main()
{
	stringstream ss;

	Sejf s1("aaa", 2);
	s1 += 0;
	ss << s1.kontroler();
	assert(ss.str() == ALARM_ZMANIPULOWANY);
	ss.str("");
	
	Sejf s2("aaa", 2);
	s2 -= 0;
	ss << s2.kontroler();
	assert(ss.str() == ALARM_ZMANIPULOWANY);
	ss.str("");
	
	Sejf s3("aaa", 2);
	s3 *= 1;
	ss << s3.kontroler();
	assert(ss.str() == ALARM_ZMANIPULOWANY);
	ss.str("");
}
