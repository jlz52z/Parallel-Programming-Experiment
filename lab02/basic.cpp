// #include <iostream>
// #include <cstdlib>
// #include <ctime>
// #include <Windows.h>

// using namespace std;

// const int MATRIX_SIZE = 512; // 示例矩阵的大小
// const int NUM_ITERATIONS = 100; // 计算次数
// float A[MATRIX_SIZE][MATRIX_SIZE];
// float b[MATRIX_SIZE]={0.0};
// float x[MATRIX_SIZE];
// void m_reset(float m[MATRIX_SIZE][MATRIX_SIZE]) {
//     for (int i = 0; i < MATRIX_SIZE; i++) {
//         for (int j = 0; j < MATRIX_SIZE; j++) {
//             m[i][j] = 0;
//             m[i][i] = 1.0f;
//             for (int k = i + 1; k < MATRIX_SIZE; k++) {
//                 m[i][k] = static_cast<float>(rand()) / RAND_MAX; // 使用单精度浮点数随机初始化
//             }
//         }
//     }
// }

// void Gaussian_Elimination(float A[MATRIX_SIZE][MATRIX_SIZE], float b[MATRIX_SIZE], int n, float x[MATRIX_SIZE]) {
//     // 消去过程
//     for (int k = 0; k < n; ++k) {
//         for (int i = k + 1; i < n; ++i) {
//             float factor = A[i][k] / A[k][k];
//             for (int j = k + 1; j < n; ++j) {
//                 A[i][j] -= factor * A[k][j];
//             }
//             b[i] -= factor * b[k];
//         }
//     }

// }

// int main() {
//     // 示例矩阵 A 和向量 b


//     // 生成示例矩阵 A
//     srand(time(NULL)); // 使用当前时间作为随机种子


//     // 获取矩阵 A 的大小
//     int n = MATRIX_SIZE;

//     // 计时器变量
//     LARGE_INTEGER frequency, start, end;

//     // 获取计时器频率
//     QueryPerformanceFrequency(&frequency);

//     double total_elapsed_time = 0.0;

//     for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
//         m_reset(A);
//         // 开始计时
//         QueryPerformanceCounter(&start);

//         // 调用高斯消元算法求解线性方程组
//         Gaussian_Elimination(A, b, n, x);
        
//         // 结束计时
//         QueryPerformanceCounter(&end);

//         // 计算程序执行时间
//         double elapsed_time = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;

//         total_elapsed_time += elapsed_time;
//     }

//     // 计算平均每次执行时间
//     double average_elapsed_time = total_elapsed_time / NUM_ITERATIONS;

//     // 输出平均每次执行时间
//     cout << "Average elapsed time per iteration: " << average_elapsed_time << " seconds" << endl;

//     return 0;
// }
