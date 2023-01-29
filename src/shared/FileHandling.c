#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "FileHandling.h"
#include "IOLib.h"
#include "StringLib.h"
#include <string.h>
#include <dirent.h>

FileHandlingStatus ReadFileNoAlloc(const char *fileAddress, char *str)
{
    FILE *rfile = fopen(fileAddress, "r");
    if (rfile == NULL)
    {
        *str = 0;
        return FILE_DIDNT_EXIST;
    }

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
    return FILE_EXISTED;
}

FileHandlingStatus WriteFile(const char *fileAddress, char *str)
{
    CreateDirectory(fileAddress);

    FILE *cfile = fopen(fileAddress, "r");
    if (cfile)
        fclose(cfile);

    FILE *wfile = fopen(fileAddress, "w");
    fprintf(wfile, "%s", str);
    fclose(wfile);

    if (cfile == NULL)
        return FILE_CREATED;
    return FILE_EXISTED;
}

FileHandlingStatus CreateFile(const char *fileAddress)
{
    FILE *cfile = fopen(fileAddress, "r");
    if (cfile)
        fclose(cfile);

    if (cfile == NULL)
    {
        WriteFile(fileAddress, "");
        return FILE_CREATED;
    }
    return FILE_EXISTED;
}

FileHandlingStatus CreateDirectory(const char *fileAddress)
{
    char str[ADDRSIZE] = {};
    int existed = 0;
    for (int i = 0; i < 100; i++)
        if (fileAddress[i] == 0)
            break;
        else
        {
            if (fileAddress[i] == '/')
                existed = mkdir(str, 0777) == -1;
            str[i] = fileAddress[i];
        }
    if (existed)
        return FILE_EXISTED;
    return FILE_CREATED;
}

FileHandlingStatus CreateBackup(const char *fileAddress)
{
    char hiddenAddress[ADDRSIZE];
    hiddenAddress[0] = '.';
    strcpy(hiddenAddress + 1, fileAddress);

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(fileAddress, file);
    WriteFile(hiddenAddress, file);
    return status;
};

FileHandlingStatus Undo(const char *fileAddress)
{
    char hiddenAddress[ADDRSIZE];
    hiddenAddress[0] = '.';
    strcpy(hiddenAddress + 1, fileAddress);

    char file[FILESIZE], ufile[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(fileAddress, file);
    FileHandlingStatus ustatus = ReadFileNoAlloc(hiddenAddress, ufile);
    if (ustatus == FILE_DIDNT_EXIST)
        return FILE_DIDNT_EXIST;
    WriteFile(fileAddress, ufile);
    if (status == FILE_EXISTED)
        WriteFile(hiddenAddress, file);
    return FILE_EXISTED;
}

FileHandlingStatus SafeWriteFile(const char *fileAddress, char *str)
{
    CreateBackup(fileAddress);
    return WriteFile(fileAddress, str);
}

int TraversDTree(char *path, char *prefix, int depth, int showAll)
{
    int plen = strlen(path);

    char(*files)[ADDRSIZE] = malloc(sizeof(char[ADDRSIZE]) * SUBDCNT);
    int fsize = 0;
    char(*dirs)[ADDRSIZE] = malloc(sizeof(char[ADDRSIZE]) * SUBDCNT);
    int dsize = 0;

    struct dirent *dent;
    DIR *srcdir = opendir(path);

    if (srcdir == NULL)
    {
        perror("opendir");
        return 1;
    }

    while ((dent = readdir(srcdir)) != NULL)
    {
        struct stat st;

        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;
        if (dent->d_name[0] == '.' && !showAll)
            continue;
        if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0)
        {
            perror(dent->d_name);
            continue;
        }

        if (S_ISDIR(st.st_mode))
            strcpy(dirs[dsize++], dent->d_name);
        else
            strcpy(files[fsize++], dent->d_name);
    }
    closedir(srcdir);

    int pos = strlen(prefix);
    for (int i = 0; i < dsize + fsize; i++)
    {
        printf("%s", prefix);
        if (i == dsize + fsize - 1)
            printf("└────");
        else
            printf("├────");

        if (i < dsize)
        {
            printf("%s\n", dirs[i]);

            char p[1000];
            strcpy(p, path);
            p[plen] = '/';
            strcpy(p + plen + 1, dirs[i]);

            if (i < dsize + fsize - 1)
                strcpy(prefix + pos, "│    ");
            else
                strcpy(prefix + pos, "     ");
            TraversDTree(p, prefix, depth + 1, showAll);
            prefix[pos] = 0;
        }
        else
            printf("%s\n", files[i - dsize]);
    }

    free(files);
    free(dirs);

    return 0;
}