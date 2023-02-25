// Подключить нашу библиотеку
#include "file.h"

void ErrorOutput(const char *chFuncFailMessage, int iErr, int iFlag)
{
    DWORD n = 0; // Код ошибки
    const DWORD size = LAB_MAX_BUF_SIZE; // Максимальный размер сообщения об ошибке
    char buf[LAB_MAX_BUF_SIZE]; // Буфер сообщения

    switch ( iFlag ) // Обработка флага
    {
        case LAB_FLAG_GET:
            n = GetLastError();
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           n,
                           MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                           buf,
                           size - 1,
                           NULL);
        break;

        case LAB_FLAG_ERRNO:
            n = errno;
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           n,
                           MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                           buf,
                           size - 1,
                           NULL);
        break;

        case LAB_FLAG_RESULT:
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           iErr,
                           MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                           buf,
                           size - 1,
                           NULL);
            break;

        default :
        sprintf(buf, "Error in programmer's function code is %d", iErr);
        break;
    }

    MessageBoxA(0, buf, chFuncFailMessage, MB_ICONERROR | MB_OK); // Вывод сообщения об ошибке
}

//---------------------------------------------------------------------------

int ReleaseHandle(HANDLE *h)
{
    int bRet = 0;
    
    //  Если хендл валидный
    if ( *h != INVALID_HANDLE_VALUE )
    {
        bRet = CloseHandle(*h); // Закрыть
        *h = INVALID_HANDLE_VALUE; // Сделать невалидным
    }
    
    return bRet;
}

//---------------------------------------------------------------------------

void ReleaseVoidMemory(void **vMemory)
{
    // Если указатель на область памяти валидный
    if ( *vMemory != NULL )
    {
        free(*vMemory); // Освободить
        *vMemory = NULL; // Сделать невалидным
    }
}

//---------------------------------------------------------------------------

