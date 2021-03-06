/* Autorzy rozwiązania : Michał Radecki, Adam Jabłonowski */

#ifndef PIEKARNIA2_BAKERY_H
#define PIEKARNIA2_BAKERY_H

#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <type_traits>


#include "cake.h"
#include "pie.h"

using namespace std;

//checks if types are unique
template <class...>
struct uniqness_check;

template <class A, class B, class ...Rest>
struct uniqness_check<A, B, Rest...> {
	static constexpr bool value = (
	uniqness_check<A, B>::value
	&& uniqness_check<A, Rest...>::value
	&& uniqness_check<B, Rest...>::value);
};

template <class A, class B>
struct uniqness_check<A, B> {
	static constexpr bool value = !is_same<A, B>::value;
};

template <class A>
struct uniqness_check<A> {
	static constexpr bool value = true;
};

//counts area of types
template <class...>
struct types_area;

template <class A, class ...P>
struct types_area<A, P...> {
	static constexpr double value = (
	A::getAreaConstexpr() + types_area<P...>::value);
};

template <class A>
struct types_area<A> {
	static constexpr double value = A::getAreaConstexpr();
};

//checks prices type correctness
template <class...>
struct price_check;

template <class Price>
struct price_check<Price> {
	static const bool value = true;
};

template <class Price, class A, class... P>
struct price_check<Price,A,P...> {
	static const bool value = !A::is_sellable || is_same<Price, class A::price_type>::value && price_check<Price,P...>::value;
};

//checks dimensions type correctness
template <class...>
struct dimensions_check;

template <class C>
struct dimensions_check<C> {
	static const bool value = true;
};

template <class C, class A, class... P>
struct dimensions_check<C,A, P...> {
	static const bool value = is_same<C, class A::dimensions_type>::value
	                          && dimensions_check<C,P...>::value;
};

template <class C, class A, A shelfArea, class... P>
class Bakery {
private:
	C profits = 0;
	tuple<P ...> product;

	static_assert(std::is_floating_point<C>::value, "Profits must be a floating point type");
	static_assert(std::is_integral<A>::value, "Shelf area must be an integral type");
	static_assert(uniqness_check<P...>::value, "Products must be unique types.");
	static_assert(types_area<P...>::value <= shelfArea, "Products area must be smaller equal shelf area.");
	static_assert(dimensions_check<A,P...>::value, "Products dimensions must be shelf area type.");
	static_assert(price_check<C,P...>::value,"Products prices must be profits type.");

public:
	Bakery(P ... products) : product(products...) {
	}
	C getProfits() {
		return profits;
	}
	template <class Product>
	int getProductStock() {
		return get<Product>(product).getStock();
	}
	template <class Product>
	void sell() {
		if (getProductStock<Product>() > 0) {
			get<Product>(product).sell();
			profits += get<Product>(product).getPrice();
		}
	}
	template <class Product>
	void restock(int additionalStock) {
		assert(get<Product>(product).isApplePie()); // sprawdzam czy jest typem ApplePie
		get<Product>(product).restock(additionalStock);
	}
};

#endif //PIEKARNIA2_BAKERY_H
