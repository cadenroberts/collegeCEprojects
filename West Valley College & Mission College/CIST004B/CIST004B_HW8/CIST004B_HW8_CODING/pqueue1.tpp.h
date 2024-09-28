//
//  pqueue1.tpp.h
//  PQueue
//
//  Created by Caden Roberts on 3/23/23.
//

#include <stdio.h>
#include "pqueue1.hpp"
#include <assert.h>
template <typename Item>
PriorityQueue<Item>::PriorityQueue( )
{
    head_ptr = nullptr;
}
template <typename Item>
PriorityQueue<Item>::PriorityQueue(const PriorityQueue& source)
{
    head_ptr = nullptr;
    Node<Item>* tail_ptr;
    Node<Item>* source_ptr;
    // Handle empty source list
    if (source.head_ptr == nullptr)
    {
        head_ptr = nullptr;
    }
    else
    {
        head_ptr = new Node<Item>;
        head_ptr->data = source.head_ptr->data;
        head_ptr->priority = source.head_ptr->priority;
        head_ptr->link = nullptr;
        tail_ptr = head_ptr;
        source_ptr = source.head_ptr->link;
        while (source_ptr != nullptr)
        {
            tail_ptr->link = new Node<Item>;
            tail_ptr = tail_ptr->link;
            tail_ptr->data = source_ptr->data;
            tail_ptr->priority = source_ptr->priority;
            tail_ptr->link = nullptr;
            source_ptr = source_ptr->link;
        }
    }
}
template <typename Item>
PriorityQueue<Item>::~PriorityQueue( )
{
    Node<Item>* remove_ptr;
    while (head_ptr != nullptr)
    {
        remove_ptr = head_ptr;
        head_ptr = head_ptr->link;
        delete remove_ptr;
    }
}
template <typename Item>
void PriorityQueue<Item>::operator =(const PriorityQueue<Item>& source)
{
    if (this == &source)
    {
        return;
    }
    while (!is_empty())
    {
        get_front();
    }
    Node<Item>* src_ptr = source.head_ptr;
    while (src_ptr != nullptr)
    {
        insert(src_ptr->data, src_ptr->priority);
        src_ptr = src_ptr->link;
    }
}
template <typename Item>
size_t PriorityQueue<Item>::size() const {
    size_t count = 0;
    Node<Item>* curr = head_ptr;
    while (curr != nullptr) {
        count++;
        curr = curr->link;
    }
    return count;
}
template <typename Item>
void PriorityQueue<Item>::insert(const Item& entry, unsigned int priority) {
    Node<Item>* new_node = new Node<Item>;
    new_node->data = entry;
    new_node->priority = priority;
    new_node->link = nullptr;
    if (head_ptr == nullptr || priority > head_ptr->priority) {
        new_node->link = head_ptr;
        head_ptr = new_node;
    } else {
        Node<Item>* cursor = head_ptr;
        while (cursor->link != nullptr && priority <= cursor->link->priority) {
            cursor = cursor->link;
        }
        new_node->link = cursor->link;
        cursor->link = new_node;
    }
}
template <typename Item>
Item PriorityQueue<Item>::get_front() {
    assert(!is_empty());
    Item front = head_ptr->data;
    Node<Item>* temp_ptr = head_ptr;
    head_ptr = head_ptr->link;
    delete temp_ptr;
    return front;
}



