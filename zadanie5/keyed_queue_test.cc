#include "keyed_queue.h"

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <utility>
#include <algorithm>
#include <boost/operators.hpp>

using std::string;

// ukradzione z https://github.com/facebook/folly/blob/master/folly/Benchmark.h
template <typename T>
auto doNotOptimizeAway(const T& datum) {
	asm volatile("" ::"m"(datum) : "memory");
}

template<class K, class V>
bool operator==(keyed_queue<K, V> &q1, keyed_queue<K, V> &q2)
{
	if (q1.size() != q2.size())
		return false;

	auto q1c = q1;
	auto q2c = q2;
	// Sprawdzamy równość i spójność kolejek.
	std::set<K> keys;
	std::multiset<std::pair<K, V>> removed_by_key;
	std::multiset<std::pair<K, V>> removed_from_front;
	size_t q1_size = q1.size();
	size_t q2_size = q2.size();
	size_t key_counts1 = 0;
	size_t key_counts2 = 0;

	// Sprawdzamy spójność zbiorów kluczy.
	// Iteratory bierzemy z oryginalnych kolejek i dlatego parametry nie mogą być const.
	// To jest dlatego, że konstruktor kopiujący może ukryć pewne problemy ze spójnością
	// wewnętrznych struktur, szczególnie jeśli jedna z nich jest budowana na podstawie drugiej.
	auto it1 = q1.k_begin(), end1 = q1.k_end(), it2 = q2.k_begin(), end2 = q2.k_end();
	for (; it1 != end1 && end2 != it2; ++it1, ++it2) {
		if (*it1 != *it2)
			return false;
		if (q1.count(*it1) != q2.count(*it2))
			return false;
		keys.insert(*it1);
		key_counts1 += q1.count(*it1);
		key_counts2 += q2.count(*it2);
	}
	if (it1 != end1 || end2 != it2)
		return false;

	for (auto &k : keys) {
		while (q1c.count(k) > 0) {
			if (q1c.first(k) != q2c.first(k))
				return false;
			removed_by_key.insert(q1c.first(k));
			q1c.pop(k);
			q2c.pop(k);
		}
	}
	assert(q1c.empty() && q1c.size() == 0 && q2c.empty() && q2c.size() == 0);
	assert(key_counts1 == q1_size && key_counts2 == q2_size);
	// Jeszcze raz kopiujemy i sprawdzamy przez front lub pop.
	q1c = q1;
	q2c = q2;
	while (!q1c.empty()) {
		if (q1c.front() != q2c.front())
			return false;
		removed_from_front.insert(q1c.front());
		q1c.pop();
		q2c.pop();
	}
	assert(removed_by_key == removed_from_front);
	assert(q2c.empty());
	return true;
}

#if TEST_NUM != 100

long const max_throw_countdown = 30;
bool gChecking = false;
long throw_countdown;
long gInstancesCounter;

int ThisCanThrow(const int& i = 0)
{
	if (gChecking) {
		if (--throw_countdown <= 0) {
			throw 0;
		}
	}
	return i;
}

void* operator new(size_t size)
{
	try {
		ThisCanThrow();
		void* p = malloc(size);
		if (!p)
			throw "operator new[]() error";
		return p;
	}
	catch (...) {
		throw ::std::bad_alloc();
	}
}

void* operator new[](size_t size)
{
	try {
		ThisCanThrow();
		void* p = malloc(size);
		if (!p)
			throw "operator new[]() error";
		return p;
	}
	catch (...) {
		throw ::std::bad_alloc();
	}
}

void operator delete (void* p) noexcept
{
	free(p);
}

void operator delete[](void *p) noexcept
{
	free(p);
}

struct Tester
{
	int* p;
	explicit Tester (int v = 0) : p(new int(ThisCanThrow(v))) {
		++gInstancesCounter;
	}

	Tester (const Tester& rhs) : p(new int(ThisCanThrow(*rhs.p))) {
		++gInstancesCounter;
	}

	Tester(Tester&& t) noexcept : p(t.p) {
		++gInstancesCounter;
		t.p = nullptr;
	}

	Tester& operator=(const Tester& rhs) {
		ThisCanThrow();
		*p = *rhs.p;
		return *this;
	}

	~Tester( ) noexcept {
		/* This CANNOT throw! */
		gInstancesCounter--;
		delete p;
	}

	bool const operator<(const Tester& rhs) const {
		ThisCanThrow();
		return *p < *rhs.p;
	}

	bool const operator>(const Tester& rhs) const {
		ThisCanThrow();
		return *p > *rhs.p;
	}

