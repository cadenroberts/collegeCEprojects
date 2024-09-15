#include<iostream>
#include<stack>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>

using namespace std;

struct Position {
  int row;
	int column;
};

int peek (int i, stack<int> s) {
	for (int b = 1; b < i; b++) {
		s.pop();
	}
	return s.top();
}

bool notokay (stack<int>& choices) {
	unsigned int i;
	unsigned int queen = choices.top();
	unsigned int other;
	for (i = 2; i <= choices.size(); i++) {
		other = peek(i, choices);
		if ((queen==other)||(queen+(i-1)==other)||(queen-(i-1)==other)) {
      return true;
		}
	}
	return false;
}

int rowmatch(Position preset[], int row, int size) {
	for (int i = 0; i < size; i++) {
		if (preset[i].row==row) return i;
	}
	return -1;
}

string queens(unsigned int n, Position preset[] ){
  if (n < 1) return "No solution";
  stack<int> choices;
	int row = n;
	int size = 0;
	while (preset[size].row!=-1) {
		size++;
	}
	bool solution = false;
	int a = rowmatch(preset, row, size);
	if (a!=-1) {
		choices.push(preset[a].column);
	} else choices.push(1);
	while (!solution && !choices.empty()) {
		if (notokay(choices)) {
			while (!choices.empty() && (rowmatch(preset, row, size)!=-1 || (unsigned int) choices.top() == n)) {
			  row++;
				choices.pop();
			}
			if (!choices.empty()) {
				int top = choices.top();
				choices.pop();
				top++;
				choices.push(top);
			}
		} else if (choices.size()==n) {
			solution = true;
		} else {
			row--;
			int i = rowmatch(preset, row, size);
			if (i!=-1) {
				choices.push(preset[i].column);
			} else choices.push(1);
		}
	}
	if (solution) {
		unsigned int max = choices.size();
		string result = "";
		for (unsigned int i = 1; i <= max; i++) {
			result += (to_string(i)+" "+to_string(choices.top())+" ");
			choices.pop();
		}
		return result;
	} else return "No solution";
}

int main(int argc, char * argv[]){
	ifstream input(argv[1]);
	ofstream output(argv[2]);
	string line;
  while (getline(input, line)) {
    vector<Position> preset;
		stringstream s(line);
		int n, row, column;
		s >> n;
		while (s >> row >> column) {
			preset.push_back({row, column});
		}
		preset.push_back({-1, -1});
		Position presets[preset.size()];
		copy(preset.begin(), preset.end(), presets);
		output << queens(n, presets) << "\n";
	}
  input.close();
	output.close();
	return 0;
}
