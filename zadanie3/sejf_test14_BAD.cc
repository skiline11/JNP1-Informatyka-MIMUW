#include "sejf.h"

using namespace std;

/* Test sprawdza możliwość kopiowania sejfów. */

int main()
{
	Sejf s1("aaa", 2);
	Sejf s4("bbb", 3);
	s4 = s1;
}
