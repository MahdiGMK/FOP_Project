#include <shared/FileHandling.h>
#include <shared/StringLib.h>
#include "RawCommand.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int _CreateFile(char *address)
{
    if (address[0] != '/')
        return 1;
    FileHandlingStatus status = CreateFile(address + 1);
    if (status == FILE_EXISTED)
    {
        LOG("File Existed");
    }
    else
    {
        LOG("File Created");
    }
    return 0;
}

int _InsertStr(char *address, char *pattern, int l, int c)
{
    if (address[0] != '/' || pattern[0] == 0 || l < 1 || c < 0)
        return 1;

    char file[1000] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    ResolveSymbols(pattern);
    char *ptr = GetPtrAt(file, l, c);
    InsertPattern(ptr, pattern);
    SafeWriteFile(address + 1, file);
    LOG("File Edited Successfully");
    return 0;
}

int _CAT(char *address)
{
    if (address[0] != '/')
        return 1;
    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    LOG("%s", file);
    return 0;
}

int _RemoveStr(char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    EraseSubstring(ptr, sz);
    SafeWriteFile(address + 1, file);
    LOG("File Edited Successfully");
    return 0;
}

char clipboard[FILESIZE];

int _Copy(char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;
    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    strcpy(clipboard, ptr);
    clipboard[sz] = 0;
    LOG("Coppied Successfully");
    return 0;
}

int _Cut(char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    strcpy(clipboard, ptr);
    clipboard[sz] = 0;
    EraseSubstring(ptr, sz);
    SafeWriteFile(address + 1, file);
    LOG("Cutted Successfully");
    return 0;
}

int _Paste(char *address, int l, int c)
{
    if (address[0] != '/' || l < 1 || c < 0)
        return 1;

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    InsertPattern(ptr, clipboard);
    SafeWriteFile(address + 1, file);
    LOG("Pasted Successfully");
    return 0;
}

int _Undo(char *address)
{
    if (address[0] != '/')
        return 1;
    FileHandlingStatus status = Undo(address + 1);
    if (status == FILE_DIDNT_EXIST)
    {
        LOG("File Didn't Exist");
        return 0;
    }

    LOG("Successfull Undo");
    return 0;
}

int TraversDTree(char *path, char *prefix, int depth, int showAll, int mxdep)
{
    if (depth == mxdep)
        return 0;
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

            char p[ADDRSIZE];
            strcpy(p, path);
            p[plen] = '/';
            strcpy(p + plen + 1, dirs[i]);

            if (i < dsize + fsize - 1)
                strcpy(prefix + pos, "│    ");
            else
                strcpy(prefix + pos, "     ");
            TraversDTree(p, prefix, depth + 1, showAll, mxdep);
            prefix[pos] = 0;
        }
        else
            printf("%s\n", files[i - dsize]);
    }

    free(files);
    free(dirs);

    return 0;
}

int _Tree(int depth)
{
    if (depth < -1)
        return 1;
    char prefix[ADDRSIZE] = {};
    printf("root\n");
    TraversDTree("./root/", prefix, 0, 0, depth);
    fflush(stdout);
    return 0;
}