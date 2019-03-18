#ifndef PROGRAMCONFIG_H
#define PROGRAMCONFIG_H

extern const char * LOG_ENV_NAME;

/**
 * @brief A structure to represent the program's configuration.
 */
typedef struct ProgramConfig
{

    /* Execution flags */
    int debug_flag; // Debug flag
    int r_flag;     // Recursion flag (for directories)
    int h_flag;     // Hash flag
    int o_flag;     // Output file flag
    int v_flag;     // Verbose flag (to use logfile)

    /* Hash Algorithms flags */
    int h_alg_md5_flag;    // MD5 hash algorithm flag
    int h_alg_sha1_flag;   // SHA1 hash algorithm flag
    int h_alg_sha256_flag; // SHA256 hash algorithm flag

    /* Execution variables */
    char *h_value; // Hash option value (comma-delimited string)
    char *o_value; // Output file option value
    char *arg;     // Argument passed to program (file or directory)

    /* Environment variables */
    char *logfile; // Logfile to be used

} ProgramConfig;

/* Resets all of the struct's values 
   Return -1 on error or 0 otherwise */
int emptyProgramConfig(ProgramConfig * program_config);

/* Recieves a string with the following format "md5,sha1,sha256" and sets the ID Algorithms flags of program_config accordingly.
   Returns -1 on error or 0 otherwise */
int setIdAlgorithmFlags(char * idAlgorithms, ProgramConfig * program_config);

/* Resets the struct's values and then sets them according to the argument given by the command line. 
   Returns -1 on error or 0 otherwise */
int setProgramConfig(int argc, char *argv[], ProgramConfig * program_config);

/* Prints the struct's values */
void printProgramConfig(ProgramConfig program_config);


#endif
