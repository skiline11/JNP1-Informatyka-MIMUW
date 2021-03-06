/* Autorzy rozwiązania : Michał Radecki, Kamil Ćwintal */

#include <iostream>
#include <exception>
#include "helper.h"
#include "decoration.h"
#include "person.h"

using namespace std;

void test_standardowy() {
	// Rodzina Kowalskich...
	auto John = make_shared<Adult>("Jan", Price(1000));
	auto Mary = make_shared<Adult>("Marysia");
	auto Ada = make_shared<Child>("Ada");

	// ... chciała mieć świąteczne drzewko...
	ChristmasTree tree("Christmas tree");

	// ... więc postanowiła kupić świeży świerk...
	auto spruce = make_shared<Tree>("Fresh spruce", Price(300), Date(18, 8, 10));

	// ... i różne ozdoby,
	// światełka za 150 zł
	auto lights = make_shared<Lights>("Bright colorful lights withs 100 LEDs",
	                                  Price(150));
	// i kilka bombek.
	auto glassBall1 = make_shared<GlassBall>("Glass ball in shape of snowman",
	                                         Price(10));
	auto glassBall2 = make_shared<GlassBall>("Glass ball with Christmas tree",
	                                         Price(15));
	auto glassBall3 = make_shared<GlassBall>("Glass ball with presents",
	                                         Price(15));

	// Za zakupy odpowiedzialny jest Jan. Żona i córka obserwują pojawianie się
	// ozdób:
	tree.attachObserver(Mary);
	tree.attachObserver(Ada);

	// Podczas gdy Marysia z Adą ozdabiają drzewko...
	tree.addDecoration(spruce);
	tree.addDecoration(lights);
	tree.addDecoration(glassBall1);
	tree.addDecoration(glassBall2);
	tree.addDecoration(glassBall3);

	// Jan płaci rachunek kurierowi.
	auto savings0 = John->getSavings();
	for (auto& x : tree) {
		John->buyDecoration(*x); // to nie chciało działać
	}
	auto savings1 = John->getSavings();
	cout << "Savings before: " << savings0 << " and after: " << savings1 << ".\n";

	// Drzewko ozdobione. Rachunek zapłacony. Od tego momentu startuje zegar.
	for (auto& time = currentDate();
	     time < Date(24, 18, 0);
	     ++time) {
		// Ada bawi się ozdobami choinkowymi o losowych porach (również nocą)...
		if (randomNumber() % 200 == 0) {
			cout << time << "\n";
			for (auto& x : tree) {
				// ... przyglądając się i manipulując losowo wybranymi ozdobami.
				if (randomNumber() % 4 == 0) {
					x->doOperation(cout);
				}
			}
		}
	}
}

void test1() {
	auto dorosly1 = make_shared<Adult>("Pan1", Price(1000));
	auto dorosly2 = make_shared<Adult>("Pan2", Price(200));

	ChristmasTree drzewko("Christmas tree");

	auto swierk = make_shared<Tree>("Świeży świerk", Price(50), Date(18, 0, 0));

	auto lampki = make_shared<Lights>("Lampeczki", Price(70));

	auto bombka1 = make_shared<GlassBall>("Bombka1", Price(20));
	auto bombka2 = make_shared<GlassBall>("Bombka2", Price(20));
	auto bombka3 = make_shared<GlassBall>("Bombka3", Price(20));
	auto bombka4 = make_shared<GlassBall>("Bombka4", Price(20));
	auto bombka5 = make_shared<GlassBall>("Bombka5", Price(20));

	drzewko.attachObserver(dorosly2);

	drzewko.addDecoration(swierk);
	drzewko.addDecoration(lampki);
	drzewko.addDecoration(bombka1);
	drzewko.addDecoration(bombka2);
	drzewko.addDecoration(bombka3);
	drzewko.addDecoration(bombka4);
	drzewko.addDecoration(bombka5);

	auto kasa = dorosly1->getSavings();
	for(auto& x : drzewko) {
		dorosly1->buyDecoration(*x);
	}
	cout << "-------------------------------------------" << endl;
	cout << "dorosly1 : Zostało " << dorosly1->getSavings() << " pieniedzy " << endl;
	cout << "Startuje zegar" << endl;

	for(auto& time = currentDate(); time < Date(24, 18, 0); ++time) { // uwaga -> nie działa time++
		for(auto& x : drzewko) {
			if(randomNumber() % 2 == 0) {
				x->doOperation(cout);
			}
		}

	}
}

