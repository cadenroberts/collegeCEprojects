#ifndef CheckingAccount_h
#define CheckingAccount_h
#pragma once
#include <string>
#include "BaseAccount.h"
class CheckingAccount : public BaseAccount {
public:
	CheckingAccount(double initialBalance, double initialFee);
	~CheckingAccount() {};
	virtual void credit(double amount);
	virtual bool debit(double amount);
protected:
	double checkingFee = 0;
};
#endif