#include "programconfig.h"
#include "utils.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *LOG_ENV_NAME = "LOGFILENAME";

int emptyProgramConfig(ProgramConfig * program_config)
{
    if (program_config == NULL)
        return -1;

    /* Set all the flags to 0 */
    program_config->r_flag = 0;
    program_config->h_flag = 0;
    program_config->v_flag = 0;
    program_config->o_flag = 0;
    program_config->debug_flag = 0;

    program_config->h_alg_md5_flag = 0;
    program_config->h_alg_sha1_flag = 0;
    program_config->h_alg_sha256_flag = 0;

    program_config->logfile = 0;

    /* Why not */
    program_config->h_value = NULL;
    program_config->o_value = NULL;
    program_config->arg = NULL;

    return 0;
}


int setIdAlgorithmFlags(char *h_value, ProgramConfig *program_config)
{
    if (h_value == NULL || program_config == NULL)
        return -1;

    char *str = NULL;
    int previousWordEnd = 0;

    for (size_t i = 0; i <= strlen(h_value); i++)
    {
        if (h_value[i] == ',' || h_value[i] == '\0')
        {
            str = substr(previousWordEnd, i, h_value);

            if (str == NULL)
                return -1;

            if (strcmp("md5", str) == 0)
            {
                program_config->h_alg_md5_flag = 1;
            }
            else if (strcmp("sha1", str) == 0)
            {
                program_config->h_alg_sha1_flag = 1;
            }
            else if (strcmp("sha256", str) == 0)
            {
                program_config->h_alg_sha256_flag = 1;
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

int setProgramConfig(int argc, char *argv[], ProgramConfig * program_config)
{
    /* reset all the values */
    if (emptyProgramConfig(program_config) < 0)
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
            program_config->debug_flag = 1;
            break;

        case 'r':
            program_config->r_flag = 1;
            break;

        case 'h':
            program_config->h_flag = 1;
            program_config->h_value = optarg;

            if (setIdAlgorithmFlags(program_config->h_value, program_config) < 0)
                return -1;

            break;

        case 'o':
            program_config->o_flag = 1;
            program_config->o_value = optarg;
            break;

        case 'v':
            program_config->v_flag = 1;
            program_config->logfile = getenv(LOG_ENV_NAME);

            if (program_config->logfile == NULL)
            {
                fprintf(stderr, "environment variable '%s' does not exist \n", LOG_ENV_NAME);
                return -1;
            }

            break;

        case '?':
            return -1;
            break;
        }
    }

    program_config->arg=argv[optind];

    if (!validPath(program_config->arg))
    {
        if (program_config->arg != NULL)
        {
            fprintf(stderr, "'%s' is not a file/directory \n", program_config->arg);
        }
        else
        {
            fprintf(stderr, "no file/directory given \n");
        }

        return -1;
    }

        return 0;
}

void printProgramConfig(ProgramConfig program_config)
{
    printf("r_flag = %d\n", program_config.r_flag);
    printf("h_flag = %d\n", program_config.h_flag);
    printf("h_algorithms = %s \n", program_config.h_value);
    printf("h_alg_md5_flag= %d \n", program_config.h_alg_md5_flag);
    printf("h_alg_sha1_flag = %d \n", program_config.h_alg_sha1_flag);
    printf("h_alg_sha256_flag = %d \n", program_config.h_alg_sha256_flag);
    printf("o_flag = %d\n", program_config.o_flag);
    printf("o_output_name = %s \n", program_config.o_value);
    printf("v_flag = %d\n", program_config.v_flag);
    printf("%s = %s \n", LOG_ENV_NAME, program_config.logfile);
    printf("d_flag = %d \n", program_config.debug_flag);
}
