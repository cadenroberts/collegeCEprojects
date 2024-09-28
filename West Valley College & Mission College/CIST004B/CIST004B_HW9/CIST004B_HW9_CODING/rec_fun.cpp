#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

void triangle(std::ostream& outs, int m, unsigned int n) {
    if (m <= n || n == 0) {
        for (int i = 0; i < (n > 0 ? m : 0); i++)
            outs << "*";
        outs << "\n";
        triangle(outs, m + (n > 0), n - 1);
        for (int i = 0; i < (n > 0 ? m : 0); i++)
            outs << "*";
        outs << "\n";
    }
}

void numbers(ostream& outs, const string& prefix, unsigned int levels) {
    if (levels == 0) {
        outs << prefix << "\n";
        return;
    }
    for (char c = '1'; c <= '9'; c++) {
        numbers(outs, prefix + c + ".", levels - 1);
    }
}

bool bears(int n) {
    if (n == 42) return true;
    if (n < 42) return false;
    if (n % 2 == 0) if (bears(n / 2)) return true;
    if (n % 5 == 0) if (bears(n - 42)) return true;
    if (n % 10 != 0 && (n % 100) / 10 != 0 && n % 3 == 0) if (bears(n - (n % 10) * ((n % 100) / 10))) return true;
    if (n % 4 == 0) if (bears(n - (n % 10) * ((n % 100) / 10))) return true;
    return false;
}

int fib_recursive(int n) {
    if (n <= 1) return n;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

int fib_iterative(int n) {
    if(n == 1 || n == 2)
        return 1;
    int A[2][2] = { { 1, 1 },{ 1, 0 } };
    int B[2][2] = { { 1, 1 },{ 1, 0 } };
    int temp[2][2];
    while (n >= 2) {
        for (int i = 0; i < 2; i++)
            for (int k = 0; k < 2; k++) {
                temp[i][k] = 0;
                for (int j = 0; j < 2; j++)
                    temp[i][k] += A[i][j] * B[j][k];
            }
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                B[i][j] = temp[i][j];
        n--;
    }
    return B[0][1];
}

int main() {
    cout << "Caden Roberts\t4/1/23\tCIST04B\n";
    cout << "\nPerforming Triangle Test...\n";
    triangle(cout, 1, 15);
    cout << "\nPerforming Numbers Test...\n";
    numbers(cout, "Bio ", 2);
    cout << "\nPerforming Bears Test...\n";
    int array[5]{250, 42, 84, 53, 41};
    for (int i = 0; i<5; i++) {
        cout << "Bears " << array[i];
        if (bears(array[i])) cout << " is true!\n";
        else cout << " is false!\n";
    }
    cout << "\nPerforming Fibonacci time tests...\n";
    cout << "  n     Recursive          Time     Iterative          Time\n";
    for (int n = 1; n <= 45; n++) {
        high_resolution_clock::time_point start_time_r = high_resolution_clock::now();
        int result_r = fib_recursive(n);
        high_resolution_clock::time_point end_time_r = high_resolution_clock::now();
        duration<float> time_span_r = end_time_r - start_time_r;
        
        high_resolution_clock::time_point start_time_i = high_resolution_clock::now();
        int result_i = fib_iterative(n);
        high_resolution_clock::time_point end_time_i = high_resolution_clock::now();
        duration<float> time_span_i = end_time_i - start_time_i;
        
        cout << right << setw(3) << n << right << setw(14) << result_r << right << setw(14) << fixed << setprecision(9) << time_span_r.count() << right << setw(14) << result_i << right << setw(14) << fixed << setprecision(9) << time_span_i.count() << endl;
    }
    return 0;
}
