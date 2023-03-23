/*
В файле записан ряд целых чисел, разделённых пробелом. Программа должна считать имя файла из первого
аргумента командной строки и рассчитать сумму квадратов записанных в файл чисел. Для расчёта суммы квадратов
программа должна создать N дочерних потоков (N передаётся вторым аргументом командной строки) и передать
каждому из них часть полученных чисел. Каждый из дочерних потоков должен рассчитать сумму квадратов
переданных ему чисел и вернуть её родительскому. Родительский поток должен просуммировать полученные от
дочерних числа и вывести на консоль итоговую сумму. Если исходный файл не существует, или в нём записано
менее 2 чисел, следует вывести соответствующее сообщение для пользователя и завершить работу программы.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <Windows.h>

CRITICAL_SECTION cs;
int globalResult = 0;

/**
 * Prints help message to console
 */
int print_help()
{
    printf("\nUsage: main <path> <N> \n");
    printf("\tpath - file to read \n");
    printf("\tN - thread number \n");
    return 1;
}

/**
 * Prints error message and help message to console
 * and closes the program
 */
int catch_error()
{
    LPSTR message;
    DWORD dwMessageLen = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&message, 0, NULL);
    printf(message);
    print_help();
    return 1;
}

DWORD WINAPI calc(void *arg)
{
    int total = 0;
    int *args = (int *)arg;
    int threadNumber = args[0];
    int numberCount = args[1];
    printf("\t [Thread %d] started \n", threadNumber);
    printf("\t [Thread %d] numberCount: %d \n", threadNumber, numberCount);

    for (int i = 2; i < numberCount + 2; i++)
    {
        // calc sum of squares
        int number = args[i];
        total += number * number;
    }

    printf("\t [Thread %d] calculated result: %d \n", threadNumber, total);

    // update global result
    EnterCriticalSection(&cs);
    globalResult += total;
    printf("\t [Thread %d] new globalResult: %d \n", threadNumber, globalResult);
    LeaveCriticalSection(&cs);

    // Sleep(50000);
    printf("\t [Thread %d] finished \n", threadNumber);
    ExitThread(0);
}

int main(int argc, char **argv)
{
    // check number of arguments
    if (argc != 3)
        return print_help();

    // get amount of numbers and file path
    char *input_path = argv[1];
    int N = atoi(argv[2]);

    // print input values
    printf("Path to file: %s \n", input_path);
    printf("Thread number: %d \n", N);

    // open file to read
    FILE *input_file = fopen(input_path, "r");
    if (!input_file)
        return catch_error();

    // get number of input digits M
    char t[12];
    int M = 0; // input size
    while (fscanf(input_file, " %12s", t) == 1)
        M++;

    printf("Input size: %d \n", M);

    // if too few numbers
    if (M < 2)
    {
        printf("Too few numbers (M must be greater than 2) \n");
        return print_help();
    }

    // if too many threads
    if (N > M / 2)
    {
        N = M / 2;
        printf("Too many threads. New thread number: %d \n", N);
    }

    // calculate division between threads
    int n = M / N;
    int n_last = n + M % N;

    printf("Each thread gets: %d numbers \n", n);
    printf("Last thread gets: %d numbers \n", n_last);

    // move to file start
    if (fseek(input_file, 0, SEEK_SET))
        return catch_error();

    // init thread arrays
    InitializeCriticalSection(&cs);
    HANDLE * threadArray = malloc(sizeof(HANDLE) * N);
    DWORD *threadIds = malloc(sizeof(HANDLE) * N);
    int** argArray = (int**)malloc(N * sizeof(int*));
    // int *argArray[N];
    for (int i = 0; i < N - 1; i++)
        argArray[i] = (int *)malloc(sizeof(int) * (n + 2));
    argArray[N - 1] = (int *)malloc(sizeof(int) * (n_last + 2));

    // create array for each process and thread
    for (int i = 0; i < N; i++)
    {
        int n_effective = (i == N - 1) ? n_last : n; // calc length of output array
        argArray[i][0] = i;                          // save thread number
        argArray[i][1] = n_effective;                // save n_effective
        for (int j = 2; j < n_effective + 2; j++)
        {
            if (!fscanf(input_file, " %12s", t))
                return catch_error();
            argArray[i][j] = atoi(t); // write number to array
        }

        // create thread
        HANDLE thread = CreateThread(NULL, 0, calc, argArray[i], CREATE_SUSPENDED, &threadIds[i]);
        if (thread == NULL)
        {
            printf("Error: %d\n", GetLastError());
            exit(5);
        }
        if (ResumeThread(thread) == -1)
        {
            printf("Error: %d\n", GetLastError());
            exit(5);
        }
        threadArray[i] = thread;
    }

    for (int i = 0; i < N; i++)
        WaitForSingleObject(threadArray[i], INFINITE);

    // release input file
    if (fclose(input_file) != 0)
        return catch_error();

    // release memory
    free(threadArray);
    for (int i = 0; i < N; i++)
        free(argArray[i]);

    printf("\nFinal Result: %d \n", globalResult);
    return 0;
}
