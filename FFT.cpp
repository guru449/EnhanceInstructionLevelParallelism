#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

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
    // Example usage of FFT
    vector<complex<double>> data = {1, 2, 3, 4, 5, 6, 7, 8};
    
    cout << "Original FFT:" << endl;
    vector<complex<double>> dataCopy = data; // Copy data to keep original intact
    fft(dataCopy, false); // Forward FFT
    for (auto& val : dataCopy) {
        cout << val << endl;
    }

    cout << "\nFFT with Loop Unrolling:" << endl;
    fftUnrolled(data, false); // Forward FFT with loop unrolling
    for (auto& val : data) {
        cout << val << endl;
    }

    return 0;
}
