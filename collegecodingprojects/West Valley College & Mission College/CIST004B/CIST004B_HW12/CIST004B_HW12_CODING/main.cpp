// FILE: tabexam.cxx

//  Updated by Howard A Miller
//  West Valley College
//  2023

// Written by: Michael Main (main@colorado.edu), Nov 6, 1997
// Non-interactive test program for the Table template class
// with improved test for heap leaks.
//
// DESCRIPTION:
// Each function of this program tests part of the Table template class,
// returning some number of points to indicate how much of the test was
// passed.
// A description and result of each test is printed to cout.
// Maximum number of points awarded by this program is determined by the
// constants POINTS[1], POINTS[2]...
//
// WHAT'S NEW:
// This new version of the test program includes an improved test for heap
// leaks by overriding the new and delete operators. Users should consider
// placing these new and delete operators into a separate cxx file, but
// I have included everything in one file for easier distribution.


#include <iostream> // Provides cout.
#include <stdlib.h>   // Provides size_t.
#include "table2.h"   // Provides the Table template class.

using namespace std;

// Descriptions and points for each of the tests:
const size_t MANY_TESTS = 5;
const int POINTS[MANY_TESTS+1] = {
    25,  // Total points for all tests.
    11, // Test 1 points
    5,  // Test 2 points
    3,  // Test 3 points
    3,  // Test 4 points
    3   // Test 5 points
};
const char DESCRIPTION[MANY_TESTS+1][256] = {
    "tests for Table template class",
    "Testing insert and the const member functions",
    "Testing remove",
    "Testing for possible heap leaks,"
    "Testing the copy constructor",
    "Testing the assignment operator"
};



// **************************************************************************
// Replacements for new and delete:
// The next two functions replace the new and delete operators. Any code
// that is linked with this .cxx file will use these replacements instead
// of the standard new and delete. The replacements provide three features:
// 1. The global variable memory_used_now keeps track of how much memory has
//    been allocated by calls to new. (The global variable is static so that
//    it cannot be accessed outside of this .cxx file.)
// 2. The new operator fills all newly allocated memory with a GARBAGE char.
// 3. Each block of newly allocated memory is preceeded and followed by a
//    border of BORDER_SIZE characters. The first part of the front border
//    contains a copy of the size of the allocated memory. The rest of the
//    border contains a BORDER char.
// During any delete operation, the border characters are checked. If any
// border character has been changed from BORDER to something else, then an
// error message is printed and the program is halted. This stops most
// cases of writing beyond the ends of the allocated memory.
// **************************************************************************

const  size_t BORDER_SIZE     = 2*sizeof(double);
const  char   GARBAGE         = 'g';
const  char   BORDER          = 'b';
static size_t memory_used_now = 0;

void* operator new(size_t size)
{
    char   *whole_block;   // Pointer to the entire block that we get from heap
    size_t *size_spot;     // Spot in the block where to store a copy of size
    char   *front_border;  // The border bytes in front of the user's memory
    char   *middle;        // The memory to be given to the calling program
    char   *back_border;   // The border bytes at the back of the user's memory
    size_t i;              // Loop control variable

    // Allocate the block of memory for the user and for the two borders.
    whole_block = (char *) malloc(2*BORDER_SIZE + size);
    if (whole_block == NULL)
    {
        cout << "Insufficient memory for a call to the new operator." << endl;
        exit(0);
    }

    // Figure out the start points of the various pieces of the block.
    size_spot = (size_t *) whole_block;
    front_border = (char *) (whole_block + sizeof(size_t));
    middle = (char *) (whole_block + BORDER_SIZE);
    back_border = middle + size;

    // Put a copy of the size at the start of the block.
    *size_spot = size;

    // Fill the borders and the middle section.
    for (i = 0; i < BORDER_SIZE - sizeof(size_t); i++)
        front_border[i] = BORDER;
    for (i = 0; i < size; i++)
        middle[i] = GARBAGE;
    for (i = 0; i < BORDER_SIZE; i++)
        back_border[i] = BORDER;

    // Update the global static variable showing how much memory is now used.
    memory_used_now += size;
    
    return middle;
}