int LabOpenFile(const char *chFileName, HANDLE *hFile, DWORD *dwFileSize)
{
    if ( chFileName == NULL ) // Если плохой буфер
    {
        ErrorOutput("Error in input buffer", LAB_BAD_BUF, LAB_FLAG_MY); // Сообщение
        return LAB_BAD_BUF; // Вернуть ошибку
    }

    // Открыть файл на чтение и запись
    *hFile = CreateFileA(chFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if ( *hFile == INVALID_HANDLE_VALUE ) // Если ошибка
    {
        ErrorOutput("Error in CreateFileA function", LAB_ERR, LAB_FLAG_GET); // Вывести сообщение
        return LAB_ERR; // Вернуть -1
    }

    // Получение размера файла
    *dwFileSize = GetFileSize(*hFile, NULL);

    if ( *dwFileSize == INVALID_FILE_SIZE ) // Если ошибка
    {
        ErrorOutput("Error in GetFileSize function", LAB_ERR, LAB_FLAG_GET); // Вывести сообщение
        return LAB_ERR; // Вернуть -1
    }

    return LAB_OK;
}

//---------------------------------------------------------------------------

int LabReadFile(HANDLE hFile, char **chFileContent, DWORD *dwFileSize)
{
    BOOL bResult = FALSE; // Возвращаемое функцией значение
    DWORD dwNumberOfBytesRead = 0; // Количество считанных байт
    DWORD dwSum = 0; // Суммарное количество считанных байт
    DWORD dwFilePointer = INVALID_SET_FILE_POINTER; // Смещение в файле

    if ( hFile == INVALID_HANDLE_VALUE ) // Если плохой хендл
    {
        ErrorOutput("Error in file", LAB_BAD_FILE, LAB_FLAG_MY); // Сообщение
        return LAB_BAD_FILE; // Вернуть ошибку
    }
    
    dwFilePointer = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    
    if ( dwFilePointer == INVALID_SET_FILE_POINTER ) // Если ошибка
    {
        ErrorOutput("Error in SetFilePointer function", LAB_ERR, LAB_FLAG_GET); // Вывести сообщение
        return LAB_ERR; // Вернуть -1
    }

    // Получение размера файла
    *dwFileSize = GetFileSize(hFile, NULL);

    if ( *dwFileSize == INVALID_FILE_SIZE ) // Если ошибка
    {
        ErrorOutput("Error in GetFileSize function", LAB_ERR, LAB_FLAG_GET); // Вывести сообщение
        return LAB_ERR; // Вернуть -1
    }

    // Выделение памяти под содержимое файла
    *chFileContent = (char *)malloc(*dwFileSize + 1);

    if ( *chFileContent == NULL ) // Если память не выделилась
    {
        ErrorOutput("Error in malloc function", LAB_ERR, LAB_FLAG_ERRNO); // Вывести сообщение
        return LAB_ERR; // Вернуть -1
    }

    // Чтение файла
    while ( dwSum < *dwFileSize )
    {
        bResult = ReadFile(hFile, (LPVOID)(*chFileContent + dwSum), *dwFileSize - dwSum, &dwNumberOfBytesRead, NULL);

        if ( bResult == FALSE ) // Если ошибка
        {
            ErrorOutput("Error in ReadFile function", LAB_ERR, LAB_FLAG_GET); // Вывести сообщение
            return LAB_ERR; // Вернуть -1
        }
        else if ( dwNumberOfBytesRead == 0 )
        {
            ErrorOutput("Error in ReadFile function: dwNumberOfBytesRead is 0", LAB_BAD_FILE, LAB_FLAG_MY); // Вывести сообщение
            return LAB_BAD_FILE; // Вернуть -1
        }

        dwSum += dwNumberOfBytesRead;
    }

    *dwFileSize = dwSum;

    (*chFileContent)[*dwFileSize] = '\0'; // Нуль-терм

    return LAB_OK;
}

//---------------------------------------------------------------------------

int LabWriteFile(HANDLE hFile, const char *chFileContent, DWORD *dwFileSize)
{
    BOOL bResult = FALSE; // Возвращаемое функцией значение
    DWORD dwFilePointer = INVALID_SET_FILE_POINTER; // Смещение в файле
    
    if ( hFile == INVALID_HANDLE_VALUE ) // Если плохой хендл
    {
        ErrorOutput("Error in file", LAB_BAD_FILE, LAB_FLAG_MY); // Сообщение
        return LAB_BAD_FILE; // Вернуть ошибку
    }
    
    dwFilePointer = SetFilePointer(hFile, 0, NULL, FILE_END);
    
    if ( dwFilePointer == INVALID_SET_FILE_POINTER ) // Если ошибка
    {
        ErrorOutput("Error in SetFilePointer function", LAB_ERR, LAB_FLAG_GET); // Вывести сообщение
        return LAB_ERR; // Вернуть -1
    }

    // Запись в файл
    bResult = WriteFile(hFile, (LPCVOID)(chFileContent), strlen(chFileContent), dwFileSize, NULL);

    if ( bResult == FALSE ) // Если ошибка
    {
        ErrorOutput("Error in WriteFile function", LAB_ERR, LAB_FLAG_GET); // Вывести сообщение
        return LAB_ERR; // Вернуть -1
    }

    return LAB_OK;
}

//---------------------------------------------------------------------------

int LabCloseFile(HANDLE *hFile)
{
    int bRet = 0;
    
    bRet = ReleaseHandle(hFile);
    
    return bRet;
}

//---------------------------------------------------------------------------

int main()
{
    DWORD len = 0;
    int ret = LAB_OK;
    char *buf = NULL;
    char *command = NULL;
    char *chFileRead = NULL;
    char chFileName[] = {"DanilSmirnov.txt"};
    char chFileToWrite[] = {"Hello world\n"};
    unsigned long i = 0;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BOOL bRet = FALSE;
    
    // Выделение памяти под "сырой" буфер с командой
    buf = (char *)malloc(LAB_MAX_BUF_SIZE + 1);
    
    // Выделение памяти под буфер с командой
    command = (char *)malloc(LAB_MAX_BUF_SIZE + 1);
    
    while ( 1 )
    {
        printf("Waiting for user input...\n");
        
        // Считать команду без двух последних символов
        fgets(buf, LAB_MAX_BUF_SIZE - 2, stdin);
        
        printf("buf is %s\n", buf);

        // Вычисление позиции первого символа-разделителя
        for ( i = 0; (i < strlen(buf)) && (buf[i] != ' ') && (buf[i] != '\t') && (buf[i] != '\n') ; i++ ) ; 

        strncpy(command, buf, i);
        command[i] = '\0';
        
        printf("command is %s\n", command);

        if ( strlen(command) == 0 )
        {
            fprintf(stderr, "command %s is empty\n", command);
        }
        else if ( strncasecmp(command, "open", strlen(command)) == 0 )
        {
            ret = LabOpenFile(chFileName, &hFile, &len);
        }
        else if ( strncasecmp(command, "write", strlen(command)) == 0 )
        {
            ret = LabWriteFile(hFile, chFileToWrite, &len);
        }
        else if ( strncasecmp(command, "read", strlen(command)) == 0 )
        {
            ret = LabReadFile(hFile, &chFileRead, &len);
            
            // Вывести на экран содержимое файла
            printf("Content of the file is:\n%s\n", chFileRead);
            ReleaseVoidMemory((void **)&chFileRead);
        }
        else if ( strncasecmp(command, "close", strlen(command)) == 0 )
        {
            ret = LabCloseFile(&hFile);
        }
        else if ( strncasecmp(command, "end", strlen(command)) == 0 )
        {
            break;
        }
        else
        {
            fprintf(stderr, "command %s error\n", command);
        }
    }
    
    // Освободить память, если не освобождена
    
    ReleaseVoidMemory((void **)&buf);
    ReleaseVoidMemory((void **)&command);
    ReleaseVoidMemory((void **)&chFileRead);
    
    // Закрыть файл, если не закрыт
    
    bRet = ReleaseHandle(&hFile);
    
    return ret;
}
