#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern void f(int32_t *array, int32_t n, int32_t shift);

int main()
{
    srand(time(NULL));

    // user input
    int32_t n, shift;

    printf("Enter array length (n > 1): ");
    scanf("%d", &n);

    printf("Enter shift (0 < shift < n): ");
    scanf("%d", &shift);
    
    // memory allocation for array
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