void operator delete(void* p) noexcept
{
    char   *whole_block;   // Pointer to the entire block that we get from heap
    size_t *size_spot;     // Spot in the block where to store a copy of size
    char   *front_border;  // The border bytes in front of the user's memory
    char   *middle;        // The memory to be given to the calling program
    char   *back_border;   // The border bytes at the back of the user's memory
    size_t i;              // Loop control variable
    size_t size;           // Size of the block being returned
    bool   corrupt;        // Set to true if the border was corrupted

    // Figure out the start of the pieces of the block, and the size.
    whole_block = ((char *) (p)) - BORDER_SIZE;
    size_spot = (size_t *) whole_block;
    size = *size_spot;
    front_border = (char *) (whole_block + sizeof(size_t));
    middle = (char *) (whole_block + BORDER_SIZE);
    back_border = middle + size;

    // Check the borders for the BORDER character.
    corrupt = false;
    for (i = 0; i < BORDER_SIZE - sizeof(size_t); i++)
        if (front_border[i] != BORDER)
            corrupt = true;
    for (i = 0; i < BORDER_SIZE; i++)
        if (back_border[i] != BORDER)
            corrupt = true;

    if (corrupt)
    {
        cout << "The delete operator has detected that the program wrote\n";
        cout << "beyond the ends of a block of memory that was allocated\n";
        cout << "by the new operator. Program will be halted." << endl;
        exit(0);
    }
    else
    {
        // Fill memory with garbage in case program tries to use it
        // even after the delete.
        for (i = 0; i < size + 2*BORDER_SIZE; i++)
            whole_block[i] = GARBAGE;
        free(whole_block);
        memory_used_now -= size;
    }
    
}



// **************************************************************************
// TRec type
//   Each table will use the following data type for its entries, with
//   keys ranging from 0 through MAX_KEY.
// **************************************************************************
struct TRec
{
    int key=0;
    double examdata;
};
const int MAX_KEY = 5000;



// **************************************************************************
// bool correct(const Table<TRec>& test, size_t s, double right_data[])
//   This function determines if the Table (test) is "correct" according to
//   these requirements for each i in the range 0..MAX_KEY:
//   a. if right_data[i] < 0 then there should be no item with key i
//      in the table.
//   b. if right_data[i] > 0, then the item with key i in the table should
//      have data of right_data[i].
//   If the requirements are met, then the function returns true. Otherwise
//   the function returns false. In either case a message about the test
//   result is printed to cout.
// **************************************************************************
bool correct(const Table<TRec>& test, double right_data[])
{
    size_t i;
    bool answer = true;
    bool find_result, is_present_result, should_be_present;
    TRec find_record;
    size_t n = 0;

    for (i = 0; answer && (i <= MAX_KEY); i++)
    {   // Test the is_present and find member functions
        should_be_present = (right_data[i] >= 0);
        is_present_result = test.is_present(static_cast<int>(i));
        test.find(static_cast<int>(i), find_result, find_record);
        // Check that the boolean values are correct
        if (should_be_present != is_present_result)
            answer = false;
        else if (should_be_present != find_result)
            answer = false;
        else if (should_be_present && (right_data[i] != find_record.examdata))
            answer = false;
        // Update the count of the size of the Table
        if (should_be_present) n++;
    }
    if (test.size( ) != n)
        answer = false;

    cout << (answer ? "Test passed.\n" : "Test failed.\n") << endl;
    return answer;
}



// **************************************************************************
// int test1( )
//   Performs some basic tests of insert and the constant member
//   functions. Returns POINTS[1] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test1( )
{
    const size_t TESTSIZE = 3000; // Size of a large test.
    
    Table<TRec> test;
    TRec test_record;
    double right_data[MAX_KEY+1];
    char test_letter = 'A';
    size_t i;
    int next;

    // Initialize each right_data[i] to -1, indicating that there is
    // no entry in the table for key i.
    for (i = 0; i <= MAX_KEY; i++)
        right_data[i] = -1;

    cout << test_letter++ << ". ";
    cout << "Testing size, is_present and find for an empty table.";
    cout << endl;
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Adding a record with key 4 to the Table, and then testing\n";
    cout << "   size, is_present and find.";
    cout << endl;
    test_record.key = 4;
    test_record.examdata = right_data[4] = 40.0;
    test.insert(test_record);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Adding a record with key 813 to the Table, and then testing\n";
    cout << "   size, is_present and find.";
    cout << endl;
    test_record.key = 813;
    test_record.examdata = right_data[813] = 8130.0;
    test.insert(test_record);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Adding a NEW record with key 4 to the Table, and then testing.\n";
    cout << "   This should change the record with key number 4.";
    cout << endl;
    test_record.key = 4;
    test_record.examdata = right_data[4] = 400.0;
    test.insert(test_record);
    if (!correct(test, right_data)) return 0;
    
    cout << test_letter++ << ". ";
    cout << "Adding a record with key 811 to the Table, and then testing.\n";
    cout << endl;
    test_record.key = 811;
    test_record.examdata = right_data[811] = 8110.0;
    test.insert(test_record);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Adding a record with key 810 to the Table, and then testing.\n";
    cout << endl;
    test_record.key = 810;
    test_record.examdata = right_data[810] = 8100.0;
    test.insert(test_record);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Adding a record with key 1611 to the Table, and then testing.\n";
    cout << endl;
    test_record.key = 1611;
    test_record.examdata = right_data[1611] = 16110.0;
    test.insert(test_record);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Adding a record with key 0 to the Table, and then testing.\n";
    cout << endl;
    test_record.key = 0;
    test_record.examdata = right_data[0] = 42.0;
    test.insert(test_record);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Inserting " << TESTSIZE << " random items with keys\n";
    cout << "between 0 and " << MAX_KEY << ", and then testing.";
    cout << endl;
    for (i = 6; i < TESTSIZE; i++)
    {
        next = rand( ) % (MAX_KEY + 1);
        test_record.key = next;
        test_record.examdata = right_data[next] = next/10.0;
        test.insert(test_record);
    }
    if (!correct(test, right_data)) return 0;

    return POINTS[1];
}



