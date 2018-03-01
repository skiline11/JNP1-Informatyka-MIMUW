#include "sejf.h"
#include <cassert>

using namespace std;

/* Test z treści zadania */

int main()
{
	Sejf s("a\xff", 3);
	assert(s[0] == 'a');
	assert(s[1] == 255);
	assert(s[1] == (int)((unsigned char) -1));
	assert(s[1] == -1);
	assert(s[1] != (int)((unsigned char) -1));
}
