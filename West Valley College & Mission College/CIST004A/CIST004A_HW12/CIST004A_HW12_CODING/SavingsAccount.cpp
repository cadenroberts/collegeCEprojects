#include "SavingsAccount.h"
#include <string>
#include "BaseAccount.h"
#include <iostream>
SavingsAccount::SavingsAccount(double initialBalance, double initialRate) : BaseAccount::BaseAccount(initialBalance) {
	interestRate = initialRate;
}
double SavingsAccount::computeInterest() {
	return ((interestRate / 100.0) * getBalance());
}
