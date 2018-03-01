/* Autorzy rozwiązania : Michał Radecki, Kacper Konecki */

#include <iostream>
#include <sstream>
#include "sejf.h"

using namespace std;

void test1() {
    int liczbaDostepow = 5;
    Sejf sejf1("sejf1", liczbaDostepow);
    Sejf sejf2("sejf2");
    cout << "znak 2 w słowie sejf2 to " << (char)sejf2[2] << endl;
    // sejf2[2] = 'x'; // wywala błąd lvalue required as left operand of assignment
    cout << "znak 2 w słowie sejf2 to " << (char)sejf2[2] << endl;
    cout << "znak 10 w slowie sejf2 to " << sejf2[10] << endl;

//    Sejf blad = sejf2;
//    cout << "znak 2 w slowie blad to " << blad[2] << endl;
//    cout << endl;
}


void test2() {
    Sejf sejf1("sejf1", 7);
    cout << (char)sejf1[3] << endl;
    sejf1 += 5;
    sejf1 -= 7;
    sejf1 -= 10;;
    sejf1 *= 3;
    cout << endl;
}

void test3() {
    Sejf s("abc", 3);
    s += (-2);

    Sejf s1("aaa", -5);

// Dopisz konstruktor przenoszącu -->
//  istnieje podejrzenie że konst.kopiujący wykonuje się ponieważ nie widzi definicji konst.przenoszącego --> zdefiniuj go !!!
    std::swap(s, s1); // Jakim cudem to ma działać skoro konstruktor kopiujący zgodnie z treścią ma zakończyć program z błędem ?


//    Sejf s2 = s1;

    cout << endl;
}

void test4() {
    Sejf s1("aaa", 2);
    s1[0];
    s1 *= 3;
    cout << endl;
}

void test5() {
    Sejf s("stary", 4);
    Sejf nowy("nowy", 10);
//    nowy = s;
//    Sejf nowszy = s;
//    Sejf nowszy(nowy);
}

void test6() {
    int x = 5;
    int y = 3;
    cout << "adres x : " << &x << endl;
    cout << "adres y : " << &y << endl;
    std::swap(x, y);
    cout << "nowy adres x : " << &x << endl;
    cout << "nowy adres y : " << &y << endl;
    cout << "Powinno wypisać 3 : " << x << endl;
    cout << "Powinno wypisać 5 : " << y << endl;
}

void testPrzykladowy() {
    Sejf s("a\xff", 3);
//    assert(s[0] == 'a');
//    assert(s[1] == 255);
//    assert(s[1] == (int)((unsigned char) -1));
//    assert(s[1] == -1);
//    assert(s[1] != (int)((unsigned char) -1));
    Sejf s1("aaa", 2);
    s1[0];
    s1 *= 3;

    Sejf s2("abc", 7);

    std::swap(s1, s2);

//    Sejf s4 = s1;
//    Sejf s5(s1);
//    Sejf s6("abc");
//    s6 = s1;


}

void testOs1() {
    Sejf s1("aaa", 2);
    s1[2];
    auto k1 = s1.kontroler();
    cout << k1 << "test\n";
    cout << k1;
    s1[2];
    s1[3];
    s1[4];
    cout << k1;
    s1[3];
    cout << k1;
    s1[2];
    cout << k1;

//    string s("abc");
//    cout << s;

}

int main()
{
//    test1();
//    test2();
//    test3();
//    test4();
//    test5();
//    test6();
//    testPrzykladowy();

    testOs1();
    return 0;
}
