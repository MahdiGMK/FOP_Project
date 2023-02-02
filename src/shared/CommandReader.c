#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <shared/FileHandling.h>
#include <shared/IOLib.h>
#include <shared/StringLib.h>
#include "RawCommand.h"

int CMD_CreateFile(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE] = {};

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case NWLINE:
        case ARMAN:
            if (_CreateFile(stream, address))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_InsertStr(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE] = {}, pattern[FILESIZE] = {};
    strcpy(pattern, inputstream);
    int l = -1, c = -1;

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          "-str",
                                          "-pos",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            ReadStrSTDIN(istream, pattern);
            break;
        case 2:
            fscanf(istream, "%d:%d", &l, &c);
            break;
        case NWLINE:
        case ARMAN:
            if (_InsertStr(stream, address, pattern, l, c))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_CAT(FILE *stream, FILE *istream, char *inputstream)
{
    char address[IOSIZE] = {};

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          NULL});
        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case NWLINE:
        case ARMAN:
            if (_CAT(stream, address))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_RemoveStr(FILE *stream, FILE *istream, char *inputstream)
{
    char address[IOSIZE];
    int l = -1, c = -1, sz = -1, b = 0, f = 0;

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          "-pos",
                                          "-size",
                                          "-f",
                                          "-b",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            fscanf(istream, "%d:%d", &l, &c);
            break;
        case 2:
            fscanf(istream, "%d", &sz);
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
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Copy(FILE *stream, FILE *istream, char *inputstream)
{
    char address[IOSIZE];
    int l = -1, c = -1, sz = -1, b = 0, f = 0;

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          "-pos",
                                          "-size",
                                          "-f",
                                          "-b",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            fscanf(istream, "%d:%d", &l, &c);
            break;
        case 2:
            fscanf(istream, "%d", &sz);
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
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Cut(FILE *stream, FILE *istream, char *inputstream)
{
    char address[IOSIZE] = {};
    int l = -1, c = -1, sz = -1, b = 0, f = 0;

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          "-pos",
                                          "-size",
                                          "-f",
                                          "-b",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            fscanf(istream, "%d:%d", &l, &c);
            break;
        case 2:
            fscanf(istream, "%d", &sz);
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
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Paste(FILE *stream, FILE *istream, char *inputstream)
{
    char address[IOSIZE] = {};
    int l = -1, c = -1;

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          "-pos",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            fscanf(istream, "%d:%d", &l, &c);
            break;
        case NWLINE:
        case ARMAN:
            if (_Paste(stream, address, l, c))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Undo(FILE *stream, FILE *istream, char *inputstream)
{
    char address[IOSIZE];
    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          NULL});
        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case NWLINE:
        case ARMAN:
            if (_Undo(stream, address))
                goto invalid;
            return opt == ARMAN;

        default:
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Tree(FILE *stream, FILE *istream, char *inputstream)
{
    int dep;
    fscanf(istream, "%d", &dep);
    int opt = ReadOption(istream, (char *[]){
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
        ConsumeSTDIN(istream);
        OUTPUT("Invalid Format");
        return -1;
    }
    return 0;
}

int CMD_Compare(FILE *stream, FILE *istream, char *inputstream)
{
    char address1[ADDRSIZE], address2[ADDRSIZE];
    ReadStrSTDIN(istream, address1);
    ReadStrSTDIN(istream, address2);
    int opt = ReadOption(istream, (char *[]){
                                      NULL});
    switch (opt)
    {
    case NWLINE:
    case ARMAN:
        if (_Compare(stream, address1, address2))
            goto invalid;
        return opt == ARMAN;

    default:
        ConsumeSTDIN(istream);
    invalid:
        OUTPUT("Invalid Format");
        return -1;
    }
    return 0;
}

int CMD_AutoIndent(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE];
    ReadStrSTDIN(istream, address);

    int opt = ReadOption(istream, (char *[]){
                                      NULL});
    switch (opt)
    {
    case NWLINE:
    case ARMAN:
        if (_AutoIndent(stream, address))
            goto invalid;
        return opt == ARMAN;

    default:
        ConsumeSTDIN(istream);
    invalid:
        OUTPUT("Invalid Format");
        return -1;
    }
    return 0;
}

int CMD_Grep(FILE *stream, FILE *istream, char *inputstream) //?
{
    char str[FILESIZE];
    strcpy(str, inputstream);

    int l = 0, c = 0;
    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
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
            if (ReadStrSTDIN(istream, str))
                goto invalid;
            break;
        case 3:
            goto files;

        default:
            ConsumeSTDIN(istream);
        case NWLINE:
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
files:
    char *addr[IOSIZE] = {};
    for (int sz = 0;; sz++)
    {
        addr[sz] = malloc(ADDRSIZE);
        int rs = ReadStrSTDIN(istream, addr[sz]);
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
            ConsumeSTDIN(istream);
            goto invalid;
        }
    }
}

int CMD_Find(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE] = {}, pattern[IOSIZE] = {};
    strcpy(pattern, inputstream);

    int count = 0, at = 0, atn = 0, byword = 0, all = 0;

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
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
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            ReadStrSTDIN(istream, pattern);
            break;
        case 2:
            count = 1;
            break;
        case 3:
            at = 1;
            fscanf(istream, "%d", &atn);
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
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

int CMD_Replace(FILE *stream, FILE *istream, char *inputstream)
{
    char address[ADDRSIZE] = {}, pattern[IOSIZE] = {}, replace[IOSIZE] = {};
    int at = 0, atn = 0, all = 0;

    while (1)
    {
        int opt = ReadOption(istream, (char *[]){
                                          "-file",
                                          "-str1",
                                          "-str2",
                                          "-at",
                                          "-all",
                                          NULL});

        switch (opt)
        {
        case 0:
            ReadStrSTDIN(istream, address);
            break;
        case 1:
            ReadStrSTDIN(istream, pattern);
            break;
        case 2:
            ReadStrSTDIN(istream, replace);
            break;
        case 3:
            at = 1;
            fscanf(istream, "%d", &atn);
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
            ConsumeSTDIN(istream);
        invalid:
            OUTPUT("Invalid Format");
            return -1;
        }
    }
    return 0;
}

void ReadCMD(FILE *istream)
{
    int opt = ReadOption(istream, (char *[]){
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
        res = CMD_CreateFile(stream, istream, inputstream);
        break;
    case 1:
        res = CMD_InsertStr(stream, istream, inputstream);
        break;
    case 2:
        res = CMD_CAT(stream, istream, inputstream);
        break;
    case 3:
        res = CMD_RemoveStr(stream, istream, inputstream);
        break;
    case 4:
        res = CMD_Copy(stream, istream, inputstream);
        break;
    case 5:
        res = CMD_Cut(stream, istream, inputstream);
        break;
    case 6:
        res = CMD_Paste(stream, istream, inputstream);
        break;
    case 7:
        res = CMD_Undo(stream, istream, inputstream);
        break;
    case 8:
        res = CMD_Tree(stream, istream, inputstream);
        break;
    case 9:
        res = CMD_Compare(stream, istream, inputstream);
        break;
    case 10:
        res = CMD_AutoIndent(stream, istream, inputstream);
        break;
    case 11:
        res = CMD_Grep(stream, istream, inputstream);
        break;
    case 12:
        res = CMD_Find(stream, istream, inputstream);
        break;
    case 13:
        res = CMD_Replace(stream, istream, inputstream);
        break;
    case NWLINE:
        break;

    default:
        ConsumeSTDIN(istream);
        OUTPUT("Invalid Format");
        break;
    }
    fclose(stream);

    if (res == 1)
        ReadCMD(istream);

    // PrintOStream();

    //    LOG("RES : %d", res);
}
