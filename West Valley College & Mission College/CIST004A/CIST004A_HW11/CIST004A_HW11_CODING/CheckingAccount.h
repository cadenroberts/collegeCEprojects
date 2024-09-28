#ifndef CheckingAccount_h
#define CheckingAccount_h
#pragma once
#include <string>
#include "BaseAccount.h"
class CheckingAccount : public BaseAccount {
public:
	CheckingAccount(double initialBalance, double initialFee);
	~CheckingAccount() {};
	void deposit(double amount);
	bool withdraw(double amount);
private:
	double checkingFee = 0;
};

#endif