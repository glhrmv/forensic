#ifndef EXECINFO_H
#define EXECINFO_H

extern const char * LOG_ENV_NAME;



/* Struct with all the possible options that the user may select */
struct executionInfo {

/* Execution flags */
int recursive_f;
int identifier_f;
int log_f;
int redirect_f;
int debug_f;

/* ID Algorithms flags */
int md5_f;
int sha1_f;
int sha256_f;

/* Execution variables */
char * idAlgorithms;
char * outFile;

/* Environment variables */
char * logOutFile;

};



/* Resets all of the struct's values 
   Return -1 on error or 0 otherwise */
int emptyExecInfo(struct executionInfo * execInfo);

/* Recieves a string with the following format "md5,sha1,sha256" and sets the ID Algorithms flags of execInfo accordingly.
   Returns -1 on error or 0 otherwise */
int setIdAlgorithmFlags(char * idAlgorithms, struct executionInfo * execInfo);

/* Resets the struct's values and then sets them according to the argument given by the command line. 
   Returns -1 on error or 0 otherwise */
int setExecInfo(int argc, char *argv[], struct executionInfo * execInfo);

/* Prints the struct's values */
void printExecInfo(struct executionInfo execInfo);


#endif
