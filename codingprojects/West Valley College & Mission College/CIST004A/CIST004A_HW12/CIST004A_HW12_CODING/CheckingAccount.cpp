#include "CheckingAccount.h"
#include <string>
#include "BaseAccount.h"
#include <iostream>
CheckingAccount::CheckingAccount(double initialBalance, double initialFee) : BaseAccount::BaseAccount(initialBalance) {
	checkingFee = initialFee;
}
void CheckingAccount::credit(double amount) {
	BaseAccount::credit(amount - checkingFee);
	if (amount > checkingFee) std::cout << "Checking Fee of $" << checkingFee << " charged.\n";
}


bool CheckingAccount::debit(double amount) {
	bool x = (BaseAccount::debit(amount + checkingFee));
	if (x) std::cout << "Checking Fee of $" << checkingFee << " charged.\n";
	return x;
}
