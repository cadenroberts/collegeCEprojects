#include <iostream>
#include <iomanip>
#include <string>
#include "BaseAccount.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
int main() {


	std::cout << "Caden Roberts\t\t11/17/22\t\tCIST004A\nCreating 3 Test Bank Accounts:\n";

	BaseAccount Account1{ 50.01 };

	SavingsAccount Account2{ 25.02, 5.00 };

	CheckingAccount Account3{ 80.03, 1.00 };

	std::cout << "Account 1 (Base) balance: $" << std::fixed << std::setprecision(2) << Account1.getBalance() << "\n";
	std::cout << "Account 2 (Savings) balance: $" << Account2.getBalance() << "\n";
	std::cout << "Account 3 (Checking) balance: $" << Account3.getBalance() << "\n\n";



	std::cout << "Bank Account Withdrawal Tests: \n";

	std::cout << "Now withdrawing $25.00 from account 1.\n";
	Account1.withdraw(25.00);

	std::cout << "Now withdrawing $30.00 from account 2.\n";
	Account2.withdraw(30.00);

	std::cout << "Now withdrawing $40.00 from account 3.\n";
	Account3.withdraw(40.00);

	std::cout << "\nAccount 1 balance: $" << Account1.getBalance() << "\n";
	std::cout << "Account 2 balance: $" << Account2.getBalance() << "\n";
	std::cout << "Account 3 balance: $" << Account3.getBalance() << "\n\n";



	std::cout << "Bank Account Deposit Tests: \n";

	std::cout << "Now depositing $40.23 to account 1.\n";
	Account1.deposit(40.23);

	std::cout << "Now depositing $65.99 to account 2.\n";
	Account2.deposit(65.99);

	std::cout << "Now depositing $20.01 to account 3.\n";
	Account3.deposit(20.01);

	std::cout << "\nAccount 1 balance: $" << Account1.getBalance() << "\n";
	std::cout << "Account 2 balance: $" << Account2.getBalance() << "\n";
	std::cout << "Account 3 balance: $" << Account3.getBalance() << "\n\n";



	std::cout << "Bank Account Interest Test: \n";

	std::cout << "Adding $" << Account2.computeInterest() << " interest to account 2.\n";
	Account2.deposit(Account2.computeInterest());

	std::cout << "The new account 2 balance is: $" << Account2.getBalance() << "\n\n";



	std::cout << "Bank Account Name Tests: \n";

	std::cout << "Account 1 name: " << Account1.getOwner() << "\n";
	std::cout << "Account 2 name: " << Account2.getOwner() << "\n";
	std::cout << "Account 3 name: " << Account3.getOwner() << "\n\n";

	std::cout << "Now naming account 1 to James Black.\n";
	Account1.setOwner("James Black");

	std::cout << "Now naming account 2 to Christie Johnson.\n";
	Account2.setOwner("Christie Johnson");

	std::cout << "Now naming account 3 to Kris Deen.\n\n";
	Account3.setOwner("Kris Deen");

	std::cout << "New account 1 name: " << Account1.getOwner() << "\n";
	std::cout << "New account 2 name: " << Account2.getOwner() << "\n";
	std::cout << "New account 3 name: " << Account3.getOwner() << "\n\n";


	return 0;
}
