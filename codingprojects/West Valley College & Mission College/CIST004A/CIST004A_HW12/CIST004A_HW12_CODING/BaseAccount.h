#ifndef BaseAccount_h
#define BaseAccount_h
#pragma once
#include <string>
class BaseAccount {
public:
	BaseAccount(double initialBalance);
	~BaseAccount() {};
	double getBalance();
	virtual void credit(double amount);
	virtual bool debit(double amount);
	void setOwner(std::string owner);
	std::string getOwner();
private:
	long int balanceInPennies = 0L;
	std::string ownerName = "There is currently no name registered to the account.";
};
#endif
