/*
 * This is free and unencumbered software released into the public domain.

 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.

 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.

 * For more information, please refer to <http://unlicense.org>
 */

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


#include "sh_limits.h"
#include "flags.h"

#ifndef SHELL_LIB_H_
#define SHELL_LIB_H_

/**************************************
 * Data structures used by the shell  *
 **************************************/

//Data structure to store a command and its arguments.
typedef struct{
	char *cmd_name;	//A single component command. Or a command out of all piped commands of the input command.
	char *args[MAX_ARGS];	//Arguments to the command in 'cmd_name'
	uint16_t argc;	//No of arguments
	cmdtype_t type;	//Type of command
} cmd_t;

//Command cmd_t with some more info
typedef struct{
	uint8_t ntokens;	//No of tokens in the command
	uint8_t npipes;		//No of piped commands
	uint8_t nbkgnds;	//No of background instructed commands
	uint8_t ncommands;	//No of commands
	cmd_t *cmd_ptr;		//Pointer to a variable size array of cmd_t structs where each element(struct) holds a command and its arguments.
} info_cmd;


/*	Fixed-valued list of inbuilt commands with there valid arguments	
 * .cmd_name is the command name, .args is the collection of valid arguments that the command accepts to be able to run.
 * .type is just an indication that it is an inbuilt command type(Not so important). Just provides identity.
 */
static cmd_t inbuilt_cmds[] =
{
	{ .cmd_name="cd", .args={".", "..", "-", "~"}, .type=INBUILT_CMD}
};


/*	Macro function to initialize shell environment	*/
#define init_shell()	\
		init_logger();	\
		init_history();	\
		init_ps1();	\
		RESET_ALL_FLAGS();

/**************************
 * Function declarations  *
 **************************/

void init_ps1();	//Initialize ps1 variable

void change_ps1(const char *username,const char *machinename);	//Change value of PS1

inline void assert_exit(char *cmd);	//Check if the input command is 'exit'.

void identify_cmd_type(cmd_t *cmd, int n);	//Identify type of command(INBUILT_CMD or REGULAR_CMD)

int parse_cmd(char *command, info_cmd *cmd_info);	//Parse command

int (*tokenize_cmd(char *cmd,char *tokens[MAX_TOKENS])) [4] ;	//Tokenize user command

int resolve_cmd_path(cmd_t *cmd_ptr);	//Resolve full pathname for the command

char *search_cmd_in_path(const char *command);	//Search for a command in each directory defined by the PATH variable

//@void chk_cmd_type(cmd_t*, int);	//Check to see if the command is a built-in command or a program

void display_command_info(info_cmd *cmd_info);	//Display command infos

int manage_execution(info_cmd *target);	//Initiate procedures that will execute the command

void execute_regular(info_cmd *target);		//Execution of regular commands

void execute_inbuilt(info_cmd *target);		//Execution of inbuilt commands

inline void fork_exec(cmd_t cmd);		//Simple fork and exec code

#endif
