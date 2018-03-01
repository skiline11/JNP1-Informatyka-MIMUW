/* Autorzy rozwiązania : Michał Radecki, Kacper Konecki */

#ifndef KONTROLER_H
#define KONTROLER_H

#include "sejf.h"
#include <iostream>

using namespace std;

class Sejf;

class Kontroler {
    friend ostream &operator<<(ostream &os, Kontroler const &kontroler);

    friend class Sejf;

private:
    Sejf const *sejf;

    explicit Kontroler(Sejf const *sejf);

public:
    Kontroler() = delete;

    Kontroler(Kontroler const &kontroler) = delete;

    Kontroler(Kontroler const &&kontroler) = delete;

    Kontroler &operator=(Kontroler const &rhs) = delete;

    Kontroler &operator=(Kontroler const &&rhs) = delete;

    explicit operator bool() const;
};

ostream &operator<<(ostream &os, Kontroler const &kontroler);

#endif //KONTROLER_H
