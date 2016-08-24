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
 *	shell.c
 *
 *	Author:	Irak Rigia
 *	Date:	20/06/16 MON 2010 hrs
 *
 *
 *	This is the entry point of the shell program.
 *
 *	This program is a basic implementation of an actual bash, ksh, or csh like shells found in Linux/Unix.
 *	Well it doesn't do much like the actual shell. Its just a small project of my own.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "flags.h"
#include "shell_lib.h"
#include "environment.h"
#include "basic_utils.h"

#define DEBUG	0	//Make it 1 to display debugging information

FILE *log_file;	//Every actions will be logged in this file.
FILE *history;	//File that stores the history of commands executed till date.

//Shell starting point
int main(void){
	char input_cmd[MAX_CMD_LENGTH]="";	//Command entered by the user including arguments,pipes,redirections,etc.

	info_cmd command_info = {.cmd_ptr=NULL};	//This will be filled by the parse_cmd() function.
	cmd_t *cmd = NULL;	//An array of structs each holding a command and its arguments.

	//TODO....
	//@init_shell();

	/* TODO....
	//Get the log file ready
	if((log_file = fopen("/var/log/ishell/shell-logs","w+a")) == NULL){
		fprintf(stderr,"NOTICE: There was a problem in creating the log file. Your current session will not be\nlogged. Do you want to continue?(y/n): ");
		//fflush(stdin);
		scanf("%c",&input);	//OH! 'input(int)' is defined by the macro FLUSH_STDIN()
		while((input != 'y') && (input !='Y')){
			if((input == 'n') || (input == 'N'))
				exit(EXIT_SUCCESS);
			printf("Sorry, wrong input. Try again!!!\nEnter 'y'|'Y'(to continue) or 'n'|'N'(to exit): ");
			//fflush(stdin);
			scanf("%c",&input);
		}
	}
	*/

	//Initialize PS1 content
	init_ps1();

	/*
	 *	The algorithm for this shell is properly demonstrated in the file 'iSHell algorithm' located in the 'docs' directory.
	 */
	while(1){

		//@reset_params();
		RESET_ALL_FLAGS();

		printf("%s", PS1);	//Display prompt string(PS1)
		if(!fgets(input_cmd, MAX_CMD_LENGTH, stdin)){	//Get input
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		if(strcmp(input_cmd, "\n") == 0)	//If Empty command then new prompt again
			continue;
		input_cmd[strlen(input_cmd)-1] = '\0';	//Replace '\n' with a '\0'

		//First check for 'exit' command.
		assert_exit(input_cmd);

		//Parse input command if it is not one of the in-built command.
		if(parse_cmd(input_cmd, &command_info) == -1){
			fprintf(stderr,"Shell Error [%s:%d]: Error while parsing the command.\n", __FILE__, __LINE__);
			continue;
		}

		#if DEBUG
		display_command_info(&command_info);
		#endif

		//TEST...
		//print_environ("PATH");

		if(manage_execution(&command_info) == -1){
			fprintf(stderr,"Shell Error [%s:%d]: Command execution ignored\n", __FILE__, __LINE__);
			continue;
		}

        //Cleanup procedure
        cleanup_all(&command_info);

	}

	return 0;
}
