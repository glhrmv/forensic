#include "forensic.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Pipe read/write indicators */
#define READ 0
#define WRITE 1

/* Program start timestamp declaration */
struct timeval start_time;

/* Global counters */
size_t files_processed = 0;
size_t dirs_processed = 0;

/* Starting process identifier */
pid_t main_pid = -1;

void sigusr1_handler(int sig) {
  if (sig != SIGUSR1) {
    fprintf(stderr, "Wrong signal received! Expected: SIGUSR1\n");
    fflush(stderr);
    exit(1);
  }

  dirs_processed++;

  fprintf(stdout, "New directory: ");
  fprintf(stdout, "%zu dirs / %zu files processed ", dirs_processed, files_processed);
  fprintf(stdout, "so far\n");
  fflush(stdout);
}

void sigusr2_handler(int sig) {
  if (sig != SIGUSR2) {
    fprintf(stderr, "Wrong signal received! Expected: SIGUSR2\n");
    fflush(stderr);
    exit(1);
  }

  files_processed++;
}

void int_handler(int sig) {
  if (sig != SIGINT) {
    fprintf(stderr, "Wrong signal received! Expected: SIGINT\n");
    fflush(stderr);
    exit(1);
  }

  if (getpid() == main_pid) {
    /* Wait for possible pending operations */
    usleep(62500);
    fprintf(stdout, "Interrupted. Exiting...\n");
    fflush(stdout);
  }

  exit(0);
}

int main(int argc, char **argv) {
  /* Assign main_pid to starting process identifier */
  main_pid = getpid();

  /* Registering signal handlers */
  struct sigaction signals;
  sigemptyset(&signals.sa_mask);
  signals.sa_flags = 0;

  /* USR1 */
  signals.sa_handler = sigusr1_handler;
  sigaction(SIGUSR1, &signals, NULL);

  /* USR2 */
  signals.sa_handler = sigusr2_handler;
  sigaction(SIGUSR2, &signals, NULL);

  /* SIGINT */
  signals.sa_handler = int_handler;
  sigaction(SIGINT, &signals, NULL);

  /* Program start timestamp assignment */
  gettimeofday(&start_time, 0);

  /* Declare and initialise program configuration */
  ProgramConfig program_config = empty_program_config;

  /* Modify program_config according to argv */
  parse_args(argc, argv, &program_config);

  /* Begin processing */
  process(program_config);

  while (waitpid(0, NULL, WNOHANG) != -1)
    ;

  /* Everything went well */
  exit(0);
}

int file_exists(const char *pathname) { 
  return access(pathname, F_OK); 
  }

int is_file(const char *pathname) {
  struct stat path_stat;
  return (stat(pathname, &path_stat) == 0 && S_ISREG(path_stat.st_mode)) ? 0 : -1;
}

