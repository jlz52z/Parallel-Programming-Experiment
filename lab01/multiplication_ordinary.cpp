#include <iostream>
#include <chrono>
const int N = 4000; //200, 1000, 2000, 4000
int sum[N];
int a[N], b[N][N];

int main()
{
    int count = 10000;

    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        for (int j = 0; j < N; j++)
        {
            b[i][j] = i + j;
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    while (count--)
    {
        for (int i = 0; i < N; i++)
        {
            sum[i] = 0.0;
            for (int j = 0; j < N; j++)
            {
                sum[i] += b[j][i] * a[j];
            }
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: "
              << duration.count() << " microseconds" << std::endl;
    return 0;
}