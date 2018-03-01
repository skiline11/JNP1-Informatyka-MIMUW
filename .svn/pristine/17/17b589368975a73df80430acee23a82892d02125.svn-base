#include "sejf.h"
#include <cassert>

using namespace std;

/* Test sprawdza niewykonywanie operacji, które zmieniają licznik w odwrotną stronę. */

int main()
{
	Sejf s1("aaa", 2);
	s1 *= 0;

	assert(s1[0] == 'a');
	assert(s1[0] == 'a');
	assert(s1[0] == -1);
}