	bool const operator<=(const Tester& rhs) const {
		ThisCanThrow();
		return *p <= *rhs.p;
	}

	bool const operator>=(const Tester& rhs) const {
		ThisCanThrow();
		return *p >= *rhs.p;
	}

	bool const operator==(const Tester& rhs) const {
		ThisCanThrow();
		return *p == *rhs.p;
	}

	bool const operator!=(const Tester& rhs) const {
		ThisCanThrow();
		return *p != *rhs.p;
	}
};

int main();

class CopyOnly {
	friend int main();
	CopyOnly() {};
public:
	CopyOnly(CopyOnly &&) = delete;
	CopyOnly(CopyOnly const &other) = default;
	CopyOnly& operator=(CopyOnly const &) = delete;
	CopyOnly& operator=(CopyOnly &&) = delete;
};

struct IdentityTester : boost::equality_comparable<IdentityTester>
{
	static size_t next;
	size_t const id;

	IdentityTester() : id(next++) {
	}

	IdentityTester(IdentityTester const &other) : id(next++) {
	}

	IdentityTester(IdentityTester &&other) = default;

	bool operator==(IdentityTester const &other) const {
		return id == other.id;
	}

};

size_t IdentityTester::next = 0;

::std::ostream & operator << (::std::ostream& os, Tester t) {
	return os << "<tester(" << t.p << ")>";
}


template <class Value, class Operation, class Result>
Result NoThrowCheck(Value &v, Operation const &op, string const &name)
{
	Result result;

	try {
		throw_countdown = 0;
		gChecking = true;
		result = op(v); /* Try the operation. */
		gChecking = false;
	} catch(...) { /* Catch all exceptions. */
		gChecking = false;
		::std::clog << "Operacja '" << name << "' okazala sie nie byc NO-THROW\n" << ::std::endl;
		assert(false);
	}

	return result;
}

template <class Value, class Operation>
void NoThrowCheckVoid(Value &v, Operation const &op, string const &name)
{
	try {
		throw_countdown = 0;
		gChecking = true;
		op(v); /* Try the operation. */
		gChecking = false;
	} catch(...) { /* Catch all exceptions. */
		gChecking = false;
		::std::clog << "Operacja '" << name << "' okazala sie nie byc NO-THROW\n";
		assert(false);
	}
}

template <class Value, class Operation, class Result>
Result StrongCheck(Value &v, Operation const &op, string const &name)
{
	Result result;
	bool succeeded = false;

	Value duplicate = v;
	try {
		gChecking = true;
		result = op(duplicate); /* Try the operation. */
		gChecking = false;
		succeeded = true;
	} catch(...) { /* Catch all exceptions. */
		gChecking = false;
		bool unchanged = duplicate == v; /* Test strong guarantee. */
		if (!unchanged)
		{
			::std::clog << "Operacja '" << name << "' okazala sie nie byc STRONG." << ::std::endl;
			assert(unchanged);
		}
	}

	if (succeeded) {
		v = duplicate;
	}
	return result;
}

template <class Value, class Operation>
void StrongCheckVoid(Value &v, Operation const &op, string const &name)
{
	bool succeeded = false;
	Value duplicate = v;
	// Wymuś kopię -- bez tego zazwyczaj nie uda się operacja COW
	// i będzie trywialnie spełniona asercja.
	if (duplicate.size() > 0)
		duplicate.front();

	try {
		gChecking = true;
		op(duplicate); /* Try the operation. */
		gChecking = false;
		succeeded = true;
	} catch(...) { /* Catch all exceptions. */
		gChecking = false;
		bool unchanged = duplicate == v; /* Test strong guarantee. */
		if (!unchanged) {
			::std::clog << "Operacja '" << name << "' okazala sie nie byc STRONG."
			            << ::std::endl;
			assert(unchanged);
		}
	}
	if (succeeded) {
		v = duplicate;
	}
}

template<class K, class V>
std::set<K> get_keys(keyed_queue<K, V> &q) {
	return std::set<K>(q.k_begin(), q.k_end());
}

