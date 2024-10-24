// Filename: linkedlist.h
// 
// Header file for the class LinkedList that represents a linked list
// 
// Japheth Frolick, August 2019
// C. Seshadhri, Jan 2020

#ifndef LIST_H
#define LIST_H

#include <string>

using namespace std;

// node struct to hold data
struct Node
{
	int data;
	Node *next;
};

class LinkedList
{
	private:
		Node *head; // Stores head of linked list
	public:
		LinkedList(); // Default constructor sets head to null
        void insert(int); // insert int into list 
        Node* find(int); // find int in list, and return pointer to node with that int. If there are multiple copies, this only finds one copy
        Node* deleteNode(int); // remove a node with int (if it exists), and return pointer to deleted node. This does not delete all nodes with the value.
        void deleteAndFreeNode(int); // remove a node with int (if it exists), and free the memory.
		void deleteList(); // deletes every node to prevent memory leaks, and frees memory
		Node* deleteKthOccurance(int, int); // delete the kth occurance of a value
        void deleteAndFreeKthOccurance(int, int); // delete and free the kth occurance of a value
		void deleteAndFreeLastOccurance(int); // delete and free the last occurance of a value
		void deleteAndFreeAllButFirstOccurance(int); // delete and free all but the first occurance of a value
		bool isListPalindrome(); // find whether a list is a palindrome
		void rotate(int);
		void reverseRotate(int);
		void reverse(int);
		void reverseLast(int);
		void reverseBlock(int);
		void reversePalindrome();
		string print(); // Construct string with data of list in order 
		int length(); // Returns the length of the linked list
};

#endif
