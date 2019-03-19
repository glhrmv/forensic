#include "utils.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


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

/*
Saves the file names of the contents present in 'directory' in 'file_names'
and the file total in 'number_of_files'
*/
void saveFileNames(char *directory, char ***file_names, int *number_of_files)
{
    const int MAX_F_NAME_SIZE = 256;
    DIR *dir;
    struct dirent *dir_info;
    int n_files = 0;

    dir = opendir(directory);

    while ((dir_info = readdir(dir)))
    {
        if (strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0)
        {
            continue;
        }

        n_files++;

        *file_names = (char **)realloc(*file_names, sizeof(char *) * n_files);
        *(*file_names + n_files - 1) = (char *)malloc(sizeof(char) * MAX_F_NAME_SIZE);

        strcpy(*(*file_names + n_files - 1), directory);
        strcat(*(*file_names+ n_files - 1), "/");
        strcat(*(*file_names+ n_files - 1), dir_info->d_name);
    }

    closedir(dir);
    *number_of_files = n_files;
}