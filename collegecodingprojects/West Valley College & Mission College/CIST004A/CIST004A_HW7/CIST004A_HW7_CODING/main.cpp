// CADEN ROBERTS CIST004A 10/13/22
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;
int main() {
	int numberOfDice{ 0 };
	int numberOfFaces{ 0 };
	int numberOfRolls{ 0 };
	int max{ 0 };
	vector <int> diceCounter;
	cout << "CADEN ROBERTS\tCIST004A\t10/13/22\n\n";
	do {
		cout << "Enter the number of Dice (at least 1): ";
		cin >> numberOfDice;
		if (cin.peek() != '\n' or cin.fail()) {
			numberOfDice = 0; cin.clear(); cin.ignore(1000, '\n');
		}
	} while (numberOfDice < 1);
	do {
		cout << "Enter the number of faces on each dice (at least 2): ";
		cin >> numberOfFaces;
		if (cin.peek() != '\n' or cin.fail()) {
			numberOfFaces = 0; cin.clear(); cin.ignore(1000, '\n');
		}
	} while (numberOfFaces < 2);
	do {
		cout << "Enter the number of Rolls (at least 1): ";
		cin >> numberOfRolls;
		if (cin.peek() != '\n' or cin.fail()) {
			numberOfRolls = 0; cin.clear(); cin.ignore(1000, '\n');
		}
	} while (numberOfRolls < 1);
	diceCounter.resize(numberOfDice * numberOfFaces + 1, 0);
	for (int i = 0; i < numberOfRolls; i++) {
		int rollTotal = 0;
		for (int i = 0; i < numberOfDice; i++) {
			rollTotal += 1 + rand() % numberOfFaces;
		}
		diceCounter.at(rollTotal)++;
	}
  for (int i : diceCounter) max = max > i ? max : i;
	for (int i = numberOfDice; i < diceCounter.size(); ++i) {
    cout << "Rolled " + to_string(i) << setw(12-to_string(i).length());
    int stars = (diceCounter.at(i) * 40 / max);
		for (int i = 0; i < stars; i++) cout << '*';
		cout << '\n';
	}
	return 0;
}


  
