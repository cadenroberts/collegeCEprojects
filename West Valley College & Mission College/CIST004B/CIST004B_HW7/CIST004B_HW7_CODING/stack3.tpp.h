// FILE: Stack3.tpp.h
// TEMPLATE CLASS IMPLEMENTED: Stack<Item> (see Stack1.h for documentation)
// This file is included in the header file, and not compiled separately.
// INVARIANT for the Stack class:
// 1. The number of items in the Stack is in the member variable used.
// 2. The actual items of the Stack are stored in a partially-filled
// array data[0]..data[used-1]. The Stack elements appear from the
// bottom (at data[0]) to the top (at data[used-1]).
#include <cassert> // Provides assert
#include "stack3.h"
namespace main_savitch_7A
{
template <typename Item>
Stack<Item>::~Stack()
{
    node<Item>* now;
    while (Top != nullptr){
        now = Top;
        Top = Top->link;
        delete now;
    }
    many_nodes=0;
}
template <class Item>
void Stack<Item>::push(const Item& entry)
// Library facilities used: cassert
{
    Top = new node<Item>(entry, Top);
    many_nodes++;
}

template <class Item>
Item Stack<Item>::pop( )
// Library facilities used: cassert
{
    assert(size() > 0); // Ensure stack is not empty.
    Item popped = Top->data;
    node<Item>* temp = Top;
    Top = Top->link;
    delete temp;
    many_nodes--;
    return popped;
    //     return result;
}
template <class Item>
Item Stack<Item>::top( ) const
// Library facilities used: cassert
{
    assert(size()>0);
    return Top->data;
}
template <class Item>
Item Stack<Item>::peek(size_t index)
// Library facilities used: cassert
{
    assert(index>=1 && index<=size());
    node<Item> * i = Top;
    for (size_t a = 1; a < index ; a++) i=i->link;
    return i->data;
}
}
