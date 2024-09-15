// FILE: pqueue1.hpp
//
//  Updated by Howard Miller on 3/18/23.
//  Copyright © 2019, 2020, 2021, 2023 Howard Miller. All rights reserved.
//
//  This assignment is worth 10 points on Canvas.
// CLASS PROVIDED: PriorityQueue (a priority queue of items)
//
// TYPEDEF for the PriorityQueue class:
//   typedef _____ Item
//     The type Item is the data type of the items in the Priority Queue.
//     It may be any of the C++ built-in types (int, char, etc.), or a class
//     with a default constructor, a copy constructor, and assignment operator.
//
// CONSTRUCTOR for the PriorityQueue class:
//   PriorityQueue( )
//     Postcondition: The PriorityQueue has been initialized with no Items.
//
// MODIFICATION MEMBER FUNCTIONS for the PriorityQueue class:
//   void insert(const Item& entry, unsigned int priority)
//     Postcondition: A new copy of entry has been inserted with the specified
//     priority.
//
//   Item get_front( )
//     Precondition: there is at a list (check the head_ptr).
//     Postcondition: The highest priority item has been returned and has been
//     removed from the PriorityQueue. (If several items have equal priority,
//     then the one that entered first will come out first.)
//
// CONSTANT MEMBER FUNCTIONS for the PriorityQueue class:
//   size_t size( ) const
//     Postcondition: Return value is the total number of items in the
//     PriorityQueue.  Used by the tester, Not in your code.
//
//   bool is_empty( ) const
//     Postcondition: Return value is true if the PriorityQueue is empty.
//     Used by the tester, Not in your code.
//
// VALUE SEMANTICS for the PriorityQueue class:
//   Assignments and the copy constructor may be used with
//   PriorityQueue objects

#ifndef pqueue1_HPP
#define pqueue1_HPP
#include <stdlib.h> // Provides size_t

template <typename Item>
struct Node
{   // Node for a linked list
    Item data;
    Node * link = nullptr;
    unsigned int priority;
};
template <class Item>
class PriorityQueue
{
public:
    PriorityQueue( );
    PriorityQueue(const PriorityQueue<Item>& source);
    ~PriorityQueue( );
    void operator =(const PriorityQueue<Item>& source);
    size_t size( ) const;
    void insert(const Item& entry, unsigned int priority);
    Item get_front( );
    bool is_empty( ) const { return head_ptr == nullptr; }
private:
    // Note: head_ptr is the head pointer for a linked list that
    // contains the items along with their priorities. These nodes are
    // kept in order from highest priority (at the head of the list)
    // to lowest priority (at the tail of the list).
    // The data type Node is completely defined below.
    Node<Item> * head_ptr = nullptr; // Make it easy to find uninitialized ptr
};
#include "pqueue1.tpp.h"
#endif
