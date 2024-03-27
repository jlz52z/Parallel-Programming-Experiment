#include <iostream>
#include <cmath>
#include <chrono>
const int N = 134217728; // 2^27
int a[N];
int sum;
int main()
{
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
    }
    int count = 20;
    auto start = std::chrono::high_resolution_clock::now();
    while (count--)
    {
        for (int m = N; m > 1; m /= 2)
        {
            for (int i = 0; i < m / 2; i += 2)
            { // 增加i的步长
                a[i] = a[i * 2] + a[i * 2 + 1];
                if (i + 1 < m / 2) // 确保不会超出范围
                    a[i + 1] = a[(i + 1) * 2] + a[(i + 1) * 2 + 1];
            }
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: "
              << duration.count() << " microseconds" << std::endl;
    return 0;
}