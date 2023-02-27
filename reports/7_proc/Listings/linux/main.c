#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

/*
В файле записан ряд целых чисел, разделённых пробелом. Программа должна считать имя файла из первого
аргумента командной строки и рассчитать сумму квадратов записанных в файл чисел. Для расчёта суммы квадратов
программа должна создать N дочерних процессов (N передаётся вторым аргументом командной строки) и передать
каждому из них часть полученных чисел. Каждый из дочерних процессов должен рассчитать сумму квадратов
переданных ему чисел и вернуть её родительскому. Родительский процесс должен просуммировать полученные от
дочерних числа и вывести на консоль итоговую сумму. Если исходный файл не существует, или в нём записано менее
2 чисел, следует вывести соответствующее сообщение для пользователя и завершить работу программы.
*/

/**
 * Prints help message to console
 */
int print_help()
{
    printf("\nUsage: main <path> <N> \n");
    printf("\tpath — file to read \n");
    printf("\tN — fork number \n");
    return 1;
}

/**
 * Prints error message and help message to console
 * and closes the program
 */
int catch_error()
{
    perror("\nError");
    print_help();
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
        pid_t res = fork();

        switch (res)
        {
        case 0: // forked
        {
            if (sprintf(t, "%d", n_effective) < 0)
                return catch_error();
            char *args[4] = {"calc", output_path, t, NULL};
            printf("Forked proc %d with output_path %s \n", getpid(), output_path);
            execve("calc", args, NULL);
        }
        case -1: // error
            return catch_error();
        }
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
        pid_t child = wait(&code);
        if (child == -1)
            return catch_error();

        // construct name of output file
        if (sprintf(t, "_%d_result.txt", child) < 0)
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

    printf("\nFinal Result: %d \n", sum);
    return 0;
}
