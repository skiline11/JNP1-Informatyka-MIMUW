#include "sejf.h"

using namespace std;

/* Test sprawdza możliwość porównywania kontrolerów. */

int main()
{
	Sejf s1("aaa", 2);
	Sejf s2("bbb", 3);

	if (s1.kontroler() < s2.kontroler())
		;
}
