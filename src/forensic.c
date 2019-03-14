#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
  /* Flags */
  int debug_flag = 0;
  int r_flag = 0, h_flag = 0, o_flag = 0, v_flag = 0;
  char* h_name; char* o_name;

  /* Using getopt to parse args */
  static char* usage = "usage: %s [-r] [-h [md5[,sha1[,sha256]]]] [-o <outfile>] [-v] <file|dir>\n";
  extern char* optarg;
  extern int optind;
  int c, err = 0;

  while ((c = getopt(argc, argv, "drh:o:v")) != -1) {
    switch (c) {
      case 'd':
        debug_flag = 1;
        break;
      case 'r':
        r_flag = 1;
        break;
      case 'h':
        h_flag = 1;
        h_name = optarg;
        break;
      case 'o':
        o_flag = 1;
        o_name = optarg;
        break;
      case 'v':
        v_flag = 1;
        break;
      case '?':
        err = 1;
        break;
    }
  }

  // In case of invalid option
  if (err) {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  // Print options
  if (debug_flag) {
    printf("r_flag = %d\n", r_flag); 
    printf("h_flag = %d\n", h_flag); 
    printf("h_name = \"%s\"\n", h_name); 
    printf("o_flag = %d\n", o_flag); 
    printf("o_name = \"%s\"\n", o_name);
    printf("v_flag = %d\n", v_flag); 
  }

  /* If no args */
  if (optind == argc) {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  } 
  
  // Argument passed
  fprintf(stdout, "file or folder: %s\n", argv[optind]);

  exit(0);
}
