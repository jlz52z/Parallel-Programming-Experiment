#include <iostream>
#include <cstdlib>
#include <ctime>
#include <arm_neon.h>
#include <time.h>

#define MATRIX_SIZE 512
const int trials = 100; // 迭代次数

float matrix[MATRIX_SIZE][MATRIX_SIZE];

void m_reset(float m[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            m[i][j] = static_cast<float>(rand()) / RAND_MAX;
        }
        m[i][i] += 10; // 保证对角线占优，防止数值不稳定
    }
}

void gauss_elimination_neon(float* A, int n) {
    for (int k = 0; k < n; ++k) {
        float32x4_t vt = vdupq_n_f32(A[k*n + k]);
        int j;
        for (j = k + 1; j + 4 <= n; j += 4) {
            float32x4_t va = vld1q_f32(&A[k*n + j]);
            va = vdivq_f32(va, vt);
            vst1q_f32(&A[k*n + j], va);
        }
        for (; j < n; ++j) {
            A[k*n + j] /= A[k*n + k];
        }
        A[k*n + k] = 1.0;
        for (int i = k + 1; i < n; ++i) {
            float32x4_t vaik = vdupq_n_f32(A[i*n + k]);
            for (j = k + 1; j + 4 <= n; j += 4) {
                float32x4_t vakj = vld1q_f32(&A[k*n + j]);
                float32x4_t vaij = vld1q_f32(&A[i*n + j]);
                float32x4_t vx = vmulq_f32(vakj, vaik);
                vaij = vsubq_f32(vaij, vx);
                vst1q_f32(&A[i*n + j], vaij);
            }
            for (; j < n; ++j) {
                A[i*n + j] -= A[k*n + j] * A[i*n + k];
            }
            A[i*n + k] = 0.0;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    struct timespec start, end;
    double totalTime = 0;

    for (int t = 0; t < trials; t++) {
        m_reset(matrix);
        clock_gettime(CLOCK_MONOTONIC, &start);
        gauss_elimination_neon(&matrix[0][0], MATRIX_SIZE);
        clock_gettime(CLOCK_MONOTONIC, &end);
        totalTime += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    }

    std::cout << "Average execution time: " << totalTime / trials << " seconds." << std::endl;
    return 0;
}