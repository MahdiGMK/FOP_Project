#ifndef _RAWCOMMAND
#define _RAWCOMMAND

#include <stdio.h>

#define LOG(str...) \
    printf(str);    \
    printf("\n");   \
    fflush(stdout);

int _CreateFile(char *address);

int _InsertStr(char *address, char *pattern, int l, int c);

int _CAT(char *address);

int _RemoveStr(char *address, int l, int c, int sz, int b, int f);

#endif