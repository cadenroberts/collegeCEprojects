// Caden Roberts 10/29/22 CIST004A

#include "Card.h"

#include <string>

const std::string Card::faceNames[] = {
  "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
};
const std::string Card::suitNames[] = {
  "Clubs", "Diamonds", "Hearts", "Spades"
};

Card::Card(size_t cardSuit, size_t cardFace) {
	refCount = 1;
	suit = cardSuit;
	face = cardFace;
}

std::string Card::cardToString() const {
	return faceNames[face] + " of " + suitNames[suit];
}

bool Card::operator > (const Card& param) {
	return ((face > param.face) || ((face == param.face) && (suit > param.suit)));
}
bool Card::operator < (const Card& param) {
	return ((face < param.face) || ((face == param.face) && (suit < param.suit)));
}
bool Card::operator== (const Card& param) {
	return ((face == param.face) && (suit == param.suit));
}
