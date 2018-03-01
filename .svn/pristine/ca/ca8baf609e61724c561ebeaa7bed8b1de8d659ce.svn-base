/* Testy do zadania 42
 * Języki i narzędzia programowania 1
 */

#define main main_disabled
#include "42.cc"
#undef main

#include <iostream>
#include <sstream>
#include <cstring>

int main() {
    unsigned char poison1[100000];
    LazyCalculator calculator;
    unsigned char poison2[100000];

    memset(poison1, 0xaa, sizeof(poison1));
    memset(poison2, 0x55, sizeof(poison2));

#if TEST_NUM == 100
    assert(main_disabled);
#endif

#if TEST_NUM == 200
    // The only literals...
    assert(calculator.calculate("0") == 0);
    assert(calculator.calculate("2") == 2);
    assert(calculator.calculate("4") == 4);

    // Built-in operators.
    assert(calculator.calculate("42+") == 6);
    assert(calculator.calculate("24-") == -2);
    assert(calculator.calculate("42*") == 8);
    assert(calculator.calculate("42/") == 2);

    assert(calculator.calculate("42-2-") == 0);
    assert(calculator.calculate("242--") == 0);
    assert(calculator.calculate("22+2-2*2/0-") == 2);
#endif

#if TEST_NUM >= 300 && TEST_NUM <= 599
    // The fun.
    calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
    assert(calculator.calculate("42!") == 42);
#endif

#if TEST_NUM == 300
    std::string buffer;
    calculator.define(',', [](Lazy a, Lazy b) { a(); return b(); });
    calculator.define('P', [&buffer](Lazy, Lazy) { buffer += "pomidor"; return 0; });
    assert(calculator.calculate("42P42P42P42P42P42P42P42P42P42P42P42P42P42P42P4"
                                "2P,,,,42P42P42P42P42P,,,42P,42P,42P42P,,,,42P,"
                                ",,42P,42P,42P,,42P,,,42P,42P42P42P42P42P42P42P"
                                "42P,,,42P,42P,42P,,,,,,,,,,,,") == 0);
    assert(buffer.length() == 42 * std::string("pomidor").length());

    std::string buffer2 = std::move(buffer);
    buffer.clear();
    calculator.define('$', manytimes);
    assert(calculator.calculate("42!42P$") == 0);
    // Notice, how std::move worked.
    assert(buffer.length() == 42 * std::string("pomidor").length());

    calculator.define('?', [](Lazy a, Lazy b) { return a() ? b() : 0; });
    assert(calculator.calculate("042P?") == 0);
    assert(buffer == buffer2);

    assert(calculator.calculate("042!42P$?") == 0);
    assert(buffer == buffer2);

    calculator.define('1', [](Lazy, Lazy) { return 1; });
    assert(calculator.calculate("021") == 1);
#endif

#if TEST_NUM == 400
    std::cout << "TU POWINNY SIĘ WYPISAĆ DWIE SERIE KOMUNIKATÓW O TRZECH WYJĄTKACH: "
                 "syntax error, operator already defined, unknown operator.\n";

    // Check if the exception "syntax error" is properly thrown.
    for (auto bad: {"", "42", "4+", "424+"}) {
        try {
            calculator.calculate(bad);
            assert(false);
        }
        catch (SyntaxError e) {
            if (strcmp(bad, "") == 0)
                std::cout << e.what() << '\n';
        }
        catch (...) {
            assert(false);
        }
    }

    // Check if the exception "operator already defined" is properly thrown.
    for (auto bad: {'!', '+', '-', '*', '/', '0', '2', '4'}) {
        try {
            calculator.define(bad, [](Lazy a, Lazy b) { return 4 * a() + 2 * b(); });
            assert(false);
        }
        catch (OperatorAlreadyDefined e) {
            if (bad == '!')
                std::cout << e.what() << '\n';
        }
        catch (...) {
            assert(false);
        }
    }

    // Check if the exception "unknown operator" is properly thrown.
    try {
        calculator.calculate("02&");
        assert(false);
    }
    catch (UnknownOperator e) {
        std::cout << e.what() << '\n';
    }
    catch (...) {
        assert(false);
    }

    // Check inheritance after std::exception.
    try {
        calculator.calculate("");
        assert(false);
    }
    catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
    catch (...) {
        assert(false);
    }
    try {
        calculator.define('!', [](Lazy, Lazy) { return 3; });
        assert(false);
    }
    catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
    catch (...) {
        assert(false);
    }
    try {
        calculator.calculate("02&");
        assert(false);
    }
    catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
    catch (...) {
        assert(false);
    }
#endif

#if TEST_NUM == 500
    std::ostringstream s;
    for (int i = 0; i < 1000; i++) {
        s << "42!";
    }
    for (int i = 2; i < 1000; i += 2) {
        s << "+-";
    }
    s << "-";
    assert(calculator.calculate(s.str()) == 84);
#endif

#if TEST_NUM == 600
    // This should fail if someone uses an array and indexes it with
    // (signed) char.
    calculator.define('\xff', [](Lazy, Lazy) { return 42; });
    assert(calculator.calculate("44\xff") == 42);
    calculator.define('\x80', [](Lazy, Lazy) { return 7; });
    assert(calculator.calculate("00\x80") == 7);
#endif

    for (size_t i = 0; i < sizeof(poison1); i++) {
        assert(poison1[i] == 0xaa);
    }
    for (size_t i = 0; i < sizeof(poison2); i++) {
        assert(poison2[i] == 0x55);
    }
}
