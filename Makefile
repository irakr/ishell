#################Makefile for building the shell program##################

#Variables for compiler commands and flags
CC=gcc
CFLAGS=-c -Wall

#Variables for directories of required files
SRC_DIR=src/
BIN_DIR=bin/

#Variables that stores pathnames of object programs for each source files
SHELL_O=$(SRC_DIR)shell.o
SHELL_LIB_O=$(SRC_DIR)shell_lib.o

#Variables that stores pathnames of each source files
SHELL_C=$(SRC_DIR)shell.c
SHELL_LIB_C=$(SRC_DIR)shell_lib.c

#Debug flag
DEBUG=-g

#The main target
main:	shell

shell:	$(SHELL_O) $(SHELL_LIB_O)	
	$(CC) $(SHELL_O) $(SHELL_LIB_O) -o $(BIN_DIR)ishell

shell.o:	$(SHELL_C)
	$(CC) $(CFLAGS) $(SHELL_C) -o $(SHELL_O)

shell_lib.o:	$(SHELL_LIB_C)
	$(CC) $(CFLAGS) $(SHELL_LIB_C) -o $(SHELL_LIB_O)
