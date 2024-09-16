// Filename: linkedlist.cpp
// 
// Contains the class LinkedList that represents a linked list. This contains some basic operations, such as insert, delete, find, length
// 
// Japheth Frolick, August 2019
// C. Seshadhri, Jan 2020

#include "linkedlist.h"
#include <cstdlib>
#include <iostream>
#include <vector> 
#include <algorithm>
#include <string>
using namespace std;

// Default constructor sets head and tail to null
LinkedList :: LinkedList()
{
	head = NULL;
}

// Insert(int val): Inserts the int val into list, at the head of the list. Note that there may be multiple copies of val in the list.
// Input: Int to insert into the linked list
// Output: Void, just inserts new Node
void LinkedList :: insert(int val)
{
	Node *to_add = new Node; // creates new Node
	to_add->data = val; // sets the data to hold input val
	
    to_add->next = head; // make to_add point to existing head
    head = to_add; // set head to to_add
}

// find(int val): Finds a Node with data "val"
// Input: int to be found
// Output: a pointer to a Node containing val, if it exists. Otherwise, it returns NULL
// Technically, it finds the first Node in the list containing val
Node* LinkedList :: find(int val)
{
    Node *curr = head; // curr is the current Node as it progresses through the linked list. Initialized to head to start at the head of the list
    // curr will look over list. At every iteration, we will check if curr->data is val. If so, we are done. Otherwise, we proceed through the list.
    while(curr != NULL) //looping over list
    {
        if (curr->data == val) // found val, so return curr
            return curr;
        curr = curr->next; // otherwise, proceed through list
    }
    // if loop terminates, val not found
    return NULL;
}

// deleteNode(int val): Delete a Node with data val, if it exists. Otherwise, do nothing.
// Input: int to be removed
// Output: pointer to Node that was deleted. If no Node is deleted, return NULL. If there are multiple Nodes with val, only the first Node in the list is deleted. Note that this removes the node from the list
//         but does not free the corresponding memory
Node* LinkedList :: deleteNode(int val)
{
    Node* prev = NULL;
    Node* curr = head;
    while(curr != NULL) // loop over list, starting from head
    {
        if(curr->data == val) // we found Node with val, so break
            break; 
        //otherwise, proceed list
        prev = curr; // move prev by one Node
        curr = curr->next; // move curr by one Node
    }
    // at this point, curr points to Node with val. If curr is null, then val is not in the list.
    if (curr == NULL) // val not found
        return NULL; 
    // val is in list. Note that curr is not NULL. There is a case analysis now. If prev is null, then curr is head. So we delete head directly.
    // Otherwise, we delete the Node after prev.
    if (prev == NULL) // we need to delete head
        head = head->next; // delete head
    else // we delete Node after prev. Note that both curr and prev are not NULL
        prev->next = curr->next; // make prev point to Node after curr. This removes curr from list
    curr->next = NULL; // set next pointer of curr to NULL, to ensure no dangling references
    return curr;
}


// deleteAndFreeNode(int val): Delete a Node with data val and free the memory, if it exists. Otherwise, do nothing.
// Input: int to be removed
// Output: void. If there are multiple Nodes with val, only the first Node in the list is deleted. Note that this frees the memory of that deleted node
void LinkedList :: deleteAndFreeNode(int val)
{
    Node* ptr = deleteNode(val); // get pointer to node with val
    if (ptr != NULL) // if node actually exists
    {
        ptr->next = NULL; // just be extra safe, probably don't need this
        delete(ptr); // free that memory
    }
    return; //DONE!
}

// Deletes every Node to prevent memory leaks.
// Input: None
// Output: Void, just deletes every Node of the list
void LinkedList :: deleteList()
{
	Node *curr = head; // curr is the current Node as it progresses through the linked list. Initialized to head to start at the head of the list
    Node *temp = NULL;

	// curr will loop over list. At every iteration, we first store a pointer to the next Node, and then delete the current Node.
    while(curr != NULL){ // Looping over list
		temp = curr->next; // store pointer to next Node in the list
		delete(curr); // delete the current Node
		curr = temp; // update curr to proceed through list
	}
	head = NULL;
}

// Prints list in order
// Input: None
// Output: string that has all elements of the list in order
string LinkedList :: print()
{
    string list_str = ""; // string that has list
	Node *curr = head; // curr is the current Node as it progresses through the linked list. Initialized to head to start at the head of the list
	// curr will loop over list. It prints the content of curr, and then moves curr to next Node.
	while(curr != NULL){ // looping over list
        list_str = list_str + to_string(curr->data) + " "; //append string with current Node's data
		curr = curr->next; // proceed through list
	}
    if (list_str.length() > 0) // string is non-empty
        list_str.pop_back(); // remove the last (extra) space from string
    return list_str;
}

