# Program name.
PROG := forensic

# Compiler flags.
CFLAGS := -Wall -Wextra -Werror

# Source files.
SRCS := $(wildcard ./src/*.c)

# Generate .o and .d filenames for each .cpp file.
# Doesn't generate the ACTUAL files (compiler does).
# Just generates the lists.
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

# Called when you run 'make'. This calls the line below.
all: $(PROG)

# Calls the compiler with flags to link all object files together.
$(PROG): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $(PROG)

# Includes the dependency lists (.d files).
-include $(DEPS)

# Removes executable, object files, and dependency files.
clean:
	rm -f $(PROG)
	rm -f $(DEPS) $(OBJS)