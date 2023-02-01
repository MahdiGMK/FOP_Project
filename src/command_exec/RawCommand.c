#include <shared/FileHandling.h>
#include <shared/StringLib.h>
#include "RawCommand.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int _CreateFile(FILE *stream, char *address)
{
    if (address[0] != '/')
        return 1;
    FileHandlingStatus status = CreateFile(address + 1);
    if (status == FILE_EXISTED)
    {
        OUTPUT("File Existed");
    }
    else
    {
        OUTPUT("File Created");
    }
    return 0;
}

int _InsertStr(FILE *stream, char *address, char *pattern, int l, int c)
{
    if (address[0] != '/' || pattern[0] == 0 || l < 1 || c < 0)
        return 1;

    char file[1000] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }
    ResolveSymbols(pattern);
    char *ptr = GetPtrAt(file, l, c);
    InsertPattern(ptr, pattern);
    SafeWriteFile(address + 1, file);
    OUTPUT("File Edited Successfully");
    return 0;
}

int _CAT(FILE *stream, char *address)
{
    if (address[0] != '/')
        return 1;
    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }
    OUTPUT("%s", file);
    return 0;
}

int _RemoveStr(FILE *stream, char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    EraseSubstring(ptr, sz);
    SafeWriteFile(address + 1, file);
    OUTPUT("File Edited Successfully");
    return 0;
}

char clipboard[FILESIZE];

int _Copy(FILE *stream, char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;
    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    strcpy(clipboard, ptr);
    clipboard[sz] = 0;
    OUTPUT("Coppied Successfully");
    return 0;
}

int _Cut(FILE *stream, char *address, int l, int c, int sz, int b, int f)
{
    if (address[0] != '/' || l < 1 || c < 0 || sz < 1 || !(b ^ f))
        return 1;

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    if (b)
        ptr -= sz;
    strcpy(clipboard, ptr);
    clipboard[sz] = 0;
    EraseSubstring(ptr, sz);
    SafeWriteFile(address + 1, file);
    OUTPUT("Cutted Successfully");
    return 0;
}

int _Paste(FILE *stream, char *address, int l, int c)
{
    if (address[0] != '/' || l < 1 || c < 0)
        return 1;

    char file[FILESIZE];
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }
    char *ptr = GetPtrAt(file, l, c);
    InsertPattern(ptr, clipboard);
    SafeWriteFile(address + 1, file);
    OUTPUT("Pasted Successfully");
    return 0;
}

int _Undo(FILE *stream, char *address)
{
    if (address[0] != '/')
        return 1;
    FileHandlingStatus status = Undo(address + 1);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }

    OUTPUT("Successfull Undo");
    return 0;
}

int TraversDTree(FILE *stream, char *path, char *prefix, int depth, int showAll, int mxdep)
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
        PRINT("%s", prefix);
        if (i == dsize + fsize - 1)
            PRINT("└────");
        else
            PRINT("├────");

        if (i < dsize)
        {
            PRINT("%s\n", dirs[i]);

            char p[ADDRSIZE];
            strcpy(p, path);
            p[plen] = '/';
            strcpy(p + plen + 1, dirs[i]);

            if (i < dsize + fsize - 1)
                strcpy(prefix + pos, "│    ");
            else
                strcpy(prefix + pos, "     ");
            TraversDTree(stream, p, prefix, depth + 1, showAll, mxdep);
            prefix[pos] = 0;
        }
        else
            PRINT("%s\n", files[i - dsize]);
    }

    free(files);
    free(dirs);

    return 0;
}

int _Tree(FILE *stream, int depth)
{
    if (depth < -1)
        return 1;
    char prefix[ADDRSIZE] = {};
    PRINT("root\n");
    TraversDTree(stream, "./root/", prefix, 0, 0, depth);
    OUTPUT(" ");
    return 0;
}

