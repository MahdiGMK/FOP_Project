#include <shared/FileHandling.h>
#include <shared/StringLib.h>
#include "RawCommand.h"
#include <string.h>

int _CreateFile(char *address)
{
    if (address[0] != '/')
        return 1;
    FileHandlingStatus status = CreateFile(address + 1);
    if (status == FILE_EXISTED)
    {
        LOG("File Existed");
    }
    else
    {
        LOG("File Created");
    }
    return 0;
}

int _InsertStr(char *address, char *pattern, int l, int c)
{
    if (address[0] != '/' || pattern[0] == 0 || l < 1 || c < 0)
        return 1;

    CreateBackup(address + 1);

    char file[1000] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    ResolveSymbols(pattern);
    char *ptr = GetPtrAt(file, l, c);
    InsertPattern(ptr, pattern);
    WriteFile(address + 1, file);
    LOG("File Edited Successfully");
    return 0;
}

int _CAT(char *address)
{
    if (address[0] != '/')
        return 1;
    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    LOG("%s", file);
    return 0;
}

int _RemoveStr(char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;

    CreateBackup(address + 1);

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    EraseSubstring(ptr, sz);
    WriteFile(address + 1, file);
    LOG("File Edited Successfully");
    return 0;
}

char clipboard[FILESIZE];

int _Copy(char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;
    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    strcpy(clipboard, ptr);
    clipboard[sz] = 0;
    LOG("Coppied Successfully");
    return 0;
}

int _Cut(char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;

    CreateBackup(address + 1);

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    strcpy(clipboard, ptr);
    clipboard[sz] = 0;
    EraseSubstring(ptr, sz);
    WriteFile(address + 1, file);
    LOG("Cutted Successfully");
    return 0;
}

int _Paste(char *address, int l, int c)
{
    if (address[0] != '/' || l < 1 || c < 0)
        return 1;

    CreateBackup(address);

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    InsertPattern(ptr, clipboard);
    WriteFile(address + 1, file);
    LOG("Pasted Successfully");
    return 0;
}

int _Undo(char *address)
{
    if (address[0] != '/')
        return 1;
    FileHandlingStatus status = Undo(address + 1);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }

    LOG("Successfull Undo");
    return 0;
}
