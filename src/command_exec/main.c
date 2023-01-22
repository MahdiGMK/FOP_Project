#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <shared/FileHandling.h>
#include <shared/IOLib.h>
#include <shared/StringLib.h>

#define LOG(str...) \
    printf(str);    \
    fflush(stdout);

void CMD_CreateFile()
{
    char address[100];
    ConsumeSTDIN("-file");
    ReadStrSTDIN(address);

    FileHandlingStatus status = CreateFile(address + 1);
    if (status == FILE_EXISTED)
    {
        LOG("File Existed\n");
    }
    else
    {
        LOG("File Created\n");
    }
}

void CMD_InsertStr()
{
    char address[100], pattern[100];
    int l, c;

    ConsumeSTDIN("-file");
    ReadStrSTDIN(address);
    ConsumeSTDIN("-str");
    ReadStrSTDIN(pattern);
    ConsumeSTDIN("-pos");
    scanf("%d:%d", &l, &c);

    char file[1000] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist\n");
        return;
    }
    ResolveSymbols(pattern);
    char *ptr = GetPtrAt(file, l, c);
    InsertPattern(ptr, pattern);
    WriteFile(address + 1, file);
    LOG("File Edited Successfully\n");
}

void CMD_CAT()
{
    char address[100];

    ConsumeSTDIN("-file");
    ReadStrSTDIN(address);

    char file[1000] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist\n");
        return;
    }
    LOG("%s\n", file);
}

void CMD_RemoveStr()
{
    char address[100];
    int l, c, sz, b;

    ConsumeSTDIN("-file");
    ReadStrSTDIN(address);
    ConsumeSTDIN("-pos");
    scanf("%d:%d", &l, &c);
    ConsumeSTDIN("-size");
    scanf("%d", &sz);
    b = ConsumeSTDIN("-f");

    LOG("%s\n", address);

    char file[1000];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist\n");
        return;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    EraseSubstring(ptr, sz);
    WriteFile(address + 1, file);
    LOG("File Edited Successfully\n");
}

int streq(char *a, char *b)
{
    return !strcmp(a, b);
}

int main()
{
    char cmd[100] = {};

    // ReadStrSTDIN(cmd);
    // LOG("%s\n", cmd);

    while (1)
    {
        scanf("%s", cmd);
        if (streq(cmd, "createfile"))
            CMD_CreateFile();
        else if (streq(cmd, "insertstr"))
            CMD_InsertStr();
        else if (streq(cmd, "cat"))
            CMD_CAT();
        else if (streq(cmd, "removestr"))
            CMD_RemoveStr();
    }
}
