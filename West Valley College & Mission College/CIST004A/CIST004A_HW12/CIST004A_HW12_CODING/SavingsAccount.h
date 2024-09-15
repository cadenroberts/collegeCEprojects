#ifndef SavingsAccount_h
#define SavingsAccount_h
#pragma once
#include <string>
#include "BaseAccount.h"
class SavingsAccount : public BaseAccount {
public:
	SavingsAccount(double initialBalance, double initialRate);
	~SavingsAccount() {};
	double computeInterest();
private:
	double interestRate = 0;
};
#endif