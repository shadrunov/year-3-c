/******************************************************************************
#####
Day 1
#####

int 4
char 1
bool 1 
float 4 
double 8 
long 4 - 8 (lin)
* 4 (x86) 8 (x64) 


*******************************************************************************/

#include <stdio.h>

int main()
{
    printf("Hello World \n\n");
    
    printf("pointers \n");
    int x = 10; 
    int * px = &x;
    int * * ppx = &px;
    
    printf("%p \n", px);
    printf("%p \n", ppx);
    printf("%d \n", ppx);


    printf("\narray \n");
    char arr[3] = {'a', 'b', 'c'};
    printf("%p \n", arr);

    char c = arr[2];
    // &c = &arr + 2 * sizeof(char)
    // 64 + 2 * 1 = 66

    // arr is a pointer to first element ('a')

    // ptr: ++, --, -
    // ptr + ptr not defined
    // ptr +/- int(long) defined

    printf("%c \n", *arr);          // a
    printf("%c \n", *arr + 1);      // b
    printf("%c \n", *(arr + 1));    // b
    printf("%c \n", 'a' + 1);       // b
    // arr + 1 is arr + 1 * sizeof(type of elements in arr)
    printf("%d \n", sizeof(char));
    printf("%d \n", sizeof(arr));


    int * py;
    int y = 5;
    py = &y;
    printf("%p \n", py + 1);

    // 2d arrays -- arrays of pointers to arrays

    printf("%c \n", arr[5]);

    // strings are arrays
    char str[4] = "abc";
    str[3];  // 0
    // memcpy(src, dst, n);
    // strlen(str);  // strings know their length because of 00 terminator in the end
    // segmentation fault

    printf("%s \n", str);
    str[1] = 0;
    printf("%s \n", str);

    // it was stack -- where all common vars are stored
    // but its size is limited
    // heap (free storage) is unlimited 

    // int * pz = new int;  // pointer itself is stored in stack
    // https://stackoverflow.com/a/2839555/20186980



    void f(int * px) {
        *px = 0;
    }

    int q = 5;
    printf("%d \n", q);

    f(&q);
    printf("%d \n", q);

    return 0;
}

