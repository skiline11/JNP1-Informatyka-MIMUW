#include "decoration.h"
#include "helper.h"
#include "person.h"
#include <cassert>
#include <memory>
#include <sstream>

#if TEST_NUM == 601
  #include <iostream>
#endif

// Opis testów
// grupa 1 - testy modułu pomocniczego
// grupa 2 - testy modułu pomocniczego (kontynuacja)
// grupa 3 - testy dla dekoracji GlassBall, Lights, Tree
// grupa 4 - testy dla ChristmasTree - część związana ze wzorcem kompozyt
// grupa 5 - testy dla Child i Adult
// grupa 6 - testy dla ChristmasTree - część związana ze wzorcem obserwator

using namespace std;

template<typename T>
bool compare(const T& t, const string& msg) {
  ostringstream oss;
  oss << t;
  string result = oss.str();
  return result == msg;
}

template<typename T>
bool compare(const T& t1, const T& t2) {
  ostringstream oss1, oss2;
  oss1 << t1;
  oss2 << t2;
  string r1 = oss1.str();
  string r2 = oss2.str();
  return r1 == r2;
}

int main() {

#if TEST_NUM == 101
  int t1 = randomNumber();
  int t2 = randomNumber();
  int t3 = randomNumber();
  assert(t1 == 1819583497);
  assert(t2 == 1650906866);
  assert(t3 == 1879422756);
#endif

#if TEST_NUM == 102
  Price price0;
  Price price1(100);
  Price price2(300);
  assert(price1 < price2);
  Price sum = price0 + price1;
  price2 -= price1;
  assert(compare(sum, "100 PLN"));
  assert(compare(price2, "200 PLN"));
#endif

#if TEST_NUM == 201
  Date date1(18, 16, 0);
  Date date2(18, 16, 1);
  Date date3(18, 17, 0);
  Date date4(19, 16, 0);

  assert(date1 < date2);
  assert(date1 < date3);
  assert(date1 < date4);
  assert(date2 < date3);
  assert(date2 < date4);
  assert(date3 < date4);
#endif

#if TEST_NUM == 202
  const int MINUTES_PER_HOUR = 60;
  const int HOURS_PER_DAY = 24;
  const int MINUTES_PER_DAY = MINUTES_PER_HOUR * HOURS_PER_DAY;

  Date date1(18, 16, 0);
  Date date2(18, 16, 1);
  Date date3(18, 17, 0);
  Date date4(19, 16, 0);

  assert(date2 - date1 == 1);
  assert(date3 - date1 == MINUTES_PER_HOUR);
  assert(date4 - date1 == MINUTES_PER_DAY);
  assert(date3 - date2 == MINUTES_PER_HOUR - 1);
  assert(date4 - date2 == MINUTES_PER_DAY - 1);
  assert(date4 - date3 == MINUTES_PER_DAY - MINUTES_PER_HOUR);
#endif

#if TEST_NUM == 203
  Date date1a(18, 16, 0);
  Date date1b(18, 16, 1);
  ++date1a;
  assert(date1b - date1a == 0);

  Date date2a(18, 16, 59);
  Date date2b(18, 17, 0);
  ++date2a;
  assert(date2b - date2a == 0);

  Date date3a(18, 23, 59);
  Date date3b(19, 0, 0);
  ++date3a;
  assert(date3b - date3a == 0);
#endif

#if TEST_NUM == 204
  Date date1(18, 16, 5);
  Date date2(20, 20, 30);
  compare(date1, "December, 18, 16:05");
  compare(date2, "December, 20, 20:30");
#endif

#if TEST_NUM == 205
  Date date(18, 16, 0);
  assert(date - currentDate() == 0);
#endif

#if TEST_NUM == 301
  string name("Glass ball");
  Price price(25);
  GlassBall glassBall(name, price);
  assert(glassBall.getName() == name);
  assert(compare(price, glassBall.getPrice()));
  ostringstream oss;
  glassBall.doOperation(oss);
  assert(oss.str() == name + ": OK\n"
      || oss.str() == name + ": broken\n"
      || oss.str() == name + ": OK"
      || oss.str() == name + ": broken");
#endif

#if TEST_NUM == 302
  string name("Lights");
  Price price(100);
  Lights lights(name, price);
  assert(lights.getName() == name);
  assert(compare(price, lights.getPrice()));
  ostringstream oss1, oss2, oss3, oss4;
  lights.doOperation(oss1);
  lights.doOperation(oss2);
  lights.doOperation(oss3);
  lights.doOperation(oss4);
  assert(oss1.str() == name + ": constant\n"
      || oss1.str() == name + ": constant");
  assert(oss2.str() == name + ": blinking\n"
      || oss2.str() == name + ": blinking");
  assert(oss3.str() == name + ": off\n"
      || oss3.str() == name + ": off");
  assert(oss4.str() == name + ": constant\n"
      || oss4.str() == name + ": constant");
#endif

#if TEST_NUM == 303
  string name("Tree");
  Date dayOfCut(18, 16, 0);
  Price price(300);
  Tree tree(name, price, dayOfCut);
  assert(tree.getName() == name);
  assert(compare(price, tree.getPrice()));
  ostringstream oss;
  tree.doOperation(oss);
  assert(oss.str() == name + ": 100% of tree covered with needles\n"
      || oss.str() == name + ": 100% of tree covered with needles");
#endif

#if TEST_NUM == 401
  string name("Christmas tree");
  ChristmasTree tree(name);
  Price priceSpruce(300);
  Price priceLights(100);
  Date dayOfCut(18, 16, 0);

  auto spruce = make_shared<Tree>("Spruce", priceSpruce, dayOfCut);
  auto lights  = make_shared<Lights>("Lights", priceLights);

  tree.addDecoration(spruce);
  tree.addDecoration(lights);

  assert(tree.begin() != tree.end());
  assert(compare(tree.getPrice(), priceSpruce + priceLights));

  ostringstream oss, null;
  spruce->doOperation(oss);
  lights->doOperation(oss);
  lights->doOperation(null);
  lights->doOperation(null);

  ostringstream oss2;
  tree.doOperation(oss2);

  assert(oss.str() == oss2.str());

  tree.removeDecoration(tree.begin());
  tree.removeDecoration(tree.begin());
#endif

#if TEST_NUM == 501
  const char* name = "Ada";
  Child child(name);
  assert(child.getName() == name);
#endif

#if TEST_NUM == 502
  const char* name = "Marysia";
  Adult adult(name);
  assert(adult.getName() == name);
#endif

#if TEST_NUM == 503
  Price savings(1000);
  Adult adult("Jan", savings);
  assert(compare(adult.getSavings(), savings));

  GlassBall glassBall("Glass ball", Price(10));
  adult.buyDecoration(glassBall);
  assert(compare(adult.getSavings(), Price(1000 - 10)));
#endif

#if TEST_NUM == 601
  string name("Christmas tree");
  ChristmasTree tree(name);
  Price priceSpruce(300);
  Price priceLights(100);
  Date dayOfCut(18, 16, 0);

  auto spruce = make_shared<Tree>("Spruce", priceSpruce, dayOfCut);
  auto lights = make_shared<Lights>("Lights", priceLights);

  auto Mary = make_shared<Adult>("Marysia");
  auto Ada = make_shared<Child>("Ada");

  cout << "@ Tutaj powinien wyświetlić się komunikat osoby dorosłej:\n";
  tree.attachObserver(Mary);
  tree.addDecoration(spruce);

  cout << "@ Tutaj powinny wyświetlić się dwa komunikaty,\n"
       << "@ osoby dorosłej oraz dziecka:\n";
  tree.attachObserver(Ada);
  tree.addDecoration(lights);
#endif

}
