#ifndef _RAWCOMMAND
#define _RAWCOMMAND

#include <stdio.h>

#define LOG(str...) \
    printf(str);    \
    printf("\n");   \
    fflush(stdout);
#define PRINT(format...) fprintf(stream, format)
#define OUTPUT(format...) \
    PRINT(format);        \
    PRINT("\n");          \
    fflush(stream);

int _CreateFile(FILE *stream, char *address);

int _InsertStr(FILE *stream, char *address, char *pattern, int l, int c);

int _CAT(FILE *stream, char *address);

int _RemoveStr(FILE *stream, char *address, int l, int c, int sz, int b, int f);

int _Copy(FILE *stream, char *address, int l, int c, int sz, int b, int f);

int _Cut(FILE *stream, char *address, int l, int c, int sz, int b, int f);

int _Paste(FILE *stream, char *address, int l, int c);

int _Undo(FILE *stream, char *addres);

int _Tree(FILE *stream, int depth);

int _Compare(FILE *stream, char *address1, char *address2);

int _AutoIndent(FILE *stream, char *address);

int _Grep(FILE *stream, char **addresses, char *pattern, int c, int l);

int _Find(FILE *stream, char *address, char *pattern, int count, int at, int atn, int byword, int all);

int _Replace(FILE *stream, char *address, char *pattern, char *replace, int at, int atn, int all);

#endif