#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "HealthRecord.h"

HealthRecord::HealthRecord(std::string personName, int personHeight, int personWeight) {
	name = personName;
	height = personHeight;
	weight = personWeight;
}
void HealthRecord::setName(std::string personName) {
	name = personName;
}
std::string HealthRecord::getName() {
	return name;
}
void HealthRecord::setHeight(int personHeight) {
	height = personHeight;
}
int HealthRecord::getHeight() {
	return height;
}
void HealthRecord::setWeight(int personWeight) {
	weight = personWeight;
}
int HealthRecord::getWeight() {
	return weight;
}
float HealthRecord::computeBMI() {
	return static_cast<float>(weight) / (2.20462f * ((static_cast<float>(height) * 0.0254f) * (static_cast<float>(height) * 0.0254f)));
}

void HealthRecord::outputCSV(std::ofstream& outputFile) {
	outputFile << std::fixed << std::setprecision(2);
	outputFile << getName() << ',';
	outputFile << getHeight() << ',';
	outputFile << getWeight() << ',';
	outputFile << computeBMI() << '\r' << '\n';
}

void HealthRecord::inputCSV(std::string theFileName) {
	std::ifstream inputFile{ theFileName, std::ios::in };
	char c;
	while (!inputFile.eof()) {
		inputFile.get(c);
		std::cout << c;
	}
	inputFile.close();
}