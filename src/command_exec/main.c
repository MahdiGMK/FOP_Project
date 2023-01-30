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
    char address[IOSIZE];
    int l = -1, c = -1, sz = -1, b = 0, f = 0;

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            "-pos",
            "-size",
            "-f",
            "-b",
            NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(address);
            break;
        case 1:
            scanf("%d:%d", &l, &c);
            break;
        case 2:
            scanf("%d", &sz);
            break;
        case 3:
            f = 1;
            break;
        case 4:
            b = 1;
            break;
        case NWLINE:
            if (_RemoveStr(address, l, c, sz, b, f))
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
void CMD_Copy()
{
    char address[IOSIZE];
    int l = -1, c = -1, sz = -1, b = 0, f = 0;

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            "-pos",
            "-size",
            "-f",
            "-b",
            NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(address);
            break;
        case 1:
            scanf("%d:%d", &l, &c);
            break;
        case 2:
            scanf("%d", &sz);
            break;
        case 3:
            f = 1;
            break;
        case 4:
            b = 1;
            break;
        case NWLINE:
            if (_Copy(address, l, c, sz, b, f))
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
void CMD_Cut()
{
    char address[IOSIZE] = {};
    int l = -1, c = -1, sz = -1, b = 0, f = 0;

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            "-pos",
            "-size",
            "-f",
            "-b",
            NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(address);
            break;
        case 1:
            scanf("%d:%d", &l, &c);
            break;
        case 2:
            scanf("%d", &sz);
            break;
        case 3:
            f = 1;
            break;
        case 4:
            b = 1;
            break;
        case NWLINE:
            if (_Cut(address, l, c, sz, b, f))
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
void CMD_Paste()
{
    char address[IOSIZE] = {};
    int l = -1, c = -1;

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            "-pos",
            NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(address);
            break;
        case 1:
            scanf("%d:%d", &l, &c);
            break;
        case NWLINE:
            if (_Paste(address, l, c))
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

void CMD_Undo()
{
    char address[IOSIZE];
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
            if (_Undo(address))
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

void CMD_Tree()
{
    int dep;
    scanf("%d", &dep);
    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
        if (_Tree(dep))
        {
            LOG("Invalid Depth");
        }
        return;

    default:
        ConsumeSTDIN();
        LOG("Invalid Format");
        return;
    }
}

void CMD_Compare()
{
    char address1[ADDRSIZE], address2[ADDRSIZE];
    ReadStrSTDIN(address1);
    ReadStrSTDIN(address2);
    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
        if (_Compare(address1, address2))
            goto invalid;
        return;

    default:
        ConsumeSTDIN();
    invalid:
        LOG("Invalid Format");
        return;
    }
}

void CMD_AutoIndent()
{
    char address[ADDRSIZE];
    ReadStrSTDIN(address);

    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
        if (_AutoIndent(address))
            goto invalid;
        return;

    default:
        ConsumeSTDIN();
    invalid:
        LOG("Invalid Format");
        return;
    }
}

void ReadCMD()
{
    int opt = ReadOption((char *[]){
        "createfile",
        "insertstr",
        "cat",
        "removestr",
        "copy",
        "cut",
        "paste",
        "undo",
        "tree",
        "compare",
        "auto-indent",
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
    case 4:
        CMD_Copy();
        break;
    case 5:
        CMD_Cut();
        break;
    case 6:
        CMD_Paste();
        break;
    case 7:
        CMD_Undo();
        break;
    case 8:
        CMD_Tree();
        break;
    case 9:
        CMD_Compare();
        break;
    case 10:
        CMD_AutoIndent();
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
    // _AutoIndent("/root/test.txt");
    while (1)
    {
        ReadCMD();
    }
}
