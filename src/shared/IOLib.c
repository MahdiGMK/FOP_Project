#include <stdio.h>
#include <string.h>
#include "IOLib.h"

int ReadOption(char **options)
{
    char opt[IOSIZE];
    while (1)
    {
        *opt = getchar();
        if (opt[0] == '\n' || opt[0] == 0)
            return NWLINE;
        if (opt[0] != ' ' && opt[0] != '\t')
            break;
    }
    scanf("%s", opt + 1);
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

int ReadStrSTDIN(char *str)
{
    scanf(" ");
    char prv = getchar();
    if (prv == '\"')
    {
        while (1)
        {
            char ch = getchar();
            if (ch == '\"' && prv != '\\')
                break;
            str[0] = ch, str++, prv = ch;
        }
    }
    else
    {
        str[0] = prv;
        scanf("%s", str + 1);
    }
    return 0;
}

void ConsumeSTDIN()
{
    char c;
    do
        c = getchar();
    while (c != '\n' && c != 0);
}