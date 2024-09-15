//
//  main.cpp
//  Sorting Algorithms
//
//  Created by Howard Miller on 4/22/19 - 5/05/2021.
//  Copyright © 2019-21 Howard Miller. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>  // Used for log10()
#include <thread>  // Used for Multi-Threaded solution
#include <cstdlib>  // Used for bult-in q-sort routine
#include <chrono>  // used for timing functions
#include <ctime>   // Used to display time

using namespace std;
using namespace std::chrono;

/*
 ************************
 ** STRUCT TO DEFINE A TEST PLUS NAME
 *************************/
struct sortRoutine {
    void (*function)( int *, size_t );
    const char * name;
};

/*
 ************************
 ** Utility Functions
 *************************/
int *getRandomNumbers (size_t num, int seed);
void printArray (int *array, size_t count);
void disarrangeArray  (int *array, size_t count, size_t numberOfChanges);
bool checkSort  (int *array, size_t count);
duration<double> sortAndTime (int* testArray, size_t theSize, sortRoutine theTest, bool print =false);

/*
 ************************
 ** Sample Sort Functions Provided
 *************************/
void shakerSort (int *array, size_t count);
void cQuickSort (int *array, size_t count);
void quickSortBook  (int *array, size_t count);


/*
 ************************
 ** Student Created Sort Function
 *************************/
void bubbleSort (int *array, size_t count);                // REQUIRED
void selectionSort(int *data, size_t n);                // REQUIRED
void insertionSort(int *data, size_t n);                // REQUIRED

void bubbleSortOptimized (int *array, size_t count);    // EXTRA CREDIT (EASY, So I recommend you do this)

void quickSort  (int *array, size_t count);                // REQUIRED / Extra Credit if you do the non-recursive verison
void mergeSort  (int *array, size_t count);                // REQUIRED / Extra Credit if you do the non-recursive verison
void heapSort  (int *array, size_t count);                // REQUIRED / Extra Credit if you do the non-recursive verison

void quickSortNonRecursive (int *array, size_t count);    // Extra Credit
void nonRecursiveMergeSort  (int *array, size_t count);    // Extra Credit

void quickSortBookOptimized  (int *array, size_t count);    // EXTRA CREDIT

void radixSort  (int *array, size_t count);                    // EXTRA CREDIT


void quickSortThreaded  (int *array, size_t count);            // EXTRA CREDIT
void quickSortHybrid  (int *array, size_t count);            // EXTRA CREDIT
void quickSortThreadedHybrid (int *array, size_t count);    //Just Crazy stuff!




/*
 ************************
 ** Sort Functions
 ** Add your function to this table
 *************************/

sortRoutine yourSortRoutines[] = {
    {shakerSort, "a sample Shaker Sort"},
    {quickSortBook, "the Quick Sort from the Book"},
    {bubbleSort, "the Bubble Sort"},
    {bubbleSortOptimized, "your Optimized Bubble Sort"},
    {selectionSort, "your Selection Sort"},
    {insertionSort, "your Insertion Sort"},
    
    //{radixSort, "your Radix Sort"},
    {cQuickSort, "the C++ Built-in Quick Sort"},
    {quickSort, "your Quick Sort"},
    //{heapSort, "your Heap Sort"},
    {quickSortNonRecursive, "your Non-Recursive Quick Sort"},
    //{quickSortHybrid, "your Hybrid Quick Sort"},  // Not worth it
    {quickSortThreaded, "your Multi-Threaded Quick Sort"},
    //{quickSortThreadedHybrid, "your Hybrid Multi-Threaded Quick Sort"},
    {mergeSort, "your Merge Sort"},
    {nonRecursiveMergeSort, "your Non-Recursive Merge Sort"},
    
    {nullptr , nullptr }  // Terminates the list
};

long long checkSum;  // Used to double check the sorted list

/*
 ************************
 ** main()
 ** Do not change this
 *************************/
