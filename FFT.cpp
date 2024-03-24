#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

const double PI = acos(-1);

void fft(vector<complex<double>>& a, bool invert) {
    int n = a.size();
    if (n == 1) return;

    vector<complex<double>> a0(n / 2), a1(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }

    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    complex<double> w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i < n / 2; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

void fftUnrolled(vector<complex<double>>& a, bool invert) {
    int n = a.size();
    if (n == 1) return;

    vector<complex<double>> a0(n / 2), a1(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }

    fftUnrolled(a0, invert);
    fftUnrolled(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    complex<double> w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i < n / 2; i += 2) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        complex<double> w_next = w * wn; // Pre-calculate next w
        if (i + 1 < n / 2) {
            a[i + 1] = a0[i + 1] + w_next * a1[i + 1];
            a[i + 1 + n / 2] = a0[i + 1] - w_next * a1[i + 1];
        }
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
            if (i + 1 < n / 2) {
                a[i + 1] /= 2;
                a[i + 1 + n / 2] /= 2;
            }
        }
        w = w_next * wn; // Update w for the next iteration of the loop
    }
}

int main() {
    // Larger example usage of FFT for performance measurement
    int N = 1 << 12; // Example size, e.g., 4096
    vector<complex<double>> data(N);
    for (int i = 0; i < N; ++i) {
        data[i] = rand() / (RAND_MAX + 1.0);
    }

    // Measure execution time for the original FFT
    auto start = std::chrono::high_resolution_clock::now();
    fft(data, false); // Forward FFT
    auto finish = std::chrono::high_resolution_clock::now();
    auto durationOriginal = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    cout << "Original FFT Execution Time: " << durationOriginal << " microseconds" << endl;

    // Reset data for fairness in measurement
    for (int i = 0; i < N; ++i) {
        data[i] = rand() / (RAND_MAX + 1.0);
    }

    // Measure execution time for the FFT with loop unrolling
    start = std::chrono::high_resolution_clock::now();
    fftUnrolled(data, false); // Forward FFT with loop unrolling
    finish = std::chrono::high_resolution_clock::now();
    auto durationUnrolled = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    cout << "FFT with Loop Unrolling Execution Time: " << durationUnrolled << " microseconds" << endl;

    return 0;
}