int max(int a, int b)
{
    return a > b ? a : b;
}
int _Compare(FILE *stream, char *address1, char *address2)
{
    if (address1[0] != '/' || address2[0] != '/')
        return 1;

    char file1[FILESIZE] = {}, file2[FILESIZE] = {};
    FileHandlingStatus status1 = ReadFileNoAlloc(address1 + 1, file1);
    FileHandlingStatus status2 = ReadFileNoAlloc(address2 + 1, file2);

    if (status1 == FILE_DIDNT_EXIST)
    {
        OUTPUT("File1 Didn't Exist");
        return 0;
    }
    if (status2 == FILE_DIDNT_EXIST)
    {
        OUTPUT("File2 Didn't Exist");
        return 0;
    }

    char *pt1 = file1, *pt2 = file2;
    int lsz = max(CountLines(file1), CountLines(file2));
    int lno = 0;
    while (++lno)
    {
        char *e1 = GetEndLine(pt1), *e2 = GetEndLine(pt2);
        char end1 = e1[0], end2 = e2[0];
        e1[0] = 0, e2[0] = 0;
        if (strcmp(pt1, pt2))
        {
            for (int i = 0; i < 20; i++)
                PRINT("=");
            PRINT(" #%d ", lno);
            for (int i = 0; i < 20; i++)
                PRINT("=");
            PRINT("\n");
            PRINT("%s\n", pt1);
            PRINT("%s\n", pt2);
        }
        e1[0] = end1, e2[0] = end2;
        if (e1[0] && e2[0])
            pt1 = e1 + 1, pt2 = e2 + 1;
        else
        {
            char *pt = NULL, simbol = 0;
            if (e1[0])
                pt = e1 + 1, simbol = '<';
            if (e2[0])
                pt = e2 + 1, simbol = '>';
            if (pt == NULL)
                break;

            for (int i = 0; i < 20; i++)
                PRINT("%c", simbol);
            PRINT(" #%d - #%d ", lno + 1, lsz);
            for (int i = 0; i < 20; i++)
                PRINT("%c", simbol);
            PRINT("\n");
            PRINT("%s\n", pt);
            break;
        }
    }
    return 0;
}

int _AutoIndent(FILE *stream, char *address)
{
    if (address[0] != '/')
        return 1;
    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }
    char res[FILESIZE] = {};

    int indlevel = 0;
    char *ptr = file, *wptr = res;
    int isLContent = 0, spc = 0;
    int addnl = 1;
    while (ptr[0])
    {
        switch (ptr[0])
        {
        case '\n':
            if (addnl)
                wptr[0] = '\n', wptr++;

            isLContent = spc = 0, addnl = 1;
            break;
        case ' ':
            spc++;
            break;
        case '\t':
            spc += 4;
            break;

        case '{':
            for (int i = 0; i < (isLContent ? 1 : 4 * indlevel); i++)
                wptr[0] = ' ', wptr++;
            wptr[0] = '{', wptr++;
            wptr[0] = '\n', wptr++;

            indlevel++;
            spc = isLContent = 0, addnl = 0;
            break;
        case '}':
            indlevel--;
            indlevel = max(indlevel, 0);

            if (isLContent)
                wptr[0] = '\n', wptr++;
            for (int i = 0; i < 4 * indlevel; i++)
                wptr[0] = ' ', wptr++;
            wptr[0] = '}', wptr++;
            wptr[0] = '\n', wptr++;

            spc = isLContent = 0, addnl = 0;
            break;

        default:
            for (int i = 0; i < (isLContent ? spc : 4 * indlevel); i++)
                wptr[0] = ' ', wptr++;
            wptr[0] = ptr[0], wptr++;

            isLContent = 1, spc = 0, addnl = 1;
            break;
        }
        ptr++;
    }

    SafeWriteFile(address + 1, res);
    OUTPUT("AutoIndented Successfully");
    // PRINT("%s\n", res);
}

int _Grep(FILE *stream, char **addresses, char *pattern, int c, int l)
{
    if (pattern[0] == 0)
        return 1;
    int cnt = 0;
    int psz = strlen(pattern);
    int lnok[FILESIZE] = {};

    for (; addresses[0]; addresses++)
    {
        char file[FILESIZE] = {};
        FileHandlingStatus status = ReadFileNoAlloc(addresses[0] + 1, file);
        if (status == FILE_DIDNT_EXIST)
        {
            OUTPUT("%s: File Didn't Exist", addresses[0]);
            continue;
        }
        int fsz = strlen(file);
        if (fsz < psz)
            continue;

        int fok = 0;

        char *ptr = file;
        int ln = 1;
        do
        {
            char tmp = ptr[psz];
            ptr[psz] = 0;
            if (strcmp(ptr, pattern) == 0)
            {
                cnt += fok = lnok[ln] = 1;
                ptr[psz] = tmp;
                ptr = GetEndLine(ptr);
            }
            else
                ptr[psz] = tmp;
            ln += ptr[0] == '\n';
        } while (ptr++[psz]);

        if (fok)
        {
            if (l)
            {
                PRINT("%s\n", addresses[0]);
            }
            else if (!c)
            {
                ln = 1, ptr = file;
                while (1)
                {
                    char *lnend = GetEndLine(ptr);
                    if (lnok[ln])
                    {
                        char end = lnend[0];
                        lnend[0] = 0;
                        PRINT("%s:%d :: %s\n", addresses[0], ln, ptr);
                        lnok[ln] = 0;
                        lnend[0] = end;
                    }
                    if (!lnend[0])
                        break;
                    ptr = lnend + 1, ln++;
                }
            }
        }
    }
    if (c)
    {
        PRINT("%d\n", cnt);
    }
    return 0;
}

