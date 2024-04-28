// #include <windows.h>
// #include <iostream>
// #include <cstdlib>
// #include <ctime>
// #include <immintrin.h>

// #define MATRIX_SIZE 512
// const int trials = 100; // 迭代次数
// float matrix[MATRIX_SIZE][MATRIX_SIZE];

// void m_reset(float m[MATRIX_SIZE][MATRIX_SIZE])
// {
//     for (int i = 0; i < MATRIX_SIZE; i++)
//     {
//         for (int j = 0; j < MATRIX_SIZE; j++)
//         {
//             m[i][j] = 0; // 初始化所有元素为0
//         }
//         m[i][i] = 1.0f; // 对角线元素设置为1
//         for (int k = i + 1; k < MATRIX_SIZE; k++)
//         {
//             m[i][k] = static_cast<float>(rand()) / RAND_MAX; // 随机初始化矩阵的上三角部分
//         }
//     }
// }

// void gauss_elimination_avx2(float *A, int n)
// {
//     for (int k = 0; k < n; ++k)
//     {
//         __m256 vt = _mm256_set1_ps(A[k * n + k]);
//         int j;
//         for (j = k + 1; j + 8 <= n; j += 8)
//         {
//             __m256 va = _mm256_loadu_ps(&A[k * n + j]);
//             va = _mm256_div_ps(va, vt);
//             _mm256_storeu_ps(&A[k * n + j], va);
//         }
//         for (; j < n; ++j)
//         {
//             A[k * n + j] /= A[k * n + k];
//         }
//         A[k * n + k] = 1.0;
//         for (int i = k + 1; i < n; ++i)
//         {
//             __m256 vaik = _mm256_set1_ps(A[i * n + k]);
//             for (j = k + 1; j + 8 <= n; j += 8)
//             {
//                 __m256 vakj = _mm256_loadu_ps(&A[k * n + j]);
//                 __m256 vaij = _mm256_loadu_ps(&A[i * n + j]);
//                 __m256 vx = _mm256_mul_ps(vakj, vaik);
//                 vaij = _mm256_sub_ps(vaij, vx);
//                 _mm256_storeu_ps(&A[i * n + j], vaij);
//             }
//             for (; j < n; ++j)
//             {
//                 A[i * n + j] -= A[k * n + j] * A[i * n + k];
//             }
//             A[i * n + k] = 0.0;
//         }
//     }
// }

// int main()
// {

//     srand(static_cast<unsigned int>(time(NULL))); // 初始化随机种子

//     LARGE_INTEGER frequency;
//     QueryPerformanceFrequency(&frequency); // 获取计时器频率
//     LARGE_INTEGER start, end;

//     double totalTime = 0;

//     for (int t = 0; t < trials; t++)
//     {
//         m_reset(matrix);                 // 初始化矩阵
//         QueryPerformanceCounter(&start); // 开始计时

//         gauss_elimination_avx2(&matrix[0][0], MATRIX_SIZE); // 执行高斯消元

//         QueryPerformanceCounter(&end); // 结束计时
//         totalTime += static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
//     }

//     std::cout << "Average execution time: " << totalTime / trials << " seconds." << std::endl;

//     return 0;
// }