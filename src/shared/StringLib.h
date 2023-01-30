#ifndef _STRING_LIB
#define _STRING_LIB

void InsertPattern(char *ptr, const char *pattern);

void EraseSubstring(char *ptr, int sz);

char *GetPtrAt(char *str, int ln, int cn);

char *GetEndLine(char *ptr);

void ResolveSymbols(char *str);

#endif