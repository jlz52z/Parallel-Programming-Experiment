#include <iostream>
#include <chrono>
const int N = 134217728;
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
while(count--)
    for (int i = 0; i < N; i++)
    {
        sum += a[i];
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: "
              << duration.count() << " microseconds" << std::endl;
    return 0;
}