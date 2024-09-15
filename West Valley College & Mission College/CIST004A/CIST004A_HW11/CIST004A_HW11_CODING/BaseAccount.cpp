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

void BaseAccount::deposit(double amount) {
	if (amount > 0) balanceInPennies += lround(amount * 100.0);
	else std::cout << "Deposit failed; amount must be positive, dollars and pennies.\n";
}

bool BaseAccount::withdraw(double amount) {
	if (static_cast <double> (balanceInPennies) / 100.0 > amount) balanceInPennies -= lround(amount * 100.0);
	else std::cout << "Insufficient funds to honor the withdrawal request.\n";
	return static_cast <double> (balanceInPennies) / 100.0 > amount;
}

void BaseAccount::setOwner(std::string owner) {
	ownerName = owner;
}

std::string BaseAccount::getOwner() {
	return ownerName;
}