template<class K, class V>
void test_push_spec(keyed_queue<K, V> &q1, K const &k, V v)
{
	auto q_old = q1;
	std::set<K> keys = get_keys(q1);

	q1.push(k, v);

	for (auto kk : get_keys(q1)) {
		// Method push() should not add any other keys.
		if (kk != k)
			assert(keys.count(kk));
	}

	// Check effects on count().
	assert(q1.count(k) == q_old.count(k) + 1);
	for (auto kk : keys) {
		if (kk != k)
			assert(q1.count(kk) == q_old.count(kk));
	}

	// Check effects on size() and empty().
	assert(q1.size() == q_old.size() + 1);
	assert(q1.size() > 1 || q_old.empty());

	// Check effects on first().
	if (q_old.count(k) > 0) {
		// If q_old already had an element with key k,
		// then first(k) should be the same.
		assert(q1.first(k) == q_old.first(k));
	}
	for (auto kk : keys) {
		// If the key was present in q_old, that means
		// it had at least one element mapped to it.
		// Therefore first(k) should give the same result on q1.
		assert(q1.first(kk) == q_old.first(kk));
	}

	// Check effects on last().
	assert(q1.last(k).second == v);
	for (auto kk : keys) {
		// Method last(k) should be preserved for unmodified keys.
		if (kk != k)
			assert(q1.last(kk) == q_old.last(kk));
	}

	// Check effects on front().
	if (q_old.size() == 0) {
		assert(q1.front().second == v);
	}

	// Check effects on back().
	assert(q1.back().second == v);
}

template<class K, class V>
void check_pop_key(keyed_queue<K, V> &q_old,
                   keyed_queue<K, V> &q1,
                   K const &k)
{
	std::set<K> keys = get_keys(q_old);

	// Method pop() should not add any keys.
	for (auto kk : get_keys(q1)) {
		assert(keys.count(kk));
	}

	// Check effects on count().
	assert(q1.count(k) == q_old.count(k) - 1);
	for (auto kk : keys) {
		if (kk != k)
			assert(q1.count(kk) == q_old.count(kk));
	}

	// Check effects on size() and empty().
	assert(q1.size() == q_old.size() - 1);
	assert(q1.size() > 0 || q1.empty());

	// Check effects on first().
	for (auto kk : keys) {
		if (kk != k)
			assert(q1.first(kk) == q_old.first(kk));
	}

	// Check effects on last().
	for (auto kk : keys) {
		if (kk != k || q_old.count(kk) > 1)
			assert(q1.last(kk) == q_old.last(kk));
	}


	// Check effects on front().
	if (q_old.size() > 1 && q_old.front().first != k) {
		assert(q1.front() == q_old.front());
	}

	// Check effects on back().
	if (q_old.size() > 1) {
		assert((q_old.count(k) == 1 && q_old.back().first == k)
		       || q1.back() == q_old.back());
	}

}

template<class K, class V>
void test_pop_key_spec(keyed_queue<K, V> &q1, K const &k)
{
	auto q_old = q1;
	auto kcopy = k;
	// Tak naprawdę chcielibyśmy, żeby tu jeszcze działała referencja, ale nie będę taki wredny.
	q1.pop(kcopy);
	// Tu już potrzeba kopii.
	check_pop_key(q_old, q1, kcopy);
}

template<class K, class V>
void test_pop_spec(keyed_queue<K, V> &q1)
{
	auto q_old = q1;
	auto k = q1.front().first;
	q1.pop();
	check_pop_key(q_old, q1, k);
}

typedef keyed_queue<Tester, Tester> TesterQ;

void empty_exception_check(const TesterQ& q, const Tester& (TesterQ::*m)() const)
{
	bool exception_occured = false;
	try
	{
		(q.*m)();
	}
	catch (lookup_error& e)
	{
		exception_occured = true;
	}
	catch (...)
	{
	}

	assert(exception_occured);
}

#endif

auto f(keyed_queue<int, int> q)
{
	return q;
}

