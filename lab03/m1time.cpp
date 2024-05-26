#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <chrono>  // For timing

#define NUM_THREADS 10 // 假设有4个线程
#define n 2000          // 假设n为10
#define REPEAT_TIMES 1

using namespace std;
using namespace std::chrono;

struct threadParam_t {
    int t_id; // 线程id
};

// 信号量定义
sem_t sem_main;
sem_t sem_workerstart[NUM_THREADS]; // 每个线程有自己专属的信号量
sem_t sem_workerend[NUM_THREADS];

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
        sem_wait(&sem_workerstart[t_id]); // 阻塞，等待主线完成除法操作（操作自己专属的信号量）

        // 循环划分任务
        for (int i = k + 1 + t_id; i < n; i += NUM_THREADS) {
            // 消去
            for (int j = k + 1; j < n; ++j) {
                A[i][j] = A[i][j] - A[i][k] * A[k][j] / A[k][k];
            }
            A[i][k] = 0.0;
        }
        sem_post(&sem_main); // 唤醒主线程
        sem_wait(&sem_workerend[t_id]); // 阻塞，等待主线程唤醒进入下一轮
    }
    pthread_exit(NULL);
}

int main() {
    // 初始化随机数种子
    srand(time(NULL));

    double total_duration = 0.0;

    for (int repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        // 生成初始化矩阵
        generateMatrix();

        // 初始化信号量
        sem_init(&sem_main, 0, 0);
        for (int i = 0; i < NUM_THREADS; ++i) {
            sem_init(&sem_workerstart[i], 0, 0);
            sem_init(&sem_workerend[i], 0, 0);
        }

        // 创建线程
        pthread_t handles[NUM_THREADS];// 创建对应的Handle
        threadParam_t param[NUM_THREADS];// 创建对应的线程数据结构
        for(int t_id = 0; t_id < NUM_THREADS; t_id++){
            param[t_id].t_id = t_id;
            pthread_create(&handles[t_id], NULL, threadFunc, (void*)&param[t_id]);
        }

        auto start = high_resolution_clock::now(); // 计时开始

        for(int k = 0; k < n; ++k) {
            // 主线程做除法操作
            for (int j = k+1; j < n; j++) {
                A[k][j] = A[k][j] / A[k][k];
            }
            A[k][k] = 1.0;

            // 开始唤醒工作线程
            for (int t_id = 0; t_id < NUM_THREADS; ++t_id) {
                sem_post(&sem_workerstart[t_id]);
            }

            // 主线程睡眠（等待所有的工作线程完成此轮消去任务）
            for (int t_id = 0; t_id < NUM_THREADS; ++t_id) {
                sem_wait(&sem_main);
            }

            // 主线程再次唤醒工作线程进入下一轮次的消去任务
            for (int t_id = 0; t_id < NUM_THREADS; ++t_id) {
                sem_post(&sem_workerend[t_id]);
            }
        }

        for(int t_id = 0; t_id < NUM_THREADS; t_id++) {
            pthread_join(handles[t_id], NULL);
        }

        // 销毁所有信号量
        sem_destroy(&sem_main);
        for (int i = 0; i < NUM_THREADS; ++i) {
            sem_destroy(&sem_workerstart[i]);
            sem_destroy(&sem_workerend[i]);
        }

        auto stop = high_resolution_clock::now(); // 计时结束

        auto duration = duration_cast<milliseconds>(stop - start); // 计算时间间隔

        total_duration += duration.count(); // 累加计时结果
    }

    double avg_duration = total_duration / REPEAT_TIMES; // 计算平均消耗时间

    cout << "Average duration for " << REPEAT_TIMES << " repeats: " << avg_duration << " milliseconds" << endl;

    return 0;
}
