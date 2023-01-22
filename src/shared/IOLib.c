#include <stdio.h>
#include <string.h>
#include "IOLib.h"

int ConsumeSTDIN(const char *pattern)
{
    scanf(" ");
    int len = strlen(pattern);
    int eq = 1;
    for (int i = 0; i < len; i++)
        eq &= getchar() == pattern[i];
    return !eq;
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