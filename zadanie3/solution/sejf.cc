/* Autorzy rozwiązania : Michał Radecki, Kacper Konecki */

#include <iostream>
#include "sejf.h"

using namespace std;

Sejf::Sejf(string const &nazwaSejfu, uint32_t const liczbaDostepow) :
        nazwaSejfu(nazwaSejfu),
        liczbaDostepow(liczbaDostepow),
        wlamanie(false),
        zmanipulowany(false) {
}

Sejf::Sejf(Sejf &&przenoszonySejf) noexcept :
        nazwaSejfu(std::move(przenoszonySejf.nazwaSejfu)),
        liczbaDostepow(przenoszonySejf.liczbaDostepow),
        wlamanie(przenoszonySejf.wlamanie),
        zmanipulowany(przenoszonySejf.zmanipulowany) {
}

int16_t Sejf::operator[](uint32_t const numerLitery) {
    int16_t wartosc;
    if (numerLitery < nazwaSejfu.size()) {
        if (this->liczbaDostepow > 0) {
            wartosc = (unsigned char) this->nazwaSejfu[numerLitery];
            this->liczbaDostepow--;
        } else {
            wartosc = -1;
            this->wlamanie = true;
        }
    } else {
        wartosc = -1;
    }
    return wartosc;
}

Sejf &Sejf::operator+=(uint32_t const ile) {
    if (UINT32_MAX - ile >= this->liczbaDostepow) {
        this->liczbaDostepow += ile;
        this->zmanipulowany = true;
    }
    return *this;
}

Sejf &Sejf::operator-=(uint32_t const ile) {
    if (this->liczbaDostepow >= ile) {
        this->liczbaDostepow -= ile;
        this->zmanipulowany = true;
    }
    return *this;
}

Sejf &Sejf::operator*=(uint32_t const ile) {
    if ((this->liczbaDostepow == 0) ||
        (ile >= 1 && this->liczbaDostepow <= UINT32_MAX / ile)) {
        this->liczbaDostepow *= ile;
        this->zmanipulowany = true;
    }
    return *this;
}

Sejf &Sejf::operator=(Sejf &&sejf) {
    swap(this->nazwaSejfu, sejf.nazwaSejfu);
    this->liczbaDostepow = sejf.liczbaDostepow;
    this->wlamanie = sejf.wlamanie;
    this->zmanipulowany = sejf.zmanipulowany;
    return *this;
}


Kontroler const Sejf::kontroler() const {
    return Kontroler(this);
}