// **************************************************************************
// int test2( )
//   Performs some basic tests of the remove function.
//   Returns POINTS[2] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test2( )
// Postcondition: A handful of removal tests have been run on the
// Table<TRec> data type. If all tests are passed, then the function
// returns 20. Otherwise the function returns zero.
{
    const size_t TESTSIZE = 3000; // Size of a large test.
    
    Table<TRec> test;
    TRec test_record;
    double right_data[MAX_KEY+1];
    char test_letter = 'A';
    size_t i;
    int next;

    // Initialize each right_data[i] to -1, indicating that there is
    // no entry in the table for key i.
    for (i = 0; i <= MAX_KEY; i++)
        right_data[i] = -1;

    cout << test_letter++ << ". ";
    cout << "Putting one record into the Table, then removing it.\n";
    test_record.key = 0;
    test_record.examdata = 42.0;
    test.insert(test_record);
    test.remove(0);
    cout << endl;
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Adding a records with keys 4,5,6 to the Table, and then\n";
    cout << "   removing the 5 before testing.";
    cout << endl;
    test_record.key = 4;
    test_record.examdata = right_data[4] = 40.0;
    test.insert(test_record);
    test_record.key = 5;
    test_record.examdata = 50.0;
    test.insert(test_record);
    test_record.key = 6;
    test_record.examdata = right_data[6] = 60.0;
    test.insert(test_record);
    test.remove(5);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Trying to remove a key that is not present.\n";
    cout << "   This should have no effect on the Table.";
    cout << endl;
    test.remove(815);
    if (!correct(test, right_data)) return 0;

    cout << test_letter++ << ". ";
    cout << "Inserting " << TESTSIZE << " random items with keys\n";
    cout << "between 0 and " << MAX_KEY << ", and then removing about half.";
    cout << endl;
    for (i = 0; i < TESTSIZE; i++)
    {
        next = rand( ) % (MAX_KEY + 1);
        test_record.key = next;
        test_record.examdata = right_data[next] = next/10.0;
        test.insert(test_record);
    }
    for (i = 0; i < MAX_KEY; i++)
    {
        if (rand( ) % 2 == 0)
        {
            test.remove(static_cast<int>(i));
            right_data[i] = -1;
        }
    }
    if (!correct(test, right_data)) return 0;

    return POINTS[2];
}


// **************************************************************************
// int test3( )
//   Tries to find a heap leak in remove, the assignment operator or
//   the destructor.
//   Returns POINTS[3] if no leaks are found. Otherwise returns 0.
// **************************************************************************
int test3( )
{
    const size_t TESTSIZE = 3000;
    Table<TRec> test, empty;
    Table<TRec>* table_ptr;
    size_t i;
    size_t base_usage;
    TRec test_record;
  
    cout << "Checking for possible heap leak." << endl;
    base_usage = memory_used_now;
    
    // Test the destructor for a heap leak
    table_ptr = new Table<TRec>;
    test_record.examdata = 42.0;
    for (i = 0; i < TESTSIZE; i++)
    {
        test_record.key = static_cast<int>(i);
        table_ptr->insert(test_record);
    }
    delete table_ptr;
    if (memory_used_now != base_usage)
    {
        cout << "    Test failed. Possible heap leak in destructor." << endl;
        return 0;
    }

    // Test the assignment operator for a heap leak.
    for (i = 0; i < TESTSIZE; i++)
    {
        test_record.key = static_cast<int>(i);
        test.insert(test_record);
    }
    test = empty; // Should return test's nodes to the heap
    if (memory_used_now != base_usage)
    {
        cout << "    Test failed. Possible heap leak in assignment operator." << endl;
        return 0;
    }

    // Test the remove operator for a heap leak.
    for (i = 0; i < TESTSIZE; i++)
    {
        test_record.key = static_cast<int>(i);
        test.insert(test_record);
    }
    for (i = 0; i < TESTSIZE; i++)
        test.remove(static_cast<int>(i));
    if (memory_used_now != base_usage)
    {
        cout << "    Test failed. Possible heap leak in remove function." << endl;
        return 0;
    }
    
    cout << "No heap leaks found." << endl;
    return POINTS[3];
}


