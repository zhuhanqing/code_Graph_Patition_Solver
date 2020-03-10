#include <cstdio>
#include <utility>
#include"Vector.h"


int main()
{
    int n = 10;
    //Vector<int. double> vec;
    Vector<int, double> vec(n);
    for (int i = 0; i < n; ++i)
    {
        vec[i] = 0.1 * i;
    }
    for (int i = 0; i < n; ++i)
    {
        printf(" %lf", vec[i]);
    }
    printf("\n");
    
    Vector<int, double> vec2(vec);
    for (int i = 0; i < n/2; ++i)
    {
        std::swap(vec2[i], vec2[n - 1 - i]);
    }
    for (int i = 0; i < n; ++i)
    {
        printf(" %lf", vec2[i]);
    }
    printf("\n");

    return 0;
}


