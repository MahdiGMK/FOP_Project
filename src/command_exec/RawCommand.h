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

int _Copy(char *address, int l, int c, int sz, int b, int f);

int _Cut(char *address, int l, int c, int sz, int b, int f);

int _Paste(char *address, int l, int c);

int _Undo(char *addres);

int _Tree(int depth);

int _Compare(char *address1, char *address2);

int _AutoIndent(char *address);

#endif