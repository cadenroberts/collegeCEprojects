#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <algorithm>
using namespace std;

unordered_map<string, vector<pair<string, string>>> graph;

string shortestPath(const string & a1, const string & a2) {
	if (graph.find(a1) == graph.end() || graph.find(a2) == graph.end()) {
		return "Not present";
	}
	if (a1 == a2) {
		return a1;
	}
	queue<vector<string>> q;
	set<string> v;
	q.push({a1});
	v.insert(a1);
	while(!q.empty()) {
		vector<string> p = q.front();
		q.pop();
		string last = p.back();
		for (const auto & n : graph[last]) {
			if (v.find(n.first) != v.end()) continue;
			vector<string> path = p;
			path.push_back(n.second);
			path.push_back(n.first);
			if (n.first == a2) {
				string r = path[0];
				for (size_t i = 1; i < path.size(); i+=2) {
					r += " -(" + path[i] + ")- " + path[i+1];
				}
				return r;
			}
			q.push(path);
			v.insert(n.first);
		}
	}
	return "Not present";
}

int main (int argc, char * argv[]) {
	if (argc!=3) {
		cout << "Incorrect Usage.\n";
		return 1;
	}
	ifstream movies("cleaned_movielist.txt");
	string line;
	while(getline(movies, line)) {
		stringstream s(line);
		string movie, actor;
		vector<string> actors;
		s >> movie;
		while (s>>actor) {
			actors.push_back(actor);
		}
		for (size_t i = 0; i < actors.size(); i++) {
			for (size_t k = i+1; k < actors.size(); k++) {
				graph[actors[i]].push_back({actors[k], movie});
				graph[actors[k]].push_back({actors[i], movie});
			}
		}
	}
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	string a1, a2;
	while (in >> a1 >> a2) {
		string r = shortestPath(a1, a2);
		out << r << endl;
	}
	in.close();
	out.close();
	return 0;
}