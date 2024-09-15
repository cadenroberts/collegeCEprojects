#include "Deck.h"

#include "Card.h"

#include <vector>

DeckOfCards::DeckOfCards() {
	for (size_t cardSuit = 0; cardSuit < 4; ++cardSuit) {
		for (size_t cardFace = 0; cardFace < 13; ++cardFace) {
			DeckOfCards::deck.emplace_back(new Card(cardSuit, cardFace));
		}
	}
	indexOfNextCard = 0;
}

void DeckOfCards::shuffleTheDeck() {
	for (size_t shuffleCount = 0; shuffleCount < 250; shuffleCount++) {
		size_t x = rand() % 52;
		size_t y = rand() % 52;
		Card* temp = (nullptr);
		temp = deck[x];
		deck[x] = deck[y];
		deck[y] = temp;
	}
	indexOfNextCard = 0;
}

Card* DeckOfCards::dealOneCard() {
	return deck[rand() % 52];
}

Card* DeckOfCards::getCard(size_t input) {
	return deck[input];
}
bool DeckOfCards::cardsLeft() const {
	return (51 >= indexOfNextCard);
}