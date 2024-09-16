// FILE: node1.h
//
//  Updated by Howard Miller on 3/01/23.
//  Copyright © 2019, 2020, 2021, 2023 Howard Miller. All rights reserved.
//
// PROVIDES: A class for a node in a linked list, and list manipulation
// functions, all within the namespace main_savitch_5
//
// TYPEDEF for the node class:
//     Each node of the list contains a piece of data and a pointer to the
//     next node. The type of the data is defined as node::value_type in a
//     typedef statement. The value_type may be any
//     of the built-in C++ classes (int, char, ...) or a class with a copy
//     constructor, an assignment operator, and a test for equality (x == y).
//
// CONSTRUCTOR for the node class:
//   node(
//     const value_type& init_data = value_type(),
//     node* init_link = nullptr
//   )
//     Postcondition: The node contains the specified data and link.
//     NOTE: The default value for the init_data is obtained from the default
//     constructor of the value_type. In the ANSI/ISO standard, this notation
//     is also allowed for the built-in types, providing a default value of
//     zero. The init_link has a default value of nullptr.
//
// NOTE:
//   Some of the functions have a return value which is a pointer to a node.
//   Each of these  functions comes in two versions: a non-const version (where
//   the return value is node*) and a const version (where the return value
//   is const node*).
// EXAMPLES:
//    const node *c;
//    c->link( ) activates the const version of link
//    list_search(c,... calls the const version of list_search
//    node *p;
//    p->link( ) activates the non-const version of link
//    list_search(p,... calls the non-const version of list_search
//
// MEMBER FUNCTIONS for the node class:
//   void set_data(const value_type& new_data)
//     Postcondition: The node now contains the specified new data.
//
//   void set_link(node* new_link)
//     Postcondition: The node now contains the specified new link.
//
//   value_type data( ) const
//     Postcondition: The return value is the data from this node.
//
//   const node* link( ) const <----- const version
//   node* link( ) <----------------- non-const version
//   See the note (above) about the const version and non-const versions:
//     Postcondition: The return value is the link from this node.
//
#ifndef MAIN_SAVITCH_NODE1_H
#define MAIN_SAVITCH_NODE1_H
namespace main_savitch_5
{
class node
{
public:
// TYPEDEF
typedef double value_type;
// CONSTRUCTOR
node( const value_type& init_data = value_type( ),
 node* init_link = nullptr )
{ data_field = init_data; link_field = init_link; }
// Member functions to set the data and link fields:
void set_data(const value_type& new_data) { data_field = new_data; }
void set_link(node* new_link)             { link_field = new_link; }
// Constant member function to retrieve the current data:
value_type data( ) const { return data_field; }
// Two slightly different member functions to retreive the current link
const node* link( ) const  { return link_field; }
node* link( ) { return link_field; }
private:
value_type data_field;
node*  link_field;
};
// Helper functions for managing a linked list
//  These functions are provided for your use.
void list_piece( const node* start_ptr, const node* end_ptr, node*&
p_head_ptr, node*& p_tail_ptr );
void list_copy(const node* source_ptr, node*& head_ptr, node*& tail_ptr);
} // End of namespace
#endif
