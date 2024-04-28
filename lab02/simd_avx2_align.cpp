#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <immintrin.h>
#include <cmath>
#define MATRIX_SIZE 512
const int trials = 500; // 迭代次数

float* matrix; // 修改为指针，以便使用对齐的内存分配

void m_reset(float* m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            m[i * MATRIX_SIZE + j] = 0; // 初始化所有元素为0
        }
        m[i * MATRIX_SIZE + i] = 1.0f; // 对角线元素设置为1
        for (int k = i + 1; k < MATRIX_SIZE; k++) {
            m[i * MATRIX_SIZE + k] = static_cast<float>(rand()) / RAND_MAX; // 随机初始化矩阵的上三角部分
        }
    }
}

void gauss_elimination_avx2(float* A, int n) {
    for (int k = 0; k < n; ++k) {
        if (fabs(A[k*n + k]) < 1e-8) { // 检查对角元素是否非常接近零
            // 找到一个合适的行来交换
            int maxRow = k;
            for (int i = k + 1; i < n; ++i) {
                if (fabs(A[i*n + k]) > fabs(A[maxRow*n + k])) {
                    maxRow = i;
                }
            }
            if (fabs(A[maxRow*n + k]) < 1e-8) {
                std::cerr << "Matrix is singular at row " << k << std::endl;
                continue; // 如果所有行都是零，则跳过这一列
            }
            // 进行行交换
            for (int j = 0; j < n; ++j) {
                std::swap(A[k*n + j], A[maxRow*n + j]);
            }
        }

        __m256 vt = _mm256_set1_ps(A[k*n + k]);
        int j;
        for (j = k + 1; j + 8 <= n; j += 8) {
            __m256 va = _mm256_loadu_ps(&A[k*n + j]);
            va = _mm256_div_ps(va, vt);
            _mm256_storeu_ps(&A[k*n + j], va);
        }
        for (; j < n; ++j) {
            A[k*n + j] /= A[k*n + k];
        }
        A[k*n + k] = 1.0;
        for (int i = k + 1; i < n; ++i) {
            __m256 vaik = _mm256_set1_ps(A[i*n + k]);
            for (j = k + 1; j + 8 <= n; j += 8) {
                __m256 vakj = _mm256_loadu_ps(&A[k*n + j]);
                __m256 vaij = _mm256_loadu_ps(&A[i*n + j]);
                __m256 vx = _mm256_mul_ps(vakj, vaik);
                vaij = _mm256_sub_ps(vaij, vx);
                _mm256_storeu_ps(&A[i*n + j], vaij);
            }
            for (; j < n; ++j) {
                A[i*n + j] -= A[k*n + j] * A[i*n + k];
            }
            A[i*n + k] = 0.0;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL))); // 初始化随机种子
    matrix = (float*)_aligned_malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(float), 32); // 对齐分配内存
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency); // 获取计时器频率
    LARGE_INTEGER start, end;
    double totalTime = 0;
    for (int t = 0; t < trials; t++) {
        m_reset(matrix); // 初始化矩阵
        QueryPerformanceCounter(&start); // 开始计时
        gauss_elimination_avx2(matrix, MATRIX_SIZE); // 执行高斯消元
        QueryPerformanceCounter(&end); // 结束计时
        totalTime += static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    }
    std::cout << "Average execution time: " << totalTime / trials << " seconds." << std::endl;
    _aligned_free(matrix); // 释放内存
    return 0;
}