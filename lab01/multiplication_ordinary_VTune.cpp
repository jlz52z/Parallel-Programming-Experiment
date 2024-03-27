#include <iostream>
const int N = 1000;  //200, 1000, 2000, 4000
int sum[N];
int a[N],b[N][N];

int main()
{
    for(int i = 0; i < N; i++)
    {
        a[i] = i;
        sum[i] = 0.0;
        for(int j = 0; j < N; j++)
        {
            b[i][j] = i + j;
        }
    }

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            sum[i] += b[j][i]*a[j];
        }
    }
    return 0;
    
}