#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

void generateMatrix(vector<vector<double>>& A, int n) {
    srand(time(nullptr));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = rand() % 10 + 1;
        }
    }
}

vector<double> Gaussian_Elimination(vector<vector<double>>& A, int n) {
    vector<double> x(n);

    #pragma omp parallel for shared(A, n) default(none)
    for (int k = 0; k < n; k++) {
        int tid = omp_get_thread_num();
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            #pragma omp simd
            for (int j = k; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }

    #pragma omp parallel for shared(A, x, n) default(none)
    for (int i = n - 1; i >= 0; i--) {
        double sum = A[i][n];
        for (int j = i + 1; j < n; j++) {
            sum -= A[i][j] * x[j];
        }
        x[i] = sum / A[i][i];
    }

    return x;
}

int main() {
    omp_set_num_threads(3);
    int n = 2000;
    vector<vector<double>> A(n, vector<double>(n + 1));

    // Repeat the process 100 times
    int repetitions = 1;
    double total_time = 0.0;

    for (int rep = 0; rep < repetitions; ++rep) {
        generateMatrix(A, n);

        auto start_time = high_resolution_clock::now();

        vector<double> x = Gaussian_Elimination(A, n);

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        total_time += duration.count() / 1000.0; // Convert to milliseconds
    }

    // Calculate average time
    double average_time = total_time / repetitions;
    cout << "Average time for " << repetitions << " repetitions: " << average_time << " milliseconds" << endl;

    return 0;
}