void testPerson1() {
	auto dorosly1 = make_shared<Adult>("Pan1", Price(1000));
	auto dorosly2 = make_shared<Adult>("Pan2", Price(200));
	auto dorosly3 = make_shared<Adult>("Pan3", Price(200));
	auto dziecko1 = make_shared<Child>("Dziecko1");
	auto dziecko2 = make_shared<Child>("Dziecko2");

	ChristmasTree drzewko("Christmas tree");

	auto swierk = make_shared<Tree>("Świeży świerk", Price(50), Date(18, 0, 0));

	auto lampki = make_shared<Lights>("Lampeczki", Price(70));

	auto bombka1 = make_shared<GlassBall>("Bombka1", Price(20));
	auto bombka2 = make_shared<GlassBall>("Bombka2", Price(20));
	auto bombka3 = make_shared<GlassBall>("Bombka3", Price(20));
	auto bombka4 = make_shared<GlassBall>("Bombka4", Price(20));
	auto bombka5 = make_shared<GlassBall>("Bombka5", Price(20));

	drzewko.attachObserver(dorosly2);
	drzewko.attachObserver(dorosly3);
	auto d1 = drzewko.attachObserver(dziecko1);

	drzewko.addDecoration(swierk);
	drzewko.addDecoration(lampki);
	drzewko.addDecoration(bombka1);
	drzewko.addDecoration(bombka2);
	drzewko.addDecoration(bombka3);

	drzewko.attachObserver(dziecko2);

	drzewko.addDecoration(bombka4);

	drzewko.detachObserver(d1);

	drzewko.addDecoration(bombka5);

	for(auto& x : drzewko) {
		dorosly2->buyDecoration(*x);
	}
	cout << "Kasa1 : " << dorosly1->getSavings() << endl;
	cout << "Kasa2 : " << dorosly2->getSavings() << endl; // czy ujemna kasa jest ok???
}

#include <limits>
#include <cassert>
void testRandom() {
	cout << "Max = " << endl << std::numeric_limits<int>::max() << endl;
	int x = 0;
	cout << "Test randomNumber()" << endl;
	while(x < 100) {
		x++;
		if(randomNumber() < 0) cout << "O nie" << endl;
	}
}

void testPrice() {
	Price p;
	cout << "p = " << p << endl;
	Price p0(); // Czy powinniśmy dopuszczać takie konstrowanie obiektów typu Price
	cout << "p0 = " << p0 << endl;
	Price p1(110);
	cout << "p1 = " << p1 << endl;
	Price p2(200);
	cout << "p2 = " << p2 << endl;
	cout << "p1 + p2 = " << p1 << " + " << p2 << " = " << p1 + p2 << endl;
	Price p3 = p1 + p2;
	cout << "p3 = " << p3 << endl;
	Price p4(80);
	cout << "p4 = " << p4 << endl;
	p3 -= p4;
	cout << "Po wykonaniu p3 -= p4, zachodzi p3 = " << p3 << endl;
	cout << "Sprawdzmy teraz czy p3(" << p3 << ") < p2(" << p2 << ") = " << (p3 < p2) << endl;
	assert((p3 < p2) == false);
	cout << "Sprawdzmy teraz czy p2(" << p2 << ") < p3(" << p3 << ") = " << (p2 < p3) << endl;
	assert(p2 < p3);
	cout << "Sprawdzmy teraz czy p3(" << p3 << ") < p3(" << p3 << ") = " << (p3 < p3) << endl;
	assert((p3 < p3) == false);
}

