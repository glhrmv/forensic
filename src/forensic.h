#ifndef FORENSIC_H
#define FORENSIC_H

#include <sys/types.h>
#include <stdio.h>
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

typedef struct ProcessedStats {
  size_t files_processed; // Number of files processed
  size_t dirs_processed; // Number of directories processed
} ProcessedStats;

/**
 * Declare a const static instance of each of our structs
 * which will have all of their fields initialized due to static initialization.
 * See: https://stackoverflow.com/a/6891731/6304441
 */
/* Empty ProgramConfig struct */
static const ProgramConfig empty_program_config;
/* Empty ProcessesStats struct */
static const ProcessedStats empty_processed_stats;

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
 * @brief Logs an event.
 * 
 * Uses the environment variable LOGFILENAME to know
 * where to write an event to.
 * 
 * Events have the following form:
 * "inst - pid - act", where:
 * inst -> Time taken (milliseconds) since starting program execution
 * pid -> Process identifier
 * act -> Event description, i.e., what happened
 * 
 * @param pid ID of the process in charge of the event
 * @param act Description of the event
 */
void log_event(pid_t pid, const char* act);

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
 * @brief Decides how to process the command line argument, 
 * performing some setup if needed.
 * 
 * If -v flag enabled, clears the logfile contents.
 * If -o flag enabled, chooses the output stream based on the option value.
 *    Uses stdout by default.
 * If the argument is a directory, it might have a trailing '/' character.
 *    If it does, it is stripped from the string.
 * If the argument is a file, calls process_file(), passing it along.
 *    Otherwise, calls process_dir(), passing it along.
 * If -o flag enabled, closes the output stream.
 * 
 * @param program_config ProgramConfig struct
 */
void process(const ProgramConfig program_config);

/**
 * @brief Processes a single, regular file.
 * 
 * Collects file data using the stat system call, parsing the returned struct 
 * values in order to present them according to project specification.
 * 
 * The file data isn't stored anywhere outside this function, it is sent
 * directly to the stream specified with the -o option, stdout if not applicable.
 * 
 * If -h flag enabled, goes through every hash option given and retrieves
 * the hash of the file in question using command_to_str().
 * 
 * @param program_config ProgramConfig struct
 * @param fname File name
 * @param outstream Output stream to write processed data to
 * @param processed_stats ProcessedStats struct
 */
void process_file(const ProgramConfig program_config, const char* fname, FILE* outstream, ProcessedStats* processed_stats);

/**
 * @brief Processes a directory, i.e., all regular files inside a directory.
 * If -r flag enabled, also processes subdirectories, recursively.
 * For every directory found, spawns a new system process to handle it, with fork().
 * 
 * @param program_config ProgramConfig struct
 * @param dname Directory name
 * @param outstream Output stream to write processed data to
 * @param processed_stats ProcessedStats struct
 */
void process_dir(const ProgramConfig program_config, const char* dname, FILE* outstream, ProcessedStats* processed_stats);

#endif

