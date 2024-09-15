// Caden Roberts 10/29/22 CIST004A
#include "Deck.h"
#include "Card.h"
#include <vector>
Deck::Deck() {
	for (size_t cardSuit = 0; cardSuit < 4; ++cardSuit) {
		for (size_t cardFace = 0; cardFace < 13; ++cardFace) {
			Deck::theDeckOfCards.emplace_back(Card{ cardSuit, cardFace });
		}
	}
	indexOfNextCard = 0;
}
void Deck::shuffleTheDeck() {
	for (size_t shuffleCount = 0; shuffleCount < 250; shuffleCount++) {
		size_t x = rand() % 52;
		size_t y = rand() % 52;
		Card temp(0, 0);
		temp = theDeckOfCards[x];
		theDeckOfCards[x] = theDeckOfCards[y];
		theDeckOfCards[y] = temp;
	}
	indexOfNextCard = 0;
}
Card Deck::dealOneCard() {
	return theDeckOfCards[indexOfNextCard++];
}
bool Deck::cardsLeft() {
	return (51 >= indexOfNextCard);
}