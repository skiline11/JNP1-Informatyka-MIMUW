/* Autorzy rozwiązania : Michał Radecki, Kamil Ćwintal */

#include <iostream>
#include "person.h"

void Adult::receive_notification(const ChristmasTree& christmasTree) const {
	std::cout << "We have new decoration!" << std::endl;
	std::cout << "Total cost is " << christmasTree.getPrice() << "." << std::endl;
}

void Adult::buyDecoration(Decoration& decoration) {
	this->savings -= (decoration.getPrice());
}

Price Adult::getSavings() const {
	return this->savings;
}

void Child::receive_notification(const ChristmasTree& christmasTree) const {
	std::cout << "Wow!" << std::endl;
	christmasTree.getLastDecoration()->doOperation(std::cout);
}
