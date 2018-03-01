/* Autorzy rozwiązania : Michał Radecki, Wojciech Ciszewski */

#include "cdictglobal"

namespace {
    unsigned long id = ::jnp1::dict_global();
}

int main() {
    return id != 0;
}
