#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <shared/FileHandling.h>
#include <shared/IOLib.h>
#include <shared/StringLib.h>
#include "RawCommand.h"

int CMD_CreateFile(FILE *stream, char *inputstream)
{
    char address[ADDRSIZE] = {};

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
        case ARMAN:
            if (_CreateFile(stream, address))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_InsertStr(FILE *stream, char *inputstream)
{
    char address[ADDRSIZE] = {}, pattern[FILESIZE] = {};
    strcpy(pattern, inputstream);
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
        case ARMAN:
            if (_InsertStr(stream, address, pattern, l, c))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_CAT(FILE *stream, char *inputstream)
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
        case ARMAN:
            if (_CAT(stream, address))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_RemoveStr(FILE *stream, char *inputstream)
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
        case ARMAN:
            if (_RemoveStr(stream, address, l, c, sz, b, f))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Copy(FILE *stream, char *inputstream)
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
        case ARMAN:
            if (_Copy(stream, address, l, c, sz, b, f))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Cut(FILE *stream, char *inputstream)
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
        case ARMAN:
            if (_Cut(stream, address, l, c, sz, b, f))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Paste(FILE *stream, char *inputstream)
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
        case ARMAN:
            if (_Paste(stream, address, l, c))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Undo(FILE *stream, char *inputstream)
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
        case ARMAN:
            if (_Undo(stream, address))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Tree(FILE *stream, char *inputstream)
{
    int dep;
    scanf("%d", &dep);
    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
    case ARMAN:
        if (_Tree(stream, dep))
        {
            OUTPUT("Invalid Depth");
            return -1;
        }
        return opt == ARMAN;

    default:
        ConsumeSTDIN();
        OUTPUT("Invalid Format");
        return -1;
    }
    return 0;
}

int CMD_Compare(FILE *stream, char *inputstream)
{
    char address1[ADDRSIZE], address2[ADDRSIZE];
    ReadStrSTDIN(address1);
    ReadStrSTDIN(address2);
    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
    case ARMAN:
        if (_Compare(stream, address1, address2))
            goto invalid;
        return opt == ARMAN;

    default:
        ConsumeSTDIN();
    invalid:
        OUTPUT("Invalid Format");
        return -1;
    }
    return 0;
}

int CMD_AutoIndent(FILE *stream, char *inputstream)
{
    char address[ADDRSIZE];
    ReadStrSTDIN(address);

    int opt = ReadOption((char *[]){
        NULL});
    switch (opt)
    {
    case NWLINE:
    case ARMAN:
        if (_AutoIndent(stream, address))
            goto invalid;
        return opt == ARMAN;

    default:
        ConsumeSTDIN();
    invalid:
        OUTPUT("Invalid Format");
        return -1;
    }
    return 0;
}

int CMD_Grep(FILE *stream, char *inputstream) //?
{
    char str[FILESIZE];
    strcpy(str, inputstream);

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
            OUTPUT("Invalid Format");
            return -1;
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
        case 0:
            break;
        case NWLINE:
        case ARMAN:
            addr[sz] = NULL;
            if (_Grep(stream, addr, str, c, l))
                goto invalid;

            for (int i = 0; i < sz; i++)
                free(addr[sz]);
            return rs == ARMAN;

        default: //?
            ConsumeSTDIN();
            goto invalid;
        }
        sz++;
    }
}

int CMD_Find(FILE *stream, char *inputstream)
{
    char address[ADDRSIZE] = {}, pattern[IOSIZE] = {};
    strcpy(pattern, inputstream);

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
        case ARMAN:
            if (_Find(stream, address, pattern, count, at, atn, byword, all))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Replace(FILE *stream, char *inputstream)
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
        case ARMAN:
            if (_Replace(stream, address, pattern, replace, at, atn, all))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN();
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
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
        "grep",
        "find",
        "replace",
        NULL});

    char inputstream[FILESIZE] = {};
    ReadOStream(inputstream);

    FILE *stream = CreateOStream();
    int res = 0;
    switch (opt)
    {
    case 0:
        res = CMD_CreateFile(stream, inputstream);
        break;
    case 1:
        res = CMD_InsertStr(stream, inputstream);
        break;
    case 2:
        res = CMD_CAT(stream, inputstream);
        break;
    case 3:
        res = CMD_RemoveStr(stream, inputstream);
        break;
    case 4:
        res = CMD_Copy(stream, inputstream);
        break;
    case 5:
        res = CMD_Cut(stream, inputstream);
        break;
    case 6:
        res = CMD_Paste(stream, inputstream);
        break;
    case 7:
        res = CMD_Undo(stream, inputstream);
        break;
    case 8:
        res = CMD_Tree(stream, inputstream);
        break;
    case 9:
        res = CMD_Compare(stream, inputstream);
        break;
    case 10:
        res = CMD_AutoIndent(stream, inputstream);
        break;
    case 11:
        res = CMD_Grep(stream, inputstream);
        break;
    case 12:
        res = CMD_Find(stream, inputstream);
        break;
    case 13:
        res = CMD_Replace(stream, inputstream);
        break;
    case NWLINE:
        break;

    default:
        ConsumeSTDIN();
        OUTPUT("Invalid Format");
        break;
    }
    fclose(stream);

    if (res == 1)
        ReadCMD();

    // PrintOStream();

    //    LOG("RES : %d", res);
}

int main()
{
    // char pattern[10] = "bro *";
    // _Find("/root/test.txt", pattern, 1, 0, 0, 0, 1);
    // _Replace("/root/test.txt", pattern, "z", 0, 0, 1);

    while (1)
    {
        fclose(CreateOStream());
        ReadCMD();
        PrintOStream();
    }
}
