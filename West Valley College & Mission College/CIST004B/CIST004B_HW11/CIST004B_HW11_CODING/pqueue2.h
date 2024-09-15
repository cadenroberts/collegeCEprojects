// FILE: pqueue2.h
// SUBMITTED BY (name and email address)
// CLASS PROVIDED: PriorityQueue (a priority queue of items)
//
// TYPEDEF and MEMBER CONSTANT for the PriorityQueue class:
//   static const size_t CAPACITY = ______
//     PriorityQueue::CAPACITY is the maximum number of entries that
//     may be be in the priority queue at any one time.
//
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
//     Precondition: size( ) > 0.
//     Postcondition: The highest priority item has been returned and has been
//     removed from the PriorityQueue. (If several items have equal priority,
//     then there is no guarantee about which one will come out first!
//     This differs from our first priority queue.)
//
// CONSTANT MEMBER FUNCTIONS for the PriorityQueue class:
//   size_t size( ) const
//     Postcondition: Return value is the total number of items in the
//     PriorityQueue.
//
//   bool is_empty( ) const
//     Postcondition: Return value is true if the PriorityQueue is empty.
//
// VALUE SEMANTICS for the PriorityQueue class:
//   Assignments and the copy constructor may be used with
//   PriorityQueue objects

#ifndef PQUEUE_H
#define PQUEUE_H
#include <stdlib.h> // Provides size_t

    class PriorityQueue
    {
    public:
        // TYPEDEF and MEMBER CONSTANT
        typedef int Item;
        static const size_t CAPACITY = 5000;
        // CONSTRUCTOR
        PriorityQueue( );
        // MODIFICATION MEMBER FUNCTIONS
        void insert(const Item& entry, unsigned int priority);
        Item get_front( );
        // CONSTANT MEMBER FUNCTIONS
        size_t size( ) const { return many_items; }
        bool is_empty( ) const { return (many_items == 0); }
        // TEMPORARY MEMBER FUNCTION FOR DEBUGGING (REMOVE BEFORE SUBMITTING)
        void print_tree(const char message[ ] = "", size_t i = 0) const;
    private:
        // STRUCT DEFINITION to store information about one item in the pqueue
        struct OneItemInfo
        {
            Item data;
            unsigned int priority;
        };
        // PRIVATE MEMBER VARIABLES
        OneItemInfo heap[CAPACITY];
        size_t many_items;
        // PRIVATE HELPER FUNCTIONS -- see pqueue2.cxx for documentation
        bool is_leaf(size_t i) const;
        size_t parent_index(size_t i) const;
        unsigned int parent_priority(size_t i) const;
        size_t big_child_index(size_t i) const;
        unsigned int big_child_priority(size_t i) const;
        void swap_with_parent(size_t i);
    };

#endif
