#ifndef UTILS_H
#define UTILS_H

/* ========= USEFUL UTILITES ========= */


/* Returns a substring of 'str' that starts at 'start' and ends at 'finish' (not including finish)
   Returns NULL on error */
char * substr(int start, int finish, char * str); 

/* Checks if the path given by 'pathname' is valid
   Returns 1 if it is or 0 if it isn't*/ 
int validPath(const char* pathname);   


#endif