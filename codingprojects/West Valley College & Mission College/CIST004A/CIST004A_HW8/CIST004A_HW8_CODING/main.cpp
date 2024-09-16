#include <iostream>
#include <iomanip>
#include <ctype.h>
using namespace std;
int countCharacters(char * sPtr) {
  int count{0};
  for (;* sPtr != '\0'; ++sPtr, ++count) {}
  return count;
}
int countVowels(char * sPtr) {
  int vowelCount{0};
  for (;* sPtr != '\0'; ++sPtr)
    switch (toupper( * sPtr)) {
    case 'A':
    case 'E':
    case 'I':
    case 'O':
    case 'U':
      ++vowelCount;
    }
  return vowelCount;
}
void firstUpper(char * sPtr) {
  * sPtr = toupper( * sPtr);
  for (;* sPtr != '\0'; ++sPtr)
    if ( * sPtr == ' ') * (sPtr + 1) = toupper( * (sPtr + 1));
}
void shout(char * sPtr) {
  for (;* sPtr != '\0';* sPtr++ = toupper( * sPtr)) {}
}
void removeSpaces(char * sPtr) {
  char * dPtr = sPtr;
  for (;* sPtr != '\0';* dPtr++ = * sPtr++)
    while ( * sPtr == ' ' and * (sPtr + 1) == ' ') sPtr++;
  * dPtr = '\0';
}
int main() {
  cout << "CADEN ROBERTS\tCIST004A\t10/22/22\n";
  char String[80];
  int Operation {6};
  int i{0};
  do {
    if (i > 0) {
      cout << "\nSelect operation to perform on this string.\n1\tCount the characters in the string.\n2\tCount the vowels in the string.\n3\tUppercase the first character of each word in the string.\n4\tUppercase every character in the string.\n5\tRemove extra spaces in the string.\n6\tEnter a new string.\n0\tExit this program.\nEnter Operation: ";
      cin >> Operation;
      if (cin.fail() or cin.peek() != '\n') {
        Operation = 7;
        cin.clear();
        cin.ignore(1000, '\n');
      }
    }
    switch (Operation) {
    case 1:
      cout << "\nThere are " << countCharacters(String) << " characters in: \"" << String << "\"\n";
      break;
    case 2:
      cout << "\nThere are " << countVowels(String) << " vowels in: \"" << String << "\"\n";
      break;
    case 3:
      firstUpper(String);
      cout << "\nUpper case first letters: \"" << String << "\".\n";
      break;
    case 4:
      shout(String);
      cout << "\nShouting: \"" << String << "\".\n";
      break;
    case 5:
      removeSpaces(String);
      cout << "\nExtra spaces removed: \"" << String << "\".\n";
      break;
    case 6:
      if (i++ > 0) {
        cin.clear();
        cin.ignore(1000, '\n');
      }
      cout << "\nEnter a String: ";
      cin.getline(String, 80);
      break;
    case 0:
      break;
    default:
      cout << "\nInvalid Operation Request.\n";
    }
  } while (Operation != 0);
  return 0;
}