#include "execinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>


int main(int argc, char **argv)
{

  struct executionInfo execInfo;

  /* Using getopt to parse args */
  static char *usage = "usage: %s [-r] [-h [md5[,sha1[,sha256]]]] [-o <outfile>] [-v] <file|dir>\n";
 
  int err = setExecInfo(argc, argv, &execInfo);

  // In case of invalid option
  if (err || argc == 1)
  {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  // Print options
  if (execInfo.debug)
  {
    printExecInfo(execInfo);
  }

  // Argument passed
  fprintf(stdout, "file or folder: %s\n", argv[argc - 1]);

  exit(0);
}
