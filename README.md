# forensic

```sh
forensic [-r] [-h [md5[,sha1[,sha256]]]] [-o <outfile>] [-v] <file|dir>
```

Command-line utility that performs a forensic analysis over a given file or directory.

```sh
$ ./forensic Makefile
Makefile,makefile,664,rw,2019-03-23T19:41:26,2019-03-23T19:41:26
```

If given a directory, the program processes every file inside the directory, individually.

```sh
$ ./forensic src
src/forensic.c,c,11598,rw,2019-03-26T16:06:48,2019-03-26T16:06:48
src/forensic.o,Mach-O,10912,rw,2019-03-26T16:07:27,2019-03-26T16:07:27
src/forensic.h,c,5463,rw,2019-03-26T15:52:34,2019-03-26T15:52:34
```

The program output is in the following format:

```sh
file_name,file_type,file_size,file_permissions,file_modified_data,file_accessed_date,md5,sha1,sha256
```

The last 3 fields are optional, depending on the usage of the `-h` flag and its possible valid options.

## Usage

Run `make` in the root directory to build an executable named `forensic`.

### Options

- `-r`: If the argument given is a directory, goes through all of that directory's subdirectories, processing all of the files therein. Ignored if the argument is a regular file.
- `-h`: Gets the hashed checksum of the file in question. Accepts `md5`, `sha1`, `sha256`, in any order (but always prints them in that order).
- `-o`: Defines the output stream where to print the processed file details to. If not specified, `stdout` is used. The output stream must be a regular file.
- `-v`: Generates a logfile with a record of events logged by the program. The logfile name is dependant on the `LOGFILENAME` environment variable sent to the program. If not set, `log.txt` is used as the default file name. The logfile is always cleared before processing begins, i.e., previous execution logs are not kept.
- `-d`: Debug flag. Prints program configuration after parsing command-line arguments.
