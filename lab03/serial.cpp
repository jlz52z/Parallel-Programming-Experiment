#include <iostream>
#include <vector>

using namespace std;

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

int main() {
    // 示例输入
    vector<vector<double>> A = {{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}};
    vector<double> b = {8, -11, -3};

    // 调用高斯消元算法
    vector<double> x = Gaussian_Elimination(A, b);

    // 输出结果
    cout << "Solution:" << endl;
    for (int i = 0; i < x.size(); i++) {
        cout << "x[" << i + 1 << "] = " << x[i] << endl;
    }

    return 0;
}
