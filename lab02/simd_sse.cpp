// #include <iostream>
// #include <immintrin.h>
// #include <windows.h>
// #include <cstdlib>
// #include <ctime>

// #define MATRIX_SIZE 512
// const int trials = 100; // 迭代次数
// float matrix[MATRIX_SIZE][MATRIX_SIZE];
// void m_reset(float m[MATRIX_SIZE][MATRIX_SIZE]) {
//     for (int i = 0; i < MATRIX_SIZE; ++i) {
//         for (int j = 0; j < MATRIX_SIZE; ++j) {
//             m[i][j] = (i == j) ? 1.0f : static_cast<float>(rand()) / RAND_MAX;
//         }
//     }
// }

// void gauss_elimination_sse(float* A, int n) {
//     for (int k = 0; k < n; ++k) {
//         __m128 vt = _mm_set1_ps(A[k*n + k]);
//         int j;
//         for (j = k + 1; j + 4 <= n; j += 4) {
//             __m128 va = _mm_loadu_ps(&A[k*n + j]);
//             va = _mm_div_ps(va, vt);
//             _mm_storeu_ps(&A[k*n + j], va);
//         }
//         for (; j < n; ++j) {
//             A[k*n + j] /= A[k*n + k];
//         }
//         A[k*n + k] = 1.0;

//         for (int i = k + 1; i < n; ++i) {
//             __m128 vaik = _mm_set1_ps(A[i*n + k]);
//             for (j = k + 1; j + 4 <= n; j += 4) {
//                 __m128 vakj = _mm_loadu_ps(&A[k*n + j]);
//                 __m128 vaij = _mm_loadu_ps(&A[i*n + j]);
//                 __m128 vx = _mm_mul_ps(vakj, vaik);
//                 vaij = _mm_sub_ps(vaij, vx);
//                 _mm_storeu_ps(&A[i*n + j], vaij);
//             }
//             for (; j < n; ++j) {
//                 A[i*n + j] -= A[k*n + j] * A[i*n + k];
//             }
//             A[i*n + k] = 0.0;
//         }
//     }
// }

// int main() {

//     srand(static_cast<unsigned int>(time(NULL))); // 初始化随机种子

//     LARGE_INTEGER frequency, start, end;
//     QueryPerformanceFrequency(&frequency);


//     double averageTime = 0;

//     for (int t = 0; t < trials; ++t) {
//         m_reset(matrix); // 初始化矩阵
//         QueryPerformanceCounter(&start);
//         gauss_elimination_sse(&matrix[0][0], MATRIX_SIZE);
//         QueryPerformanceCounter(&end);
//         averageTime += static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
//     }

//     averageTime /= trials;
//     std::cout << "Average Time: " << averageTime << " seconds." << std::endl;

//     return 0;
// }