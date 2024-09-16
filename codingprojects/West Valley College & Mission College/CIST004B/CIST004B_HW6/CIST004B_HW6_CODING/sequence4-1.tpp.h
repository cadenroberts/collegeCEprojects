//
// sequence4.tpp.h
// Templated Linked List Program
//
// Created by Caden Roberts on 3/16/23.
//
#include "sequence4.h"
#include "assert.h"
using namespace main_savitch_6B;
template <class Item>
sequence<Item>::sequence()
{
    head_ptr = nullptr;
    tail_ptr = nullptr;
    cursor = nullptr;
    precursor = nullptr;
    many_nodes = 0;
}
template <typename Item>
sequence<Item>::sequence(const sequence& source)
{
    head_ptr = tail_ptr = cursor = precursor = nullptr;
    many_nodes = 0;
    node<Item> *tail_copy = nullptr;
    for (node<Item> *p = source.head_ptr; p != nullptr; p = p->link())
    {
        node<Item> *new_node = new node<Item>(p->data());
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
template <typename Item>
sequence<Item>::~sequence()
{
    while (head_ptr != nullptr)
    {
        node<Item>* removed_node = head_ptr;
        head_ptr = head_ptr->link();
        delete removed_node;
    }
    many_nodes = 0;
}
template <typename Item>
void sequence<Item>::start()
{
    cursor = head_ptr;
    precursor = nullptr;
}
template <typename Item>
void sequence<Item>::advance()
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
template <typename Item>
void sequence<Item>::insert(const Item& entry)
{
    if (cursor == nullptr) {
        cursor = new node<Item>(entry, head_ptr);
        if (many_nodes == 0) tail_ptr = cursor;
        head_ptr = cursor;
        precursor = nullptr;
    } else {
        cursor = new node<Item>(entry, cursor);
        if (precursor == nullptr) head_ptr = cursor;
        else precursor->set_link(cursor);
    }
    ++many_nodes;
}
template <typename Item>
void sequence<Item>::attach(const Item& entry)
{
    if (cursor==nullptr) {
        if (tail_ptr != nullptr) {
            tail_ptr->set_link(new node<Item>(entry));
            precursor = tail_ptr;
            cursor = tail_ptr = tail_ptr->link();
        } else {
            cursor = tail_ptr = head_ptr = new node<Item>(entry);
        }
    } else {
        precursor = cursor;
        cursor->set_link(new node<Item>(entry, cursor->link()));
        if (cursor == tail_ptr) {
            tail_ptr = cursor->link();
        }
        cursor = cursor->link();
    }
    many_nodes++;
}
template <typename Item>
void sequence<Item>::operator=(const sequence& source)
{
    if (this == &source)
        return;
    while (head_ptr != nullptr)
    {
        node<Item>* removed_node = head_ptr;
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
        node<Item>* p = head_ptr;
        node<Item>* source_p = source.head_ptr;
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
template <typename Item>
void sequence<Item>::remove_current()
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
template <typename Item>
Item sequence<Item>::current() const
{
    assert(is_item());
    return cursor->data();}
