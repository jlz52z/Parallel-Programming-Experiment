#include <iostream>
#include <pthread.h>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <pthread.h> // For pthread_barrier_*
#include <chrono>   // For high_resolution_clock

#define NUM_THREADS 10 // 假设有4个线程
#define n 2000          // 假设n为10
#define REPEAT_TIMES 1 // 重复执行次数
using namespace std;
using namespace std::chrono;

struct threadParam_t {
    int t_id; // 线程id
};

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
pthread_barrier_t barrier_Divsion;
pthread_barrier_t barrier_Elimination;

void *threadFunc(void *param) {
    threadParam_t *p = (threadParam_t*)param;
    int t_id = p->t_id;

    for (int k = 0; k < n; ++k) {
        // t_id 为0 的线程做除法操作，其它工作线程先等待
        // 这里只采用了一个工作线程负责除法操作，同学们可以尝试采用多个工作线程完成除法操作
        if (t_id == 0) {
            for (int j = k+1; j < n; j++) {
                A[k][j] = A[k][j] / A[k][k];
            }
            A[k][k] = 1.0;
        }

        // 第一个同步点
        pthread_barrier_wait(&barrier_Divsion);

        // 循环划分任务
        for(int i=k+1+t_id; i < n; i += NUM_THREADS) {
            // 消去
            for (int j = k + 1; j < n; ++j) {
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            }
            A[i][k] = 0.0;
        }

        // 第二个同步点
        pthread_barrier_wait(&barrier_Elimination);
    }
    pthread_exit(NULL);
}

int main() {
    // 初始化随机数种子
    srand(time(NULL));

    // 初始化barrier
    pthread_barrier_init(&barrier_Divsion, NULL, NUM_THREADS);
    pthread_barrier_init(&barrier_Elimination, NULL, NUM_THREADS);

    // 用于存储执行时间的变量
    high_resolution_clock::time_point start, end;
    duration<double> cpu_time_used;




    // 计时开始
    start = high_resolution_clock::now();

    for (int i = 0; i < REPEAT_TIMES; ++i) {
        // 生成初始化矩阵
        generateMatrix();

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
    }

    // 计时结束
    end = high_resolution_clock::now();
    cpu_time_used = duration_cast<duration<double>>(end - start);

    // 销毁所有的barrier
    pthread_barrier_destroy(&barrier_Divsion);
    pthread_barrier_destroy(&barrier_Elimination);

    // 计算平均时间
    double average_time = cpu_time_used.count() / REPEAT_TIMES;
    cout << "Average execution time over " << REPEAT_TIMES << " loops: " << average_time << " seconds" << endl;

    return 0;
}
