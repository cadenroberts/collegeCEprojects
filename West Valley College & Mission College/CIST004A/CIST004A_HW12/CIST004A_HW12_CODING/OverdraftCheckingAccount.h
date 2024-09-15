#ifndef OverdraftCheckingAccount_h
#define  OverdraftCheckingAccount_h
#pragma once
#include <string>
#include "SavingsAccount.h"
#include "CheckingAccount.h"
class OverdraftCheckingAccount : public CheckingAccount {
public:
	OverdraftCheckingAccount(double intialBalance, double initialFee, double overdraft, SavingsAccount* Ptr);
	~OverdraftCheckingAccount() {};
	bool debit(double amount);
private:
	double overdraftFee;
	SavingsAccount* link;
};
#endif