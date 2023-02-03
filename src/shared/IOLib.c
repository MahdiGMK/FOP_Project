#include <stdio.h>
#include <string.h>
#include "IOLib.h"

int ReadOption(FILE *istream, char **options)
{
    char opt[IOSIZE];
    while (1)
    {
        *opt = fgetc(istream);
        if (opt[0] == '\n' || opt[0] == 0)
            return NWLINE;
        if (opt[0] != ' ' && opt[0] != '\t')
            break;
    }
    fscanf(istream, "%s", opt + 1);
    if (strcmp(opt, "=D") == 0)
        return ARMAN;

    int id = 0;
    while (options[0])
    {
        if (strcmp(options[0], opt) == 0)
            return id;
        id++;
        options++;
    }
    return id;
}

int ReadStrSTDIN(FILE *istream, char *str)
{
    char prv;
    do
    {
        prv = fgetc(istream);
        if (prv == 0 || prv == '\n')
        {
            ungetc(prv, istream);
            return NWLINE;
        }
    } while (prv == ' ');

    // scanf(" ");
    // char prv = getchar();

    if (prv == '\"')
    {
        while (1)
        {
            char ch = fgetc(istream);
            if (ch == '\"' && prv != '\\')
                break;
            str[0] = ch, str++, prv = ch;
        }
    }
    else
    {
        char *beg = str;
        str++[0] = prv;
        while ((str[0] = fgetc(istream)) != '\n' && str[0] != 0 && str[0] != ' ')
            str++;
        ungetc(str[0], istream), str[0] = 0;
        if (strcmp(beg, "=D") == 0)
            return ARMAN;
    }
    return 0;
}

void ConsumeSTDIN(FILE *istream)
{
    char c;
    do
        c = fgetc(istream);
    while (c != '\n' && c != 0);
}