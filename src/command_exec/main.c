#include <stdio.h>
#include <stdlib.h>
#include <shared/FileHandling.h>

int main()
{
    char *file = ReadFile("file.txt", 1000);
    WriteFile("cpfile.txt", file);
}