void testDate() {
	Date d1(1, 1, 1);
	cout << "d1 = " << d1 << endl;
	try {
		Date d2(-1, 1, 1);
	} catch(date_error e) {
		cout << "Złapałem data_error" << endl;
	}
	cout << "Przed wykonaniem operatora preincrementacji d1 = " << d1 << endl;
	++d1;
	cout << "Po wykonaniu operatora preincrementacji ++d1, d1 = " << d1 << endl;

	Date d3(1, 23, 59);
	cout << "d3 = " << d3 << endl;
	++d3;
	cout << "Po wykonaniu operatora preincrementacji ++d3, d3 = " << d3 << endl;

	Date d4(31, 23, 59);
	cout << "d4 = " << d4 << endl;
	++d4;
	cout << "Po wykonaniu operatora preincrementacji ++d4, d4 = " << d4 << endl;
	cout << "CZY TO NA PEWNO JEST POPRAWNE ZACHOWANIE PROGRAMU ????????????????????????????????????????????????????" << endl;

	cout << "Różnica w minutach pomiędzy d4 = " << d4 << ", a d3 = " << d3 << " wynosi " << (d4 - d3) << endl;

	cout << "A między d3 i d4 wynosi " << (d3 - d4) << endl;
	cout << "Czy powinna być ujemna? A możę powinniśmy zwracać wartość bezwzględną różnicy" << endl;


	Date d5(25, 10, 10);
	Date d6(26, 10, 10);
	cout << "Czy d5 < d6 ?  Odpowiedz = " << (d5 < d6) << endl;
	cout << "Czy d6 < d5 ?  Odpowiedz = " << (d6 < d5) << endl;

	int roznica = d6 - d5;
	cout << "roznica = d6 - d5 i wynosi teraz roznica = " << roznica << ", powinna wynosic 60 * 24 = 1440" << endl;

}

void testDrzewko() {
	auto a0 = make_shared<Adult>("KolesZero");
	auto a1 = make_shared<Adult>("Adam");
	auto c1 = make_shared<Child>("Dzieciak1");
	auto c2 = make_shared<Child>("Dzieciak2");
	auto c3 = make_shared<Child>("Dzieciak3");
	Price p1(103);
	Price p2(77);
	Price p3(20);
	Price p4(400);
	Date d1(18, 1, 1);

	auto glassBall1 = make_shared<GlassBall>("gb1_Name", p1);
	auto glassBall2 = make_shared<GlassBall>("gb2_Name", p2);
	auto lights1 = make_shared<Lights>("l1_Name", p3);
	auto tree1 = make_shared<Tree>("t1_Name", p4, d1);
	ChristmasTree choinka("Drzewko");

	cout << "---Dołączam pierwszego dorosłego" << endl;
	choinka.attachObserver(a0);
	cout << "---Dodaję dekoracje" << endl;
	cout << "---Dekoracja 1" << endl;
	auto it_gb1 = choinka.addDecoration(glassBall1);
	cout << "---Dekoracja 2" << endl;
	auto it_gb2 = choinka.addDecoration(glassBall2);
	cout << "---Dekoracja 3" << endl;
	auto it_l1 = choinka.addDecoration(lights1);
	cout << "---Dołączam drugiego dorosłego" << endl;
	choinka.attachObserver(a1);
	cout << "---Dołączam 3 dzieciaków" << endl;
	choinka.attachObserver(c1);
	choinka.attachObserver(c2);
	auto it_c3 = choinka.attachObserver(c3);
	cout << "---Dodaje drzewko" << endl;
	choinka.addDecoration(tree1);
	choinka.addDecoration(glassBall1);

	cout << "---Odłączam 1 dzieciaka (pozostało 2 dzieciaków)" << endl;
	choinka.detachObserver(it_c3);

	cout << "---Całkowita cena drzewka = " << choinka.getPrice() << ", powinna wynosić 703" << endl;
	choinka.removeDecoration(it_gb1);
	cout << "---Całkowita cena drzewka = " << choinka.getPrice() << ", powinna wynosić 600" << endl;
	choinka.removeDecoration(it_gb2);
	cout << "---Całkowita cena drzewka = " << choinka.getPrice() << ", powinna wynosic 523" <<endl;
	// choinka.removeDecoration(it_gb2); // Czy to powinniśmy obsłużyć i zignorować

	cout << "---Dodaje dekoracje" << endl;
	choinka.addDecoration(glassBall1);
	cout << "---Dodaje dekoracje" << endl;
	choinka.addDecoration(glassBall2);
	auto it = choinka.begin();
	cout << "---Nazwa 1 elementu drzewka : " << (*it)->getName() << ", powinna wynosic l1_Name" << endl;
	it++;
	cout << "---Nazwa 2 elementu drzewka : " << (*it)->getName() << ", powinna wynosić t1_Name" << endl;

	/*
	for (auto& time = currentDate();
	     time < Date(24, 18, 0);
	     ++time) {
		// Ada bawi się ozdobami choinkowymi o losowych porach (również nocą)...
		if (randomNumber() % 200 == 0) {
			cout << time << "\n";
			for (auto& x : choinka) {
				// ... przyglądając się i manipulując losowo wybranymi ozdobami.
				if (randomNumber() % 4 == 0) {
					x->doOperation(cout);
				}
			}
		}
	}
	*/

}

