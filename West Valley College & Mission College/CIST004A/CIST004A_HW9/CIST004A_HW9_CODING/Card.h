// Caden Roberts 10/29/22 CIST004A
#ifndef Card_h
#define Card_h
#pragma once
#include <string>
class Card {
private:
	size_t face, suit;
	static const std::string namesOfFaces[], namesOfSuits[];
public:
	Card(size_t cardSuit, size_t cardFace);
	~Card() {};
	size_t getSuitValue(), getFaceValue();
	std::string cardToString();
};
#endif