int main(int argc, const char * argv[]) {
    size_t inputSize{0};
    char inputChar;
    
    cout << "CIST 004B Sorting Assignment.  Worth 20 points\n\n" << flush;
    
    do {
        cout << "Enter the size of Array you wish to sort (not more than 100,000,000): ";
        cin >> inputSize;
        if (inputSize > 100000000)
            cout << "Please try again!\n" << endl;
    } while (inputSize > 100000000 );
    
    do {
        cout << "Do you wish a single run (Y/N): ";
        cin >> inputChar;
        inputChar = toupper (inputChar);
    } while (inputChar != 'Y' && inputChar != 'N');
    
    if (inputChar == 'Y'){
        for ( size_t i{0} ;yourSortRoutines[i].function != nullptr ; ++ i  ){
            cout << fixed << setprecision(6);
            int* testArray = getRandomNumbers (inputSize, 0);
            
            
            cout<< "\nNow executing " << yourSortRoutines[i].name << " of "<< inputSize <<" items." << endl;
            duration<double> time_span_r = sortAndTime(testArray, inputSize, yourSortRoutines[i], true);
            cout << static_cast<char>(toupper(yourSortRoutines[i].name[0])) << &yourSortRoutines[i].name[1]
            << " took " << setw(7) << time_span_r.count() << " Seconds." << endl;
            
            disarrangeArray  (testArray, inputSize, inputSize*0.0005 +1); // Swap 0.05% of the numbers in the sorted array...so 99.9% right
            cout<< "Now Executing " << yourSortRoutines[i].name << " of "<< inputSize <<" mostly (99.9%) sorted items." << endl;
            time_span_r = sortAndTime(testArray, inputSize, yourSortRoutines[i], true);
            cout << static_cast<char>(toupper(yourSortRoutines[i].name[0])) << &yourSortRoutines[i].name[1]
            << " took " << setw(8) << time_span_r.count() << " Seconds." << endl;
            
            cout<< "Now Executing " << yourSortRoutines[i].name << " of "<< inputSize <<" pre-sorted items." << endl;
            time_span_r = sortAndTime(testArray, inputSize, yourSortRoutines[i], true);
            cout << static_cast<char>(toupper(yourSortRoutines[i].name[0])) << &yourSortRoutines[i].name[1]
            << " took " << setw(8) << time_span_r.count() << " Seconds." << endl;
            
            delete[] (testArray-2);  // NOTE:  Adjusted for underrun Padding
        }
        cout<< endl;
    } else {
        size_t ArraySize{0};
        time_t rawtime;
        cout << "\nTesting every Sort Routine with array sizes from 0 to " << inputSize << " numbers."<< endl;;
        do {
            if (ArraySize % 100 == 1 ) {
                if (ArraySize == 1)
                    cout << "Starting at:\t\t\t";
                time (&rawtime);
                // If you can't get the time display to work, text me and try this LAST
                // cout << endl;
                // Code for Old School C++.  MS does not like this!
                // cout  << '\t' << ctime(&rawtime) << setw(log10(inputSize)+1)<< ArraySize-1;
                // C++ 20 code...Works on Most current compilers
                char str[26];
                cout  << '\t' << ctime_r( &rawtime, str )<< setw(log10(inputSize)+1)<< ArraySize-1 ;
                // Code for MS Visual Studio Users.  xCode does not like this!
                // Put this at the top of the file:
                // #define __STDC_WANT_LIB_EXT1__ 1
                // Then the timing code goes here:
                // char str[26];
                // ctime_s(str,sizeof str,&rawtime);
                // cout  << '\t' << str << setw(log10(inputSize)+1)<< ArraySize-1;
            }
            if (ArraySize % 10 == 9 )
                cout <<" ." << flush;
            for ( size_t i{0} ;yourSortRoutines[i].function != nullptr ; ++ i  ){
                int* testArray = getRandomNumbers (ArraySize, 0);
                sortAndTime(testArray, ArraySize, yourSortRoutines[i], true);      // Run sort and print array if an error
                delete[] (testArray-2);  // NOTE:  Adjusted for underrun Padding
            }
        }  while(ArraySize++ < inputSize);
        cout <<endl;
        time (&rawtime);
        cout  << "Finished at:\t\t\t\t" << ctime(&rawtime)<<endl; // Note: ctime has built-in '\n'
        // MS USERS     ctime_s(str,sizeof str,&rawtime);
        //              cout  << "Finished at:\t\t\t\t" << str <<endl; // Note: ctime has built-in '\n'
    }
    return 0;
}


// YOUR SORT ROUTINES

/*
 ************************
 ** Sample SHAKER SORT
 *************************/
