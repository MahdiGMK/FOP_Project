#include <stdio.h>
#include <string.h>
#include "IOLib.h"

int ReadOption(char **options)
{
    scanf(" ");
    char opt[100];
    scanf("%s", opt);
    int id = 0;
    while (*options)
    {
        if (strcmp(*options, opt) == 0)
            return id;
        id++;
        options++;
    }
    return -1;
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
        while (1)
        {
            if (prv == ' ' || prv == '\n')
                break;
            str[0] = prv, str++;
            prv = getchar();
        }
    }
    str[0] = 0;
    return 0;
}