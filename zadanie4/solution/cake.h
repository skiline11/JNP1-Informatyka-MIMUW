/* Autorzy rozwiązania : Michał Radecki, Adam Jabłonowski */

#ifndef PIEKARNIA2_CAKE_H
#define PIEKARNIA2_CAKE_H

#include<iostream>
#include<typeinfo>
#include<cassert>
#include<cmath>

using namespace std;

constexpr double count_ln2(double x, double krok) {
    return krok < 0.00000001 ? x : (exp(x) > 2 ? count_ln2(x - krok, krok / 2) : count_ln2(x + krok, krok / 2));
}

constexpr double LN2 = count_ln2(1, 0.5);

template<class T, T lenght, T width, class P, bool isSellable>
class Cake {
protected:
    int stock;
    P price;
public:
    Cake();

    Cake(int initialStock, P price = 0.2f) {
        assert(initialStock > 0);
        static_assert(is_integral<T>::value, "Dimension type should be floating point type\n");
        static_assert(lenght > 0 && width > 0, "Length and width should be greater than zero");
        static_assert(is_floating_point<P>::value, "Price type should be floating point type\n");
        this->stock = initialStock;
        this->price = price;
    }

    bool isApplePie() {
        return false;
    }

    static constexpr double getAreaConstexpr() {
        return lenght * width * LN2;
    }

    static double getArea() {
        return lenght * width * LN2;
    }

    int getStock() {
        return stock;
    }

    template<bool s = isSellable, typename = typename std::enable_if<s, void>::type>
    void sell() {
        if (this->stock > 0) {
            this->stock--;
        }
    };

    template<bool s = isSellable, typename = typename std::enable_if<s, void>::type>
    P getPrice() {
        return price;
    }

    using dimensions_type = T;
    using price_type = P;
    static constexpr bool is_sellable = isSellable;
};

template<class T, T length, T width>
using CheeseCake = Cake<T, length, width, float, false>;

template<class T, T length, T width, class P>
using CreamCake = Cake<T, length, width, P, true>;

#endif //PIEKARNIA2_CAKE_H
