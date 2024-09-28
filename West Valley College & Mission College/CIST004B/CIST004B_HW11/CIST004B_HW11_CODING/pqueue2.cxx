// FILE: pqueue2.cxx
// IMPLEMENTS: PriorityQueue (See pqueue2.h for documentation.)
// IMPLEMENTED BY: (name and email address)
//
// NOTE: You will need -lm at the end of your compile line to pick up
// the math library!

// INVARIANT for the PriorityQueue Class:
//   1. The member variable many_items is the number of items in the
//      PriorityQueue.
//   2. The items themselves are stored in the member variable heap,
//      which is a partially filled array organized to follow the usual
//      heap storage rules from Chapter 11 of the class notes.
// NOTE: Private helper functions are implemented at the bottom of this
// file along with their precondition/postcondition contracts.

#include <cassert>    // Provides assert function
#include <iomanip>    // Provides setw
#include <iostream>   // Provides cin, cout
#include <cmath>      // Provides log2
#include "pqueue2.h"
using namespace std;

PriorityQueue::PriorityQueue( )
{
    many_items=0;
    return;
    // -- Student will implement this.
}

void PriorityQueue::insert(const Item& entry, unsigned int priority)
{
    assert (many_items < CAPACITY);
    size_t new_index = many_items;
    heap[new_index].data = entry;
    heap[new_index].priority = priority;
    many_items++;
    while (new_index > 0 && parent_priority(new_index) < priority)
    {
        swap_with_parent(new_index);
        new_index = parent_index(new_index);
    }
    return;
    // -- Student will implement this.
}

PriorityQueue::Item PriorityQueue::get_front( )
{
    if (is_empty()) return PriorityQueue::Item(NULL);
    Item result = heap[0].data;
    heap[0] = heap[--many_items];
    size_t i = 0;
    while (!is_leaf(i)) {
        size_t big_child = big_child_index(i);
        if (!(heap[i].priority < heap[big_child].priority)) break;
        swap_with_parent(big_child);
        i = big_child;
    }
    
    return result;
    
    // -- Student will implement this.
}

bool PriorityQueue::is_leaf(size_t i) const
// Precondition: (i < many_items)
// Postcondition: If heap[i] has no children in the heap, then the function
// returns true. Otherwise the function returns false.
{
    if (i<many_items) return ((2 * i) + 1 > many_items - 1);
    return false;
    // -- Student will implement this.
}

size_t PriorityQueue::parent_index(size_t i) const
// Precondition: (i > 0) && (i < many_items)
// Postcondition: The return value is the index of the parent of heap[i].
{
    if ((i > 0) && (i < many_items)) return (i - 1) / 2;
    return size_t (NULL);
    // -- Student will implement this.
}

unsigned int PriorityQueue::parent_priority(size_t i) const
// Precondition: (i > 0) && (i < many_items)
// Postcondition: The return value is the priority of the parent of heap[i].
{
    if ((i > 0) && (i < many_items)) return heap[parent_index(i)].priority;
    return unsigned (NULL);
    // -- Student will implement this.
}

size_t PriorityQueue::big_child_index(size_t i) const
// Precondition: !is_leaf(i)
// Postcondition: The return value is the index of one of heap[i]'s children.
// This is the child with the larger priority.
{
    size_t a=2*i+1, b=heap[a].priority, c=heap[a+1].priority;
    return (a+1 >= many_items || b > c) ? a : a+1;
    return size_t (NULL);
    // -- Student will implement this.
}

unsigned int PriorityQueue::big_child_priority(size_t i) const
// Precondition: !is_leaf(i)
// Postcondition: The return value heap[big_child_index(i)].priority
{
    size_t a=2*i+1;
    unsigned int b=heap[a].priority, c=heap[a+1].priority;
    return (a+1 >= many_items || b > c) ? b : c;
    return unsigned (NULL);
    // -- Student will implement this.
}

void PriorityQueue::swap_with_parent(size_t i)
// Precondition: (i > 0) && (i < many_items)
// Postcondition: heap[i] has been swapped with heap[parent_index(i)]
{
    std::swap(heap[parent_index(i)], heap[i]);
    return;
}

void PriorityQueue::print_tree(const char message[ ], size_t i) const
// Postcondition: If the message is non-empty, then that has been written
// to cout. After the message, the portion of the heap with root at node
// node i has been written to the screen. Each node's data is indented
// 4*d, where d is the depth of the node.
// NOTE: The default argument for message is the empty string, and the
// default argument for i is zero. For example, to print the entire
// tree of a PriorityQueue p, with a message of "The tree:", you can call:
//     p.print_tree("The tree:");
// This call uses the default argument i=0, which prints the whole tree.
{
    const char NO_MESSAGE[ ] = "";
    size_t depth;
    
    if (message[0] != '\0')
        cout << message << endl;
    
    if (i >= many_items)
        cout << "NO NODES." << endl;
    else
    {
        depth = int(log(double(i+1))/log(2.0));
        cout << setw(static_cast<int>(depth)*4) << "";
        cout << heap[i].data;
        cout << " (priority " << heap[i].priority << ")" << endl;
        if (2*i + 1 < many_items)
            print_tree(NO_MESSAGE, 2*i + 1);
        if (2*i + 2 < many_items)
            print_tree(NO_MESSAGE, 2*i + 2);
    }
}
