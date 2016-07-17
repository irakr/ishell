#################Makefile for building the shell program##################

#Variables for compiler commands and flags
CC=gcc
CFLAGS=-c -Wall

#Special directory variables for . and ..
CURRENT_DIR=./
PREVIOUS_DIR=../

#Variables for directories of required files
SRC_DIR=$(CURRENT_DIR)src/
BIN_DIR=$(CURRENT_DIR)bin/

#Variables that stores pathnames of each source files
SHELL_C=$(SRC_DIR)shell.c
SHELL_LIB_C=$(SRC_DIR)shell_lib.c

#Variables that stores pathnames of object programs for each source files
SHELL_O=$(SRC_DIR)shell.o
SHELL_LIB_O=$(SRC_DIR)shell_lib.o
ALL_OBJECTS=SHELL_O SHELL_LIB_O

#Pathname of the file executable file
ISHELL=$(BIN_DIR)ishell

#Debug flag
DEBUG=-g

#The main target
$(ISHELL):	$(SHELL_O) $(SHELL_LIB_O)
	$(CC) $(SHELL_O) $(SHELL_LIB_O) -o $(ISHELL)

$(SHELL_O):	$(SHELL_C)
	$(CC) $(CFLAGS) $(SHELL_C) -o $(SHELL_O)

$(SHELL_LIB_O):	$(SHELL_LIB_C)
	$(CC) $(CFLAGS) $(SHELL_LIB_C) -o $(SHELL_LIB_O)

clean:
	-rm -rf $(ALL_OBJECTS)
