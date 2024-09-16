#include <iostream>
#include <iomanip>
#include <string>
#include "BaseAccount.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "OverdraftCheckingAccount.h"
#include <vector>
int main() {
	std::cout << "Caden Roberts\t\t11/17/22\t\tCIST004A\n";
	std::vector <BaseAccount*>  Vector(4);
	Vector[0] = new SavingsAccount(25.00, 3.00);
	Vector[1] = new CheckingAccount(80.00, 2.00);
	Vector[2] = new SavingsAccount(200.00, 1.50);
	Vector[3] = new OverdraftCheckingAccount(400.00, 1.50, 35.00, dynamic_cast<SavingsAccount*>(Vector[2]));
	double input = -1;
	for (int i = 0; i < 4; i++) {
		std::cout << "\nAccount " << i + 1 << " balance: $" << std::fixed << std::setprecision(2) << Vector[i]->getBalance() << "\n";
		int a = 0;
		do {
			if (a > 0) { std::cin.clear(); std::cin.ignore(1000, '\n'); std::cout << "Error. Please enter a positive number.\n"; }
			std::cout << "Enter an amount to withdraw from Account " << i + 1 << ": ";
			std::cin >> input;
		} while ((std::cin.peek() != '\n') || (input < 0) || (std::cin.fail()));
		Vector[i]->debit(input);
		a = 0;
		do {
			if (a > 0) { std::cin.clear(); std::cin.ignore(1000, '\n'); std::cout << "Error. Please enter a positive number.\n"; }
			std::cout << "Enter an amount to deposit to Account " << i + 1 << ": ";
			std::cin >> input;
		} while ((std::cin.peek() != '\n') || (input < 0) || (std::cin.fail()));
		Vector[i]->credit(input);
		if (dynamic_cast <SavingsAccount*> (Vector[i]) != nullptr) {
			std::cout << "Adding $" << dynamic_cast<SavingsAccount*>(Vector[i])->computeInterest() << " to Account " << i + 1 << " (Savings Account)\n";
			Vector[i]->credit(dynamic_cast<SavingsAccount*>(Vector[i])->computeInterest());
		}
		std::cout << "Updated Account " << i + 1 << " balance: $" << Vector[i]->getBalance() << "\n";
	}
	return 0;
}