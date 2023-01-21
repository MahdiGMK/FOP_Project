#include <stdio.h>
#include "FileHandling.h"
#include <stdlib.h>

char *ReadFile(const char *fileAddress, int bufferSize)
{
    char *str = malloc(bufferSize);
    ReadFileNoAlloc(fileAddress, str);
    return str;
}

void ReadFileNoAlloc(const char *fileAddress, char *str)
{
    FILE *rfile = fopen(fileAddress, "r");
    char *ptr = str;
    while (1)
    {
        *ptr = fgetc(rfile);

        if (*ptr == EOF)
        {
            *ptr = 0;
            break;
        }
        ptr++;
    }
    fclose(rfile);
}

void WriteFile(const char *fileAddress, char *str)
{
    FILE *wfile = fopen(fileAddress, "w");
    fprintf(wfile, "%s", str);
    fclose(wfile);
}