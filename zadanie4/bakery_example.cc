#include <iostream>
#include <iomanip>
#include "bakery.h"
#include "pie.h"
#include "cake.h"

int main() {
    //==== Bakery ====
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie <int, 1>,
        ApplePie<int, 1, float>,
        ApplePie<int, 2, float>,
        CheeseCake<int, 2, 3>
    >(
        CherryPie<int, 1>(3),
        ApplePie<int, 1, float>(3, 2.4f),
        ApplePie<int, 2, float>(2, 4.5f),
        CheeseCake<int, 2, 3>(3)
    );

    bakery.sell<ApplePie<int, 2, float>>();
    std::cout << bakery.getProfits() << std::endl;
    std::cout << bakery.getProductStock<ApplePie<int, 2, float>>() << std::endl;

    bakery.sell<ApplePie<int, 2, float>>();
    std::cout << bakery.getProfits() << std::endl;
    std::cout << bakery.getProductStock<ApplePie<int, 2, float>>() << std::endl;

    bakery.restock<ApplePie<int, 2, float>>(4);
    std::cout << bakery.getProductStock<ApplePie<int, 2, float>>() << std::endl;

/* Ten program wypisuje:
4.5
1
9
0
4
*/

/* Poniższe przykłady nie kompilują się. */

//    auto bakery2 = Bakery<
//        int, // Nie jest typem zmiennoprzecinkowym.
//        int,
//        10,
//        CherryPie <int, 1>
//    >(
//        CherryPie<int, 1>(3)
//    );

//    auto bakery3 = Bakery<
//        float,
//        float, // Nie jest typem całkowitym.
//        10,
//        CherryPie<int, 1>
//    >(
//        CherryPie<int, 1>(3)
//    );

//    auto bakery4 = Bakery<
//        float,
//        int,
//        10,
//        CherryPie<int, 1>,
//        CherryPie<int, 1> // Typy nie mogą się powtarzać.
//    >(
//        CherryPie<int, 1>(3),
//        CherryPie<int, 1>(4)
//    );

//    auto bakery5 = Bakery<
//        float,
//        int,
//        1, // Za mała przestrzeń na półkach.
//        CherryPie<int, 2>
//    >(
//        CherryPie<int, 2>(3)
//    );

//    auto bakery6 = Bakery<
//        float,
//        long, // Niekompatybilne typy wymiaru w produktach.
//        10,
//        CherryPie<int, 1>
//    >(
//        CherryPie<int, 1>(3)
//    );

//    auto bakery7 = Bakery<
//        double, // Niekompatybilne typy ceny w produktach.
//        int,
//        10,
//        ApplePie<int, 1, float>
//    >(
//        ApplePie<int, 1, float>(3, 1.4f)
//    );

//    bakery.sell<CherryPie<int, 1>>(); // CherryPie nie jest na sprzedaż.
//    bakery.sell<ApplePie<int, 4, float>>(); // Takiego typu nie ma na stanie.
//    bakery.getProductStock<ApplePie<int, 4, float>>(); // Takiego typu nie ma na stanie.
//    bakery.restock<CherryPie<int, 1>>(1); // Tylko typy szablonu ApplePie mogą mieć uzupełniony stan magazynowy.
//    bakery.restock<ApplePie<int, 4, float>>(1); // Takiego typu nie ma na stanie.

//==== Pie ====
//    auto cherryPie = CherryPie<float, 1>(3); // Typ promienia musi być liczbą całkowitą.
//    auto applePie = ApplePie<int, 1, int>(3, 1); // Typ ceny musi być liczbą zmiennoprzecinkową.

//==== Cake ====
//    auto cheeseCake = CheeseCake<float, 1, 1>(3); // Typ wymiarów musi być liczbą całkowitą.
//    auto creamCake = CreamCake<int, 1, 1, int>(3, 1); // Typ ceny musi być liczbą zmiennoprzecinkową.
}