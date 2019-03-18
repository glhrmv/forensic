#ifndef UTILS_H
#define UTILS_H

/* ========= STRINGS ========= */

/* Returns a substring of 'str' that starts at 'start' and ends at 'finish' (not including finish)
   Returns NULL on error */
char * substr(int start, int finish, char * str); 



/* ========= FILES / DIRECTORIES ========= */

/* Checks if the path given by 'pathname' is a file
    Returns 1 if it is or 0 if it isn't*/
int isFile(const char* pathname); 


/* Checks if the path given by 'pathname' is a directory
    Returns 1 if it is or 0 if it isn't*/
int isDirectory(const char* pathname); 


/*
Saves the file names of the contents present in 'directory' in 'file_names'
and the file total in 'number_of_files'
*/
void saveFileNames(char *directory, char ***file_names, int *number_of_files);


#endif