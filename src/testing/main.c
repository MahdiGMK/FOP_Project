#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int TraversDTree(char *path, char *prefix, int depth, int showAll)
{
    // printf("%s\n", prefix);

    int plen = strlen(path);

    char(*files)[100] = malloc(sizeof(char[100]) * 1000);
    int fsize = 0;
    char(*dirs)[100] = malloc(sizeof(char[100]) * 1000);
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

            int pos = strlen(prefix);
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

int main()
{
    char prefix[10000] = {};
    TraversDTree("/home/mahdigmk/Desktop", prefix, 0, 0);
}