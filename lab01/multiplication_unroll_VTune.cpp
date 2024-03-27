#include <iostream>
#include <chrono>
const int N = 4000; //200, 1000, 2000, 4000
int sum[N];
int a[N], b[N][N];

int main()
{

    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        for (int j = 0; j < N; j++)
        {
            b[i][j] = i + j;
        }
    }
    for (int i = 0; i < N; i++)
    {
        sum[i] = 0.0;
    }

    for (int j = 0; j < N; j += 4)
    { // 外层循环每次增加10
        for (int i = 0; i < N; i++)
        {
            // 对内层循环进行展开，每次处理10个迭代
            sum[i] += b[j][i] * a[j];
            sum[i] += b[j + 1][i] * a[j + 1];
            sum[i] += b[j + 2][i] * a[j + 2];
            sum[i] += b[j + 3][i] * a[j + 3];
        }
    }
    return 0;
}
