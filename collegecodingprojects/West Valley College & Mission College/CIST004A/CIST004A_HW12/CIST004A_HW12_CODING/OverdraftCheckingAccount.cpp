#include "CheckingAccount.h"
#include <string>
#include "BaseAccount.h"
#include <iostream>
#include <iomanip>
#include "SavingsAccount.h"
#include "OverdraftCheckingAccount.h"
OverdraftCheckingAccount::OverdraftCheckingAccount(double initialBalance, double initialFee, double overdraft, SavingsAccount* Ptr) : CheckingAccount::CheckingAccount(initialBalance, initialFee) {
	overdraftFee = overdraft;
	link = Ptr;
}
bool OverdraftCheckingAccount::debit(double amount) {
	if (CheckingAccount::debit(amount)) return true;
	else if (link->BaseAccount::debit(amount + checkingFee + overdraftFee - getBalance())) {
		BaseAccount::debit(getBalance());
		std::cout << "$" << overdraftFee << " overdraft fee charged and $" << checkingFee << " checking fee charged. $" << link->getBalance() << " remaining in Savings Account.\n";
		return true;
	}
	else return false;
}