int main() {

// Test z treści zadania
#if TEST_NUM == 100
	int keys[] = {3, 1, 2};

	keyed_queue<int, int> kq1 = f({});

	for (int i = 0; i < 3; ++i) {
		kq1.push(keys[i], i);
	}
	auto &ref = kq1.front().second;

	keyed_queue<int, int> kq2(kq1);
	keyed_queue<int, int> kq3;
	kq3 = kq2;

	ref = 10;
	assert(kq1.front().second == 10);
	assert(kq2.front().second != 10);

	kq2.pop();
	assert(kq2.size() == 2);
	assert(kq2.count(3) == 0);
	assert(kq2.count(2) == 1);

	assert(kq3.size() == 3);
	assert(kq3.count(3) == 1);

	kq2.push(1, 3);
	kq2.move_to_back(1);
	assert(kq2.size() == 3);
	assert(kq2.front().second == 2
	       && kq2.first(1).second == 1
	       && kq2.last(1).second == 3
	       && kq2.back().second == 3);

	keyed_queue<int, int> const kq4 = kq2;
	assert(kq4.front().second == 2
	       && kq4.first(1).second == 1
	       && kq4.last(1).second == 3
	       && kq4.back().second == 3);

	int i = 1;
	for (auto k_it = kq1.k_begin(), k_end = kq1.k_end(); k_it != k_end;
	     ++k_it, ++i) {
		assert(i <= 3 && *k_it == i);
	}

	auto kq5 = std::make_unique<keyed_queue<int, int>>();
	kq5->push(4, 0);
	assert(kq5->front().first == 4 && kq5->front().second == 0);
	auto kq6(*kq5);
	kq5.reset();
	assert(kq6.front().first == 4 && kq6.front().second == 0);

	std::swap(kq1, kq2);
	std::vector<keyed_queue<int, int>> vec;
	for (int i = 0; i < 100000; i++) {
		kq1.push(i, i);
	}
	for (int i = 0; i < 1000000; i++) {
		vec.push_back(kq1);
	}

#endif

// Testy poprawnościowe: sprawdzają, czy wszystkie funcje dają wyniki i efekty zgodne z treścią zadania.
// Ten test nie sprawdza kwestii związanych z wyjątkami.

// Konstruktor bezparametrowy, konstruktor kopiujący, konstruktor przenoszący, operator przypisania
#if TEST_NUM == 200
	keyed_queue<int, int> q;

	keyed_queue<int, int> r = keyed_queue<int, int>(q);
	assert(r == q);

	keyed_queue<int, int> s(f(q));
	assert(s == q);

	keyed_queue<int, int> t;
	t = q;
	assert(t == q);

	r.push(3,33);
	r.push(4,44);
	r.push(5,44);

	keyed_queue<int, int> u = keyed_queue<int, int>(r);
	assert(u == r);

	keyed_queue<int, int> v = keyed_queue<int, int>(f(r));
	assert(v == r);

	t = r;
	assert(t == r);

	t = f(r);
	assert(t == r);
#endif

// empty, size, push, pop(), pop(k), front, back
#if TEST_NUM == 201
	keyed_queue<int, int> q;
	keyed_queue<int, int> const &qc = q;
	assert(q.empty());
	assert(q.size() == 0);

	for (int i = 0; i < 1000; i++)
		test_push_spec(q, i, i);

	assert(qc.front().first == 0 && qc.front().second == 0);
	assert(qc.back().first == 999 && qc.back().second == 999);
	assert(q.size() == 1000);
	assert(!q.empty());

	test_pop_spec(q);
	assert(qc.front().first == 1 && qc.front().second == 1);
	assert(qc.back().first == 999 && qc.back().second == 999);
	assert(!q.empty());
	assert(q.size() == 999);

	test_pop_key_spec(q, 999);
	assert(qc.front().first == 1 && qc.front().second == 1);
	assert(qc.back().first == 998 && qc.back().second == 998);
	assert(!q.empty());
	assert(q.size() == 998);

	for (int i = 1; i < 500; i++) {
		assert(qc.front().first == i && qc.front().second == i);
		assert(qc.back().first == 999 - i && qc.back().second == 999 - i);
		test_pop_spec(q);
		test_pop_key_spec(q, q.back().first);
	}

	assert(q.size() == 0);
	assert(q.empty());

	for (int j = 0; j < 100; j++)
		for (int i = 0; i < 10; i++)
			test_push_spec(q, i, j);

	assert(q.size() == 1000);
	assert(q.front().first == 0 && q.front().second == 0);
	assert(q.back().first == 9 && q.back().second == 99);
	assert(!q.empty());

	test_pop_spec(q);
	assert(!q.empty());
	assert(q.front().first == 1 && q.front().second == 0);
	assert(q.back().first == 9 && q.back().second == 99);
	assert(q.size() == 999);

	test_pop_key_spec(q, 1);
	assert(!q.empty());
	assert(q.front().first == 2 && q.front().second == 0);
	assert(q.back().first == 9 && q.back().second == 99);
	assert(q.size() == 998);

	for (int j = 0; j < 100; j++) {
		for (int i = 0; i < 10; i += 2) {
			if (j == 0 && i == 0) continue;
			test_pop_key_spec(q, i + 1);
			assert(qc.front().first == i && qc.front().second == j);
			if (j < 99)
				assert(qc.first(i + 1).first == i + 1
				       && qc.first(i + 1).second == j + 1);
			test_pop_spec(q);
		}
	}
	assert(q.empty());
	assert(q.size() == 0);
#endif

// first, last, count
#if TEST_NUM == 202
	keyed_queue<int, int> q;

	for (int i = 0; i < 100; i++)
	{
		assert(q.count(i) == 0);
		q.push(i,i);
		assert(q.count(i) == 1);
		assert(q.first(i).first == i && q.first(i).second == i);
		assert(q.last(i).first == i && q.last(i).second == i);
	}

	for (int i = 0; i < 100; i++)
	{
		q.push(i,i + 1);
		assert(q.count(i) == 2);
		assert(q.first(i).first == i && q.first(i).second == i);
		assert(q.last(i).first == i && q.last(i).second == i + 1);
	}

	for (int i = 0; i < 100; i++)
	{
		q.pop(i);
		assert(q.count(i) == 1);
		assert(q.first(i).first == i && q.first(i).second == i + 1);
		assert(q.last(i).first == i && q.last(i).second == i + 1);
	}

	for (int i = 0; i < 100; i++)
	{
		q.push(i, i - 1);
		assert(q.count(i) == 2);
		assert(q.first(i).first == i && q.first(i).second == i + 1);
		assert(q.last(i).first == i && q.last(i).second == i - 1);
	}

	for (int i = 2; i < 100; i += 3)
	{
		q.pop(i);
		assert(q.count(i) == 1);
		assert(q.first(i).first == i && q.first(i).second == i - 1);
		assert(q.last(i).first == i && q.last(i).second == i - 1);
	}
#endif

// k_begin, k_end, iteratory
#if TEST_NUM == 203
	keyed_queue<int, int> q;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 10; j++) {
			q.push(j,i);
		}
	}

	int i = 0;
	for (auto it = q.k_begin(), end = q.k_end(); it != end; ++it, ++i) {
		assert(*it == i);
	}
	assert(i == 10);

	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 10; j++) {
			q.push(j,i);
		}
	}
	i = 0;
	for (auto it = q.k_begin(), end = q.k_end(); it != end; ++it, ++i) {
		assert(*it == i);
	}
	assert(i == 10);


	keyed_queue<int, int> r;
	for (int i = 0; i < 100; i++) {
		q.push(i, i);
	}
	i = 0;
	for (auto it = q.k_begin(), end = q.k_end(); it != end; ++it, ++i) {
		assert(*it == i);
	}
	assert(i == 100);
