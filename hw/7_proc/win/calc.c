#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

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
    printf("\nUsage: calc <path> <N> \n");
    printf("\tpath - file to read \n");
    printf("\tN - amount of numbers in input file \n");
    ExitProcess(EXIT_FAILURE);
}

/**
 * Prints error message and help message to console
 * and closes the program
 */
int catch_error()
{
    perror("\nError");
    print_help();
    ExitProcess(EXIT_FAILURE);
}

int main(int argc, char **argv)
{

    // print PID
    pid_t pid = getpid();
    printf("\n[Child %d] \t New proc \n", pid);

    // check number of arguments
    if (argc != 3)
        return print_help();

    // get byte numbers and file path
    char *input_path = argv[1];
    int n = atoi(argv[2]);

    // print input values
    printf("[Child %d] \t Path to file with numbers: %s \n", pid, input_path);
    printf("[Child %d] \t Input length: %d \n", pid, n);

    // open file to read
    FILE *input_file = fopen(input_path, "r");
    if (!input_file)
        return catch_error();

    // move to file start
    if (fseek(input_file, 0, SEEK_SET))
        return catch_error();

    // perform calculations
    char t[30];
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        if (!fscanf(input_file, " %12s", t))
            return catch_error();

        // calculate
        int cur_number = atoi(t);
        int square = cur_number * cur_number;
        sum += square;
        printf("[Child %d] \t Current number: %d \t Square: %d \t Sum: %d \n", pid, cur_number, square, sum);
    }

    // release input file
    if (fclose(input_file) != 0)
        return catch_error();

    printf("[Child %d] \t Result: %d \n", pid, sum);

    // construct name of output file
    if (sprintf(t, "_%d_result.txt", pid) < 0)
        return catch_error();

    FILE *output_file = fopen(t, "w");
    if (!output_file)
        return catch_error();

    if (fprintf(output_file, "%d", sum) < 0)
        return catch_error();

    // release input file
    if (fclose(output_file) != 0)
        return catch_error();

    Sleep(5000);
    ExitProcess(EXIT_SUCCESS);
}