shared_ptr<ChristmasTree> dajChoinke() {
	Price p1(103);
	Price p2(77);
	Price p3(20);
	Price p4(400);
	Date d1(18, 1, 1);

	auto glassBall11 = make_shared<GlassBall>("gb1_Name", p1);
	auto glassBall12 = make_shared<GlassBall>("gb2_Name", p2);
	auto lights1 = make_shared<Lights>("l1_Name", p3);
	auto tree1 = make_shared<Tree>("t1_Name", p4, d1);
	auto choinka1 = make_shared<ChristmasTree>("Drzewko");
	choinka1->addDecoration(glassBall11);
	choinka1->addDecoration(glassBall12);
	choinka1->addDecoration(lights1);
	choinka1->addDecoration(tree1);

	return choinka1;
}

void testDrzewkoDrzewek() {
	auto a0 = make_shared<Adult>("KolesZero");
	auto a1 = make_shared<Adult>("Adam");
	auto c1 = make_shared<Child>("Dzieciak1");
	auto c2 = make_shared<Child>("Dzieciak2");
	auto c3 = make_shared<Child>("Dzieciak3");

	auto choinka1 = dajChoinke();
	cout << "cena1 = " << choinka1->getPrice() << endl;

	auto choinka2 = dajChoinke();
	cout << "cena2 = " << choinka2->getPrice() << endl;
	auto choinka3 = dajChoinke();
	cout << "cena3 = " << choinka3->getPrice() << endl;

	cout << "--------------------------------------------" << endl;
//
	auto choinkaChoinek = make_shared<ChristmasTree>("BigDrzewko");
	choinkaChoinek->addDecoration(choinka1);
	cout << "Cena tego monstrum wynosi = " << choinkaChoinek->getPrice() << endl;

	choinkaChoinek->addDecoration(choinka2);
	choinkaChoinek->addDecoration(choinka3);

	cout << "Cena tego monstrum wynosi = " << choinkaChoinek->getPrice() << endl;
}


int main()
{
	// test_standardowy();
	// test1();
	// testPerson1();

	// testRandom();
	// testPrice();
	// testDate();
//	testDrzewko();
	testDrzewkoDrzewek();

	return 0;
}
