#include "programconfig.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

/* Prints file's hashes according to the algorithm flags present in program_config */
int printFileHash(const char *pathname, ProgramConfig program_config);

/*Creates a new process that executes the hashing algorithm while the parent process waits for him*/
int execAlgorithm(const char *pathname, const char *algorithm);


/* Main */
int main(int argc, char **argv)
{
  ProgramConfig config;
  static char *usage = "usage: %s [-r] [-h [md5[,sha1[,sha256]]]] [-o <outfile>] [-v] <file|dir>\n";

  // In case of invalid options
  if (setProgramConfig(argc, argv, &config) < 0 || argc == 1)
  {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  // Print ExecutionInfo's state if the debug flag is on
  if (config.debug_flag)
  {
    printProgramConfig(config);
  }

  if (isDirectory(config.arg))
  {
    char **files = NULL;
    int n_files = 0;

    saveFileNames(config.arg, &files, &n_files);

    for (int i = 0; i < n_files; i++)
    {
      if (isFile(files[i]))
      {
        printFileHash(files[i], config);
        printf("\n");
      }
    }
  }

  
  else if (isFile(config.arg))
  {
    printFileHash(config.arg, config);
  }

  // Prints the name of the entry directory/file
  printf("entry file/directory: %s \n", config.arg);

  exit(0);
}




int execAlgorithm(const char *pathname, const char *algorithm)
{

  int forkResult = fork();

  if (forkResult < 0)
  {
    return -1;
  }
  else if (forkResult == 0)
  {
    execlp(algorithm, algorithm,"--tag", pathname, NULL); // --tag is just for debugging purposes
    return -1;
  }
  else
  {
    wait(NULL);
  }


  return 0;
}



int printFileHash(const char *pathname, ProgramConfig program_config)
{

  int numberOfAlgorithms = program_config.h_alg_md5_flag + program_config.h_alg_sha1_flag + program_config.h_alg_sha256_flag;


  if (program_config.h_alg_md5_flag)
  {
   

    if (execAlgorithm(pathname, "md5sum") < 0)
      return -1;

    numberOfAlgorithms--;

    if (numberOfAlgorithms != 0)
      write(STDOUT_FILENO, ",", 2);
 
  }

  if (program_config.h_alg_sha1_flag)
  {

    if (execAlgorithm(pathname, "sha1sum") < 0 )
      return -1;

    numberOfAlgorithms--;

    if (numberOfAlgorithms != 0)
      write(STDOUT_FILENO, ",", 2);
  }

  if (program_config.h_alg_sha256_flag)
  {

    if (execAlgorithm(pathname, "sha256sum") < 0)
      return -1;

  }

  return 0;
}
