#include "BaseAccount.h"
#include <string>
#include <iostream>

BaseAccount::BaseAccount(double initialBalance) {
	if ((balanceInPennies = lround(initialBalance * 100.0)) < 0L) {
		std::cout << "Failed Account Creation (Low Fund Entry)\n";
		balanceInPennies = 0L;
	}
}
double BaseAccount::getBalance() {
	return static_cast <double> (balanceInPennies) / 100.0;
}
void BaseAccount::credit(double amount) {
	if (lround(amount * 100.0) >= 0) balanceInPennies += lround(amount * 100.0);
	else std::cout << "Insufficient amount to warrant a deposit.\n";
}
bool BaseAccount::debit(double amount) {
	if (balanceInPennies >= lround(amount * 100.0)) balanceInPennies -= lround(amount * 100.0);
	else std::cout << "Insufficient funds to withdraw from this account.\n";
	return balanceInPennies >= lround(amount * 100.0);
}
void BaseAccount::setOwner(std::string owner) {
	ownerName = owner;
}
std::string BaseAccount::getOwner() {
	return ownerName;
}