int is_directory(const char *pathname) {
  struct stat path_stat;
  return (stat(pathname, &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) ? 0 : -1;
}

char *time_to_iso_str(const time_t tm) {
  char *buffer = malloc(256);
  strftime(buffer, 256, "%FT%T", localtime(&tm));
  return buffer;
}

char *command_to_str(const char *fmt, const char *arg) {
  /* Fill out command string according to format */
  char command[256];
  sprintf(command, fmt, arg);

  /* Open pipe & run command */
  FILE *fp = popen(command, "r");

  if (fp == NULL) {
    fprintf(stderr, "error running command\n");
    exit(1);
  }

  /* Fill char* */
  char *str = malloc(256);
  fgets(str, 256, fp);
  /* Remove trailing newline */
  str[strlen(str) - 1] = '\0';

  /* Close pipe */
  pclose(fp);

  return str;
}

void log_event(const char *act) {
  FILE *logfile = fopen(getenv("LOGFILENAME"), "a");

  /* Measure time until this point */
  struct timeval this_time;
  gettimeofday(&this_time, 0);
  float elapsed;
  elapsed = (this_time.tv_sec - start_time.tv_sec) * 1000.0f;
  elapsed += (this_time.tv_usec - start_time.tv_usec) / 1000.0f;

  /* Log the event */
  fprintf(logfile, "%.2f - %08d - %s\n", elapsed, getpid(), act);

  fclose(logfile);
}

void parse_args(int argc, char **argv, ProgramConfig *program_config) {
  /* Program usage */
  const char *usage = "usage: %s [-r] [-h [md5[,sha1[,sha256]]]] [-o <outfile>] [-v] <file|dir>\n";

  /* Using getopt to parse args */
  extern char *optarg;
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
    char *str_to_parse = malloc(strlen(program_config->h_value) + 1);
    strcpy(str_to_parse, program_config->h_value);

    char *token;
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
    if (!program_config->h_alg_md5_flag && !program_config->h_alg_sha1_flag &&
        !program_config->h_alg_sha256_flag) {
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

  /* If v option set */
  if (program_config->v_flag) {
    /* If the env variable is not set, or if it does not point to a file */
    if (getenv("LOGFILENAME") == NULL || is_file(getenv("LOGFILENAME")) != 0) {
      printf("LOGFILENAME env variable not set, using 'log.txt' by default\n");
      setenv("LOGFILENAME", "log.txt", 1);
    }

    /* Don't allow the user to use the output file as the logfile or vice-versa  */
    if (program_config->o_flag) {
      if (strcmp(getenv("LOGFILENAME"), program_config->o_value) == 0) {
        fprintf(stderr, "cannot use the logfile as the output file or vice-versa\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
      }
    }
  }

  /* The first argument after options is what we want */
  char *arg = argv[optind];

  /* Check if it's something that actually exists */
  if (file_exists(arg) != 0) {
    fprintf(stderr, "%s : does not exist\n", argv[optind]);
    exit(1);
  }

  /* Check if it's either a file or a directory (i.e., not socket, FIFO, symlink, etc.) */
  if (is_file(arg) != 0 && is_directory(arg) != 0) {
    fprintf(stderr, "%s : not a file/directory\n", argv[optind]);
    exit(1);
  }

  /* All good: set it */
  program_config->arg = arg;

  /* Print options (when debug flag on) */
  if (program_config->debug_flag) {
    printf("--------------\n");
    printf("program config:\n");
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
    printf("--------------\n");
  }
}

void process(const ProgramConfig program_config) {
  /* If -v flag enabled, clear the logfile before proceeding */
  if (program_config.v_flag)
    fclose(fopen(getenv("LOGFILENAME"), "w"));

  /* Choose output stream (stdout if -o flag disabled) */
  FILE *outstream = program_config.o_flag ? fopen(program_config.o_value, "w") : stdout;

  /* In case of a directory, strip the trailing '/' character from it if exists */
  if (program_config.arg[strlen(program_config.arg) - 1] == '/')
    program_config.arg[strlen(program_config.arg) - 1] = 0;

  /* Are we processing a regular file, or a directory? */
  if (is_file(program_config.arg) == 0)
    process_file(program_config, program_config.arg, outstream);
  else
    process_dir(program_config, program_config.arg, outstream);

  /* Close output stream (if -o flag enabled) */
  if (program_config.o_flag) {
    fclose(outstream);
  }
}

void process_file(const ProgramConfig program_config, const char *fname, FILE *outstream) {
  sigset_t prevention;
  sigemptyset(&prevention);
  sigaddset(&prevention, SIGINT);
  sigprocmask(SIG_BLOCK, &prevention, NULL);

  kill(main_pid, SIGUSR2);

  /* Fill a file statistics struct from arg passed */
  struct stat file_stat;
  if (stat(fname, &file_stat) < 0) {
    fprintf(stderr, "an error occurred retrieving file details\n");
    exit(1);
  }

  /* Gather file data */
  const char *file_name = fname;
  off_t file_size = file_stat.st_size;
  char *file_type = command_to_str("file -b \"%s\" | cut -d ',' -f1", fname);
  char *file_modified_date = time_to_iso_str(file_stat.st_ctime);
  char *file_accessed_date = time_to_iso_str(file_stat.st_mtime);

  /* Print file data */
  fprintf(outstream, "%s,", file_name);
  fprintf(outstream, "%s,", file_type);
  fprintf(outstream, "%lld,", file_size);

  /* File permissions stuff */
  if (S_ISDIR(file_stat.st_mode))
    fprintf(outstream, "d");

  if (file_stat.st_mode & S_IRUSR)
    fprintf(outstream, "r");

  if (file_stat.st_mode & S_IWUSR)
    fprintf(outstream, "w");

  if (file_stat.st_mode & S_IXUSR)
    fprintf(outstream, "x");

  fprintf(outstream, ",%s,", file_modified_date);
  fprintf(outstream, "%s", file_accessed_date);

  /* Are we human or are we hashing? */
  if (program_config.h_flag) {
    if (program_config.h_alg_md5_flag) {
      /* Get MD5 checksum */
      // char* md5_hash = command_to_str("md5 \"%s\" | rev | cut -d ' ' -f1 | rev", fname); // macOS
      char *md5_hash = command_to_str("md5sum \"%s\" | rev | cut -d ' ' -f1 | rev", fname); // Linux
      fprintf(outstream, ",%s", md5_hash);
      free(md5_hash);
    }

    if (program_config.h_alg_sha1_flag) {
      /* Get SHA1 checksum */
      char *sha1_hash = command_to_str("shasum \"%s\" | awk '{print $1}'", fname);
      fprintf(outstream, ",%s", sha1_hash);
      free(sha1_hash);
    }

    if (program_config.h_alg_sha256_flag) {
      /* Get SHA256 checksum */
      char *sha256_hash = command_to_str("shasum -a 256 \"%s\" | awk '{print $1}'", fname);
      fprintf(outstream, ",%s", sha256_hash);
      free(sha256_hash);
    }
  }

  /* We're done, insert a newline */
  fprintf(outstream, "\n");

  /* Log this event */
  if (program_config.v_flag) {
    char act[100];
    sprintf(act, "PROCESSED FILE %s", fname);
    log_event(act);
  }

  /* Free dynamically allocated memory */
  free(file_type);
  free(file_modified_date);
  free(file_accessed_date);

  sigemptyset(&prevention);
  sigaddset(&prevention, SIGINT);
  sigprocmask(SIG_UNBLOCK, &prevention, NULL);
}

void process_dir(const ProgramConfig program_config, const char *dname, FILE *outstream) {
  kill(main_pid, SIGUSR1);

  /* Create a new process */
  pid_t pid = fork();

  if (pid == 0) {
    /* Child process */
    struct dirent *ent;
    DIR *dir;

    /* Open directory */
    if ((dir = opendir(dname)) != NULL) {
      /* Go through each file in this directory */
      while ((ent = readdir(dir)) != NULL) {
        /* Ignore anything that isn't a file or a directory */
        if (ent->d_type != DT_DIR && ent->d_type != DT_REG)
          continue;

        /* Ignore the '.' and '..' directories */
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
          continue;

        /* Prepend this directory name to file name */
        char name[256];
        strcpy(name, dname);
        strcat(name, "/");
        strcat(name, ent->d_name);

        if (ent->d_type == DT_DIR && program_config.r_flag)
          /* Found a subdirectory, process it if -r flag enabled */
          process_dir(program_config, name, outstream);
        else
          /* Found a file, process it */
          process_file(program_config, name, outstream);
      }
    } else {
      /* Error opening directory */
    }

    /* Exit from child process */
    exit(0);
  } else if (pid > 0) {
    /* Parent process */
    wait(NULL);

    /* Log this event */
    if (program_config.v_flag) {
      char act[256];
      sprintf(act, "PROCESSED DIR %s", dname);
      log_event(act);
    }
  } else {
    /* Error creating process */
  }
}
