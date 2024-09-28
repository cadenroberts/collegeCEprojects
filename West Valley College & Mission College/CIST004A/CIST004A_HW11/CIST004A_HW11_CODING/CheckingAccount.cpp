#include "CheckingAccount.h"
#include <string>
#include "BaseAccount.h"
#include <iostream>
CheckingAccount::CheckingAccount(double initialBalance, double initialFee) : BaseAccount::BaseAccount(initialBalance) {
	checkingFee = initialFee;
}

void CheckingAccount::deposit(double amount) {
	BaseAccount::deposit(amount - checkingFee);
	std::cout << "$" << checkingFee << " checking account transaction fee charged.\n";
}

bool CheckingAccount::withdraw(double amount) {
	if (amount + checkingFee < getBalance()) std::cout << "$" << checkingFee << " checking transaction fee charged.\n";
	return (BaseAccount::withdraw(amount + checkingFee));
}
