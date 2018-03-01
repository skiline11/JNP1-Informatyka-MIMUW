/* Autorzy rozwiązania : Michał Radecki, Kacper Konecki */

#include <iostream>
#include "kontroler.h"


Kontroler::Kontroler(Sejf const *sejf) : sejf(sejf) {
}

ostream &operator<<(ostream &os, Kontroler const &kontroler) {
    if (kontroler.sejf->wlamanie) {
        os << "ALARM: WLAMANIE\n";
    } else if (kontroler.sejf->zmanipulowany) {
        os << "ALARM: ZMANIPULOWANY\n";
    } else os << "OK\n";
    return os;
}

Kontroler::operator bool() const {
    return sejf->liczbaDostepow > 0;
}
