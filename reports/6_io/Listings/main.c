#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/*
Программа должна выводить в консоль первые N байт содержимого файла, название которого передано в качестве
аргумента командной строки. Число N передаётся в качестве второго аргумента командной строки. Если число N
больше количества имеющихся данных, необходимо вывести те данные, которые доступны. Если файл пуст или не
существует, необходимо вывести соответствующее сообщение об ошибке и завершить работу программы.
*/

void print(const void *buf, size_t n)
{
    write(STDOUT_FILENO, buf, n);
    write(STDOUT_FILENO, "\n", 1);
}

/**
 * Prints help message to console
 */
int print_help()
{
    print("\nUsage: main <path> <N>", 23);
    print("\tpath — file to read", 22);
    print("\tN — byte number", 18);
    return 1;
}

int catch ()
{
    perror("\nError");
    print_help();
}

int main(int argc, char **argv)
{
    // check number of arguments
    if (argc != 3)
        return print_help();

    // get byte numbers and file path
    char *path = argv[1];
    int N = atoi(argv[2]);

    // print input values
    print("Path to file:", 13);
    print(path, strlen(path));
    print("Byte number:", 12);
    print(argv[2], strlen(argv[2]));
    print("", 0);

    // open file to read
    int file = open(path, O_RDONLY);
    if (file < 0)
        return catch ();

    // allocate N bytes
    char *readbuf = malloc(N);
    if (!readbuf)
        return catch ();

    // read N bytes from file
    int bytesRead = read(file, readbuf, N);
    if (bytesRead < 0)
        return catch ();
    if (bytesRead == 0)
    {
        print("File is empty", 13);
        return 0;
    }

    // print buffer to stdout
    print("Read string:", 12);
    int bytesWrite = write(STDOUT_FILENO, readbuf, bytesRead);
    if (bytesWrite < 0)
        return catch ();

    // release memory and resources
    int file_closed = close(file);
    if (file_closed < 0)
        return catch ();
    free(readbuf);

    return 0;
}