// Computes the length of the linked list
// Input: None
// Output: Int, length of list
int LinkedList :: length()
{
    int length = 0; // initialize length to zero
	Node *curr = head; // curr is ths current Node as it progresses through the linked list. Initialized to head to start at the head of the list
	
	while(curr != NULL){ // looping over list
		length++; // increment length
		curr = curr->next; // proceed to next Node
	}
	return length;
}

Node* LinkedList :: deleteKthOccurance(int value, int k) {
	if (k<1) return NULL; // 0th and negative occurances aren't real
    Node* prev = NULL; // prev pointer
    Node* curr = head; // current pointer
	int a = 1; // to track # of occurances
    while(curr != NULL) // loop over list, starting from head
    {
        if(curr->data == value && a++ == k) // we found kth Node with val, so break
            break; 
        //otherwise, proceed list
        prev = curr; // move prev by one Node
        curr = curr->next; // move curr by one Node
    }
    // at this point, curr points to kth Node with val. If curr is null, then kth val is not in the list.
    if (curr == NULL) // val not found
        return NULL; 
    // kth val is in list. Note that curr is not NULL. There is a case analysis now. If prev is null, then curr is head. So we delete head directly.
    // Otherwise, we delete the Node after prev.
    if (prev == NULL) // we need to delete head
        head = head->next; // delete head
    else // we delete Node after prev. Note that both curr and prev are not NULL
        prev->next = curr->next; // make prev point to Node after curr. This removes curr from list
    curr->next = NULL; // set next pointer of curr to NULL, to ensure no dangling references
    return curr; // return the removed node
}

void LinkedList :: deleteAndFreeKthOccurance(int value, int k){
    Node* ptr = deleteKthOccurance(value, k); // get pointer to kth node with val
    if (ptr != NULL) // if node actually exists
    {
        ptr->next = NULL; // just be extra safe, probably don't need this
        delete(ptr); // free that memory
    }
    return; //DONE!
}

void LinkedList :: deleteAndFreeLastOccurance(int value) {
    Node* prev = NULL; // prev pointer
    Node* curr = head; // current pointer
	Node* last = NULL; // pointer to hold final found occurance
	Node* lastprev = NULL; // pointer to hold previous to final found occurance
    while(curr != NULL) // loop over list, starting from head
    {
        if(curr->data == value){ // set possible last
            last = curr; // set last
			lastprev = prev; // set lastprev
		}
        //proceed list
        prev = curr; // move prev by one Node
        curr = curr->next; // move curr by one Node
    }
    // at this point, last points to last Node with val. If last is null, then val is not in the list.
    if (last == NULL) // val not found
        return; 
    // val is in list. Note that last is not NULL. There is a case analysis now. If lastprev is null, then last is head. So we delete head directly.
    // Otherwise, we delete the Node after lastprev.
    if (lastprev == NULL) // we need to delete head
        head = head->next; // delete head
    else // we delete Node after lastprev. Note that both last and lastprev are not NULL
        lastprev->next = last->next; // make lastprev point to Node after last. This removes last from list
    last->next = NULL; // set next pointer of last to NULL, to ensure no dangling references
    delete(last); // free that memory
    return;
}

void LinkedList :: deleteAndFreeAllButFirstOccurance(int value) {
    Node* matches = NULL; // pointer for matches to delete
	int k = 2; // k value to pass to deleteKthOccurance, excludes 1
	do {
	  if (matches!=NULL) delete(matches); // free memory
      matches = deleteKthOccurance(value, k++); // find match for k value, post increment k
	} while (matches != NULL); // loop until no match
    return;
}

bool LinkedList :: isListPalindrome() {
    int length = 0; // initialize length to zero
	Node *curr = head; // curr is ths current Node as it progresses through the linked list. Initialized to head to start at the head of the list
	while(curr != NULL){ // looping over list
		length++; // increment length
		curr = curr->next; // proceed to next Node
	}
	curr = head; // set curr back to head
	Node* currmatch = head; // pointer to match with curr
	int end = length % 2; // find if length is even or odd
	while (1) { // loop until return
		if (length==end) return true; // if we reach length 0 or 1 there is a palindrome
		for (int i = 1; i < length; i++) currmatch = currmatch->next; // set currmatch to proper node to check against
		if ((currmatch->data) != (curr->data)) return false; // if there is not a match return false
		curr = curr->next; // move curr
		currmatch = curr; // set currmatch to curr
		length -= 2; // decrement length for 2 nodes that matched
	}
}

