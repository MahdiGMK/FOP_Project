#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char prefix[10000] = {};
    TraversDTree("/home/mahdigmk/Desktop", prefix, 0, 0);
}