/* Autorzy rozwiązania : Michał Radecki, Kamil Ćwintal */

#ifndef DECORATION_H_
#define DECORATION_H_

#include <string>
#include <list>
#include <memory>
#include <exception>
#include "helper.h"

class Decoration;

#include "person.h"

class Decoration {
	protected:
		std::string name;
		Price my_price;

	public:
		Decoration(const std::string& name, Price my_price) :
			name(name), my_price(my_price) {}
		virtual ~Decoration() = default;

		std::string getName() const {
			return name;
		}

		virtual Price getPrice() const = 0;
		virtual void doOperation(std::ostream& os) = 0;
};

class Single_Decoration : public Decoration {
	public:
		Single_Decoration(const std::string& name, Price my_price) :
			Decoration(name, my_price) {}

		Price getPrice() const {
			return my_price;
		}
};

enum GlassBallStatus { Okay, Broken };
enum LightsStatus { Off, Constant, Blinking };

class GlassBall : public Single_Decoration {
	private:
		GlassBallStatus status;

	public:
		GlassBall(const std::string& name, Price my_price) :
			Single_Decoration(name, my_price), status(Okay) {}

		void doOperation(std::ostream& os);
};

class Lights : public Single_Decoration {
	private:
		LightsStatus status;

	public:
		Lights(const std::string& name, Price my_price) :
			Single_Decoration(name, my_price), status(Off) {}

		void doOperation(std::ostream& os);
};

class cut_date_error: public std::exception {
	public:
		const char* what() const noexcept {
			return "Nieprawidlowy czas sciecia drzewka";
		}
};

class Tree : public Single_Decoration {
	private:
		Date cut_date;

	public:
		Tree(const std::string& name, Price my_price, Date cut_date) :
			Single_Decoration(name, my_price), cut_date(cut_date) {
			if(currentDate() < cut_date) throw cut_date_error();
		}

		void doOperation(std::ostream& os);
};

class Multi_Decoration : public Decoration {
	public:
		Multi_Decoration(const std::string& name) :
			Decoration(name, Price()) {}
};

using DecorationPtr = std::shared_ptr<Decoration>;
using PersonPtr = std::shared_ptr<Person>;
using DecorationIterator = std::list<DecorationPtr>::iterator;
using PersonIterator = std::list<PersonPtr>::iterator;

class invalid_iterator_error: public std::exception {
	public:
		const char* what() const noexcept {
			return "Iterator nie wskazuje na dekoracje";
		}
};

class empty_tree_error: public std::exception {
	public:
		const char* what() const noexcept {
			return "Proba pobrania dekoracji z pustego drzewka";
		}
};

class ChristmasTree : public Multi_Decoration {
	private:
		std::list<DecorationPtr> christmasTreeDecorations;
		std::list<PersonPtr> observers;

	public:
		ChristmasTree(const std::string& name) :
			Multi_Decoration(name) {}

		void doOperation(std::ostream& os);
		Price getPrice() const;
		DecorationIterator addDecoration(DecorationPtr ptr_to_decoration);
		void removeDecoration(DecorationIterator it);
		DecorationIterator begin();
		DecorationIterator end();
		PersonIterator attachObserver(PersonPtr observer);
		void detachObserver(PersonIterator observer);
		void notify() const;
		DecorationPtr getLastDecoration() const;
};

#endif  // DECORATION_H_
