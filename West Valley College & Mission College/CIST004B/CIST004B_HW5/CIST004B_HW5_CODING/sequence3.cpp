//
//  sequence3.cpp
//  Linked List Program
//
//  Created by Caden Roberts on 3/9/23.
//
#include "sequence3.hpp"
#include "assert.h"
using namespace main_savitch_5;

sequence::sequence()
{
    head_ptr = nullptr;
    tail_ptr = nullptr;
    cursor = nullptr;
    precursor = nullptr;
    many_nodes = 0;
}

sequence::sequence(const sequence& source)
{
    head_ptr = tail_ptr = cursor = precursor = nullptr;
    many_nodes = 0;
    
    node *tail_copy = nullptr;
    for (node *p = source.head_ptr; p != nullptr; p = p->link())
    {
        node *new_node = new node(p->data());
        if (tail_copy == nullptr)
        {
            head_ptr = new_node;
        }
        else
        {
            tail_copy->set_link(new_node);
        }
        tail_copy = new_node;
        
        if (p == source.cursor)
        {
            cursor = new_node;
        }
        if (p == source.precursor)
        {
            precursor = new_node;
        }
        ++many_nodes;
    }
    
    tail_ptr = tail_copy;
    
}

sequence::~sequence()
{
    while (head_ptr != nullptr)
    {
        node* removed_node = head_ptr;
        head_ptr = head_ptr->link();
        delete removed_node;
    }
    many_nodes = 0;
}

void sequence::start()
{
    cursor = head_ptr;
    precursor = nullptr;
}

void sequence::advance()
{
    if (cursor != nullptr) {
        precursor=cursor;
        cursor=cursor->link();
    }
    else if (head_ptr != nullptr)
    {
        precursor = tail_ptr;
        cursor = nullptr;
    }
}

void sequence::insert(const value_type& entry)
{
    if (cursor == nullptr) {
        cursor = new node(entry, head_ptr);
        if (many_nodes == 0) tail_ptr = cursor;
        head_ptr = cursor;
        precursor = nullptr;
    } else {
        cursor = new node(entry, cursor);
        if (precursor == nullptr) head_ptr = cursor;
        else precursor->set_link(cursor);
    }
    ++many_nodes;
    
}

void sequence::attach(const value_type& entry)
{
    if (cursor==nullptr) {
        if (tail_ptr != nullptr) {
            tail_ptr->set_link(new node(entry));
            precursor = tail_ptr;
            cursor = tail_ptr = tail_ptr->link();
        } else {
            cursor = tail_ptr = head_ptr = new node(entry);
        }
    } else {
        precursor = cursor;
        cursor->set_link(new node(entry, cursor->link()));
        if (cursor == tail_ptr) {
            tail_ptr = cursor->link();
        }
        cursor = cursor->link();
    }
    many_nodes++;
}

void sequence::operator=(const sequence& source)
{
    if (this == &source)
        return;
    while (head_ptr != nullptr)
    {
        node* removed_node = head_ptr;
        head_ptr = head_ptr->link();
        delete removed_node;
    }
    many_nodes = 0;
    list_copy(source.head_ptr, head_ptr, tail_ptr);
    many_nodes = source.many_nodes;
    if (source.cursor == nullptr) {
        cursor = nullptr;
        precursor = nullptr;
    } else {
        node* p = head_ptr;
        node* source_p = source.head_ptr;
        while (source_p != source.cursor) {
            p = p->link();
            source_p = source_p->link();
        }
        cursor = p;
        precursor = nullptr;
        if (p != head_ptr) {
            precursor = head_ptr;
            while (precursor->link() != p)
                precursor = precursor->link();
        }
    }
}

void sequence::remove_current()
{
    if (cursor == nullptr && precursor == nullptr) return;
    if (cursor == head_ptr)
    {
        head_ptr = head_ptr->link();
        delete cursor;
        cursor = head_ptr;
        if (head_ptr == nullptr) tail_ptr = nullptr;
    }
    else
    {
        precursor->set_link(cursor->link());
        if (cursor == tail_ptr) tail_ptr = precursor;
        delete cursor;
        cursor = precursor->link();
    }
    --many_nodes;
}

value_type sequence::current() const
{
    assert(is_item());
    return cursor->data();
}
