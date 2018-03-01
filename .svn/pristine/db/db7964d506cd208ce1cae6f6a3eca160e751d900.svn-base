#include "pie.h"
#include "cake.h"
#include "bakery.h"
#include <cassert>
#include <iomanip>

// Opis testów
// grupa 1 - tworzenie obiektów
// grupa 2 - poprawność funkcji dostępowych
// grupa 3 - walidacje parametrów szablonów dla produktów
// grupa 4 - walidacje parametrów szablonu dla piekarni
// grupa 5 - poprawność funkcji sell
// grupa 6 - poprawność funkcji restock

#if TEST_NUM == 203
constexpr double PI_VALUE = 3.141593;
#endif
#if TEST_NUM == 204
constexpr double LN2_VALUE = 0.693147;
#endif
#if TEST_NUM == 203 || TEST_NUM == 204
constexpr double EPSILON = 0.000001;
#endif

using namespace std;

int main() {
    #if TEST_NUM == 101
    //Poprawne tworzenie placków
    ApplePie<int, 1, float>(3, 1.3f);
    CherryPie<int, 1>(1);
    #endif

    #if TEST_NUM == 102
    //Poprawne tworzenie ciast
    CreamCake<int, 1, 1, float>(3, 1.3f);
    CheeseCake<int, 1, 1>(1);
    #endif

    #if TEST_NUM == 103
    //Poprawne tworzenie piekarni
    Bakery<
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
    #endif

    #if TEST_NUM == 104
    //Błąd kompilacji przy pustym konstruktorze
    AppliePie();
    #endif

    #if TEST_NUM == 105
    //Błąd kompilacji przy pustym konstruktorze
    CherryPie();
    #endif

    #if TEST_NUM == 106
    //Błąd kompilacji przy pustym konstruktorze
    CheeseCake();
    #endif

    #if TEST_NUM == 107
    //Błąd kompilacji przy pustym konstruktorze
    CreamCake();
    #endif

    #if TEST_NUM == 108
    //Błąd kompilacji, gdy nie przekaże się produktów do piekarni
    Bakery<float, int, 30>();
    #endif

    #if TEST_NUM == 109
    //Błąd kompilacji przy wywołaniu konstruktora CherryPie z drugim argumentem
    CherryPie<int, 1>(1, 1.0f);
    #endif

    #if TEST_NUM == 110
    //Błąd kompilacji przy wywołaniu konstruktora ApplePie z tylko jednym argumentem
    ApplePie<int, 1, float>(1);
    #endif

    #if TEST_NUM == 111
    //Błąd kompilacji przy wywołaniu konstruktora CheeseCake z drugim argumentem
    CheeseCake<int, 1, 1>(1, 1.0f);
    #endif

    #if TEST_NUM == 112
    //Błąd kompilacji przy wywołaniu konstruktora CreamCake z tylko jednym argumentem
    CreamCake<int, 1, 1, float>(1);
    #endif

    #if TEST_NUM == 201
    //Poprawne wywołanie getProfits dla nowej piekarni
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie <int, 1>,
        ApplePie<int, 1, float>
    >(CherryPie<int, 1>(3), ApplePie<int, 1, float>(3, 2.4f));

    assert(bakery.getProfits() == 0);
    #endif

    #if TEST_NUM == 202
    //Poprawne wywołanie getProductStock dla nowej piekarni
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie <int, 1>,
        ApplePie<int, 1, float>
    >(CherryPie<int, 1>(3), ApplePie<int, 1, float>(4, 2.4f));

    auto cherryPieStock = bakery.getProductStock<CherryPie <int, 1>>();
    auto applePieStock = bakery.getProductStock<ApplePie<int, 1, float>>();
    assert(cherryPieStock == 3);
    assert(applePieStock == 4);
    #endif

    #if TEST_NUM == 203
    //Poprawne obliczenie powierzchni placka
    auto cherryPie1 = CherryPie<int, 1>(1);
    assert(abs(cherryPie1.getArea() - PI_VALUE) <= EPSILON);

    auto cherryPie2 = CherryPie<int, 2>(1);
    assert(abs(cherryPie2.getArea() - 2 * 2 * PI_VALUE) <= 2 * 2 * EPSILON);
    #endif

    #if TEST_NUM == 204
    //Poprawne obliczenie powierzchni ciasta
    auto cheeseCake1 = CheeseCake<int, 1, 1>(1);
    assert(abs(cheeseCake1.getArea() - LN2_VALUE) <= EPSILON);

    auto cheeseCake2 = CheeseCake<int, 2, 3>(1);
    assert(abs(cheeseCake2.getArea() - 2 * 3 * LN2_VALUE) <= 2 * 3 * EPSILON);
    #endif

    #if TEST_NUM == 205
    //Poprawne działanie getStock
    auto cherryPie = CherryPie<int, 1>(1);
    assert(cherryPie.getStock() == 1);

    auto applePie = ApplePie<int, 1, float>(2, 1.0f);
    assert(applePie.getStock() == 2);

    auto cheeseCake = CheeseCake<int, 1, 1>(3);
    assert(cheeseCake.getStock() == 3);

    auto creamCake = CreamCake<int, 1, 1, float>(4, 1.0f);
    assert(creamCake.getStock() == 4);
    #endif

    #if TEST_NUM == 206
    //Poprawne działanie getPrice dla produktów na sprzedaż
    auto applePie = ApplePie<int, 1, float>(1, 1.0f);
    assert(applePie.getPrice() == 1.0f);

    auto creamCake = CreamCake<int, 1, 1, float>(1, 1.4f);
    assert(creamCake.getPrice() == 1.4f);
    #endif

    #if TEST_NUM == 207
    //Błąd kompilacji przy wywołaniu getPrice dla obiektu CherryPie
    auto cherryPie = CherryPie<int, 1>(1);
    cherryPie.getPrice();
    #endif

    #if TEST_NUM == 208
    //Błąd kompilacji przy wywołaniu getPrice dla obiektu CheeseCake
    auto cheeseCake = CheeseCake<int, 1, 1>(3);
    cheeseCake.getPrice();
    #endif

    #if TEST_NUM == 209
    //Błąd kompilacji przy wywołaniu getProductStock z produktem niedostępnym w piekarni
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie <int, 1>,
        ApplePie<int, 1, float>
    >(CherryPie<int, 1>(3), ApplePie<int, 1, float>(4, 2.4f));

    bakery.getProductStock<CherryPie<int, 2>>();
    #endif

    #if TEST_NUM == 210
    //Błąd kompilacji przy wywołaniu getProductStock z typem nieproduktowym
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie <int, 1>,
        ApplePie<int, 1, float>
    >(CherryPie<int, 1>(3), ApplePie<int, 1, float>(4, 2.4f));

    bakery.getProductStock<bool>();
    #endif

    #if TEST_NUM == 211
    //Błąd kompilacji przy wywołaniu getProductStock z wartością logiczną
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie <int, 1>,
        ApplePie<int, 1, float>
    >(CherryPie<int, 1>(3), ApplePie<int, 1, float>(4, 2.4f));

    bakery.getProductStock<true>();
    #endif

    #if TEST_NUM == 301
    //Tworzenie CherryPie z całkowitymi typami promienia
    CherryPie<int,       1>(1);
    CherryPie<long,      1>(1);
    CherryPie<short,     1>(1);
    CherryPie<long long, 1>(1);
    CherryPie<bool,      1>(1);
    #endif

    #if TEST_NUM == 302
    //Tworzenie ApplePie z całkowitymi typami promienia
    ApplePie<int,       1, float>(1, 1.0f);
    ApplePie<long,      1, float>(1, 1.0f);
    ApplePie<short,     1, float>(1, 1.0f);
    ApplePie<long long, 1, float>(1, 1.0f);
    ApplePie<bool,      1, float>(1, 1.0f);
    #endif

    #if TEST_NUM == 303
    //Tworzenie ApplePie ze zmiennoprzecinkowymi typami ceny
    ApplePie<int, 1, float      >(1, 1.0f);
    ApplePie<int, 1, double     >(1, 1.0f);
    ApplePie<int, 1, long double>(1, 1.0f);
    #endif

    #if TEST_NUM == 304
    //Tworzenie CheeseCake z całkowitymi typami długości boków
    CheeseCake<int,       1, 1>(1);
    CheeseCake<long,      1, 1>(1);
    CheeseCake<short,     1, 1>(1);
    CheeseCake<long long, 1, 1>(1);
    CheeseCake<bool,      1, 1>(1);
    #endif

    #if TEST_NUM == 305
    //Tworzenie CreamCake z całkowitymi typami długości boków
    CreamCake<int,       1, 1, float>(1, 1.0f);
    CreamCake<long,      1, 1, float>(1, 1.0f);
    CreamCake<short,     1, 1, float>(1, 1.0f);
    CreamCake<long long, 1, 1, float>(1, 1.0f);
    CreamCake<bool,      1, 1, float>(1, 1.0f);
    #endif

    #if TEST_NUM == 306
    //Tworzenie CreamCake ze zmiennoprzecinkowymi typami ceny
    CreamCake<int, 1, 1, float      >(1, 1.0f);
    CreamCake<int, 1, 1, double     >(1, 1.0f);
    CreamCake<int, 1, 1, long double>(1, 1.0f);
    #endif

    #if TEST_NUM == 307
    //Błąd kompilacji przy niecałkowitym typie promienia w CherryPie
    CherryPie<float, 1>(1);
    #endif

    #if TEST_NUM == 308
    //Błąd kompilacji przy niecałkowitym typie promienia w ApplePie
    ApplePie<float, 1, float>(1, 1.0f);
    #endif

    #if TEST_NUM == 309
    //Błąd kompilacji przy całkowitym typie ceny w ApplePie
    ApplePie<float, 1, int>(1, 1);
    #endif

    #if TEST_NUM == 310
    //Błąd kompilacji przy niecałkowitym typie długości boków w CheeseCake
    CheeseCake<float, 1, 1>(1);
    #endif

    #if TEST_NUM == 311
    //Błąd kompilacji przy niecałkowitym typie długości boków w CreamCake
    CreamCake<float, 1, 1, float>(1);
    #endif

    #if TEST_NUM == 312
    //Błąd kompilacji przy całkowitym typie ceny w CreamCake
    CreamCake<float, 1, 1, int>(1, 1);
    #endif

    #if TEST_NUM == 401
    //Tworzenie piekarni ze zmiennoprzecinkowymi typami dochodu
    Bakery<float,       int, 30, CherryPie<int, 1>>(CherryPie<int, 1>(1));
    Bakery<double,      int, 30, CherryPie<int, 1>>(CherryPie<int, 1>(1));
    Bakery<long double, int, 30, CherryPie<int, 1>>(CherryPie<int, 1>(1));
    #endif

    #if TEST_NUM == 402
    //Tworzenie piekarni z całkowitymi typami przestrzeni na półkach
    Bakery<float, int,       30, CherryPie<int,       1>>(CherryPie<int,       1>(1));
    Bakery<float, long,      30, CherryPie<long,      1>>(CherryPie<long,      1>(1));
    Bakery<float, short,     30, CherryPie<short,     1>>(CherryPie<short,     1>(1));
    Bakery<float, long long, 30, CherryPie<long long, 1>>(CherryPie<long long, 1>(1));
    #endif

    #if TEST_NUM == 403
    //Produkty ledwo mieszczą się na półkach
    Bakery<float, int, 308, CherryPie<int, 7>, ApplePie<int, 7, float>>(CherryPie<int, 7>(1), ApplePie<int, 7, float>(1, 1.0f));
    #endif

    #if TEST_NUM == 404
    //Błąd kompilacji, gdy produkty nie mieszczą się na półkach
    Bakery<float, int, 307, CherryPie<int, 7>, ApplePie<int, 7, float>>(CherryPie<int, 7>(1), ApplePie<int, 7, float>(1, 1.0f));
    #endif

    #if TEST_NUM == 405
    //Błąd kompilacji, gdy typ wymiaru części produktów nie zgadza się z typem powierzchni na półkach
    Bakery<float, int, 30, CherryPie<int, 7>, ApplePie<long, 1, float>>(CherryPie<int, 7>(1), ApplePie<long, 1, float>(1, 1.0f));
    #endif

    #if TEST_NUM == 406
    //Błąd kompilacji, gdy typy produktów się duplikują
    Bakery<
        float,
        int,
        30,
        ApplePie<long, 1, float>,
        CherryPie<int, 1>,
        CherryPie<int, 1>
    >(
        ApplePie<long, 1, float>(1, 1.0f),
        CherryPie<int, 1>(1),
        CherryPie<int, 1>(1)
    );
    #endif

    #if TEST_NUM == 407
    //Błąd kompilacji, gdy typ ceny części produktów nie zgadza się z typem dochodu piekarni
    Bakery<
        float,
        int,
        30,
        CreamCake<int, 1, float>,
        ApplePie<int, 1, double>
    >(
        CreamCake<int, 1, float>(1, 1.0f),
        ApplePie<int, 1, double>(1, 1.0f)
    );
    #endif

    #if TEST_NUM == 501
    //Poprawne wywołanie metody sell
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie<int, 1>,
        ApplePie<int, 2, float>,
        CreamCake<int, 1, 1, float>
    >(
        CherryPie<int, 1>(3),
        ApplePie<int, 2, float>(2, 4.5f),
        CreamCake<int, 1, 1, float>(3, 3.1f)
    );

    bakery.sell<ApplePie<int, 2, float>>();
    assert(bakery.getProfits() == 4.5f);
    auto applePieStock = bakery.getProductStock<ApplePie<int, 2, float>>();
    assert(applePieStock == 1);

    bakery.sell<ApplePie<int, 2, float>>();
    assert(bakery.getProfits() == 9);
    applePieStock = bakery.getProductStock<ApplePie<int, 2, float>>();
    assert(applePieStock == 0);

    bakery.sell<ApplePie<int, 2, float>>();
    assert(bakery.getProfits() == 9);
    applePieStock = bakery.getProductStock<ApplePie<int, 2, float>>();
    assert(applePieStock == 0);

    bakery.sell<CreamCake<int, 1, 1, float>>();
    assert(bakery.getProfits() == 12.1f);
    auto creamCakeStock = bakery.getProductStock<CreamCake<int, 1, 1, float>>();
    assert(creamCakeStock == 2);
    #endif

    #if TEST_NUM == 502
    //Błąd kompilacji przy próbie sprzedaży niedostępnego produktu
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie<int, 1>,
        ApplePie<int, 2, float>,
        CreamCake<int, 1, 1, float>
    >(
        CherryPie<int, 1>(3),
        ApplePie<int, 2, float>(2, 4.5f),
        CreamCake<int, 1, 1, float>(3, 3.1f)
    );

    bakery.sell<ApplePie<int, 1, float>>();
    #endif

    #if TEST_NUM == 503
    //Błąd kompilacji przy wywołaniu metody sell z produktem, który nie jest na sprzedaż
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie<int, 1>,
        ApplePie<int, 2, float>,
        CreamCake<int, 1, 1, float>
    >(
        CherryPie<int, 1>(3),
        ApplePie<int, 2, float>(2, 4.5f),
        CreamCake<int, 1, 1, float>(3, 3.1f)
    );

    bakery.sell<CherryPie<int, 1>>();
    #endif

    #if TEST_NUM == 504
    //Błąd kompilacji przy wywołaniu metody sell z typem nieproduktowym
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie<int, 1>,
        ApplePie<int, 2, float>,
        CreamCake<int, 1, 1, float>
    >(
        CherryPie<int, 1>(3),
        ApplePie<int, 2, float>(2, 4.5f),
        CreamCake<int, 1, 1, float>(3, 3.1f)
    );

    bakery.sell<bool>();
    #endif

    #if TEST_NUM == 505
    //Błąd kompilacji przy wywołaniu metody sell z wartością logiczną
    auto bakery = Bakery<
        float,
        int,
        30,
        CherryPie<int, 1>,
        ApplePie<int, 2, float>,
        CreamCake<int, 1, 1, float>
    >(
        CherryPie<int, 1>(3),
        ApplePie<int, 2, float>(2, 4.5f),
        CreamCake<int, 1, 1, float>(3, 3.1f)
    );

    bakery.sell<true>();
    #endif

    #if TEST_NUM == 601
    //Poprawny restock
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

    bakery.restock<ApplePie<int, 2, float>>(4);
    auto updatedStock = bakery.getProductStock<ApplePie<int, 2, float>>();
    assert(updatedStock == 6);
    #endif

    #if TEST_NUM == 602
    //Błąd kompilacji przy wywołaniu restock z użyciem złego typu dostępnego w piekarni
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

    bakery.restock<CherryPie<int, 1>>(4);
    #endif

    #if TEST_NUM == 603
    //Błąd kompilacji przy wywołaniu restock z użyciem złego typu niedostępnego w piekarni
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

    bakery.restock<bool>(4);
    #endif

    #if TEST_NUM == 604
    //Błąd kompilacji przy wywołaniu restock z użyciem wartości logicznej
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

    bakery.restock<true>(4);
    #endif

    return 0;
}
