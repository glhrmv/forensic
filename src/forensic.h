#ifndef FORENSIC_H
#define FORENSIC_H

#include <time.h>

/**
 * @brief A structure to represent the program's configuration.
 */
typedef struct ProgramConfig {
  int debug_flag; // Debug flag
  int r_flag; // Recursion flag (for directories)
  int h_flag; // Hash flag
  char* h_value; // Hash option value (comma-delimited string)
  int h_alg_md5_flag; // MD5 hash algorithm flag
  int h_alg_sha1_flag; // SHA1 hash algorithm flag
  int h_alg_sha256_flag; // SHA256 hash algorithm flag
  int o_flag; // Output file flag
  char* o_value; // Output file option value
  int v_flag; // Verbose flag (to use logfile)
  char* logfile; // Logfile to be used
  char* arg; // Argument passed to program (file or directory)
} ProgramConfig;

/**
 * Declare a const static instance of ProgramConfig
 * which will have all of its fields initialized due to static initialization.
 * See: https://stackoverflow.com/a/6891731/6304441
 */
static const ProgramConfig empty_program_config;

/**
 * @brief Checks if file exists on disk.
 * 
 * @param pathname Path name
 * @return Zero on success, non-zero otherwise
 */
int file_exists(const char* pathname);

/**
 * @brief Checks if a pathname is a regular file.
 * 
 * @param pathname Path name
 * @return int Zero on success, non-zero otherwise
 */
int is_file(const char* pathname);

/**
 * @brief Checks if a pathname is a directory.
 * 
 * @param pathname Path name
 * @return int Zero on success, non-zero otherwise
 */
int is_directory(const char* pathname);

/**
 * @brief Converts time_t to char*, in ISO 8601 format (e.g., 2018-01-04T16:30:19).
 * 
 * @param tm time_t value to convert
 * @return char* Formatted string (dynamic, must be freed)
 */
char* time_to_iso_str(const time_t time);

/**
 * @brief Calls a specified command, returning its stdout output.
 * Only supports single-argument program calls.
 * 
 * For example, when invoked with command_to_str("file -b %s", "Makefile"),
 * it will return `makefile script text, ASCII text`
 * 
 * @param fmt Format string
 * @param arg Argument to send to command
 * @return char* Output of command as string (dynamic, must be freed)
 */
char* command_to_str(const char* fmt, const char* arg);

/**
 * Parses arguments (using getopt) and fills a ProgramConfig struct.
 * If any of the parameters are invalid/unexpected, prints error message(s)
 * to stderr and exits with status code 1.
 * 
 * @param argc Argument count (from main())
 * @param argv Argument vector (from main())
 * @param program_config ProgramConfig struct to fill
 */
void parse_args(int argc, char** argv, ProgramConfig* program_config);

/**
 * @brief Main program logic function.
 * 
 * @param program_config ProgramConfig struct
 */
void process(const ProgramConfig program_config);

#endif

