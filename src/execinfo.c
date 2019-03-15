#include "execinfo.h"
#include "utils.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int emptyExecInfo(struct executionInfo *execInfo)
{
    if (execInfo == NULL)
        return -1;

    /* Set all the flags to 0 */
    execInfo->recursive = 0;
    execInfo->identifier = 0;
    execInfo->log = 0;
    execInfo->redirect = 0;
    execInfo->debug = 0;

    execInfo->md5 = 0;
    execInfo->sha1 = 0;
    execInfo->sha256 = 0;

    /* Why not */
    execInfo->idAlgorithms = NULL;
    execInfo->outFile = NULL;

    return 0;
}

int setIdAlgorithmFlags(char *idAlgorithms, struct executionInfo *execInfo)
{
    if (idAlgorithms == NULL || execInfo == NULL)
        return -1;

    char *str = NULL;
    int previousWordEnd = 0;

    for (size_t i = 0; i <= strlen(idAlgorithms); i++)
    {
        if (idAlgorithms[i] == ',' || idAlgorithms[i] == '\0' )
        {
            str = substr(previousWordEnd, i, idAlgorithms);

            if (str == NULL)
                return -1;

            if (strcmp("md5", str) == 0)
            {
                execInfo->md5 = 1;
            }
            else if (strcmp("sha1", str) == 0)
            {
                execInfo->sha1 = 1;
            }
            else if (strcmp("sha256", str) == 0)
            {
                execInfo->sha256 = 1;
            }
            else 
            {
                return -1;
            }

            free(str);
            str = NULL;
            i++;
            previousWordEnd = i;
        }
    }

    return 0;
}

int setExecInfo(int argc, char *argv[], struct executionInfo *execInfo)
{
    /* reset all the values */
    if (emptyExecInfo(execInfo) < 0)
        return -1;

    int c;
    extern int optind;
    extern char *optarg;

    /* set the values according to the arguments given via the command line */
    while ((c = getopt(argc, argv, "drh:o:v")) != -1)
    {
        switch (c)
        {
        case 'd':
            execInfo->debug = 1;
            break;

        case 'r':
            execInfo->recursive = 1;
            break;

        case 'h':
            execInfo->identifier = 1;
            execInfo->idAlgorithms = optarg;
            if (setIdAlgorithmFlags(execInfo->idAlgorithms, execInfo) < 0)
                return -1;

            break;

        case 'o':
            execInfo->redirect = 1;
            execInfo->outFile = optarg;
            break;

        case 'v':
            execInfo->log = 1;
            break;

        case '?':
            return -1;
            break;
        }
    }

    return 0;
}

void printExecInfo(struct executionInfo execInfo)
{
    printf("r_flag = %d\n", execInfo.recursive);
    printf("h_flag = %d\n", execInfo.identifier);
    printf("h_algorithms = %s \n", execInfo.idAlgorithms);
    printf("md5_flag = %d \n", execInfo.md5);
    printf("sha1_flag = %d \n", execInfo.sha1);
    printf("sha256_flag = %d \n", execInfo.sha256);
    printf("o_flag = %d\n", execInfo.redirect);
    printf("o_output_name = %s \n", execInfo.outFile);
    printf("v_flag = %d\n", execInfo.log);
    printf("d_flag = %d \n", execInfo.debug);
}
