// File: queens.cxx
// Written by: Michael Main (main@colorado.edu) Nov 20, 2002
// Stack Pretest by: Howard Miller (howard.miller@wvm.edu) 3/15/2021
//
// This is a solution to the n-queens problem from Chapter 7
// of "Data Structures and Other Objects Using C++".
// This solution varies a bit from the problem in that the
// top of the Stack will be the column choice for row 1.
// Below that is the column choice for row 2, and so on.
// For example, with n set to 5, the solution shown in the book is:
// Q - - - -
// - - Q - -
// - - - - Q
// - Q - - -
// - - - Q -
// My Stack would record this solution as:
// [Bottom of Stack] 4 2 5 3 1 [Top of Stack].
// So, I don't actually record the choices of the row numbers, and
// furthermore, it's not possible to ever have two queens in the same row.
// Each time I push a new number on top of the Stack, the other rows get
// shifted to the next higher row number.

#include <cassert>
#include <iostream>
#include <ctime>
#include "stack3.h" // Provides the Stack class with a peek(i) function.
using namespace std;
using namespace main_savitch_7A;

void print_solution( Stack<unsigned int>& choices )
{
    size_t maxSize {choices.size( )};
    
    cout << "Solution for n = " << choices.size( ) << endl;
    for (size_t i = 1 ; i < maxSize; i++ ) cout << "=="; cout <<"="<<endl;
    
    for (size_t i = 1 ; !choices.empty( ); i++)
    {
        unsigned int col = choices.pop( );
        for ( int r=1 ; r<=maxSize ; r++)
            cout << (r == col? "Q " : "- ");
        cout << " Queen in row " << i << ", column " << col << endl;
    }
    
    for (size_t i = 1 ; i < maxSize; i++ ) cout << "=="; cout <<"="<<endl;
}

bool is_conflict( Stack<unsigned int>& choices )
{
    unsigned int i;
    unsigned int newqueen = choices.top( );
    unsigned int other;
    
      //  std::cout << "Checking against, New Queen Column: "<<newqueen <<std::endl;
    for (i = 2; i <= choices.size( ); i++)
    {   // Check whether queen on top of the Stack (which is in row 1)
        // conflicts with the queen in row number i.  If so, then return
        // true right away because we found a conflict.
        other = choices.peek(i);
        //        std::cout<<"\tAgainst col: "<<other<<std::endl;
        if ((newqueen == other)          // Queens in the same column
            || (newqueen+(i-1) == other) // Diagonal left-to-right
            || (newqueen-(i-1) == other) // Diagonal right-to-left
            ){
           //            std::cout <<"Conflict"<<std::endl;
            return true;
        }
    }
    //  std::cout <<"No conflict"<<std::endl;
    return false;
}

void queens(unsigned int nQueens)
{
   // clock_t a = clock();
    Stack<unsigned int> choices;
    bool success = false;
    assert (nQueens >= 1);
    
    choices.push(1);    // Start with 1st column (pushed) in the 1st row (last popped)
    
    while (!success && !choices.empty( ))
    {
      //  if (clock()-a>3000000) break;
        if (is_conflict(choices))
        {
            while (!choices.empty() && choices.top() == nQueens)
            {
                choices.pop();
            }
            if (!choices.empty())
            {
                unsigned int top_choice = choices.top();
                choices.pop();
                top_choice++;
                choices.push(top_choice);
            }

        }
        else if (choices.size( ) == nQueens)
            success = true;     // All queens placed
        else
            choices.push(1);    // Start at 1 for the next row
    }
    
    if (success)
        print_solution(choices);
    else
        cout << "No solutions were found." << endl;
}

int main( )
{
    // Stack Pre-Testto Make sure tour stack code works correctly
    
    {  // Stack Test Implementation
        class Card{
            bool    didYouCorrectlyTemplitize_used_and_size;
        };
        Stack<Card> test_used_and_size;
        test_used_and_size.size();            // Just call size()
        
        Stack<int> testStack;
        assert (testStack.empty());                // New Stacks are Empty
        assert (0 == testStack.size());            // New Stacks has Zero size
        
        for (int i=0; i < 10 ; ++ i){
            assert (i == testStack.size());        // Size should match
            testStack.push (i);                    // Add item to Stack
            assert (i+1 == testStack.size());    // Size should match
            assert (!testStack.empty());        // The stack should not be empty
            assert ( i == testStack.top() );    // The top() should match what was just pushed
            
          for (size_t j=1 ; j <= testStack.size() ; ++j )     //    Check the Whole Stack
               assert( testStack.size()-j == testStack.peek(j) );
    }
        
        for (int i=9; i >=0 ; -- i){
            assert ( i+1 == testStack.size());    // Size should match
            assert ( i == testStack.top() );    // The top() should match
            assert ( i == testStack.pop() );    // And so should the pop()
        }
        
        assert (0 == testStack.size());            // And now it should be Empty
        assert (testStack.empty());
    }    // Destructor called when we leaving scope
    cout << "Caden Roberts\t3/23/23\tCIST04B" << endl;
    cout << "Stack Pre-Test Passed!\n" << endl;
    
    
    
    unsigned int n;
    
    cout << "Please enter n for the n-queens problem (n >= 1): ";
    cin >> n;
    queens(n);
    return 0;
}
