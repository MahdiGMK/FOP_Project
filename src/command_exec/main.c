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
    if (status == FILE_EXISTED)
    {
        LOG("%s\n", file);
        return;
    }
    LOG("File Didn't Exist\n");
}

void CMD_RemoveStr()
{
    char address[100];
    int l, c, sz, f;

    ConsumeSTDIN("-file");
    ConsumeSTDIN("-pos");
    scanf("%d:%d", &l, &c);
    ConsumeSTDIN("-size");
    scanf("%d", &sz);
    f = ConsumeSTDIN("-b");
}

int main()
{
    char cmd[100] = {};

    // ReadStrSTDIN(cmd);
    // LOG("%s\n", cmd);

    while (1)
    {
        scanf("%s", cmd);
        if (strcmp(cmd, "createfile") == 0)
            CMD_CreateFile();
        else if (strcmp(cmd, "insertstr") == 0)
            CMD_InsertStr();
        else if (strcmp(cmd, "cat") == 0)
            CMD_CAT();
        else if (strcmp(cmd, "removestr"))
            CMD_RemoveStr();
    }
}
