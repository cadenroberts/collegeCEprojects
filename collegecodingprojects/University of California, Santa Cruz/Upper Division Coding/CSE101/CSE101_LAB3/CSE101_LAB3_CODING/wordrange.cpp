#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

struct Node {
	Node * left;
	Node * right;
	Node * parent;
	string key;
	int height;
};

void update(Node * root) {
	if (root==NULL) return;
	root->height = root->right ? root->right->height+1 : (root->left ? root->left->height+1 : 1);
}

Node * ll (Node * root) {
	Node * temp = root->left;
	root->left = temp->right;
	if (temp->right != NULL) temp->right->parent = root;
	temp->right = root;
	temp->parent = root->parent;
	root->parent = temp;
	if (temp->parent != NULL && root->key < temp->parent->key) {
		temp->parent->left = temp;
	} else if (temp->parent!=NULL) {
		temp->parent->right = temp;
	}
	root = temp;
	update(root->left);
	update(root->right);
	update(root);
	update(root->parent);
	return root;
}

Node * rr (Node * root) {
	Node * temp = root->right;
	root->right = temp->left;
	if (temp->left != NULL) temp->left->parent = root;
	temp->left = root;
	temp->parent = root->parent;
	root->parent = temp;
	if (temp->parent != NULL&& root-> key < temp->parent->key) {
		temp->parent->left = temp;
	} else if (temp->parent != NULL) {
		temp->parent->right = temp;
	}
	root = temp;
	update(root->left);
	update(root->right);
	update(root);
	update(root->parent);
	return root;
}

Node * lr (Node * root) {
	root->left = rr(root->left);
	return ll(root);
}

Node * rl (Node * root) {
	root->right = ll(root->right);
	return rr(root);
}
Node * insert (Node * root, Node * parent, string key) {
	if (root==NULL) {
		root = new Node();
		root->height = 1;
		root->left = NULL;
		root->right = NULL;
		root->parent = parent;
		root->key = key;
	} else if (root->key > key) {
		root->left = insert(root->left, root, key);
		int h1 = (root->left!=NULL) ? root->left->height : 0;
		int h2 = (root->right!=NULL) ? root->right->height : 0;
		if (abs(h1-h2)==2) {
			if (root->left != NULL && key < root->left->key) {
				root = ll(root);
			} else {
				root = lr(root);
			}
		}
	} else if (root->key < key) {
		root->right = insert(root->right, root, key);
		int h1 = (root->left!=NULL) ? root->left->height : 0;
		int h2 = (root->right!=NULL) ? root->right->height : 0;
		if (abs(h1-h2)==2) {
			if (root->right != NULL && key < root->right->key) {
				root = rl(root);
			} else {
        root = rr(root);
			}
		}
	}
	update(root);
	return root;
}

int range (Node * root, string s1, string s2) {
	if (!root) return 0;
	int count = 0;
	Node * curr = root;
	while (curr) {
		if (curr->key < s1) curr = curr->right;
		else if (curr->key > s2) curr = curr->left;
		else if (curr->left == NULL) {
			if (curr->key <= s2 && curr->key >= s1) count++;
			curr = curr->right;
		} else {
			Node * prev = curr->left;
			while (prev->right != NULL && prev->right != curr) prev = prev->right;
			if (prev->right == NULL) {
				prev->right = curr;
				curr = curr->left;
			} else {
				prev->right = NULL;
				if (curr->key <= s2 && curr->key >= s1) count++;
				curr = curr->right;
			}
		}
	}
	return count;
}

int main (int argc, char * argv[]) {
	if (argc != 3) {
		cout << "Incorrect usage. ./wordrange <input file> <ouputfile>" << endl;
	}
	ifstream in;
	ofstream out;
	in.open(argv[1]);
	out.open(argv[2]);
	string c;
	char * command, * str, * str2, * operation;
	Node * root = NULL;
	while (getline(in, c)) {
		if (c.length()==0) continue;
		command = strdup(c.c_str());
		operation = strtok(command, " \t");
		str = strtok(NULL, " \t");
		if (strcmp(command, "i") == 0) {
			root = insert(root, NULL, str);
		}
		if (strcmp(command, "r") == 0) {
			str2 = strtok(NULL, " \t");
			out << range(root, str, str2) << endl;
		}
		if (operation != NULL) delete(operation);
	}
	out.close();
	in.close();
	return 0;
}