#include "sejf.h"

using namespace std;

/* Test sprawdza możliwość tworzenia kontrolera. */

int main()
{
	Sejf s1("aaa", 2);
	auto k1 = s1.kontroler();
	decltype(k1) k2(k1);
}
