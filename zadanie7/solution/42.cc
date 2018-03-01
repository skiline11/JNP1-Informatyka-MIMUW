/* Autor rozwiązania : Michał Radecki */

#include <stack>
#include <string>
#include <functional>
#include <exception>
#include <cassert>

#include <iostream>
#include <map>

using Lazy = std::function<int(void)>;
// two arguments function type
using twoArgsFunType = std::function<int(Lazy, Lazy)>;

class DivisionByZero : public std::exception {};
class OperatorAlreadyDefined : public std::exception {};
class SyntaxError : public std::exception {};
class UnknownOperator : public std::exception {};

class LazyCalculator {
private:
	std::map<char, twoArgsFunType> map_of_operators;
	std::map<char, Lazy> map_of_literals;
public:
	LazyCalculator() {
		define('+',[](Lazy a, Lazy b) -> int { return a() + b(); });
		define('-',[](Lazy a, Lazy b) -> int { return a() - b(); });
		define('*',[](Lazy a, Lazy b) -> int { return a() * b(); });
		define('/',[](Lazy a, Lazy b) -> int {
			if(b() == 0) throw DivisionByZero();
			else return a() / b();
		});
		std::pair<char, int> p_tab[3] = {
			std::make_pair('0', 0),
			std::make_pair('2', 2),
			std::make_pair('4', 4)
		};
		for(std::pair<char, int> p : p_tab) {
			int p_sec = p.second;
			Lazy fun = [p_sec]() -> int {
				return p_sec;
			};
			map_of_literals.insert(std::pair<char, Lazy>(p.first, fun));
		}
	};

	Lazy parse(const std::string& s) const {
		std::stack<Lazy> my_stack;
		for(char c : s) {
			if(map_of_literals.count(c) == 0 && map_of_operators.count(c) == 0) {
				throw UnknownOperator();
			}
			else if(map_of_literals.count(c) != 0) {
				Lazy l1 = map_of_literals.at(c);
				my_stack.push(l1);
			}
			else if(map_of_operators.count(c) != 0) {
				if(my_stack.size() < 2) {
					throw SyntaxError();
				}
				else {
					Lazy x1, x2;
					Lazy result;
					x2 = my_stack.top();
					my_stack.pop();
					x1 = my_stack.top();
					my_stack.pop();

					twoArgsFunType fun = map_of_operators.at(c);
					result = [fun, x1, x2]() -> int {
						return fun(x1, x2);
					};
					my_stack.push(result);
				}
			}
		}
		if(my_stack.size() != 1) throw SyntaxError();
		else {
			return my_stack.top();
		}
	}

	int calculate(const std::string& s) const {
		return parse(s)();
	}

	void define(char c, std::function<int(Lazy, Lazy)> fn) {
		if(map_of_literals.count(c) != 0 || map_of_operators.count(c) != 0) {
			throw OperatorAlreadyDefined();
		}
		map_of_operators.insert(std::pair<char, twoArgsFunType >(c, fn));
	}

};


std::function<void(void)> operator*(int n, std::function<void(void)> fn) {
	return [=]() {
		for (int i = 0; i < n; i++)
			fn();
	};
}

int manytimes(Lazy n, Lazy fn) {
	(n() * fn)();  // Did you notice the type cast?
	return 0;
}

int main() {
	LazyCalculator calculator;

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

	// The fun.
	calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
	assert(calculator.calculate("42!") == 42);

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

	for (auto bad: {"", "42", "4+", "424+"}) {
		try {
			calculator.calculate(bad);
			assert(false);
		}
		catch (SyntaxError) {
		}
	}

	try {
		calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
		assert(false);
	}
	catch (OperatorAlreadyDefined) {
	}

	try {
		calculator.define('0', [](Lazy, Lazy) { return 0; });
		assert(false);
	}
	catch (OperatorAlreadyDefined) {
	}

	try {
		calculator.calculate("02&");
		assert(false);
	}
	catch (UnknownOperator) {
	}

	return 0;
}
