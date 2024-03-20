#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

using namespace std;

const double PI = acos(-1);

// Recursive FFT function
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

int main() {
    // Example usage of FFT
    vector<complex<double>> data = {1, 2, 3, 4, 5, 6, 7, 8};
    fft(data, false); // Forward FFT
    // Print FFT result
    for (auto& val : data) {
        cout << val << endl;
    }
    return 0;
}
