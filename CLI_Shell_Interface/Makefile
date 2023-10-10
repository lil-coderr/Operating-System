# Compiler and flags
CC = g++
CFLAGS = -std=c++11 

# Object files
OBJS = main.o Shell.o

# Default target
all: Shell

# Linking
Shell: $(OBJS)
	$(CC) $(OBJS) -o Shell

# Compiling
main.o: main.cpp Shell.h
	$(CC) $(CFLAGS) -c main.cpp

Shell.o: Shell.cpp Shell.h
	$(CC) $(CFLAGS) -c Shell.cpp

# Cleanup
clean:
	rm -f *.o Shell