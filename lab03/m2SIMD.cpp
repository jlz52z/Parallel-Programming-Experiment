#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <immintrin.h> // For SSE intrinsics
#include <chrono> // For time measurement

#define NUM_THREADS 5 // 假设有4个线程
#define n 2000          // 假设n为1000

using namespace std;
using namespace std::chrono;

struct threadParam_t {
    int t_id; // 线程id
};

// 信号量定义
sem_t sem_leader;
sem_t sem_Divsion[NUM_THREADS-1];
sem_t sem_Elimination[NUM_THREADS-1];

// 矩阵定义
double A[n][n];

// 生成初始化矩阵
void generateMatrix() {
    // 生成上三角矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            A[i][j] = rand() % 10 + 1; // 生成1到10的随机数
        }
    }

    // 对行进行加减
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            A[i][j] -= A[i][i];
        }
    }
}

// 线程函数定义
void *threadFunc(void *param) {
    threadParam_t *p = (threadParam_t*)param;
    int t_id = p->t_id;

    for (int k = 0; k < n; ++k) {
        if (t_id == 0) {
            // 使用 SSE 指令进行除法操作
            __m128d divisor = _mm_set1_pd(A[k][k]);
            for (int j = k+1; j < n; j+=2) {
                __m128d dividend = _mm_loadu_pd(&A[k][j]);
                __m128d result = _mm_div_pd(dividend, divisor);
                _mm_storeu_pd(&A[k][j], result);
            }
            A[k][k] = 1.0;
        } else {
            sem_wait(&sem_Divsion[t_id-1]);
        }

        if (t_id == 0) {
            for (int i = 0; i < NUM_THREADS-1; ++i) {
                sem_post(&sem_Divsion[i]);
            }
        }

        for (int i = k + 1 + t_id; i < n; i += NUM_THREADS) {
            // 使用 SSE 指令进行消去操作
            __m128d factor = _mm_set1_pd(A[i][k]);
            for (int j = k + 1; j < n; j+=2) {
                __m128d temp1 = _mm_loadu_pd(&A[i][j]);
                __m128d temp2 = _mm_loadu_pd(&A[k][j]);
                __m128d product = _mm_mul_pd(factor, temp2);
                temp1 = _mm_sub_pd(temp1, product);
                _mm_storeu_pd(&A[i][j], temp1);
            }
            A[i][k] = 0.0;
        }

        if (t_id == 0) {
            for (int i = 0; i < NUM_THREADS-1; ++i) {
                sem_wait(&sem_leader);
            }

            for (int i = 0; i < NUM_THREADS-1; ++i) {
                sem_post(&sem_Elimination[i]);
            }
        } else {
            sem_post(&sem_leader);
            sem_wait(&sem_Elimination[t_id-1]);
        }
    }
    pthread_exit(NULL);
}

int main() {
    // 初始化随机数种子
    srand(time(NULL));

    // 记录执行时间
    duration<double> total_time(0);
    for (int i = 0; i < 1; ++i) {
        // 生成初始化矩阵
        generateMatrix();

        // 初始化信号量
        sem_init(&sem_leader, 0, 0);
        for (int i = 0; i < NUM_THREADS-1; ++i) {
            sem_init(&sem_Divsion[i], 0, 0);
            sem_init(&sem_Elimination[i], 0, 0);
        }

        auto start_time = high_resolution_clock::now();

        // 创建线程
        pthread_t handles[NUM_THREADS];// 创建对应的Handle
        threadParam_t param[NUM_THREADS];// 创建对应的线程数据结构
        for(int t_id = 0; t_id < NUM_THREADS; t_id++) {
            param[t_id].t_id = t_id;
            pthread_create(&handles[t_id], NULL, threadFunc, (void*)&param[t_id]);
        }

        for(int t_id = 0; t_id < NUM_THREADS; t_id++) {
            pthread_join(handles[t_id], NULL);
        }

        auto end_time = high_resolution_clock::now();
        duration<double> elapsed_time = end_time - start_time;
        total_time += elapsed_time;

        // 销毁所有信号量
        sem_destroy(&sem_leader);
        for (int i = 0; i < NUM_THREADS-1; ++i) {
            sem_destroy(&sem_Divsion[i]);
            sem_destroy(&sem_Elimination[i]);
        }
    }

    cout << "Average execution time over 100 runs: " << total_time.count() / 1 << " seconds" << endl;

    return 0;
}
