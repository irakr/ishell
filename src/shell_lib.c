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
 *	shell_lib.c
 *
 *	This file contains the definitions of the functions in the shell_lib.h header file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell_lib.h"
#include "flags.h"

//Parse the 'command' and store <command,arg_list> pairs in the array of 'cmd' structs.
int parse_cmd(const char *command, info_cmd *cmd_info){
	
	char *tokens[MAX_TOKENS];	//Tokens of a command
	int *counts;	//No of tokens(counts[0]), pipes(counts[1]) '|' and background(counts[2]) '&' 
	int i, j, k;
	
	
	//First thing to do is to tokenize the command with delimiter ' '(WHITESPACE)
	if((counts=tokenize_cmd(command, tokens)) == NULL){
		//--------LOGGING----------
		fprintf(stderr,"Shell Error: Too may tokens in command.\n");
		return -1;
	}
	
	//Set the flag for type of command. Flags PIPED_CMD, BKGROUND_CMD and SINGLE_CMD will be set by this function only.
	if(counts[1] && counts[2])
		cmd_type = PIPED_CMD | BKGROUND_CMD;	//Piped as well as backgrounded commands exists
	else if(counts[1])
		cmd_type = PIPED_CMD;
	else if(counts[2])
		cmd_type = BKGROUND_CMD;	
	else if(counts[0])
		cmd_type = SINGLE_CMD;
	
	//Store count infos
	cmd_info->ntokens = counts[0];
	cmd_info->npipes = counts[1];
	cmd_info->nbkgnds = counts[2];
	
	//TODO...First release memory from the argument 'cmd_t *cmd_ptr' for processing the next command, if it is already assigned some address.
	if(cmd_info->cmd_ptr)
		free(cmd_info->cmd_ptr);
	
	//NOTE:	If no. of pipes = x then no. of command-names = x+1
	cmd_info->cmd_ptr = (cmd_t*)calloc(cmd_info->npipes+1, sizeof(cmd_t));
	if(cmd_info->cmd_ptr == NULL){
		//------------LOGGING---------------
		fprintf(stderr,"Error: Memory allocation issue at parse_cmd()\n");
		return -1;
	}
	
	//Extract command names and arguments from the tokens.
	//Sample:	`sudo fdisk -l | grep /dev/sda8`
	//NOTE:	Here the first command 'sudo' with arguments 'fdisk' and '-l'.
	//	The second command is 'grep' with argument 'abc'
	//TODO...problem with struct notations
	cmd_info->((*cmd_ptr)[0].cmd_name) = tokens[0];
	for(i=1,j=0,k=0; i<counts[0]; i++){
		
		//If current token is a '|' and the next token is not null then take that next token as the second command.
		if((strcmp(tokens[i], "|")==0) && (tokens[i+1]!=(char*)0)){
			j++;
			cmd_info->((*cmd_ptr)[j].cmd_name) = tokens[++i];
			k=0;
		}
		else{	//Otherwise consider as arguments
			cmd_info->((*cmd_ptr)[j].args[k]) = tokens[i];
			k++;
		}
	}
	
	//TODO
	//The logic here is if any inbuilt command is encountered anywhere, the whole command will be discarded and will cause no effect.
	//Eg: `$ ls -l | cd ..` will not cause any effect.Try it on a bash shell. But the reverse somehow works, if you try in a real bash shell.
	//But at this point we will consider it simple....[Truth is I don't know the logic behind it yet]
	identify_cmd_type(cmd_info->cmd_ptr,i);	//i = No. of commands(i.e., no of cmd_t structs). Although counts[1]+1 is also the same.
	
	return 0;
}

//Break the command 'cmd' into individual tokens(a WHITESPACE as a delimiter) and store it in 'tokens'
//Returns the no. of tokens, pipes and background commands.
int (*tokenize_cmd(const char *cmd,char *tokens[MAX_TOKENS])) [3] {
	//char *tokens[MAX_TOKENS] = {NULL};	//Pointers to each tokens of a command
	int i=1;
	static int counts[3]={0,0,0};
	//counts[0]->token_count
	//counts[1]->pipe_count('|')
	//counts[2]->background_count('&')
	
	tokens[0] = strtok(cmd," ");	//Get each token w.r.t. white spaces
	for(; i<MAX_TOKENS; i++){
		tokens[i] = strtok(NULL," ");
		if(tokens[i] == NULL){
			//tokens[i+1] = (char*)0;	//Terminating NULL. Useful when passing to exec()
			break;
		}
		else if(strcmp(tokens[i],"|") == 0)	//Pipe operator encountered
			counts[1]++;
		else if(strcmp(tokens[i],"&") == 0)	//Background operator encountered
			counts[2]++;
	}
	if(i == MAX_TOKENS)	//Too many tokens in command
		return NULL;
	counts[0] = i;
	return counts;
}

