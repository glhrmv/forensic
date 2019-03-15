#include "execinfo.h"

#include <unistd.h>
#include <stdio.h>

void emptyExecInfo(struct executionInfo *execInfo)
{
    /* Set all the flags to 0 */
    execInfo->recursive = 0;
    execInfo->identifier = 0;
    execInfo->log = 0;
    execInfo->redirect = 0;
    execInfo->debug = 0;

    /* Why not */
    execInfo->idAlgorithm = NULL;
    execInfo->outFile = NULL;
}

int setExecInfo(int argc, char *argv[], struct executionInfo *execInfo)
{
    /* reset all the values */
    emptyExecInfo(execInfo);

    int c;
    extern int optind;
    extern  char * optarg;

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
            execInfo->idAlgorithm = optarg;

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
    printf("h_algorithm_name = \"%s\"\n", execInfo.idAlgorithm);
    printf("o_flag = %d\n", execInfo.redirect);
    printf("o_output_name = \"%s\"\n", execInfo.outFile);
    printf("v_flag = %d\n", execInfo.log);
    printf("d_flag = %d \n", execInfo.debug);
}
