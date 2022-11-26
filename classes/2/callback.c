#include <stdio.h>

float f1(float x)
{
    return x * x;
}

float f2(float x)
{
    return -1.0 / x;
}

void printTable(float(f)(float))
{
    for (float x = -10; x < 10; x += 1)
        printf("%f: \t %f \n", x, f(x));
}

int main()
{
    // функции обратного вызова
    // callback functions

    printf("\n f1: \n");
    printTable(f1);
    printf("\n f2: \n");
    printTable(f2);
    return 0;
}
