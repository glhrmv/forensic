#include "programconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

int main(int argc, char **argv)
{
  ProgramConfig config;
  static char *usage = "usage: %s [-r] [-h [md5[,sha1[,sha256]]]] [-o <outfile>] [-v] <file|dir>\n";
 
  // In case of invalid options
  if (setProgramConfig(argc,argv,&config) < 0 || argc == 1)
  {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  // Print ExecutionInfo's state if the debug flag is on
  if (config.debug_flag)
  {
    printProgramConfig(config);
  }


  // Prints the name of the entry directory/file
  printf("entry file/directory: %s \n", argv[argc - 1]);

  exit(0);
}
