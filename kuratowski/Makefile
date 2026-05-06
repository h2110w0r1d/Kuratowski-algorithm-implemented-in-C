# Compiler to use
CC = gcc

# Compiler flags: -Wall for warnings, -g for debugging
CFLAGS = -Wall -g

# Target executable name
TARGET = kuratowski_app

# Source files
SRCS = main.c AdjList.c stack.c TarjanDFS.c JohnsonCycleAlgo.c LinkList.c step2.c

# Object files (automatically generated from SRCS)
OBJS = $(SRCS:.c=.o)

# Header files (for dependency tracking)
DEPS = AdjList.h stack.h TarjanDFS.h JohnsonCycleAlgo.h LinkList.h step2.h

# Default rule: builds the target
all: $(TARGET)

# Linking the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compiling source files to object files
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