void shakerSort (int *array, size_t count) {
    if (count <= 1)
        return; // No work for an empty or 1 element array.
    
    size_t p, i;
    for (p = 1; p <= count / 2; p++) {
        bool doneEarlyOptimization {true};
        for (i = p - 1; i < count - p; i++){  // Up Pass
            if (array[i] > array[i+1]) {
                int temp = array[i+1];
                array[i+1] = array[i];
                array[i] = temp;
                doneEarlyOptimization = false;
            }
        }
        for (i = count - p - 1; i >= p; i--) {  // Down Pass
            if (array[i] < array[i-1]) {
                int temp = array[i-1];
                array[i-1] = array[i];
                array[i] = temp;
                doneEarlyOptimization = false;
            }
        }
        if (doneEarlyOptimization)
            break;
    }
    return;
}

/*
 ************************
 ** BUILT-IN C-QUICK SORT
 *************************/
int cQuickSortCompare (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void cQuickSort (int *array, size_t count) {
    qsort (array, count, sizeof(int), cQuickSortCompare);
    return;
}


/*
 ************************
 ** BUBBLE SORT
 *************************/
void bubbleSort (int *array, size_t count) {
    if (count <= 1) return;
    for (int i = 0, a; i < count-1; i++)
        for (int j = 0; j < count-i-1; j++)
            if (array[j] > array[j+1]) {
                a = array[j];
                array[j] = array[j+1];
                array[j+1] = a;
            }
    return;
}

/*
 ************************
 ** Optimized BUBBLE SORT
 *************************/
void bubbleSortOptimized (int *array, size_t count){
    if (count <= 1) return;
    for (int i=0, a=1; i < count-1; i++, a=1) {
        for (int j = 0; j < count-i-1; j++) {
            if (array[j] > array[j+1]) {
                a=array[j];
                array[j] = array[j+1];
                array[j+1]=a;
                a=0;
            }
        }
        if (a) break;
    }
    return;
}

/*
 ************************
 ** SELECTION SORT
 *************************/
void selectionSort(int *data, size_t n){
    size_t i, j, index_of_largest;
    int largest;
    if (n <= 1) return; // No work for an empty array
    for (i = n-1; i > 0; --i) {
        largest = data[0];
        index_of_largest = 0;
        for (j = 1; j <= i; ++j) {
            if (data[j] > largest) {
                largest = data[j];
                index_of_largest = j;
            }
        }
        largest = data[i];
        data[i] = data[index_of_largest];
        data[index_of_largest] = largest;
    }
};


/*
 ************************
 ** INSERTION SORT
 *************************/
void insertionSort(int *data, size_t n){
    int key, j;
    for (int i = 1; i < n; i++) {
        key = data[i];
        j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j+1] = data[j];
            j--;
        }
        data[j+1] = key;
    }
};

/*
 ************************
 ** RADIX SORT
 *************************/
/*void radixSort(int* array, size_t count) {
 // Find the maximum number in the array
 int max = abs(array[0]);
 for (size_t i = 1; i < count; ++i) {
 int absVal = abs(array[i]);
 if (absVal > max) {
 max = absVal;
 }
 }
 
 // Sort by each digit, starting from the least significant digit
 for (int exp = 1; max / exp > 0; exp *= 10) {
 // Count the frequency of each digit
 int digitCount[10] = {0};
 for (size_t i = 0; i < count; ++i) {
 int digit = abs(array[i]) / exp % 10;
 digitCount[digit]++;
 }
 
 // Calculate the starting index for each digit in the output array
 int startIndex[10] = {0};
 for (int i = 1; i < 10; ++i) {
 startIndex[i] = startIndex[i-1] + digitCount[i-1];
 }
 
 // Put each element in the array in its sorted position in the output array
 int* output = new int[count];
 for (int i = count - 1; i >= 0; --i) {
 int digit = abs(array[i]) / exp % 10;
 int index = startIndex[digit]++;
 output[index] = array[i];
 }
 
 // Copy the sorted output array back to the input array
 for (size_t i = 0; i < count; ++i) {
 array[i] = output[i];
 if (array[i] < 0) {
 array[i] *= -1; // Restore the original sign
 }
 }
 
 delete[] output;
 }
 }*/


/*
 ************************
 ** MERGE SORT
 *************************/
