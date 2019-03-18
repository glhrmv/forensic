#ifndef UTILS_H
#define UTILS_H

/* ========= USEFUL UTILITES ========= */


/* Returns a substring of 'str' that starts at 'start' and ends at 'finish' (not including finish)
   Returns NULL on error */
char * substr(int start, int finish, char * str); 

/* Checks if a file exists 
   Returns 1 if it does or 0 if it doesn't*/ 
int fileExists(const char* pathname);   


#endif