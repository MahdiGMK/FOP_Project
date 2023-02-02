#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define ADDRSIZE (1000)
#define SUBDCNT (1000)

int main()
{
    char *stream = "10 20";

    int n1, n2;

    sscanf(stream, "%d", &n1);
    sscanf(stream, "%d", &n2);

    printf("%d %d", n1, n2);
}