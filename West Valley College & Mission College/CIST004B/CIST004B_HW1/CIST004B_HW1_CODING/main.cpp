#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// create functions
void MinMaxMeanMedianMode(std::vector<int> list) {
	// sort list to calculate min and max
	std::sort(list.begin(), list.end());
	std::cout << "\nThe list of numbers has the following characteristics:\nThe Minimum is : " << list[0] << "\nThe Maximum is: " << list[list.size() - 1];

	// count up all values in list and return average
	std::cout << "\nThe Mean is: " << static_cast<float>(std::accumulate(list.rbegin(), list.rend(), 0)) / static_cast<float>(list.size());

	// determine whether odd/even numbers in list then calculate median accordingly
	std::cout << "\nThe Median is: ";
	if (list.size() % 2 == 0) std::cout << (list[list.size() / 2] + list[list.size() / 2 - 1]) / 2;
	else std::cout << list[(list.size() - 1) / 2];

	// find the spread of number occurances and the max
	std::vector<int> numfreq(list[list.size()-1]+1, 0);
	int modefreq{0};
	for (auto a : list) {
		numfreq[a] += 1;
		modefreq = modefreq > numfreq[a] ? modefreq : numfreq[a];
	}
	// if a numfreq is equal to the modefreq, store that num in modes
	std::vector<int> modes;
	for (int i = 0; i < numfreq.size(); i++) {
		if (numfreq[i] == modefreq) modes.insert(modes.end(), i);
	}
	// read mode(s) off accordingly
	if (modes.size() == 1) std::cout << "\nThe Mode is: " << modes[0];
	else {
		std::cout << "\nThe list is multimodal, and contains " << modes.size() << " modes. The modes are: ";
		for (auto a : modes) {
			if (a == modes[modes.size() - 2]) std::cout << a << " and ";
			else if (a == modes[modes.size() - 1]) std::cout << a << ".";
			else std::cout << a << ", ";
		}
	}
}

std::vector<int> InquireUserList() {
	int number;
	std::vector<int> list;
	// collect number of numbers user wants to input
	do {
		std::cout << "How many numbers are in your list (1 to 100)?: ";
		std::cin >> number;
		if (number < 1) std::cout << "Hey come on!  The list must have at least 1 number!\n";
		else if (number > 100) std::cout << "Really!  You are prepared to enter 101 numbers?  I don't think so!\n";
	} while (number < 1 || number>100);
	list.resize(number);

	// collect numbers in list from user
	std::cout << "\nYour list may have numbers between 1 and 1000.\n\n";
	for (int i = 1; i <= list.size(); i++) {
		do {
			if (i%100 >= 11 && i%100 <= 19) std::cout << "Enter the " << i << "th number: ";
			else switch (i % 10) {
			case 1:
				std::cout << "Enter the " << i << "st number: ";
				break;
			case 2:
				std::cout << "Enter the " << i << "nd number: ";
				break;
			case 3:
				std::cout << "Enter the " << i << "rd number: ";
				break;
			default: std::cout << "Enter the " << i << "th number: ";
			}
			std::cin >> number;
			if (number < 1 || number > 1000) std::cout << "Please enter a valid number between 1 and 1000!\n";
		} while (number < 1 || number > 1000);
		list[i - 1] = number;
	}
	return list;
}

int main() {
	std::cout << "Caden W Roberts\t\t\t\tCIST004B  1/31/23\nMinimum, Maximum, Mean, Median and Mode Calculator with extra polish and multimodal support.\n";

	//invoke user input function, passing returned list to defined MMMMM function 
	MinMaxMeanMedianMode(InquireUserList());
	return 0;
}