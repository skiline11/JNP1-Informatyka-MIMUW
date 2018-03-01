/* Autorzy rozwiązania : Michał Radecki, Kamil Ćwintal */

#ifndef HELPER_H_
#define HELPER_H_
#include <ostream>
#include <exception>

extern int randomNumber();

const int MINS_IN_HOUR = 60;
const int HRS_IN_DAY = 24;
const int DAYS_IN_DEC = 31;

class overdraft_error: public std::exception {
	public:
		const char* what() const noexcept {
			return "Ujemny stan oszczednosci\n";
		}
};

class date_error: public std::exception {
	public:
		const char* what() const noexcept {
			return "Niepoprawny format daty\n";
		}
};

class Date {
	private:
		int mins_passed;

	public:
		Date(int day, int hour, int minute) :
		mins_passed(minute + MINS_IN_HOUR*hour + MINS_IN_HOUR*HRS_IN_DAY*(day-1)) {
			bool correct_date = (1 <= day && day <= DAYS_IN_DEC &&
			       				 0 <= hour && hour < HRS_IN_DAY &&
								 0 <= minute && minute < MINS_IN_HOUR);
			if(not correct_date) {
				throw date_error();
			}
		}

		void operator++() {
			mins_passed++;
		}

		int operator-(const Date& dt) {
			return (mins_passed - dt.mins_passed);
		}

		bool operator<(const Date& dt) {
			return (mins_passed < dt.mins_passed);
		}

		friend std::ostream& operator<<(std::ostream&, const Date&);
};

extern Date& currentDate();

class Price {
	private:
		int value;

	public:
		Price(int pln = 0) : value(pln) {
			if(pln < 0) throw overdraft_error();
		}

		Price operator+(const Price& p) {
			return Price(value+p.value);
		}

		void operator-=(const Price& p) {
			value -= p.value;
			if(value < 0) throw overdraft_error();
		}

		bool operator<(const Price& p) {
			return (value < p.value);
		}

		friend std::ostream& operator<<(std::ostream&, const Price&);
};

#endif  // HELPER_H_
