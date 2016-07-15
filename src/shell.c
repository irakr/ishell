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
#include "shell_lib.h"

FILE *log_file;	//Every actions will be logged in this file.
FILE *history;	//File that stores the history of commands executed till date.

//Shell starting point
int main(void){
	char input_cmd[MAX_CMD_LENGTH]="";	//Command entered by the user including arguments,pipes,redirections,etc.

	info_cmd command_info;	//This will be filled by the parse_cmd() function.
	cmd_t *cmd = NULL;	//An array of structs each holding a command and its arguments.
	
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
	 *	The algorithm for this shell is simply to:
	 *
	 *	1) Read input command from user.
	 *	2) If input is blank, prompt for next command, i.e., go to step-1.
	 *	3) Check for 'exit' command.
	 *	4) Parse the command by the parse_cmd() procedure.
	 *	5) During parsing, determine the type of command; whether in-built or regular.
	 *	6) If in-built then execute codes for that, else if regular then check for pipes, redirections and do fork-and-exec.
	 *	7) Repeat from step-1 until 'exit' command.
	 */
	while(1){
		
		RESET_ALL_FLAGS();
		
		printf("%s",PS1);	//Display prompt string(PS1)
		if(!fgets(input_cmd,MAX_CMD_LENGTH,stdin)){	//Get input
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		if(strcmp(input_cmd,"\n") == 0)	//If Empty command then new prompt again
			continue;
		input_cmd[strlen(input_cmd)-1] = '\0';	//Replace '\n' with a '\0'
		
		//First check for 'exit' command.
		assert_exit(input_cmd);
		
		//Parse input command if it is not one of the in-built command.
		if(parse_cmd(input_cmd,&command_info) == -1){
			fprintf(stderr,"Shell Error: Error while parsing the command.\n");
			continue;
		}
		
		//Send info to the execution modules
		execute_cmd(command_info.cmd_ptr, command_info.npipes, command_info.nbkgnds);

		//TODO.....Latest work!!!!!!!!!! Starts here!!!!!!!!!
		//Resolve full pathnames for each command parsed.
		//@resolve_cmd_path(&cmd);		
		
	}
	return 0;
}