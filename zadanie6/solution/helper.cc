/* Autorzy rozwiązania : Michał Radecki, Kamil Ćwintal */

#include <iomanip>
#include <random>
#include "helper.h"

int randomNumber() {
	static std::uniform_int_distribution<int> distribution;
	static std::mt19937 rand_engine(0);
	return distribution(rand_engine);
}

std::ostream& operator<<(std::ostream& os, const Date& dt) {
	int days = dt.mins_passed/(MINS_IN_HOUR*HRS_IN_DAY)+1;
	int hours = (dt.mins_passed-MINS_IN_HOUR*HRS_IN_DAY*(days-1))/MINS_IN_HOUR;
	int minutes = dt.mins_passed%MINS_IN_HOUR;

	os << "December, " << days << ", " << hours << ":" << std::setfill('0') <<
	std::setw(2) << minutes << std::setfill(' ');

	return os;
}

Date& currentDate() {
	static Date sim_date(18, 16, 0);
	return sim_date;
}

std::ostream& operator<<(std::ostream& os, const Price& p) {
	os << p.value << " PLN";
	return os;
}
