#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <shared/FileHandling.h>
#include <shared/IOLib.h>
#include <shared/StringLib.h>
#include "RawCommand.h"

void CMD_CreateFile()
{
    char address[IOSIZE] = {};

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(address);
            break;
        case NWLINE:
            if (_CreateFile(address))
                goto invalid;
            return;

        default:
            ConsumeSTDIN();
        invalid:
            LOG("Invalid Format");
            return;
        }
    }
}

void CMD_InsertStr()
{
    char address[IOSIZE] = {}, pattern[IOSIZE] = {};
    int l = -1, c = -1;

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            "-str",
            "-pos",
            NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(address);
            break;
        case 1:
            ReadStrSTDIN(pattern);
            break;
        case 2:
            scanf("%d:%d", &l, &c);
            break;
        case NWLINE:
            if (_InsertStr(address, pattern, l, c))
                goto invalid;
            return;

        default:
            ConsumeSTDIN();
        invalid:
            LOG("Invalid Format");
            return;
        }
    }
}

void CMD_CAT()
{
    char address[100];

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            NULL});
        switch (opt)
        {
        case 0:
            ReadStrSTDIN(address);
            break;
        case NWLINE:
            if (_CAT(address))
                goto invalid;
            return;

        default:
            ConsumeSTDIN();
        invalid:
            LOG("Invalid Format");
            return;
        }
    }
}

void CMD_RemoveStr()
{
    char address[100];
    int l, c, sz, b;

    // ConsumeSTDIN("-file");
    ReadStrSTDIN(address);
    // ConsumeSTDIN("-pos");
    scanf("%d:%d", &l, &c);
    // ConsumeSTDIN("-size");
    scanf("%d", &sz);
    // b = ConsumeSTDIN("-f");

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

void ReadCMD()
{
    int opt = ReadOption((char *[]){
        "createfile",
        "insertstr",
        "cat",
        "removestr",
        NULL});

    switch (opt)
    {
    case 0:
        CMD_CreateFile();
        break;
    case 1:
        CMD_InsertStr();
        break;
    case 2:
        CMD_CAT();
        break;
    case 3:
        CMD_RemoveStr();
        break;

    default:
        ConsumeSTDIN();
    case NWLINE:
        LOG("Invalid Format");
        break;
    }
}

int main()
{
    // int opt1 = ReadOption((char *[]){
    //     "createfile",
    //     "insertstr",
    //     "cat",
    //     "removestr",
    //     NULL});
    // int opt2 = ReadOption((char *[]){
    //     "-file",
    //     NULL});

    // LOG("%d%d", opt1, opt2);

    while (1)
    {
        ReadCMD();
    }
}
