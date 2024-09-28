// Caden Roberts CIST004A 9/1/22
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

float Height1, Height2, Weight1, Weight2, BMI1, BMI2;
string Person1, Person2;

int main() {
    cout << "Please Enter The First Person's Name: \n";
      getline (cin, Person1);
    cout << "Please Enter " << Person1 << "'s height in inches: \n";
      cin >> Height1;
    cout << "Please Enter " << Person1 << "'s weight in pounds: \n";
      cin >> Weight1;
    cout << "Please Enter The Second Person's Name: \n";
      getline (cin >> ws, Person2);
    cout << "Please Enter " << Person2 << "'s height in inches: \n";
      cin >> Height2;
    cout << "Please Enter " << Person2 << "'s weight in pounds: \n";
      cin >> Weight2;
    BMI1 = ((Weight1/2.20462)/((Height1*2.540/100)*(Height1*2.540/100)));
    BMI2 = ((Weight2/2.20462)/((Height2*2.540/100)*(Height2*2.540/100)));
    cout << Person1 << "'s BMI is " << BMI1 << ".\n";
    cout << Person2 << "'s BMI is " << BMI2 << ".\n";
    if(BMI1 < BMI2) {cout << Person1 << " is 'healthier' than " << Person2 <<   ".\n";}
      else if(BMI2 < BMI1) {cout << Person2 << " is 'healthier' than " << Person1 << ".\n";}
      else {cout << Person1 << " is equally as 'healthy' as " << Person2 << ".\n";}
  return 0;
}