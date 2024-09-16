// Caden Roberts 10/29/22 CIST004A
#ifndef Deck_h
#define Deck_h
#pragma once
#include "Card.h"
#include <vector>
class Deck {
private:
	std::vector < Card > theDeckOfCards;
	size_t indexOfNextCard;
public:
	Deck();
	~Deck() {};
	void shuffleTheDeck();
	Card dealOneCard();
	bool cardsLeft();
};
#endif