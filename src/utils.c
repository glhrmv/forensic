#include "utils.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>


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


int fileExists(const char* pathname)
{
    return !access(pathname, F_OK);
}