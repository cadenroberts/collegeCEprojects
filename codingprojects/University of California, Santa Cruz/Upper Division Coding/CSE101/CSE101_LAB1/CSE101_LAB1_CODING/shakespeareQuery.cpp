#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
  string word;
	int freq;
	Node * next;
};

class LinkedList {
public:
   LinkedList() : head(nullptr) {}
	 
	 void add(const string& word) {
	   Node * current = head;
	   Node * prev = nullptr;
		 while ((current != nullptr) && (current->word != word)) {
       prev = current;
		   current = current->next;
		 }
		 if (current != nullptr) {
       current->freq++;
		 } else {
			 Node * n = new Node();
			 n->word = word;
			 n->freq = 1;
			 n->next = nullptr;
			 if (prev == nullptr) {
				 head = n;
			 } else {
				 prev->next = n;
			 }
		 }
	 }
   
	 string getRank (int r) {
		 Node * current = head;
		 int cR = 0;
		 while (current != nullptr) {
       if (cR == r) {
         return current->word;
		   }
			 current = current->next;
			 cR++;
		 }
		 return "-";
	 }

	 void sortWords() {
		 if (!head) return;
		 Node * current = head;
		 while (current != nullptr) {
			 Node * n = current->next;
			 while (n != nullptr) {
				 if (current->freq < n->freq || (current->freq==n->freq && current->word > n->word)) {
					 swap(current->word, n->word);
					 swap(current->freq, n->freq);
				 }
				 n = n->next;
			 }
			 current = current->next;
		 }
	 }
private:
   Node * head;
};

int main(int argc, char* argv[]) {
	 string inFile = argv[1];
	 string outFile = argv[2];
	 LinkedList wordsList[37];
   ifstream file("shakespeare-cleaned5.txt");
   string word;
   while (file >> word) {
		 int l = word.length();
	   wordsList[l].add(word);
   }
	 file.close();
	 for (int i = 0; i<37; i++) {
		 wordsList[i].sortWords();
	 }

	 ifstream iFile(inFile);
	 ofstream oFile(outFile);
	 int l, r;
	 while (iFile >> l >> r) {
		 if (l < 37) {
		   oFile << wordsList[l].getRank(r) << '\n';
		 } else {
			 oFile << "-\n";
		 }
	 }
	 iFile.close();
	 oFile.close();
   return 0;
}