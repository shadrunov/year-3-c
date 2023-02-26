#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

/*
Программа должна выводить в консоль первые N байт содержимого файла, название которого передано в качестве
аргумента командной строки. Число N передаётся в качестве второго аргумента командной строки. Если число N
больше количества имеющихся данных, необходимо вывести те данные, которые доступны. Если файл пуст или не
существует, необходимо вывести соответствующее сообщение об ошибке и завершить работу программы.
*/


/**
 * Prints string buffer and CR to console
 */
void print(const void *buf, DWORD n)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(hConsole, buf, n, NULL, NULL);
    WriteConsole(hConsole, "\n", 1, NULL, NULL);
}

/**
 * Prints help message to console
 */
int print_help()
{
    print("\nUsage: main <path> <N>", 23);
    print("\tpath - file to read", 22);
    print("\tN - byte number", 18);
    return 1;
}

/**
 * Prints error message and help message to console
 * and closes the program
 */
int catch ()
{
    LPSTR message;
    DWORD dwMessageLen = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&message, 0, NULL);
    print(message, dwMessageLen);
    print_help();
    return 1;
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
    HANDLE hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return catch ();

    // allocate N bytes
    HANDLE hHeap = HeapCreate(0, 0x01000, 0);	// create heap
    if (hHeap == NULL)
        return catch();

    // LPSTR readbuf = (char*)HeapAlloc(hHeap, 0, N);
    LPSTR readbuf = HeapAlloc(hHeap, 0, N);
    if (readbuf==NULL)  // if error allocating
        if (HeapDestroy(hHeap) == 0)  // if error destroying heap
            return catch();

    DWORD bytesRead = 0;

    // read N bytes from file
    BOOL result = ReadFile(hFile, readbuf, N, &bytesRead, NULL);
    if (!result)
        return catch ();
    if (bytesRead == 0)
    {
        print("File is empty", 13);
        return 0;
    }

    // print buffer to stdout
    LPDWORD dwBytesWritten;
    print("Read string:", 12);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL bytesWrite = WriteConsoleA(hConsole, readbuf, bytesRead, dwBytesWritten, NULL);
    if (!bytesWrite)
        return catch ();

    // release memory and resources
    BOOL file_closed = CloseHandle(hFile);

    if (!file_closed)
        return catch ();
    if (HeapFree(hHeap, 0, readbuf) == 0)
		return catch();
    if (HeapDestroy(hHeap) == 0)  // if error destroying heap
        return catch();

    return 0;
}
