#include <iostream>
#include <vector>
#include <chrono> // For high_resolution_clock

using namespace std;
using namespace std::chrono;
#define num 2000          // 假设n为10
#define REPEAT_TIMES 1 // 重复执行次数
vector<double> Gaussian_Elimination(vector<vector<double>>& A, vector<double>& b) {
    int n = A.size();

    // 消去过程
    for (int k = 0; k < n; k++) {
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k + 1; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
    }

    // 回代过程
    vector<double> x(n);
    x[n - 1] = b[n - 1] / A[n - 1][n - 1];
    for (int i = n - 2; i >= 0; i--) {
        double sum = b[i];
        for (int j = i + 1; j < n; j++) {
            sum -= A[i][j] * x[j];
        }
        x[i] = sum / A[i][i];
    }

    return x;
}

void generateMatrix(vector<vector<double>>& A, int n) {
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

int main() {


    // 初始化随机数种子
    srand(time(NULL));

    vector<vector<double>> A(num, vector<double>(num));
    vector<double> b(num);

    double total_time = 0.0;

    for (int iter = 0; iter < REPEAT_TIMES; ++iter) {
        // 生成初始化矩阵
        generateMatrix(A, num);

        // 随机生成b向量
        for (int i = 0; i < num; ++i) {
            b[i] = rand() % 10 + 1;
        }

        // 计算开始时间
        high_resolution_clock::time_point start = high_resolution_clock::now();

        // 调用高斯消元算法
        vector<double> x = Gaussian_Elimination(A, b);

        // 计算结束时间
        high_resolution_clock::time_point end = high_resolution_clock::now();

        // 计算消耗时间并累加
        total_time += duration_cast<duration<double>>(end - start).count();
    }

    // 计算平均消耗时间
    double average_time = total_time / REPEAT_TIMES;

    // 输出结果
    cout << "Average time for " << REPEAT_TIMES << " iterations: " << average_time << " seconds." << endl;

    return 0;
}
