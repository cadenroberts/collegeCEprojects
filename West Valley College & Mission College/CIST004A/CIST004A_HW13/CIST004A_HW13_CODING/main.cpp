#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "HealthRecord.h"
std::ofstream myOutFile;
std::ifstream myInFile;
std::fstream myWRfile;
std::streampos begin, end;
bool getHealthInfo(HealthRecord* record) {
	bool x = (!(record->getName() == "") && !(record->getHeight() == 1) && !(record->getWeight() == 0));
	if (!x) std::cout << "Record will not be created.\n";
	return x;
};
std::streampos getFileLength(std::string theFileName) {
	std::ifstream file(theFileName, std::ios::binary | std::ios::in);
	begin = file.tellg();
	file.seekg(0, std::ios::end);
	end = file.tellg();
	file.close();
	return (end - begin);
}
int main() {
	std::cout << "Caden Roberts\t\t12/5/22\t\tCIST004A\n";
	std::vector <HealthRecord*> Vector;
	int i = 0, personWeight = 0, personHeight = 1;
	std::string personName, fileName;
	char C = 'Y';
	do {
		Vector.resize(++i);
		std::cout << "\nEnter Heath Record Data. Hit <return> on any blank line to end.\nHealth Record #" << i << "\nEnter the name(enter return when done): ";
		std::getline(std::cin, personName);
		if (personName != "") {
			std::cout << "Enter " << personName << "'s height in inches: ";
			std::cin >> personHeight;
			std::cin.ignore(1000, '\n');
			std::cout << "Enter " << personName << "'s weight in pounds: ";
			std::cin >> personWeight;
			std::cin.ignore(1000, '\n');
		}
		Vector[static_cast<std::vector<HealthRecord*, std::allocator<HealthRecord*>>::size_type>(i) - 1] = new HealthRecord(personName, personHeight, personWeight);
		if (!getHealthInfo(Vector[static_cast<std::vector<HealthRecord*, std::allocator<HealthRecord*>>::size_type>(i) - 1])) {
			Vector.pop_back(); break;
		}
	} while (1);

	do {
		std::cout << "\nEnter a new file name to create: ";
		std::getline(std::cin, fileName);
		if (std::fstream(fileName)) {
			C = 'z';
			std::cout << "File name " << fileName << " already exists.\n";
			do {
				std::cout << "Would you like to overwrite the file named \"" << fileName << "\"? (Y / N) :";
				std::cin >> C;
				std::cin.ignore(1000, '\n');
			} while (toupper(C) != 'Y' && toupper(C) != 'N');
		}
		if (toupper(C) == 'Y') {
			std::ofstream file(fileName);
			if (file) {
				std::cout << "The file \"" << fileName << "\" was successfully created.\n";
				for (int r = 0; r < i - 1; r++) {
					Vector[r]->outputCSV(file);
				}
				std::cout << "The file was written.\n";
				break;
			}
			else {
				std::cout << "The file \"" << fileName << "\" could not be created.\n";
			}
		}
	} while (1);

	std::cout << "The file \"" << fileName << "\" is " << getFileLength(fileName) << " bytes long.\n\n";

	std::cout << "Enter the final Health Record:\nEnter the name: ";
	std::getline(std::cin, personName);
	if (personName != "") {
		std::cout << "Enter " << personName << "'s height in inches: ";
		std::cin >> personHeight;
		std::cin.ignore(1000, '\n');
		std::cout << "Enter " << personName << "'s weight in pounds: ";
		std::cin >> personWeight;
		std::cin.ignore(1000, '\n');
	}
	std::ofstream file(fileName, std::ios::app);
	if (getHealthInfo(new HealthRecord(personName, personHeight, personWeight))) HealthRecord(personName, personHeight, personWeight).outputCSV(file);
	file.close();

	std::cout << "The file \"" << fileName << "\" is now " << getFileLength(fileName) << " bytes long.\n\nNow reading finalized file:\n";
	std::ifstream File(fileName);
	Vector[0]->inputCSV(fileName);

	return 0;
}