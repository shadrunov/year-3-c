#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/*
Написать программу, в которой создается одномерный числовой массив.
После заполнения значениями (случайными числами) массива в нем нужно
выполнить циклическую перестановку элементов. Количество позиций
для циклической перестановки вводится пользователем с клавиатуры.
*/

extern void f(int32_t *array, int32_t n, int32_t shift);

int main()
{
    srand(time(NULL));

    int32_t n, shift;
    printf("Enter array length (n > 1): ");
    scanf("%d", &n);

    printf("Enter shift (0 < shift < n): ");
    scanf("%d", &shift);

    int32_t *array = malloc(n * sizeof(int32_t));

    if (array)
    {
        printf("Array before: \n");
        for (int i = 0; i < n; ++i)
        {
            // populate array
            array[i] = rand() % 100;
            printf("%d ", array[i]);
        }

        f(array, n, shift);

        // print array
        printf("\nArray after: \n");
        for (int i = 0; i < n; ++i)
            printf("%d ", array[i]);
    }

    free(array);
    return 0;
}
