// Caden Roberts CIST004A 9/13/22
#include <iostream>
#include <iomanip>
using namespace std;
float GPA{0}, Low{4.0f}, High{0}, Average{0};
int n{1};
// enter -1 to indicate you are done adding student GPAs
int main() {
	cout << "Caden Roberts CIST004A 9/13/22\n";
	while (GPA != -1.0f) {
		cout << "\nEnter a GPA (0.0-4.0) for Student #" << n << ": ";
		cin >> GPA;
		while (GPA > 4.0f || GPA < 0.0f && GPA != -1.0f) {
			cout << "\nEnter a valid GPA (0.0-4.0) for Student #" << n << ": ";
			cin >> GPA;
		}
		if (GPA == -1.0f) continue;
		Low = Low < GPA ? Low : GPA;
		High = High > GPA ? High : GPA;
		Average = Average + GPA;
		n++;
	}
	Average = Average / (n - 1);
	cout << fixed << setprecision(3);
	cout << "\n\nThe lowest GPA entered was: " << Low;
	cout << "\n\nThe highest GPA entered was: " << High;
	cout << "\n\nThe average GPA was: " << Average << "\n";
	return 0;
}