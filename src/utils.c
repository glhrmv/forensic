#include "utils.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


/* ========= USEFUL UTILITES ========= */

char *substr(int start, int finish, char *str)
{
    if (finish <= start || str == NULL)
        return NULL;

    char *substr = (char *)malloc((finish - start) * sizeof(char));
    memcpy(substr, str + start , (finish - start) * sizeof(char));
    substr[finish-start] = '\0';

    return substr;
}


int isFile(const char *pathname)
{
    struct stat path_stat;
    stat(pathname, &path_stat);

    return S_ISREG(path_stat.st_mode);
}

int isDirectory(const char *pathname)
{
    struct stat path_stat;
    stat(pathname, &path_stat);

    return S_ISDIR(path_stat.st_mode);
}