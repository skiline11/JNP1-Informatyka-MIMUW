#include "sejf.h"
#include <cassert>

using namespace std;

/* Test z treści zadania */

int main()
{
	Sejf s1("aaa", 2);
	if (s1.kontroler())
		assert(true);
	else
		assert(false);
}
