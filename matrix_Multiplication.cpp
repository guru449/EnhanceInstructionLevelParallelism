#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Standard matrix multiplication
void matrixMultiplyStandard(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Optimized matrix multiplication with loop unrolling
void matrixMultiplyUnrolled(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; k += 4) { // Unrolling by a factor of 4
                C[i][j] += A[i][k] * B[k][j];
                if (k + 1 < n) C[i][j] += A[i][k + 1] * B[k + 1][j];
                if (k + 2 < n) C[i][j] += A[i][k + 2] * B[k + 2][j];
                if (k + 3 < n) C[i][j] += A[i][k + 3] * B[k + 3][j];
            }
        }
    }
}

int main() {
    int n = 512; // Example size, adjust based on your system's capabilities
    vector<vector<int>> A(n, vector<int>(n, 1)), B(n, vector<int>(n, 1)), C(n, vector<int>(n, 0));

    // Measure execution time for the standard approach
    auto start = high_resolution_clock::now();
    matrixMultiplyStandard(A, B, C);
    auto stop = high_resolution_clock::now();
    auto durationStandard = duration_cast<milliseconds>(stop - start);
    cout << "Standard Execution Time: " << durationStandard.count() << " milliseconds" << endl;

    // Reset matrix C
    for (int i = 0; i < n; ++i) {
        fill(C[i].begin(), C[i].end(), 0);
    }

    // Measure execution time for the unrolled approach
    start = high_resolution_clock::now();
    matrixMultiplyUnrolled(A, B, C);
    stop = high_resolution_clock::now();
    auto durationUnrolled = duration_cast<milliseconds>(stop - start);
    cout << "Unrolled Execution Time: " << durationUnrolled.count() << " milliseconds" << endl;

    return 0;
}
