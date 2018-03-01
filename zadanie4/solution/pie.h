/* Autorzy rozwiązania : Michał Radecki, Adam Jabłonowski */

#ifndef PIEKARNIA2_PIE_H
#define PIEKARNIA2_PIE_H

#include<iostream>
#include<typeinfo>
#include<cassert>
#include<cmath>

using namespace std;

//Nilakantha series method
constexpr double count_pi(double x) {
    return x < 400 ? 4 / ((2 * x * (2 * x + 1) * (2 * x + 2))) - count_pi(x + 1) : 0;
    // Recurrence can have max 512 levels, used 400, 100 gives precision of 6 digits
};
constexpr double PI = 3 + count_pi(1);

template<class R, R radius, class P, bool isSellable>
class Pie {
protected:
    int stock;
    P price;
public:
    Pie();

    Pie(int initialStock, P price = 0.2f) {
        assert(initialStock > 0);
        static_assert(is_integral<R>::value, "Dimension type must be an integral type\n");
        static_assert(is_floating_point<P>::value, "Price type must be floating point type\n");
        this->stock = initialStock;
        this->price = price;
    }

    bool isApplePie() {
        return isSellable;
    }

    static constexpr double getAreaConstexpr() {
        return PI * radius * radius;
    }

    static double getArea() {
        return PI * radius * radius;
    }

    int getStock() {
        return stock;
    }

    template<bool s = isSellable, typename = typename std::enable_if<s, void>::type>
    void sell() {
        if (this->stock > 0) {
            this->stock--;
        }
    }

    template<bool s = isSellable, typename = typename std::enable_if<s, void>::type>
    P getPrice() {
        return price;
    }

    template<bool s = isSellable, typename = typename std::enable_if<s, void>::type>
    // dodana funkcja na potrzeby bakery.h aby móc robić restock na ApplePie
    void restock(int additionalStock) {
        this->stock += additionalStock;
    };


    using dimensions_type = R;
    using price_type = P;
    static constexpr bool is_sellable = isSellable;

};

template<class R, R radius>
using CherryPie = Pie<R, radius, float, false>;

template<class R, R radius, class P>
using ApplePie = Pie<R, radius, P, true>;

#endif //PIEKARNIA2_PIE_H