// **************************************************************************
// int test4( )
//   Performs some tests of the copy constructor.
//   Returns POINTS[4] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test4( )
{
    Table<TRec> test;
    double right_data[MAX_KEY+1];
    TRec test_record;
    size_t i;

    // Initialize each right_data[i] to -1, indicating that there is
    // no entry in the table for key i.
    for (i = 0; i <= MAX_KEY; i++)
        right_data[i] = -1;

    cout << "A. Testing that copy constructor works okay for empty table...";
    cout << flush;
    Table<TRec> copy1(test);
    if (!correct(copy1, right_data)) return 0;

    cout << "B. Testing copy constructor with Table that has 4,5,6,7 keys...";
    cout << flush;
    for (i = 4; i <= 7; i++)
    {
        test_record.key = static_cast<int>(i);
        test_record.examdata = right_data[i] = i/10.0;
        test.insert(test_record);
    }
    Table<TRec> copy2(test);
    test_record.key = 8;
    test.insert(test_record); // Alter the original, but not the copy
    if (!correct(copy2, right_data)) return 0;

    cout << "Copy constructor seems okay." << endl;
    return POINTS[4];
}



// **************************************************************************
// int test5( )
//   Performs some tests of the assignment operator.
//   Returns POINTS[5] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test5( )
{
    Table<TRec> test;
    double right_data[MAX_KEY+1];
    TRec test_record;
    char *oldbytes = new char[sizeof(Table<TRec>)];
    char *newbytes = new char[sizeof(Table<TRec>)];
    size_t i;

    // Initialize each right_data[i] to -1, indicating that there is
    // no entry in the table for key i.
    for (i = 0; i <= MAX_KEY; i++)
        right_data[i] = -1;

    cout << "A. Testing that assignment works okay for empty table...";
    cout << flush;
    Table<TRec> copy1;
    copy1 = test;
    if (!correct(copy1, right_data)) return 0;

    cout << "B. Testing assignment with Table that has 4,5,6,7 keys...";
    cout << flush;
    for (i = 4; i <= 7; i++)
    {
        test_record.key = static_cast<int>(i);
        test_record.examdata = right_data[i] = i/10.0;
        test.insert(test_record);
    }
    Table<TRec> copy2;
    copy2 = test;
    test_record.key = 8;
    test.insert(test_record); // Alter the original, but not the copy
    if (!correct(copy2, right_data)) return 0;

    cout << "C. Testing assignment operator for a self-assignment...";
    cout << flush;
    memcpy(oldbytes, &test, sizeof(Table<TRec>));
    test = test;
    memcpy(newbytes, &test, sizeof(Table<TRec>));
    for (i=0; i < sizeof(Table<TRec>); i++)
        if (oldbytes[i] != newbytes[i])
        {
            cout << "failed." << endl;
            return 0;
        }
    cout << "passed.\n";

    cout << "Assignment operator seems okay." << endl;
    return POINTS[5];
}


int run_a_test(int number, const char message[], int test_function( ), int max)
{
    int result;
    
    cout << endl << "START OF TEST " << number << ":" << endl;
    cout << message << " (" << max << " points)." << endl;
    result = test_function( );
    if (result > 0)
    {
        cout << "Test " << number << " got " << result << " points";
        cout << " out of a possible " << max << "." << endl;
    }
    else
        cout << "Test " << number << " failed." << endl;
    cout << "END OF TEST " << number << "." << endl << endl;
    
    return result;
}


// **************************************************************************
// int main( )
//   The main program calls all tests and prints the sum of all points
//   earned from the tests.
// **************************************************************************
int main( )
{
    int sum = 0;
    
    
    cout << "Running " << DESCRIPTION[0] << endl;

    sum += run_a_test(1, DESCRIPTION[1], test1, POINTS[1]);
    sum += run_a_test(2, DESCRIPTION[2], test2, POINTS[2]);
    sum += run_a_test(3, DESCRIPTION[3], test3, POINTS[3]);
    sum += run_a_test(4, DESCRIPTION[4], test4, POINTS[4]);
    sum += run_a_test(5, DESCRIPTION[5], test5, POINTS[5]);

    std::cerr << "\nYOUR RESULTS:\n";
        
    std::cerr << "You have successfully earned " << sum << " points out of a possible ";
    std::cerr << POINTS[0] << " points.\n\n";
        
    std::cerr << "If you submit this Binary Tree Program to Canvas now, this part of the \n";
    std::cerr << "grade could be as high as " << static_cast <int> ((sum* 10.0f)/POINTS[0]) << " points out of 10 points.\n\n";


    return EXIT_SUCCESS;

}
