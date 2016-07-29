#################Makefile for building the shell program##################

#Variables for compiler commands and flags
CC=gcc
CFLAGS=-c -Wall

#Debug flag
GDB=-g

#Special directory variables for . and ..
CURRENT_DIR=./
PREVIOUS_DIR=../

#Variables for directories of required files
SRC_DIR=$(CURRENT_DIR)
BIN_DIR=$(PREVIOUS_DIR)bin/

#Variables that stores pathnames of each source files
SHELL_C=$(SRC_DIR)shell.c
SHELL_LIB_C=$(SRC_DIR)shell_lib.c
ENVIRONMENT_C=$(SRC_DIR)environment.c
BASIC_UTILS_C=$(SRC_DIR)basic_utils.c

#Variables that stores pathnames of object programs for each source files
SHELL_O=$(SRC_DIR)shell.o
SHELL_LIB_O=$(SRC_DIR)shell_lib.o
ENVIRONMENT_O=$(SRC_DIR)environment.o
BASIC_UTILS_O=$(SRC_DIR)basic_utils.o
ALL_OBJECTS=$(SHELL_O) $(SHELL_LIB_O) $(ENVIRONMENT_O) $(BASIC_UTILS_O)

#Pathname of the file executable file
ISHELL=$(BIN_DIR)ishell

#The main target
$(ISHELL):	$(SHELL_O) $(SHELL_LIB_O) $(ENVIRONMENT_O) $(BASIC_UTILS_O)
	$(CC) $(BASIC_UTILS_O) $(ENVIRONMENT_O) $(SHELL_LIB_O) $(SHELL_O) -o $(ISHELL)

$(SHELL_O):	$(SHELL_C)
	$(CC) $(CFLAGS) $(SHELL_C) -o $(SHELL_O)

$(SHELL_LIB_O):	$(SHELL_LIB_C)
	$(CC) $(CFLAGS) $(SHELL_LIB_C) -o $(SHELL_LIB_O)

$(ENVIRONMENT_O):	$(ENVIRONMENT_C)
	$(CC) $(CFLAGS) $(ENVIRONMENT_C) -o $(ENVIRONMENT_O)

$(BASIC_UTILS_O):	$(BASIC_UTILS_C)
	$(CC) $(CFLAGS) $(BASIC_UTILS_C) -o $(BASIC_UTILS_O)



#For debugging purpose
debug:
	$(CC) $(GDB) $(ENVIRONMENT_C) $(BASIC_UTILS_C) $(SHELL_LIB_C) $(SHELL_C) -o ishell

clean:
	-rm -rf $(ALL_OBJECTS)
	-rm -rf *~
