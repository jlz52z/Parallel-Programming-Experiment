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
    for (int i = 0; i < N; i++)
    {
        sum += a[i];
    }
    return 0;
}