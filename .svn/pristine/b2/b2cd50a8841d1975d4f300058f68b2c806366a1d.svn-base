#include "sejf.h"
#include <cassert>

using namespace std;

/* Test sprawdza poprawność liczników. */

int main()
{
	Sejf s1("aaa", 2);
	assert(s1[0] == 'a');
	assert(s1[0] == 'a');
	assert(s1[0] == -1);

	Sejf s2("aaa", 0);
	assert(s2[0] == -1);

	Sejf s3("aaa", 3);
	assert(s3[0] == 'a');
	/* Teraz licznik jest 2. */
	s3 += 2;
	/* Teraz licznik jest 4. */
	s3 *= 3;
	/* Teraz licznik jest 12. */
	s3 -= 1;
	/* Teraz licznik jest 11. */
	for (int i = 0; i < 11; ++i)
		assert(s3[0] == 'a');

	assert(s3[0] == -1);
}