void LinkedList :: rotate(int x) {
	if (x<1) return; // no rotation
    int length = 0; // initialize length to zero
	Node *curr = head; // curr is the current Node as it progresses through the linked list. Initialized to head to start at the head of the list
	while(curr != NULL) { // looping over list
		length++; // increment length
		curr = curr->next; // proceed to next Node
	}
	if (length<2) return; // no need to rotate if less than 2 nodes
	x = x % length; // modulus rotation by length
	if (x<1) return; // no rotation
    curr = head; // reset curr
	Node * prev = NULL; // pointer for prev node
	int i = 0; // set i
	while (i++ < length-x) { // until curr is the first appearing node needing to be rotated
		prev = curr; // store prev
		curr = curr->next; // advance curr
	}
	Node * end = curr; // set end to curr
	i = 1; // set i
	while (i++ < x) { //
		end=end->next; // advance end
	}
	prev->next = NULL; // set new end of list next to null
	end->next = head; // attach the tail of the list to the head
	head = curr; // set head to the current beginning of rotated nodes
	return;
}

void LinkedList :: reverseRotate(int x) {
    int length = 0; // initialize length to zero
	Node *curr = head; // curr is ths current Node as it progresses through the linked list. Initialized to head to start at the head of the list
	Node *tail = head; // use to store tail of list
	while (curr != NULL) { // looping over list
		length++; // increment length
		tail = curr; // set tail
		curr = curr->next; // proceed to next Node
	}
	if (length<2) return; // no need to rotate anything
	x = x % length; // modulus rotation by length
	if (x<1) return; // no need to rotate less than 1
    curr = head; // reset curr to head
	Node * prev = NULL; // pointer for prev
	int i = 0; // set i
    while(i++ < x){ // loop x times
		prev = curr; // prev is curr
		curr = curr->next; // advance curr
	}
	prev->next = NULL; // set prev (new tail) to NULL
	tail->next = head; // set tail of list to beginning of rotated items
	head = curr; // set head to the first unrotated node
}

void LinkedList :: reverse(int k) {
	if (head==NULL) return;
	Node * curr = head;
	Node * prev = head;
	int length = 0;
	while(curr != NULL){ // looping over list
		length++; // increment length
		curr = curr->next; // proceed to next Node
	}
	if (length<2) return; // 
	if (k>0 && k%length==0){
		k = length;
	} else {
		k = k % length;
		if (k<1) return;
	}
	curr = head;
	for (int i = 1; i<k; i++) {
		for (int k = 0; k<i; k++){
			prev = curr;
			curr = curr->next;
		}
		prev->next = curr->next;
		curr->next = head;
		head = curr;
	}
}

void LinkedList :: reverseLast(int k) {
	if (head==NULL) return;
	Node * curr = head;
	Node * prev = NULL;
	int length = 0;
	while(curr != NULL){ // looping over list
		length++; // increment length
		curr = curr->next; // proceed to next Node
	}
	if (length<2) return;
	if (k>0 && k%length==0){
		Node * prev = NULL;
		curr = head;
		Node * next = NULL;
		while (curr != NULL) {
			next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		head = prev;
		return;
	} else {
		k = k % length;
		if (k<1) return;
	}
	Node * start = head;
	for (int i=1; i<length-k; i++) {
		start = start->next;
	}
	curr = start->next;
    Node * tail = curr;
	Node * next = NULL;
	for (int i = 0; i<k; i++) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	start->next = prev;
	tail->next = curr;
}

void LinkedList :: reverseBlock(int k) {
	if (head==NULL) return;
	Node * curr = head;
	int length = 0;
	while (curr != NULL) { // looping over list
		length++; // increment length
		curr = curr->next; // proceed to next Node
	}
	if (length<2) return;
	if (k>0 && k%length==0){
		k = length;
	} else {
		k = k % length;
		if (k<1) return;
	}
	Node * prev = NULL, * start = NULL, * end = NULL, * next = NULL;
	curr = head;
    while (length > 0) {
		int block = (length >= k) ? k : length;
		start = curr;
		prev = NULL;
		for (int i = 0; i<block; i++) {
			next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		if (end != NULL) {
			end->next = prev;
		} else {
			head = prev;
		}
		end = start;
		length -= block;
	}
	end->next = NULL;
}

void LinkedList :: reversePalindrome() {
	if (head==NULL) return;
	Node * curr = head;
	int length = 0;
	while(curr != NULL){ // looping over list
		length++; // increment length
		curr = curr->next; // proceed to next Node
	}
	if (length<2) return; // change to 1 if a single item should double up
	/*
	else if (length<2) {
		Node * temp = new Node;
		temp->data = head->data;
		temp->next = NULL;
		head->next = temp;
		return;
	}       in case a single list item should double up
	*/
	Node * temp = new Node;
	temp->data = head->data;
	curr = head->next;
	Node * prev = NULL;
	while (curr!=NULL) {
		Node *to_add = new Node; // creates new Node
	    to_add->data = curr->data; // sets the data to hold input val
		to_add->next = temp;
		temp = to_add;
		prev = curr;
		curr = curr->next;
	}
	prev->next = temp;
}
