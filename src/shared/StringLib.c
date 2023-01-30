#include "StringLib.h"
#include <string.h>

void InsertPattern(char *ptr, const char *pattern)
{
    int ptrlen = strlen(pattern);
    char *ed = ptr;
    while (*ed != 0)
        ed++;
    while (ed >= ptr)
    {
        ed[ptrlen] = ed[0];
        ed--;
    }
    for (int i = 0; i < ptrlen; i++)
        ptr[i] = pattern[i];
}

void EraseSubstring(char *ptr, int sz)
{
    ptr += sz;
    while (*ptr)
        ptr[-sz] = ptr[0], ptr++;
    for (int i = 0; i <= sz; i++)
        ptr[-i] = 0;
}

char *GetPtrAt(char *str, int ln, int cn)
{
    char *ptr = str;
    while (ln > 1)
    {
        if (*ptr == 0)
            return ptr;
        ln -= *ptr == '\n', ptr++;
    }
    while (cn)
    {
        if (*ptr == 0 || *ptr == '\n')
            return ptr;
        ptr++, cn--;
    }
    return ptr;
}

char *GetEndLine(char *ptr)
{
    while (ptr[0] != '\n' && ptr[0] != 0)
        ptr++;
    return ptr;
}

int CountLines(char *str)
{
    int res = 1;
    while (str[0])
        res += str[0] == '\n', str++;
    return res;
}

void ResolveSymbols(char *str)
{
    char *ptr = str;
    char *save = str;
    while (*ptr)
    {
        if (ptr[0] == '\\')
        {
            switch (ptr[1])
            {
            case 'n':
                save[0] = '\n';
                break;
            case '\\':
                save[0] = '\\';
                break;
            case '\"':
                save[0] = '\"';
                break;
            }
            ptr++;
        }
        else
            save[0] = ptr[0];
        ptr++, save++;
    }
    while (save <= ptr)
        save[0] = 0, save++;
}
