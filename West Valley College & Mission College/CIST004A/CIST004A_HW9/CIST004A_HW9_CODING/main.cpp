// Caden Roberts 10/29/22 CIST004A
#include <iostream>
#include <iomanip>
#include "Card.h"
#include "Deck.h"
int main() {
	int i{ 1 };
	std::cout << "Caden Roberts\t10/29/22\tCIST004A\n";
	Deck theDeckOfCards;
	do {
		std::cout << '\n';
		for (int j = i; j > 0; j--) theDeckOfCards.shuffleTheDeck();
		int counter{ 1 };
		while (theDeckOfCards.cardsLeft()) {
			std::cout << std::left << std::setw(20) <<
				(theDeckOfCards.dealOneCard()).cardToString();
			if (counter++ % 4 == 0) std::cout << '\n';
		}
		std::cout << "\nEnter # of times to shuffle again, or enter any non-integer key to exit : ";
		std::cin >> i;
	} while (!std::cin.fail());
	return 0;
}
