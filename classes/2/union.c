/******************************************************************************

struct
*******************************************************************************/

#include <stdio.h>


struct JournalEntry
{
    char name [128];
    unsigned short marks [10];
    // int marks [10];
};  // mandatory

union U
{
    int x;
    char arr[4];
    short sarr[2];
};

union U1
{
    int x;
    char arr[sizeof(int) / sizeof(char)];
    short sarr[sizeof(int) / sizeof(short)];
};


int main()
{
    printf("Hello World \n");

    int x;
    struct JournalEntry ent;
    scanf("%s", ent.name);
    // gets(ent.name);


    for (int i = 0; i < 10; i++)
    {
        scanf("%d", &ent.marks[i]);
    }
    
    for (int i = 0; i < 10; i++)
    {
        printf("%d  ", ent.marks[i]);
    }


    // функции обратного вызова
    // callback functions


    return 0;
}