#endif

// swap (lokalny i globalny)
#if TEST_NUM == 204
	keyed_queue<int, int> q;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 10; j++) {
			q.push(j,i);
		}
	}
	keyed_queue<int, int> qq = keyed_queue<int, int>(q);

	keyed_queue<int, int> r;
	for (int i = 0; i > -1000; i--) {
		for (int j = 0; j < 10; j++) {
			r.push(-j,i);
		}
	}
	keyed_queue<int, int> rr = keyed_queue<int, int>(r);

	std::swap(q, r);
	assert(q == rr && r == qq);

	std::swap(q, r);
	assert(q == qq && r == rr);

	std::swap(r, q);
	assert(q == rr && r == qq);

	std::swap(q, r);
	assert(q == qq && r == rr);

	std::swap(q, q);
	assert(q == qq);

	keyed_queue<int, int> s, t;
	std::swap(s, t);
	assert(s.empty() && s == t);
#endif

// Sprawdza, czy front, back, first, last rzeczywiście zwracają tę samą wartość, kiedy potrzeba.
#if TEST_NUM == 205
	keyed_queue<int, IdentityTester> q;

	for (int i = 0; i < 100; i++) {
		q.push(i, IdentityTester());
	}

	for (int i = 0; i < 100; i++) {
		assert(q.first(i).second == q.last(i).second);
	}

	assert(q.front().second == q.first(0).second);
	assert(q.front().second == q.last(0).second);
	assert(q.back().second == q.first(99).second);
	assert(q.back().second == q.last(99).second);

	for (int i = 0; i < 100; i++) {
		q.push(i, IdentityTester());
	}

	for (int i = 0; i < 100; i++) {
		assert(q.first(i).second != q.last(i).second);
	}
	assert(q.front().second == q.first(0).second);
	assert(q.back().second == q.last(99).second);
#endif

// move_to_back
#if TEST_NUM == 206
	keyed_queue<int, int> q;

	q.push(2, 1);
	q.push(1, 1);
	q.push(1, 2);
	q.push(2, 2);
	q.push(1, 1);
	q.push(2, 1);
	q.push(2, 3);

	q.move_to_back(2);
	assert(q.count(2) == 4);
	assert(q.back().second == 3);
	assert(q.front().first == 1);
	for (int i = 0; i < 3; i++) {
		assert(q.front().first == 1);
		assert(q.front().second == i%2 + 1);
		q.pop();
	}
	for (int i = 0; i < 3; i++) {
		assert(q.front().first == 2);
		assert(q.front().second == i%2 + 1);
		q.pop();
	}
	assert(q.front().first == 2);
	assert(q.front().second == 3);
	assert(!q.empty());
	assert(q.count(2) == 1);
	assert(q.size() == 1);
