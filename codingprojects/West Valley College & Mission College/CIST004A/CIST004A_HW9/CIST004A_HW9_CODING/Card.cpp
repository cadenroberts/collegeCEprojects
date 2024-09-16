// Caden Roberts 10/29/22 CIST004A
#include "Card.h"
#include <string>
const std::string Card::namesOfFaces[] = {
  "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack",
"Queen", "King", "Ace"
};
const std::string Card::namesOfSuits[] = {
  "Hearts", "Clubs", "Diamonds", "Spades"
};
Card::Card(size_t cardSuit, size_t cardFace) {
	suit = cardSuit;
	face = cardFace;
}
size_t Card::getSuitValue() {
	return suit;
}
size_t Card::getFaceValue() {
	return face;
}
std::string Card::cardToString() {
	return namesOfFaces[getFaceValue()] + " of " + namesOfSuits[getSuitValue()];
}