void mergeSort(int data[ ], size_t n){
    size_t n1, n2;
    if (n > 1) {
        n1 = n / 2;
        n2 = n - n1;
        mergeSort(data, n1);
        mergeSort((data + n1), n2);
        int *temp = new int[n1+n2];
        size_t copied = 0, copied1 = 0, copied2 = 0, i;
        while ((copied1 < n1) && (copied2 < n2)) {
            if (data[copied1] < (data + n1)[copied2]) temp[copied++] = data[copied1++];
            else temp[copied++] = (data + n1)[copied2++];
        }
        while (copied1 < n1) temp[copied++] = data[copied1++];
        while (copied2 < n2) temp[copied++] = (data+n1)[copied2++];
        for (i = 0; i < n1+n2; ++i) data[i] = temp[i];
        delete [ ] temp;
    }
}
/*
 ************************
 ** Non-recursive MERGE SORT
 *************************/
void nonRecursiveMergeSort(int* array, size_t count) {
    size_t n1, n2;
    for (size_t size = 1; size < count; size *= 2) {
        for (size_t left_start = 0; left_start < count; left_start += 2 * size) {
            n1 = size;
            if (left_start + n1 > count) n1 = count - left_start;
            n2 = size;
            if (left_start + n1 + n2 > count) n2 = count - left_start - n1;
            int temp[n1 + n2];
            size_t i1 = 0, i2 = 0;
            for (size_t i = 0; i < n1 + n2; ++i) {
                if (i1 < n1 && i2 < n2) {
                    if (array[left_start + i1] < array[left_start + n1 + i2]) {
                        temp[i] = array[left_start + i1];
                        ++i1;
                    } else {
                        temp[i] = array[left_start + n1 + i2];
                        ++i2;
                    }
                } else if (i1 < n1) {
                    temp[i] = array[left_start + i1];
                    ++i1;
                } else {
                    temp[i] = array[left_start + n1 + i2];
                    ++i2;
                }
            }
            for (size_t i = 0; i < n1 + n2; ++i) {
                array[left_start + i] = temp[i];
            }
        }
    }
}

/*
 ************************
 ** HEAP SORT
 *************************/

/*void heapSort(int *array, size_t count) {
 if (count<=1) return;
 int temp;
 size_t unsorted=count;
 
 for (int i = 1, k; i<count; ++i){
 k=i;
 while((k!=0)&&(array[k]>array[(k-1)/2])){
 temp = array[k];
 array[k] = array[(k-1)/2];
 array[(k-1)/2] = temp;
 k = (k-1)/2;
 }
 }
 
 
 while(unsorted>1){
 --unsorted;
 temp = array[0];
 array[0] = array[unsorted];
 array[unsorted] = temp;
 size_t current=0, bigkid;
 bool sorted=false;
 while ((!sorted)&&(2*current+2<unsorted)){
 bigkid = array[2*current+1]>array[2*current+2] ? 2*current+1 : 2*current+2;
 if (array[current]<array[bigkid]){
 temp=array[current];
 array[current]=array[bigkid];
 array[bigkid]=temp;
 current = bigkid;
 } else sorted=true;
 
 }
 
 }
 }*/

/*
 ************************
 ** QUICK SORT
 *************************/
void quickSort  (int *array, size_t count){
    if (count <= 1) return;
    int pivot = array[0], swapper;
    size_t i = 1, j = count - 1;
    while (i <= j) {
        while (i < count && array[i] < pivot) i++;
        while (j > 0 && array[j] >= pivot) j--;
        if (i < j) {
            swapper = array[j];
            array[j] = array[i];
            array[i] = swapper;
        }
    }
    swapper = array[0];
    array[0] = array[j];
    array[j] = swapper;
    quickSort(array, j);
    quickSort(array + j + 1, count - j - 1);
}

/*
 ************************
 ** QUICK SORT NON-RECURSIVE
 *************************/

