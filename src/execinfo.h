#ifndef EXECINFO_H
#define EXECINFO_H


/* Struct with all the possible options that the user may select */
struct executionInfo {

/* execution flags */
int recursive;
int identifier;
int log;
int redirect;
int debug;

/* execution variables */
char * idAlgorithm;
char * outFile;
};

/* Resets all of the struct's values */
void emptyExecInfo(struct executionInfo * execInfo);

/* Resets the struct's values and then sets them according to the argument given by the command line */
int setExecInfo(int argc, char *argv[], struct executionInfo * execInfo);

/* Prints the struct's values */
void printExecInfo(struct executionInfo execInfo);


#endif
