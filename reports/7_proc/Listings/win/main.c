#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <Windows.h>

/*
В файле записан ряд целых чисел, разделённых пробелом. Программа должна считать имя файла из первого
аргумента командной строки и рассчитать сумму квадратов записанных в файл чисел. Для расчёта суммы квадратов
программа должна создать N дочерних процессов (N передаётся вторым аргументом командной строки) и передать
каждому из них часть полученных чисел. Каждый из дочерних процессов должен рассчитать сумму квадратов
переданных ему чисел и вернуть её родительскому. Родительский процесс должен просуммировать полученные от
дочерних числа и вывести на консоль итоговую сумму. Если исходный файл не существует, или в нём записано менее
2 чисел, следует вывести соответствующее сообщение для пользователя и завершить работу программы.
*/

typedef int pid_t;

/**
 * Prints help message to console
 */
int print_help()
{
    printf("\nUsage: main <path> <N> \n");
    printf("\tpath - file to read \n");
    printf("\tN - fork number \n");
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
    printf("Fork number: %d \n", N);

    // open file to read
    FILE *input_file = fopen(input_path, "r");
    if (!input_file)
        return catch_error();

    // get number of input digits M
    char t[100];
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

    // if too many forks
    if (N > M / 2)
    {
        N = M / 2;
        printf("Too many forks. New fork number: %d \n", N);
    }

    // calculate division between processes
    int n = M / N;
    int n_last = n + M % N;

    printf("Each proc gets: %d numbers \n", n);
    printf("Last proc gets: %d numbers \n", n_last);

    // move to file start
    if (fseek(input_file, 0, SEEK_SET))
        return catch_error();

    // create files for each process and fork
    HANDLE *hProcesses = malloc(sizeof(HANDLE) * N);
    DWORD *dwProcessIds = malloc(sizeof(DWORD) * N);
    for (int i = 0; i < N; i++)
    {
        // construct name of output file
        char output_path[30];
        if (sprintf(output_path, "_%d_input.txt", i) < 0)
            return catch_error();

        // open file to write
        FILE *output_file = fopen(output_path, "w");
        if (!output_file)
            return catch_error();

        // write numbers to output file
        int n_effective = (i == N - 1) ? n_last : n;
        for (int j = 0; j < n_effective; j++)
        {
            if (!fscanf(input_file, " %12s", t))
                return catch_error();
            if (fprintf(output_file, "%s ", t) < 0)
                return catch_error();
        }

        // release file
        if (fclose(output_file) != 0)
            return catch_error();

        // start process
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        GetStartupInfo(&si);
        if (sprintf(t, "calc %s %d", output_path, n_effective) < 0)
            return catch_error();
        BOOL res = CreateProcess("calc", t, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

        if (!res)
            return catch_error();

        printf("Forked proc %d with output_path %s \n", pi.dwProcessId, output_path);
        hProcesses[i] = pi.hProcess;
        dwProcessIds[i] = pi.dwProcessId;
    }

    // release input file
    if (fclose(input_file) != 0)
        return catch_error();

    // stop process to see zombies
    getchar();

    // calc results
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        int code = 0;
        DWORD dwRes = WaitForSingleObject(hProcesses[i], INFINITE);
        if (dwRes == WAIT_FAILED)
            return catch_error();

        // construct name of output file
        if (sprintf(t, "_%d_result.txt", dwProcessIds[i]) < 0)
            return catch_error();

        // open result file to read
        FILE *result_file = fopen(t, "r");
        if (!result_file)
            return catch_error();

        // read result from file
        if (!fscanf(result_file, "%12s", t))
            return catch_error();

        // release input file
        if (fclose(result_file) != 0)
            return catch_error();

        // calculate
        sum += atoi(t);
    }

    free(hProcesses);
    free(dwProcessIds);
    printf("\nFinal Result: %d \n", sum);
    return 0;
}
