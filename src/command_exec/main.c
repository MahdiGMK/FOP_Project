#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <shared/FileHandling.h>
#include <shared/IOLib.h>
#include <shared/StringLib.h>
#include "RawCommand.h"

void CMD_CreateFile(FILE *stream)
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
            if (_CreateFile(stream, address))
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

void CMD_InsertStr(FILE *stream)
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
            if (_InsertStr(stream, address, pattern, l, c))
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

void CMD_CAT(FILE *stream)
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
            if (_CAT(stream, address))
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

void CMD_RemoveStr(FILE *stream)
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
            if (_RemoveStr(stream, address, l, c, sz, b, f))
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
void CMD_Copy(FILE *stream)
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
            if (_Copy(stream, address, l, c, sz, b, f))
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
void CMD_Cut(FILE *stream)
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
            if (_Cut(stream, address, l, c, sz, b, f))
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
void CMD_Paste(FILE *stream)
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
            if (_Paste(stream, address, l, c))
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

void CMD_Undo(FILE *stream)
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
            if (_Undo(stream, address))
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

void CMD_Tree(FILE *stream)
{
    int dep;
    scanf("%d", &dep);
    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
        if (_Tree(stream, dep))
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

void CMD_Compare(FILE *stream)
{
    char address1[ADDRSIZE], address2[ADDRSIZE];
    ReadStrSTDIN(address1);
    ReadStrSTDIN(address2);
    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
        if (_Compare(stream, address1, address2))
            goto invalid;
        return;

    default:
        ConsumeSTDIN();
    invalid:
        LOG("Invalid Format");
        return;
    }
}

void CMD_AutoIndent(FILE *stream)
{
    char address[ADDRSIZE];
    ReadStrSTDIN(address);

    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
        if (_AutoIndent(stream, address))
            goto invalid;
        return;

    default:
        ConsumeSTDIN();
    invalid:
        LOG("Invalid Format");
        return;
    }
}

void CMD_Grep(FILE *stream)
{
    char str[IOSIZE];
    int l = 0, c = 0;
    while (1)
    {
        int opt = ReadOption((char *[]){
            "-l",
            "-c",
            "-str",
            "-files",
            NULL});
        switch (opt)
        {
        case 0:
            l = 1;
            break;
        case 1:
            c = 1;
            break;
        case 2:
            if (ReadStrSTDIN(str))
                goto invalid;
            break;
        case 3:
            goto files;

        default:
            ConsumeSTDIN();
        case NWLINE:
        invalid:
            LOG("Invalid Format");
            return;
        }
    }
files:
    char *addr[IOSIZE] = {};
    int sz = 0;
    while (1)
    {
        addr[sz] = malloc(ADDRSIZE);
        int rs = ReadStrSTDIN(addr[sz]);
        switch (rs)
        {
        case NWLINE:
            addr[sz] = NULL;
            _Grep(stream, addr, str, c, l);
            goto done;

        default:
            break;
        }
        sz++;
    }

done:
    for (int i = 0; i < sz; i++)
        free(addr[sz]);
}

void CMD_Find(FILE *stream)
{
    char address[ADDRSIZE] = {}, pattern[IOSIZE] = {};
    int count = 0, at = 0, atn = 0, byword = 0, all = 0;

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            "-str",
            "-count",
            "-at",
            "-all",
            "-byword",
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
            count = 1;
            break;
        case 3:
            at = 1;
            scanf("%d", &atn);
            break;
        case 4:
            all = 1;
            break;
        case 5:
            byword = 1;
            break;
        case NWLINE:
            if (_Find(stream, address, pattern, count, at, atn, byword, all))
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

void CMD_Replace(FILE *stream)
{
    char address[ADDRSIZE] = {}, pattern[IOSIZE] = {}, replace[IOSIZE] = {};
    int at = 0, atn = 0, all = 0;

    while (1)
    {
        int opt = ReadOption((char *[]){
            "-file",
            "-str1",
            "-str2",
            "-at",
            "-all",
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
            ReadStrSTDIN(replace);
            break;
        case 3:
            at = 1;
            scanf("%d", &atn);
            break;
        case 4:
            all = 1;
            break;
        case NWLINE:
            if (_Replace(stream, address, pattern, replace, at, atn, all))
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

void ReadCMD(FILE *stream)
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
        "grep",
        "find",
        "replace",
        NULL});

    switch (opt)
    {
    case 0:
        CMD_CreateFile(stream);
        break;
    case 1:
        CMD_InsertStr(stream);
        break;
    case 2:
        CMD_CAT(stream);
        break;
    case 3:
        CMD_RemoveStr(stream);
        break;
    case 4:
        CMD_Copy(stream);
        break;
    case 5:
        CMD_Cut(stream);
        break;
    case 6:
        CMD_Paste(stream);
        break;
    case 7:
        CMD_Undo(stream);
        break;
    case 8:
        CMD_Tree(stream);
        break;
    case 9:
        CMD_Compare(stream);
        break;
    case 10:
        CMD_AutoIndent(stream);
        break;
    case 11:
        CMD_Grep(stream);
        break;
    case 12:
        CMD_Find(stream);
        break;
    case 13:
        CMD_Replace(stream);
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
    // char pattern[10] = "bro *";
    // _Find("/root/test.txt", pattern, 1, 0, 0, 0, 1);
    // _Replace("/root/test.txt", pattern, "z", 0, 0, 1);

    while (1)
    {
        ReadCMD(stdout);
    }
}
