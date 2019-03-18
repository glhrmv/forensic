#include "forensic.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char** argv) {
  /* Declare and initialize program config */
  ProgramConfig program_config = empty_program_config;

  /* Modify program_config according to argv */
  parse_args(argc, argv, &program_config);

  /* Begin processing */
  process(program_config);

  exit(0);
}

int file_exists(const char* pathname) {
  return access(pathname, F_OK);
}

int is_file(const char* pathname) {
 struct stat path_stat;
 stat(pathname, &path_stat);
 return S_ISREG(path_stat.st_mode); 
}

int is_directory(const char* pathname) {
 struct stat path_stat;
 stat(pathname, &path_stat);
 return S_ISDIR(path_stat.st_mode); 
}
void parse_args(int argc, char** argv, ProgramConfig* program_config) {
  /* Program usage */
  char* usage = "usage: %s [-r] [-h [md5[,sha1[,sha256]]]] [-o <outfile>] [-v] <file|dir>\n";

  /* Using getopt to parse args */
  extern char* optarg;
  extern int optind;
  int c, err = 0;

  while ((c = getopt(argc, argv, "drh:o:v")) != -1) {
    switch (c) {
      case 'd':
        program_config->debug_flag = 1;
        break;
      case 'r':
        program_config->r_flag = 1;
        break;
      case 'h':
        if (program_config->h_flag)
          fprintf(stdout, "(warning) -h is set multiple times\n");

        program_config->h_flag = 1;
        program_config->h_value = optarg;
        break;
      case 'o':
        if (program_config->o_flag)
          fprintf(stdout, "(warning) -o is set multiple times\n");

        program_config->o_flag = 1;
        program_config->o_value = optarg;
        break;
      case 'v':
        program_config->v_flag = 1;

        if (getenv("LOGFILENAME")) {
          program_config->logfile = getenv("LOGFILENAME");
        } else {
          printf("LOGFILENAME env variable not set, using 'log.txt' by default\n");
          program_config->logfile = "log.txt";
        }
        break;
      case '?':
        err = 1;
        break;
    }
  }

  /* If invalid option */
  if (err) {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  /* If no arg (no file/dir specified) */
  if (optind == argc) {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  } 

  /* If h option set */
  if (program_config->h_flag) {
    /* Parse the comma-delimited string */
    char* str_to_parse = malloc(strlen(program_config->h_value) + 1);
    strcpy(str_to_parse, program_config->h_value);

    char* token;
    while ((token = strsep(&str_to_parse, ","))) {
      if (strcmp(token, "md5") == 0) {
        program_config->h_alg_md5_flag = 1;
        continue;
      }

      if (strcmp(token, "sha1") == 0) {
        program_config->h_alg_sha1_flag = 1;
        continue;
      }

      if (strcmp(token, "sha256") == 0) {
        program_config->h_alg_sha256_flag = 1;
        continue;
      }

      fprintf(stderr, "(warning) -h : invalid hash algorithm sent: \"%s\" (ignored)\n", token);
    }

    /* Delete created copy */
    free(str_to_parse);

    /* If none of the valid algorithms were input */
    if (!program_config->h_alg_md5_flag && !program_config->h_alg_sha1_flag && !program_config->h_alg_sha256_flag) {
      fprintf(stderr, "-h : no valid option found\n");
      fprintf(stderr, usage, argv[0]);
      exit(1);
    }
  }

  /* If o option set */
  if (program_config->o_flag) {
    /* Check if given file already exists */
    if (file_exists(program_config->o_value) == 0) {
      fprintf(stderr, "-o %s : file already exists\n", program_config->o_value);
      exit(1);
    }
  }

  /* The first argument after options is what we want */
  char* arg = argv[optind];

  /* Check if it's something that actually exists */
  if (file_exists(arg) != 0) {
    fprintf(stderr, "%s : file/directory does not exist\n", argv[optind]);
    exit(1);
  } 

  /* Check if it's either a file or a directory (i.e., not socket, FIFO, symlink, etc.) */
  if (is_file(arg) != 0 && is_directory(arg) != 0) {
    fprintf(stderr, "%s : that's not a file/directory\n", argv[optind]);
    exit(1);
  } 

  /* All good: set it */
  program_config->arg = arg;

  /* Print options (when debug flag on) */
  if (program_config->debug_flag) {
    printf("debug_flag = %d\n", program_config->debug_flag); 
    printf("r_flag = %d\n", program_config->r_flag); 
    printf("h_flag = %d\n", program_config->h_flag); 
    printf("h_value = \"%s\"\n", program_config->h_value); 
    printf("h_alg_md5_flag = %d\n", program_config->h_alg_md5_flag);
    printf("h_alg_sha1_flag = %d\n", program_config->h_alg_sha1_flag);
    printf("h_alg_sha256_flag = %d\n", program_config->h_alg_sha256_flag);
    printf("o_flag = %d\n", program_config->o_flag); 
    printf("o_value = \"%s\"\n", program_config->o_value);
    printf("v_flag = %d\n", program_config->v_flag); 
    printf("logfile = \"%s\"\n", program_config->logfile); 
    printf("arg value = \"%s\"\n", program_config->arg); 
  }
}

void process(const ProgramConfig program_config) {
  fprintf(stdout, "file or folder: %s\n", program_config.arg);

  /* TOOD: Begin processing argument passed */
}
