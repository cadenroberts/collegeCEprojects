// Caden Roberts 10/29/22 CIST004A

#ifndef Card_h
#define Card_h
#pragma once

#include <string>

class Card {
public:
	Card(size_t cardSuit, size_t cardFace);
	std::string cardToString() const;
	bool operator > (const Card&);
	bool operator < (const Card&);
	bool operator == (const Card&);
private:
	int refCount;
	size_t face;
	size_t suit;

	static const std::string faceNames[13];
	static const std::string suitNames[4];
};

#endif