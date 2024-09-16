#pragma once
#ifndef HealthRecord_h
#define HealthRecord_h
#include <string>
#include <fstream>
class HealthRecord {
public:
	HealthRecord(std::string personName = "", int personHeight = 1, int personWeight = 0);
	~HealthRecord() {};
	void setName(std::string personName);
	std::string getName();
	void setHeight(int personHeight);
	int getHeight();
	void setWeight(int personWeight);
	int getWeight();
	float computeBMI();
	void outputCSV(std::ofstream& outFile);
	void inputCSV(std::string theFileName);
private:
	std::string name;
	int height;
	int weight;
};


#endif