int _Find(FILE *stream, char *address, char *pattern, int count, int at, int atn, int byword, int all)
{
    if (address[0] != '/' || pattern[0] == 0 || (all && at))
        return 1;

    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }

    int psz = strlen(pattern);
    int begs = pattern[0] == '*';
    if (begs)
        pattern++, psz--;
    int ends = pattern[psz - 1] == '*' && (psz <= 1 || pattern[psz - 2] != '\\');
    if (ends)
        pattern[psz - 1] = 0, psz--;

    if (!at)
        atn = 1;
    int cnt = 0;
    char *lprint = NULL;
    for (char *ptr = FindPattern(file, pattern); ptr; ptr = FindPattern(ptr + 1, pattern))
    {
        int wind = GetWordIndex(file, ptr - begs);
        for (char *pptr = begs ? GetWordBegin(file, ptr - 1) : ptr; pptr <= ptr; pptr++)
        {
            int val = pptr - file;
            if (byword)
                val = wind;
            cnt++;
            if (all)
            {
                if (cnt == 1)
                    PRINT("%d", val);
                else
                    PRINT(", %d", val);
            }
            else
            {
                if (cnt == atn)
                {
                    PRINT("%d", val);
                    goto done;
                }
            }
        }
    }
    if (cnt < atn)
        PRINT("-1");
done:
    OUTPUT(" ");

    if (count)
    {
        OUTPUT("count : %d", cnt);
    }
    return 0;
}

int _Replace(FILE *stream, char *address, char *pattern, char *replace, int at, int atn, int all)
{
    if (address[0] != '/' || pattern[0] == 0 || (all && at))
        return 1;

    char file[FILESIZE] = {};
    FileHandlingStatus status = ReadFileNoAlloc(address + 1, file);
    if (status == FILE_DIDNT_EXIST)
    {
        OUTPUT("File Didn't Exist");
        return 0;
    }

    int psz = strlen(pattern);
    int begs = pattern[0] == '*';
    if (begs)
        pattern++, psz--;
    int ends = pattern[psz - 1] == '*' && (psz <= 1 || pattern[psz - 1] != '\\');
    if (ends)
        pattern[psz - 1] = 0, psz--;

    if (!at)
        atn = 1;
    int cnt = 0;
    char *lprint = NULL;
    for (char *ptr = FindPattern(file, pattern); ptr; ptr = FindPattern(ptr + 1, pattern))
    {
        if (all)
        {
            char *begpt = ptr;
            char *endpt = ptr + psz;
            if (begs)
                begpt = GetWordBegin(file, begpt - 1);
            if (ends)
                endpt = GetWordEnd(endpt);

            EraseSubstring(begpt, endpt - begpt);
            InsertPattern(begpt, replace);
            ptr = begpt;
        }
        else
            for (char *pptr = begs ? GetWordBegin(file, ptr - 1) : ptr; pptr <= ptr; pptr++)
            {
                cnt++;
                if (cnt == atn)
                {
                    char *begpt = pptr;
                    char *endpt = pptr + psz;
                    if (begs)
                        begpt = GetWordBegin(file, begpt - 1);
                    if (ends)
                        endpt = GetWordEnd(endpt);

                    EraseSubstring(begpt, endpt - begpt);
                    InsertPattern(begpt, replace);
                    goto done;
                }
            }
    }
    if (cnt < atn)
    {
        OUTPUT("Invalid Index");
        return 0;
    }
done:
    SafeWriteFile(address + 1, file);
    OUTPUT("Replacement Successful");
    return 0;
}