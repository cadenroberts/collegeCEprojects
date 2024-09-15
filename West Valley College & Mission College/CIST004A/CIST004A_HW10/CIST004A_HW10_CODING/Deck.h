// Caden Roberts 10/29/22 CIST004A

#ifndef Deck_h
#define Deck_h
#pragma once

#include "Card.h"

#include <vector>

class DeckOfCards {
public:
	DeckOfCards();
	void shuffleTheDeck();
	Card* dealOneCard();
	Card* getCard(size_t);
	bool cardsLeft() const;
private:
	std::vector < Card* > deck;
	size_t indexOfNextCard;
};

#endif