//Identify type of command(INBUILT_CMD or REGULAR_CMD)
void identify_cmd_type(cmd_t *cmd, int n){
	//n = No of cmd_t structs
	int i;
	for(i=0;i<n;i++){
		if(strcmp(cmd[0].cmd_name, "cd") == 0){	//There may be other inbuilt commands. For now we consider only 'cd' command
			cmd_type |= INBUILT_CMD;	//Inbuild command
			return;
		}
	}
	cmd_type |= REGULAR_CMD;	//Executable commands
}

//Check for 'exit' command
inline void assert_exit(char *cmd){
	char temp[MAX_CMD_LENGTH] = "";
	strcpy(temp,cmd);
	if(!strcmp(strtok(temp," "),"exit"))
		exit(0);
	
}

//Search for a command in each directory defined by the PATH variable
char *search_cmd(const char *command){
	/* static */ char PATH[ENV_PATH_SIZE];	//Environment variable PATH
	static char cmd[PATH_MAX]="";	//Command that will be searched
	FILE *cmd_file;		//File that will try to open the filename in 'cmd' to verify whether the file is in that directory

	if(strcpy(PATH,getenv("PATH")) == NULL){	//This will get the value of environment variable PATH
		return NULL;	//if PATH environment var is empty by any chance
	}
	//Try to open the command file using each directories defined in PATH variable. If openning with anyone of the paths is successful,
	//it means the file is located there. Then update the original 'command' argument with the complete pathname.
	strcpy(cmd,strtok(PATH,":"));

	strcat(strcat(cmd,"/"),command);	//Eg: /usr/bin/ls....TODO...Caution: This may be a bug in the future.[DONE]
	if((cmd_file=fopen(cmd,"r"))){
		fclose(cmd_file);
		return cmd;
	}
	while(strcpy(cmd,strtok(NULL,":")) != NULL){
		strcat(strcat(cmd,"/"),command);	//Eg: /usr/sbin/ls
		if((cmd_file=fopen(cmd,"r"))){
			fclose(cmd_file);
			return cmd;
		}
		
	}
	return NULL;
}

//This macro defines the codes for a fork-and-exec structure.It is the core function of a shell. Rest are shell features.
//So this macro maybe used by different routines responsible for executing different types of command.
//ALGORITHM: Execute command by invoking the respective program for it using fork-and-exec procedure.
//TODO....
#define fork_exec()	int cpid,child_err_code;	\
			cpid = fork();	\
			if(cpid == -1){	\
				perror("fork");	\
				return;	\
			}	\
			if(cpid == 0){		\
				if(execv(cmd->cmd_name,cmd->args) == -1){	\
					perror("execv");	\
					exit(EXIT_FAILURE);	\
				}	\
			}		\
			wait(&child_err_code);

void execute_cmd(cmd_t *cmd, int8_t pipe_count, int8_t bkgnd_count){
	
	//Act according to command type
	switch (cmd_type){
		case REG_SINGLE:
			printf("INFO: Command type = REGULAR AND SINGLE\n");
			break;
		case REG_PIPED:
			printf("INFO: Command type = REGULAR AND PIPED\n");
			break;
		case REG_BKGROUND:
			printf("INFO: Command type = REGULAR AND BKGROUND\n");
			break;
		case INBUILT:
			printf("INFO: Command type = INBUILT\n");
			break;
		default:	//Invalid type. Discard the command and prompt for next command
			fprintf(stderr,"INFO: Command = INVALID\n");
	}
	
}


//Init PS1
void init_ps1(){
	change_ps1("irakr","lenovo-S510P");
}

//Change PS1
void change_ps1(const char *user,const char *machinename){
	if(is_null(user))	//Atleast 1st arg should be non-empty
		return;
	
	//For initial PS1 value
	if((machinename!=(char*)0) || (strcmp(machinename,"")!=0)){
		//Get current working directory
		char dir[PATH_MAX];
		if(getcwd(dir,PATH_MAX) == (char*)0){
			perror("getcwd");
			return;
		}
		strcpy(PS1,user);	//username portion(Eg: irakr)
		strcat(PS1,"@");	//irakr@
		strcat(PS1,machinename);	//irakr@lenovo-s510p
		strcat(PS1,":");	//irakr@lenovo-s510p:
		strcat(PS1,dir);	//irakr@lenovo-s510p:~/Programs/Unix/my_shell
		strcat(PS1,"$ ");	//irakr@lenovo-s510p:~/Programs/Unix/my_shell$<space>
	}
	else{	//For user directed change(If the user entered the command :	$PS1=<some-string>)
		strcpy(PS1,user);
	}
}
