// Caden Roberts CIST004A 9/30/22
#include <iostream>
#include <iomanip>
char GradeInput, GradeMod;
int CLASS = 1;
float GradePoints(0), CreditHours(0), GPA(0), TGP(0), TCH(0);
using namespace std;
int main()
{
	cout << "Caden Roberts CIST004A 9/30/22\n\nIntermediate GPA Calc Program\n\nTo calculate your GPA, enter each letter grade in either upper or lower case with an optional + or - followed by the [Return] key. (If you have no more grades, enter z followed by the [Return] key.) Next, enter the number of units for the course followed by the [Return] key.\n";
	do {
		cout << "\nPlease Enter Grade for Class " << CLASS << " (Z to exit): ";
		cin >> GradeInput;
		if (cin.peek() != '\n') cin.get(GradeMod);
		else GradeMod = ' ';
		if ((GradeInput == '-') or (GradeInput == '+')) {
			swap(GradeInput, GradeMod);
			cout << "Watch where you put the +/-!\n";
		}
		GradeInput = toupper(GradeInput);
		switch (GradeInput) {
		case 'A':
			GradePoints = 4.0f;
			break;
		case 'B':
			GradePoints = 3.0f;
			break;
		case 'C':
			GradePoints = 2.0f;
			break;
		case 'D':
			GradePoints = 1.0f;
			break;
		case 'F':
			GradePoints = 0.0f;
			break;
		case 'Z':
			GradePoints = 0.0f;
			continue;
		default:
			cout << "Please Enter a Valid Letter Grade (A,B,C,D,F)\n";
			continue;
		}
		switch (GradeMod) {
		case '+':
			if (GradeInput == 'F') cout << "We'll throw you a bone on the F+!\n";
			if (GradeInput != 'A') GradePoints = GradePoints + 0.3f;
			else cout << "An A+ will score no better than an A!\n";
			break;
		case '-':
			if (GradeInput != 'F') GradePoints = GradePoints - 0.3f;
			else cout << "Luckily, an F is the lowest you can go!\n";
			break;
		case ' ':   break;
		default:
			cout << "Please Enter Valid Grade Sign Modifier (+/-/ )\n";
			continue;
		}
		CreditHours = 0.0f;
		while ((CreditHours < 0.5f) or (CreditHours > 45.0f)) {
			cout << "\nPlease Enter Credit hours for Class " << CLASS << ": ";
			cin >> ws;
			cin >> CreditHours;
			if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); CreditHours = 0.0f; };
			if ((CreditHours < 0.5f) or (CreditHours > 45.0f)) cout << "Please enter a valid # of Credit hours for class " << CLASS << "(0.5-45)\n";
		}
		GradePoints = GradePoints * CreditHours;
		TGP = TGP + GradePoints;
		TCH = TCH + CreditHours;
		GPA = TGP / TCH;
		CLASS++;
	} while (GradeInput != 'Z');
	if (CLASS - 1 == 0) cout << "\nNo Valid Classes Entered.";
	else if (CLASS - 1 == 1) cout << "\nYour GPA for the 1 valid class entry is " << fixed << setprecision(3) << GPA << "\n";
	else cout << "\nYour GPA for the " << CLASS - 1 << " valid class entries is " <<
		fixed << setprecision(3) << GPA << "\n";
	return 0;
}