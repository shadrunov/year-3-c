#ifndef FILE_H
#define FILE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winerror.h>

//---------------------------------------------------------------------------

// Коды ошибок

#define LAB_OK 0
#define LAB_ERR -1
#define LAB_BAD_BUF -2
#define LAB_NOT_FOUND -3
#define LAB_STOP -4
#define LAB_SERVER_DISCONNECTED_OK -5
#define LAB_BAD_FILE -6

// Флаги для работы функции о выводе ошибки

#define LAB_FLAG_WSA 0
#define LAB_FLAG_GET 1
#define LAB_FLAG_ERRNO 2
#define LAB_FLAG_MY 3
#define LAB_FLAG_RESULT 4

// Константы

#define LAB_MAX_BUF_SIZE 128
#define LAB_MIN_BUF_LEN 1
#define LAB_MAX_COUNT_CLIENTS FD_SETSIZE
#define LAB_BUF_SIZE 64
#define LAB_HOST_NAME_SIZE 64

//---------------------------------------------------------------------------

// Прототипы функций:

// Вывод сообщения об ошибке
void ErrorOutput(
        const char *chFuncFailMessage, // Заголовок сообщения
        int iErr, // Номер ошибки, возвращенный функцией, где произошла ошибка
        int iFlag // Флаг, как получить сообщение об ошибке
        );
        
//---------------------------------------------------------------------------

int ReleaseHandle(
        HANDLE *h // Хендл 
        );

//---------------------------------------------------------------------------

void ReleaseVoidMemory(
        void **vMemory // Указатель на указатель на память любого типа
        );

//---------------------------------------------------------------------------

int LabOpenFile(
        const char *chFileName, // Имя файла
        HANDLE *hFile, // Указатель на хендл
        DWORD *dwFileSize // Указатель на переменную с размером файла   
        );

//---------------------------------------------------------------------------

int LabReadFile(
        HANDLE hFile, // Хендл
        char **chFileContent, // Указатель на указатель на контент файла
        DWORD *dwFileSize // Указатель на переменную с размером файла
        );

//---------------------------------------------------------------------------

int LabWriteFile(
        HANDLE hFile, // Хендл
        const char *chFileContent, // Указатель на контент файла
        DWORD *dwFileSize // Указатель на переменную с размером файла
        );

//---------------------------------------------------------------------------

int LabCloseFile(
        HANDLE *hFile // Указатель на хендл
        );

//---------------------------------------------------------------------------

#endif // FILE_H