#endif

// czy move_to_back kopiuje wartości?
#if TEST_NUM == 207
	keyed_queue<int, IdentityTester> q;
	q.push(2, IdentityTester());
	q.push(1, IdentityTester());
	q.push(1, IdentityTester());
	q.push(2, IdentityTester());
	auto front_id = q.front().second.id;
	auto back_id = q.back().second.id;

	q.move_to_back(2);
	q.pop(); q.pop();
	assert(q.front().second.id == front_id && q.back().second.id == back_id);
#endif

// Dodatkowe testy zgodności z treścią:
	// - testy gwarancji no-throw: konstruktor przenoszący, destruktor
	// - testy założeń nt. typu V

// empty -- to nie jest wyraźnie napisane w treści, ale nie wyobrażam sobie
//          dobrego powodu, dla którego coś tu mogłoby rzucać
#if TEST_NUM == 300
	TesterQ q;

	auto f = [](auto &q) { return q.empty(); };
	bool empty = NoThrowCheck<TesterQ, decltype(f), bool>(q, f, "empty");
	assert(empty);
#endif

// size
#if TEST_NUM == 301
	TesterQ q;

	auto f = [](auto &q) { return q.size(); };
	bool size = NoThrowCheck<TesterQ, decltype(f), bool>(q, f, "size");
	assert(size == 0);
#endif

// std::swap (konstruktor przenoszący)
#if TEST_NUM == 302
	TesterQ q1, q2;

	for (int i = 1; i < 1000; i++) {
		for (int j = 1; j < 10; j++) {
			q1.push(Tester(j), Tester(i));
			q2.push(Tester(-j), Tester(-i));
		}
	}

	TesterQ q1Copy(q1);
	TesterQ q2Copy(q2);

	NoThrowCheckVoid(q1, [&q2](auto &q) { std::swap(q, q2); }, "std::swap");
	assert(q1Copy == q2);
	assert(q2Copy == q1);

	NoThrowCheckVoid(q1, [&q2](auto &q) { std::swap(q, q2); }, "std::swap");
	assert(q1Copy == q1);
	assert(q2Copy == q2);
#endif

// destruktor
#if TEST_NUM == 303
	char buf[sizeof(TesterQ)];
	TesterQ *q = new (buf) TesterQ();

	for (int i = 1; i < 1000; i++) {
		for (int j = 1; j < 10; j++) {
			q->push(Tester(j), Tester(i));
		}
	}

	NoThrowCheckVoid(*q, [](auto &q) { q.~TesterQ(); }, "destruktor");
#endif

// Czy rozwiązanie nie zakłada za dużo na temat typu wartości?
#if TEST_NUM == 304
	keyed_queue<int, CopyOnly> q;
	q.push(0, CopyOnly());
	q.first(0);
	q.last(0);
	q.front();
	q.back();
	q.move_to_back(0);
	q.pop();
	q.clear();
#endif

// Czy rozwiązanie nie przechowuje zbyt dużo kopii wartości?
#if TEST_NUM == 305
	keyed_queue<int, Tester> q1;

	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 10; j++) {
			q1.push(j, Tester(i));
		}
	}

	assert(q1.size() == 10000);
	assert(gInstancesCounter == 10000);
	doNotOptimizeAway(q1);
#endif

// Testy gwarancji silnych

// push
#if TEST_NUM == 400
	// Żeby było trochę bardziej złośliwie -- nie rzucaj przy pierwszej możliwej operacji.
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		TesterQ q;

		Tester t1 = Tester(-1);
		Tester t2 = Tester(-100);
		q.push(t1, t2);
		Tester t3 = Tester(0);
		Tester t4 = Tester(100);
		StrongCheckVoid(q, [&](auto &q) { q.push(t3, t4); }, "push");

		for (int i = 100; i < 110; i++) {
			Tester t1 = Tester(i);
			Tester t2 = Tester(100);
			StrongCheckVoid(q, [&](auto &q) { q.push(t1, t2); }, "push");
		}

		for (int i = 0; i < 10; i++) {
			int k = (48271 * i) % 31;
			q.push(Tester(k), Tester(100));
		}

		for (int i = 0; i < 10; i++) {
			int k = (16807 * i) % 31;
			Tester t1 = Tester(k);
			Tester t2 = Tester(100);
			StrongCheckVoid(q, [&](auto &q) { q.push(t1, t2); }, "push");
		}

		for (auto it = q.k_begin(), end = q.k_end(); it != end; ++it) {
			assert(q.count(*it) > 0);
		}
	}
#endif