void quickSortNonRecursive(int *array, size_t count) {
    if (count <= 1) return;
    int stack[count];
    int top = -1;
    stack[++top] = 0;
    stack[++top] = count - 1;
    while (top >= 0) {
        int end = stack[top--];
        int start = stack[top--];
        int pivot = array[start];
        int i = start + 1;
        int j = end;
        while (i <= j) {
            while (i <= end && array[i] < pivot) i++;
            while (j > start && array[j] >= pivot) j--;
            if (i < j) {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
        int temp = array[start];
        array[start] = array[j];
        array[j] = temp;
        if (j - 1 > start) {
            stack[++top] = start;
            stack[++top] = j - 1;
        }
        if (j + 1 < end) {
            stack[++top] = j + 1;
            stack[++top] = end;
        }
    }
}



/*
 ************************
 ** Multi-Threaded QUICK SORT
 *************************/
void quickSortThreaded  (int *array, size_t count){
    static size_t threads;
    if (count <= 1) return;
    int pivot = array[0], swapper;
    size_t i = 1, j = count - 1;
    while (i <= j) {
        while (i < count && array[i] < pivot) i++;
        while (j > 0 && array[j] >= pivot) j--;
        if (i < j) {
            swapper = array[j];
            array[j] = array[i];
            array[i] = swapper;
        }
    }
    swapper = array[0];
    array[0] = array[j];
    array[j] = swapper;
    if (threads<500){
        threads+=2;
        thread leftThread(quickSortThreaded, array, j);
        thread rightThread(quickSortThreaded, array + j + 1, count - j - 1);
        leftThread.join();
        rightThread.join();
    } else {
        quickSortThreaded(array, j);
        quickSortThreaded(array + j + 1, count - j - 1);
    }
};

/*
 ************************
 ** Hybrid QUICK SORT
 *************************/


/*
 ************************
 ** Threaded Hybrid QUICK SORT
 *************************/


/*
 ************************
 ** QUICK SORT FROM BOOK
 *************************/
void partitionBook(int data[ ], size_t n, size_t& pivot_index)
// Library facilities used: itemtool.h, stdlib.h
//
// NOTES FROM THE IMPLEMENTOR:
// How the partition works on small arrays:
//
// Notice that n=0 is not permitted by the precondition.
//
// If n=1, then too_big_index is initialized as 1, and too_small_index is
// initialized as 0. Therefore, the body of the loop is never executed,
// and after the loop pivot_index is set to zero.
//
// If n=2, then both too_big_index and too_small_index are initialized as 1.
// The loop is entered, and there are two cases to consider:
// -- if data[1] <= pivot, then too_big_index increases to 2, and
//    too_small_index stays at 1. The if-statement at the bottom of the loop
//    is then skipped, and after the loop we copy data[1] down to data[0],
//    and copy the pivot into data[0]. Thus, the smaller element is in
//    data[0], and the larger element (the pivot) is in data[1].
// -- if data[1] > pivot, then too_big_index stays at 1, and too_small_index
//    decreases to 0. The if-statement at the bottom of the loop is then
//    skipped, and after the loop we end up copying the pivot onto data[0]
//    (leaving data[1] alone). Thus, the smaller element (the pivot) remains
//    at data[0], leaving the larger element at data[1].
{
    int pivot = data[0];
    size_t too_big_index = 1;     // Index of first item after pivot
    size_t too_small_index = n-1; // Index of last item
    
    // Partition the array, using pivot as the pivot element
    while (too_big_index <= too_small_index)
    {
        while ((too_big_index < n) && (data[too_big_index] <= pivot))
            too_big_index++;
        while (data[too_small_index] > pivot)
            too_small_index--;
        if (too_big_index < too_small_index){
            int temp = data[too_small_index];
            data[too_small_index] = data[too_big_index];
            data[too_big_index] = temp;
            // Too Slow            swap(data[too_small_index], data[too_big_index]);
        }
    }
    // Move the pivot element to its correct position
    pivot_index = too_small_index;
    data[0] = data[pivot_index];
    data[pivot_index] = pivot;
}

void quickSortBook(int data[ ], size_t n) {
    size_t pivot_index; // Array index for the pivot element
    size_t n1;          // Number of elements before the pivot element
    size_t n2;          // Number of elements after the pivot element
    
    if (n > 1)
    {
        // Partition the array, and set the pivot index
        partitionBook(data, n, pivot_index);
        
        // Compute the sizes of the subarrays
        n1 = pivot_index;
        n2 = n - n1 - 1;
        
        // Recursive calls to sort the subarrays
        quickSortBook(data, n1);
        quickSortBook((data + pivot_index + 1), n2);
    }
}







/*
 ************************
 ** Utility Functions
 ** Do not change these
 *************************/
duration<double> sortAndTime (int* testArray, size_t theSize, sortRoutine theTest, bool print){
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    theTest.function (testArray, theSize);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    
    if ( !checkSort  (testArray, theSize) && print){
        cout<< "Array size of: "<< theSize << " contents after " << theTest.name << " completed." << endl;
        printArray (testArray, theSize);
    }
    
    return t2 - t1;
}

//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/
// Build and Checksum the Test Array
//  WITH
// Padding for Underrun and Overrun
//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/
int* getRandomNumbers(size_t num, int seed =0) {
    int *arr = nullptr;
    checkSum = 0LL;
    
    arr = new int[num +4];    // Array to hold the numbers plus Padding on each end!
    
    arr[0] = arr [1] = 999999;  // Pad for array Underrun
    arr += 2;    // The user array starts after the padding.  Adjust back at delete[]
    
    srand(seed);
    int rnum = static_cast<int>(num); // Randomize the range of number
    for (int count = 0; count < num; count++){
        arr[count] = rand() % (rnum*11) - rnum*11/2;
        checkSum += static_cast <long long>(arr[count]);
    }
    arr[num+1] = arr [num] = -999999;  // Pad for array Overrun
    
    return arr;
}

//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/
// Check that the Array is Sorted
//  AND
// Check that All the numbers are there
//  AND
// Check the Padding for over/under-write
//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/
bool checkSort( int *a, size_t count) {
    if (count < 1)
        return true; // No work for an empty array...none was allocated in the first place!
    
    bool sorted = true;
    long long testCheckSum =0;
    for ( size_t i{0} ; i < count-1 ; ++i){
        testCheckSum += static_cast<long long>(a[i]);
        if ( a[i] > a[i+1] ) {
            sorted = false;
            cout << "Error Encountered!"<< endl;
            cout << "\tArray ["<< setw(log10(count)+1) <<i   << "] = " << a[i]   << endl;
            cout << "\tArray ["<< setw(log10(count)+1) <<i+1 << "] = " << a[i+1] << endl;
            break;
        }
    }
    // Check that all the values are there (using a checksum)
    if (testCheckSum+ static_cast<long long>(a[count-1]) != checkSum ) {
        cout <<"\nChecksum of Array does not match.  Orginal: " << checkSum;
        cout <<" New: " << testCheckSum+ static_cast<long long>(a[count-1]);
        cout <<"\nNot All numbers not present in the resulting array!\n";
        printArray(a, count);
        sorted = false;
    }
    // Check Padding
    if ( *(a-1) != 999999  or  *(a-2) != 999999 ){  // Check for under run
        cout << "Error Encountered!"<< endl;
        cout << "\tArray [-2] is "<< *(a-2) <<endl;
        cout << "\tArray [-1] is "<< *(a-1) <<endl;
        cout << "\tBoth should be -999999.  You wrote Before the start of the array"<<endl;
        sorted =  false;
    }
    if ( a[count] != -999999  or  a[count+1] != -999999 ){  // Check for overrun
        cout << "Error Encountered!"<< endl;
        cout << "\tArray ["<< setw(log10(count)+1) <<count   << "] = " << a[count]   << endl;
        cout << "\tArray ["<< setw(log10(count)+1) <<count+1 << "] = " << a[count+1] << endl;
        cout << "\tBoth should be 999999.  You wrote After the end of the array"<<endl;
        sorted =  false;
    }
    return sorted;
}


//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/
// Move Around Just a Few Items!
// Leaves the List Mostly Sorted
//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/
void disarrangeArray  (int *array, size_t count, size_t numberOfChanges){
    if (count < 10  || count < numberOfChanges*4 )  // Won't do Small Arrays / large number of changes
        return;
    
    srand(42);    //Contant seed value
    
    for ( ; numberOfChanges > 0 ; --numberOfChanges ){
        size_t x = rand()%(count+1);
        size_t y = rand()%(count+1);
        int tmp = array[x];
        array[x] = array[y];
        array[y] = tmp;
    }
    return;
}

//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/
// Print the Arry Nicely Formatted
//\_/\_/\_/\_/\_/\_/\_/\_/\_/\_/

void printArray (int* array, size_t count){
    for ( size_t i{0} ; i < count ; ++i){
        cout << "Array [" << setw(log10(count)+1) <<i<< "] = " << array[i] << endl;
    }
}
