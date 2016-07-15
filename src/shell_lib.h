/*
 *	shell_lib.h
 *
 *	Author:	Irak Rigia
 *	Date:	20/06/16 MON 2010 hrs
 *
 *	This file contains declarations of various functions used by the shell program.
 */

/*	Null condition checker for strings. You can use this macro for checking null conditions for strings anywhere. Yields 0(false) or 1(true)
 *	WARNING: (NOT FULLY IMPLEMENTED YET) May cause segmentation-fault. Not sure!
 */

#ifndef SHELL_LIB_H_
#define SHELL_LIB_H_

#define is_null(str)	((str==(char*)0) || (strcmp(str,"")==0))		

//Input stream flusher
#define FLUSH_STDIN() 	int input = getchar();	\
			while(input!='\n' && input!=EOF);

#include "sh_limits.h"
#include "flags.h"

//Data structure to store a command and its arguments.
typedef struct{
	char *cmd_name;	//A single component command. Or a command out of all piped commands of the input command.
	char *args[MAX_ARGS];	//Arguments to the command in 'cmd_name'
} cmd_t;

//Command cmd_t with some more info
typedef struct{
	int8_t ntokens;	//No of tokens in the command
	int8_t npipes;	//No of piped commands
	int8_t nbkgnds;	//No of background instructed commands
	cmd_t *cmd_ptr;	//Pointer to the array of cmd_t structs where each struct holds a command and its arguments.
} info_cmd;
/*
 *	PS1 is the string that will be displayed as the prompt screen.
 *	By default PS1 = "user-name@machine-name:absolute-path-of-current-working-directory$"
 *	The ending character is '$' for a user and a '#' for root.
 */
char PS1[MAX_PS1_LENGTH];	//PS1 variable of the shell

void init_ps1();	//Initialize ps1 variable

void change_ps1(const char *username,const char *machinename);	//Change value of PS1

inline void assert_exit(char *cmd);	//Check if the input command is 'exit'.

void identify_cmd_type(cmd_t *cmd, int n);	//Identify type of command(INBUILT_CMD or REGULAR_CMD)

int parse_cmd(const char *command, info_cmd *cmd_info);	//Parse command

int (*tokenize_cmd(const char *cmd,char *tokens[MAX_TOKENS])) [3] ;	//Tokenize user command

void resolve_cmd_path(cmd_t **);	//Resolve full pathname for the command

char *search_cmd(const char *command);	//Search for a command in each directory defined by the PATH variable

//@void chk_cmd_type(cmd_t*, int);	//Check to see if the command is a built-in command or a program

void execute_cmd(cmd_t *cmd, int8_t pipe_count, int8_t bkgnd_count);	//Execution of a command
#endif
