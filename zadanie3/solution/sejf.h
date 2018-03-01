/* Autorzy rozwiązania : Michał Radecki, Kacper Konecki */

#ifndef SEJF_H
#define SEJF_H

#include "kontroler.h"
#include <cstdint>
#include <iostream>

class Kontroler;

using namespace std;

class Sejf {
    friend class Kontroler;

    friend ostream &operator<<(ostream &os, Kontroler const &kontroler);

private:
    static const uint32_t DOMYSLNA_LICZBA_DOSTEPOW = 42;
    string nazwaSejfu;
    uint32_t liczbaDostepow;
    bool wlamanie, zmanipulowany;
public:
    Sejf(string const &nazwaSejfu, uint32_t const liczbaDostepow = DOMYSLNA_LICZBA_DOSTEPOW);

    Sejf() = delete;

    Sejf(Sejf &kopiowanySejf) = delete;

    Sejf(Sejf &&przenoszonySejf) noexcept;

    int16_t operator[](uint32_t const numerLitery);

    Sejf &operator+=(uint32_t const ile);

    Sejf &operator-=(uint32_t const ile);

    Sejf &operator*=(uint32_t const ile);

    Sejf &operator=(Sejf const &sejf) = delete;

    Sejf &operator=(Sejf &&sejf);

    Kontroler const kontroler() const;
};


#endif //SEJF_H
