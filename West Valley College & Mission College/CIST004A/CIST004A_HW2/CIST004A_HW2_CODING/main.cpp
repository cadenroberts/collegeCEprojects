// Caden Roberts CIST004A 9/1/22
#include <iostream>
#include <iomanip>
#include <string>
float Height, Weight, BMI;
using namespace std;

int main() {
  cout << "Caden Roberts CIST004A\n";
  cout << "Please Enter Height in Inches: ";
  cin >> Height;
  Height = Height*2.540f/100.0f;
  cout << "Please Enter Weight in Pounds: ";
  cin >> Weight;
  Weight = Weight / 2.20462f;
  BMI = (Weight/(Height*Height));
  cout << "Your height is " << Height << " meters.\n";
  if(Height <= 0.3f) {cout << "If you aren't 2 years old, you may have a world record!\n";};
  if(Height >= 2.1336f) {cout << "You should call the Lakers Immediately.\n";};
  cout << "Your weight is " << Weight << " kilograms.\n";
  if(Weight <= 23.0f) {cout << "Pick up the cheetos!!\n";}
  if(Weight >= 182.0f) {cout << "Put the cheetos down!!\n";}
  cout << "Your BMI is " << BMI << ".\n";
  if(BMI < 18.0f) {
      cout << "You are underweight.\n";
  } else if(BMI < 25.0f) {
      cout << "You are a healthy weight.\n";
  } else if(BMI < 30.0f) {
      cout << "You are overweight.\n";
  } else if(BMI < 40.0f) {
      cout << "You are obese.\n";
  } else {
      cout << "You are extremely obese.\n";
  }
  
  double test1 = 1.0/3.0;
  float test2 = 1.0f/3.0f;
  long double test3 = 1.0L/3.0L;

  cout << setprecision(25);
  cout << "double result: " << test1 << "\n";
  cout << "float result: " << test2 << "\n";
  cout << "long double result: " << test3 << "\n";
  
  return 0;
}