// Dla first i last to raczej nie powinno być problemem, ale dla kompletności można przetestować.
#if TEST_NUM == 401
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		TesterQ q;

		q.push(Tester(1), Tester(42));
		q.push(Tester(2), Tester(13));

		Tester t1 = Tester(1);
		Tester t2 = Tester(2);
		StrongCheckVoid(q, [&](auto &q) { q.first(t2); }, "first");
		StrongCheckVoid(q, [&](auto &q) { q.last(t1); }, "last");
	}
#endif

// pop
#if TEST_NUM == 402
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		TesterQ q;

		q.push(Tester(1), Tester(42));
		q.push(Tester(2), Tester(13));

		auto popfun = [](auto &q) { q.pop(); };
		StrongCheckVoid(q, popfun, "pop");
		StrongCheckVoid(q, popfun, "pop");
		StrongCheckVoid(q, popfun, "pop");
	}
#endif

// pop(k)
#if TEST_NUM == 403
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		TesterQ q;

		q.push(Tester(1), Tester(42));
		q.push(Tester(2), Tester(13));

		Tester t1 = Tester(1);
		Tester t2 = Tester(2);
		StrongCheckVoid(q, [&](auto &q) { q.pop(t2); }, "pop(k)");
		StrongCheckVoid(q, [&](auto &q) { q.pop(t1); }, "pop(k)");
		StrongCheckVoid(q, [&](auto &q) { q.pop(t2); }, "pop(k)");
	}
#endif

// Dla clear zależnie od implementacji może to mieć sens.
#if TEST_NUM == 404
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		TesterQ q;

		q.push(Tester(1), Tester(100));
		q.push(Tester(2), Tester(100));
		q.push(Tester(3), Tester(300));

		StrongCheckVoid(q, [](auto &q) { q.clear(); }, "clear");

		TesterQ r;

		r.push(Tester(1), Tester(100));
		r.push(Tester(2), Tester(100));
		r.push(Tester(3), Tester(300));
		q = r;

		StrongCheckVoid(q, [](auto &q) { q.clear(); }, "clear");
	}
#endif

// przypisanie
#if TEST_NUM == 405
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		TesterQ p, q;

		StrongCheckVoid(p, [&q](auto &p) { p = q; }, "operator=");

		q.push(Tester(3), Tester(9));
		q.push(Tester(4), Tester(16));

		StrongCheckVoid(p, [&q](auto &p) { p = q; }, "operator=");

		TesterQ r = q;
		StrongCheckVoid(p, [&r](auto &p) { p = r; }, "operator=");
	}
#endif

// Ważność iteratorów
// Uwaga: ten test naprawdę powinien być wykonany pod valgrindem.
#if TEST_NUM == 406
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		keyed_queue<int, Tester> q1;
		q1.push(0, Tester());
		q1.push(1, Tester());
		q1.push(2, Tester());
		auto q2 = std::make_unique<keyed_queue<int, Tester>>(q1);
		q2->push(3, Tester());
		keyed_queue<int, Tester> q3 = *q2;
		auto it = q3.k_begin();
		auto end = q3.k_end();
		auto tester = Tester();
		try {
			gChecking = true;
			q3.push(0, tester);
			gChecking = false;
			break; // Jeśli się udało, to poniżej będą problemy.
		} catch (...) {
			gChecking = false;
		}
		q2.reset();
		for (; it != end; ++it) {
			volatile auto _ = *it;
			(void)_;
		}
	}
#endif

// move_to_back
#if TEST_NUM == 407
	for (int i = 0; i < max_throw_countdown; i++, throw_countdown = i) {
		TesterQ q;

		q.push(Tester(1), Tester(42));
		q.push(Tester(2), Tester(13));
		q.push(Tester(3), Tester());
		q.push(Tester(2), Tester());

		StrongCheckVoid(q, [](auto &q) { q.move_to_back(Tester(2)); }, "move_to_back");
	}
#endif

// Czy rzucane są wyjątki zgodnie ze specyfikacją?

// move_to_back
#if TEST_NUM == 500
	TesterQ q;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			q.push(Tester(j), Tester(i));

	bool exception_occured = false;
	try {
	        q.move_to_back(Tester(10));
	} catch (lookup_error &e) {
	    exception_occured = true;
	} catch (...) {
	}

	assert(exception_occured);
#endif

// pop(k)
#if TEST_NUM == 501
	TesterQ q;

	q.push(Tester(0), Tester(0));
	bool exception_occured = false;
	try
	{
		q.pop(Tester(1));
	}
	catch (lookup_error &e)
	{
		exception_occured = true;
	}
	catch (...)
	{
	}

	assert(exception_occured);
#endif

