#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <chrono>  // For timing

#define NUM_THREADS 10 // 假设有4个线程
#define n 100          // 假设n为10
#define REPEAT_TIMES 100 // 重复执行次数

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
        // t_id 为0 的线程做除法操作，其它工作线程先等待
        // 这里只采用了一个工作线程负责除法操作，同学们可以尝试采用多个工作线程完成除法操作
        // 比信号量更简洁的同步方式是使用barrier
        if (t_id == 0) {
            for (int j = k+1; j < n; j++) {
                A[k][j] = A[k][j] / A[k][k];
            }
            A[k][k] = 1.0;
        } else {
            sem_wait(&sem_Divsion[t_id-1]); // 阻塞，等待完成除法操作
        }

        // t_id 为0 的线程唤醒其它工作线程，进行消去操作
        if (t_id == 0) {
            for (int i = 0; i < NUM_THREADS-1; ++i) {
                sem_post(&sem_Divsion[i]);
            }
        }

        // 循环划分任务
        for (int i = k + 1 + t_id; i < n; i += NUM_THREADS) {
            // 消去
            for (int j = k + 1; j < n; ++j) {
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            }
            A[i][k] = 0.0;
        }

        if (t_id == 0) {
            for (int i = 0; i < NUM_THREADS-1; ++i) {
                sem_wait(&sem_leader); // 等待其它worker 完成消去
            }

            for (int i = 0; i < NUM_THREADS-1; ++i) {
                sem_post(&sem_Elimination[i]); // 通知其它worker 进入下一轮
            }
        } else {
            sem_post(&sem_leader); // 通知leader, 已完成消去任务
            sem_wait(&sem_Elimination[t_id-1]); // 等待通知，进入下一轮
        }
    }
    pthread_exit(NULL);
}

int main() {
    // 初始化随机数种子
    srand(time(NULL));

    // 计时开始
    high_resolution_clock::time_point start = high_resolution_clock::now();

    for (int repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        // 生成初始化矩阵
        generateMatrix();

        // 初始化信号量
        sem_init(&sem_leader, 0, 0);
        for (int i = 0; i < NUM_THREADS-1; ++i) {
            sem_init(&sem_Divsion[i], 0, 0);
            sem_init(&sem_Elimination[i], 0, 0);
        }

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

        // 销毁所有信号量
        sem_destroy(&sem_leader);
        for (int i = 0; i < NUM_THREADS-1; ++i) {
            sem_destroy(&sem_Divsion[i]);
            sem_destroy(&sem_Elimination[i]);
        }
    }

    // 计时结束
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(end - start);
    double average_time = time_span.count() / REPEAT_TIMES;

    cout << "Average time taken for " << REPEAT_TIMES << " repetitions: " << average_time << " seconds" << endl;

    return 0;
}
