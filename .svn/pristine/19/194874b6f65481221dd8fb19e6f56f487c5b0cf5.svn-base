#include "sejf.h"
#include <cassert>
#include <algorithm>

using namespace std;

int main()
{
	/* Kontroler powinien nadal być związany z sejfem po swap(). */
	Sejf s1("aaa", 2);
	Sejf s2("bbb", 0);

	auto k1 = s1.kontroler();

	swap(s1, s2);

	if (k1)
		assert(false);
}