// pop()
#if TEST_NUM == 502
	TesterQ q;

	bool exception_occured = false;
	try
	{
		q.pop();
	}
	catch (lookup_error &e)
	{
		exception_occured = true;
	}
	catch (...)
	{
	}

	assert(exception_occured);
#endif

// last
#if TEST_NUM == 503
	TesterQ q;

	q.push(Tester(0), Tester(0));
	bool exception_occured = false;
	try
	{
		q.last(Tester(1));
	}
	catch (lookup_error &e)
	{
		exception_occured = true;
	}
	catch (...)
	{
	}

	assert(exception_occured);
#endif

// first
#if TEST_NUM == 504
	TesterQ q;

	q.push(Tester(0), Tester(0));
	bool exception_occured = false;
	try
	{
		q.first(Tester(1));
	}
	catch (lookup_error &e)
	{
		exception_occured = true;
	}
	catch (...)
	{
	}

	assert(exception_occured);
#endif

// back
#if TEST_NUM == 505
	TesterQ q;

	bool exception_occured = false;
	try
	{
		q.back();
	}
	catch (lookup_error &e)
	{
		exception_occured = true;
	}
	catch (...)
	{
	}

	assert(exception_occured);
#endif

// front
#if TEST_NUM == 506
	TesterQ q;

	bool exception_occured = false;
	try
	{
		q.front();
	}
	catch (lookup_error &e)
	{
		exception_occured = true;
	}
	catch (...)
	{
	}

	assert(exception_occured);
#endif

// Testy kopiowania

// test COW
#if TEST_NUM == 600
	auto kq1 = std::make_unique<keyed_queue<int, int>>();
	std::vector<keyed_queue<int, int>> vec;
	for (int i = 0; i < 100000; i++) {
		kq1->push(i, i);
	}
	for (int i = 0; i < 1000000; i++) {
		vec.push_back(*kq1);
		doNotOptimizeAway(vec[i]);
	}
	kq1.reset();
	for (int i = 0; i < 10; i++) {
		vec[i].push(i, i);
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 11; j++) {
			if (i == j)
				continue;
			assert(!(vec[i] == vec[j]));
		}
	}
	assert(!(vec[0] == vec[999999]));
#endif

// zmiana wartości w prostej wersji
#if TEST_NUM == 601
	keyed_queue<int, int> kq1;

	for (int i = 0; i < 3; ++i) {
		kq1.push(i, i);
	}

	keyed_queue<int, int> kq2(kq1);

	kq1.front().second = 10;

	assert(kq1.front().second == 10);
	assert(kq2.front().second != 10);
#endif

// move_to_back
#if TEST_NUM == 602
	keyed_queue<int, int> q;

	q.push(1, 42);
	q.push(2, 13);
	q.push(3, 0);
	q.push(2, 0);

	keyed_queue<int, int> qCopy1(q);
	keyed_queue<int, int> qCopy2;
	qCopy2 = q;

	q.move_to_back(2);

	assert(!(qCopy1 == q));
	assert(qCopy1 == qCopy2);
	assert(!(qCopy2 == q));
#endif

// clear
#if TEST_NUM == 603
	keyed_queue<int, int> q;

	q.push(1, 42);
	q.push(2, 13);
	q.push(3, 0);
	q.push(2, 0);

	keyed_queue<int, int> qCopy1(q);
	keyed_queue<int, int> qCopy2;
	qCopy2 = q;

	q.clear();

	assert(q.empty());
	assert(!qCopy1.empty());
	assert(!qCopy2.empty());
	assert(!(qCopy1 == q));
	assert(qCopy1 == qCopy2);
	assert(!(qCopy2 == q));
#endif

// Czy flaga unshareable albo jej odpowiednik jest resetowana po modyfikacji?
#if TEST_NUM == 604
	auto kq1 = keyed_queue<int, int>();
	std::vector<keyed_queue<int, int>> vec;
	for (int i = 0; i < 100000; i++) {
		kq1.push(i, i);
	}
	kq1.front(); // Ustaw flagę unshareable.
	kq1.push(-1, -1);

	for (int i = 0; i < 1000000; i++) {
		vec.push_back(kq1);
		doNotOptimizeAway(vec[i]);
	}
#endif

// Test sprawdzający, czy jest header guard.

#if TEST_NUM == 700
#include "keyed_queue.h"
#include "keyed_queue.h"
#endif

// Test sprawdzający, czy wyjątek dziedziczy po std::exception.

#if TEST_NUM == 800
	try {
		throw lookup_error();
	}
	catch (std::exception e) {
		return 0;
	}
	catch (...) {
		assert(false);
	}
#endif
	return 0;
}
