// Caden Roberts 10/29/22 CIST004A

#include <iostream>

#include <iomanip>

#include "Card.h"

#include "Deck.h"

int main() {
	size_t input;
	size_t cinput;
	int Comp{ 0 };
	int You{ 0 };
	std::string x;
	std::cout << "Caden Roberts\t\t\t11/7/22\t\t\t\tCIST004A";
	DeckOfCards deck;
	do {
		std::cout << "\n\n* * * * * * * * *\t\tCurrent Score\t\t* * * * * * * * *\n" << "|||\tYou\t:\t" << You << "\t\t|||\tComputer\t:\t" << Comp;
		deck.shuffleTheDeck();
		do {
			std::cout << "\n\nPlease Enter a card number from 0 to 51: ";
			std::cin >> input;
			if ((std::cin.fail()) or (std::cin.peek() != '\n')) input = 52; std::cin.clear(); std::cin.ignore(1000, '\n');
		} while ((input > 51) or (input < 0));
		cinput = rand() % 52;
		std::cout << "\nYour selected card is:  " << (*(deck.getCard(input))).cardToString() << ".";
		std::cout << "\nThe Computer's card is: " << (*(deck.getCard(cinput))).cardToString() << ".";
		if ((*(deck.getCard(cinput))) > (*(deck.getCard(input)))) { x = "lose"; Comp++; }
		else if ((*(deck.getCard(cinput))) < (*(deck.getCard(input)))) { x = "win"; You++; }
		else x = "tied";
		std::cout << "\nYou " + x + " this round!";
	} while ((Comp - You) < 2 and (You - Comp) < 2);
	std::cout << "\nAnd you " + x + " the game!\n\nFINAL\t\tSCORE\t\tYou - " << You << "\t:\tComp - " << Comp << std::endl;
	return 0;
}