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
#include "flags.h"
#include "shell_lib.h"
#include "environment.h"
#include "basic_utilities.h"

//Parse the 'command' and store <command,arg_list> pairs in the array of 'cmd_t' structs.
int parse_cmd(char *original_command, info_cmd *cmd_info){
	
	char *tokens[MAX_TOKENS];	//Tokens of a command
	int (*counts)[4];	//Go over to (*tokenize_cmd())[4] and find out what this array represents.
	int i, j, k;
	
	char *command = (char*)calloc(strlen(original_command)+1, sizeof(char));
	if(command){
		strcpy(command, original_command);
	}
	else{
		//------- LOGGING ---------------
		fprintf(stderr,"Shell Error: Memory allocation failed at parse_cmd():%d\n",__LINE__);
		return -1;
	}
	/* First thing to do is to tokenize the command with delimiter ' '(WHITESPACE)
	 * and also get the no of tokens, pipes and background operators in the command.
	 */
	if((counts=tokenize_cmd(command, tokens)) == NULL){
		//--------LOGGING----------
		fprintf(stderr,"Shell Error: Too may tokens in command.\n");
		return -1;
	}
	
	//Set the flag for type of command. Flags PIPED_CMD, BKGROUND_CMD and SINGLE_CMD will be set by this function only.
	if((*counts)[1] && (*counts)[2])
		cmd_type = PIPE_BK;	//Piped as well as backgrounded commands exists
	else if((*counts)[1])
		cmd_type = PIPED_CMD;
	else if((*counts)[2])
		cmd_type = BKGROUND_CMD;	
	else if((*counts)[0])
		cmd_type = SINGLE_CMD;
	
	//Store count infos
	cmd_info->ntokens = (*counts)[0];
	cmd_info->npipes = (*counts)[1];
	cmd_info->nbkgnds = (*counts)[2];
	cmd_info->ncommands = (*counts)[3];
	
	/* TODO...First release memory from the argument 'cmd_t *cmd_ptr' for processing the next command,
	 * if it is already assigned some address(maybe for the previous command).
	 */
	if(cmd_info->cmd_ptr){
		free(cmd_info->cmd_ptr);
		//cmd_info->cmd_ptr = NULL;
	}
	
	//NOTE:	If no. of pipes = x then no. of commands = x+1
	cmd_info->cmd_ptr = (cmd_t*)calloc(cmd_info->ncommands, sizeof(cmd_t));
	if(cmd_info->cmd_ptr == NULL){
		//------------LOGGING---------------
		fprintf(stderr,"Error: Memory allocation issue at parse_cmd()\n");
		return -1;
	}
	
	/* 
	 * TODO
	 * Extract command names and arguments from the tokens and put them in cmd_ptr appropriately.
	 * Example:	`sudo fdisk -l | grep /dev/sda8`
	 * NOTE:	Here the first command 'sudo' with arguments 'fdisk' and '-l'.
	 * The second command is 'grep' with argument '/dev/sda8'
	 * '|', '&' or any other symbols are stored as part of the arguments.
	 */
	cmd_info->cmd_ptr[0].cmd_name = cmd_info->cmd_ptr[0].args[0] = tokens[0];
	for(i=1,j=0,k=1; i<(*counts)[0]; i++){
		/*
		 * i -> Points through each token.
		 * j -> Points through each cmd_info->cmd_ptr[], i.e., the struct cmd_t consecutive commands.
		 * k -> Points through each cmd_info->cmd_ptr[].args[], i.e., the argument list of each command.
		 * k is initialized to 1 because the 0th argument is always the name of the command itself and the exec() family
		 * of system calls(which will be used later) uses the same convention.
		 */
		 
		// If current token is either '|' or '&&' and the immediate next token is not null,
		// then take that next token as the next command-name.
		if( ( (strcmp(tokens[i], "|")==0) || (strcmp(tokens[i], "&")==0) ) && (tokens[i+1]!=(char*)0) ){
			cmd_info->cmd_ptr[j].args[k] = tokens[i];
			cmd_info->cmd_ptr[j].argc = k;
			j++;
			cmd_info->cmd_ptr[j].cmd_name = tokens[++i];
			k=1;
		}
		else{	//Otherwise consider the token as an argument
			cmd_info->cmd_ptr[j].args[k] = tokens[i];
			k++;
		}
	}
	if(cmd_info->cmd_ptr[j].argc == 0)
		cmd_info->cmd_ptr[j].argc = k-1;	//argc for the last command.
	//--i;
	
	//TODO
	/*
	 * The logic here is if any inbuilt command is encountered anywhere, the whole command will be ignored by the shell.
	 * Eg: `ls -l | cd ..` will not cause any effect. Try it on a bash shell. But the reverse somehow works, if you try in a real bash shell.
	 * But at this point we will consider it simple....[Truth is I don't know the logic behind it yet]
	 */
	identify_cmd_type(cmd_info->cmd_ptr, cmd_info->ncommands);
	
	return 0;
}

//Break the command 'cmd' into individual tokens(a WHITESPACE as a delimiter) and store it in 'tokens'
//Returns the no. of tokens, pipes and background commands.
int (*tokenize_cmd(char *cmd, char *tokens[MAX_TOKENS])) [4] {
	//char *tokens[MAX_TOKENS] = {NULL};	//Pointers to each tokens of a command
	int i=1;
	static int counts[4];
	//counts[0]->token_count
	//counts[1]->pipe_count('|')
	//counts[2]->background_count('&')
	//counts[3]->no of commands
	
	//Should always be set to 0. Otherwise previous values will retain and cause invalid results.
	int c;
	for(c=0; c<4; c++)
		counts[c] = 0;
	
	if(is_null(cmd)){
		return NULL;
	}
	tokens[0] = strtok(cmd," ");	//Get each token w.r.t. white spaces
	counts[3]++;	//1st command marked
	for(; i<MAX_TOKENS; i++){
		tokens[i] = strtok(NULL," ");
		if(tokens[i] == NULL)
			break;
		else if(strcmp(tokens[i],"|") == 0){	//Pipe operator encountered
			counts[1]++;
			counts[3]++;
		}
		else if(strcmp(tokens[i],"&") == 0){	//Background operator encountered
			counts[2]++;
			counts[3]++;
		}
	}
	if(i >= MAX_TOKENS)	//Too many tokens in command
		return NULL;
	counts[0] = i;
	return counts;
}

//Identify command type for individual commands pointed to by *commands 
void identify_cmd_type(cmd_t *commands, int n){
	//n = No of cmd_t structs
	int i;
	for(i=0; i<n; i++){
		//FYI...this will always be false for i=0
		if(commands[i].cmd_name == NULL) {
			
			//Command will be in continuation mode if '|' symbol is encountered.
			if(IS_ATLEAST_PRE_PIPE(commands[i-1].type))
				cmd_type |= INCOMPLETE_CMD;
			break;
		}
		if(strcmp(commands[i].cmd_name, "cd") == 0){	//There may be other inbuilt commands, but currently we consider only 'cd' command
			commands[i].type = INBUILT;
			if(IS_ATLEAST_PIPE(cmd_type) || IS_ATLEAST_BKGROUND(cmd_type))
				cmd_type = IGNORE;	//Ignore the whole command
			else
				cmd_type |= INBUILT_CMD;	//Inbuild command
			return;
		}
		else{	//Consider all else regular
			commands[i].type = REGULAR_CMD;
			
			//Check for '|' or '&' or any other operator in the last argument of each command to know the type of command.
			if(strcmp(commands[i].args[commands[i].argc], "|")==0)
				commands[i].type |= PRE_PIPE;
			else if(strcmp(commands[i].args[commands[i].argc], "&")==0)
				commands[i].type |= BKGROUND_CMD;
			
			//Now check if the previous command was a PRE_PIPE so that the current command will be a POST_PIPE
			if(i > 0){
				if(IS_ATLEAST_PRE_PIPE(commands[i-1].type))
					commands[i].type |= POST_PIPE;
			}
		}
	}//for
	cmd_type |= REGULAR_CMD;	//Indicate regular commands
}

//Check for 'exit' command
inline void assert_exit(char *cmd){
	char temp[MAX_CMD_LENGTH] = "";
	strcpy(temp,cmd);
	if(!strcmp(strtok(temp," "),"exit"))
		exit(0);
	
}

//Search for a command in each directory defined by the PATH variable
char *search_cmd_in_path(const char *command){
	/* static */ char PATH[ENV_PATH_SIZE];	//Environment variable PATH
	static char cmd[PATH_MAX]="";	//Full pathname of the command that will be searched
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

//Resolve full pathname for the command
int resolve_cmd_path(cmd_t *cmd_ptr){
	char *temp_buff;
	
	//Search in the PATH environment variable
	if((temp_buff=search_cmd_in_path(cmd_ptr->cmd_name))){
		cmd_ptr->cmd_name = temp_buff;	//If bug then try strcpy()
		return 0;
	}
	return -1;	//Not found
}
//Displays the command infos. Useful for debugging.
void display_command_info(info_cmd *cmd_info){
	
	printf("No. of tokens: %d\n", cmd_info->ntokens);
	printf("No. of pipe operators '|': %d\n", cmd_info->npipes);
	printf("No. of background operators '&': %d\n", cmd_info->nbkgnds);
	printf("No. of commands: %d\n\n", cmd_info->ncommands);
	
	//Global command type flag
	printf("Original command type: ");
	switch (cmd_type){
		case REG_SINGLE:
			printf("REGULAR AND SINGLE\n");
			break;
		case REG_PIPED:
			printf("REGULAR AND PIPED\n");
			break;
		case REG_BKGROUND:
			printf("REGULAR AND BKGROUND\n");
			break;
		case INBUILT_SINGLE:
			printf("INBUILT AND SINGLE\n");
			break;
		case INBUILT:
			printf("INBUILT\n");
			break;
		case IGNORE:
			printf("IGNORED\n");
			break;
		case PIPE_BK:
			printf("PIPED and BKGROUND\n");
			break;
		case PRE_PIPE:
			printf("IGNORED\n");
			break;
		case REG_PREPIPE:
			printf("REGULAR and PRE_PIPE\n");
			break;
		case REG_POSTPIPE:
			printf("REGULAR and POSTPIPE\n");
			break;
		case REG_INCOMPLETE:
			printf("REGULAR, PIPED and INCOMPLETE\n");
			break;

		default:	//Invalid type. Discard the command and prompt for next command
			fprintf(stderr,"INVALID\n");
	}
	printf("\n");
	//Individual command types
	int i;
	for(i=0; i<cmd_info->ncommands; i++){
		printf("Command[%d] type: ", i);
		switch (cmd_info->cmd_ptr[i].type){
			case INBUILT:
				printf("INBUILT");
				break;
			case REGULAR_CMD:
				printf("REGULAR");
				break;
			case REG_BKGROUND:
				printf("REGULAR and BKGROUND");
				break;
			case REG_PREPIPE:
				printf("REGULAR and PREPIPE");
				break;
			case REG_POSTPIPE:
				printf("REGULAR and POSTPIPE");
				break;
			case REG_PREPOST_PIPE:
				printf("REGULAR, PREPIPE and POSTPIPE");
				break;
			case REG_BK_POSTPIPE:
				printf("REGULAR, BKGROUND and POSTPIPE");
				break;
			case NONE:
				printf("NONE");
				break;
			default:
				printf("INVALID");
		}
		printf("\n");
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

//Manage the execution procedure
int manage_execution(info_cmd *target){
	//TODO
	if(IS_ATLEAST_REG(cmd_type)){
		//Execute procedure for regular commands...fork-and-exec
		execute_regular(target);
	}
	else if(IS_ATLEAST_INBUILT(cmd_type)){
		//Execute procedure for inbuilt commands...cd
		execute_inbuilt(target);
	}
	else	//Invalid command type
		return -1;	
	return 0;
}

//Some useful shortcuts
#define TOKENS 		target->ntokens
#define PIPES 		target->npipes
#define BKGNDS		target->nbkgnds
#define COMMANDS	target->ncommands

/* Macro for a simple fork-and-exec mechanism. The code section of the parent process is not included since it depends on the type of command.
 * Therefore, the programmer should write codes for the parent process immediately after the invokation of this macro function.
 */
#define fork_exec(cmd)	pid_t cpid;							\
				if((cpid=fork()) == -1){					\
					fprintf(stderr,"Shell Error: ");			\
					perror("fork");	}					\
				if(cpid == 0){							\
					if(execv(cmd.cmd_name, cmd.args) == -1){	\
						fprintf(stderr,"Shell Error: ");		\
						perror("execv");	}			\
				}
/*
//Simple fork and exec code
inline void fork_exec(cmd_t cmd){
	pid_t cpid;
	if((cpid=fork()) == -1){
		fprintf(stderr,"Shell Error: ");
		perror("fork");
	}
	if(cpid == 0){
		if(execv(cmd.cmd_name, cmd.args) == -1){
			fprintf(stderr,"Shell Error: ");
			perror("execv");
		}
	}
}
*/

//Execution of regular commands. It handles all the execution situations like piping, backgroun, conditional, etc.
void execute_regular(info_cmd *target){
	//pid_t cpid;
	cmd_t *cmd_ptr;
	//Pipe may be necessary.
	int pipefd[2], c_exit_status;
	
	if(target==(info_cmd*)0)
		return;
	cmd_ptr = target->cmd_ptr;
	
	//Resolve the complete pathname of the command.
	if(resolve_cmd_path(cmd_ptr) == -1){
		fprintf(stderr,"Shell Error: Command not found.\n");
		return;
	}
	
	//if(COMMANDS == 1)
	int i;
	for(i=0; i<COMMANDS; i++){
		if(cmd_ptr[i].type == REGULAR_CMD){
			fork_exec(cmd_ptr[i]);
			wait(&c_exit_status);	//Wait for child process to exit
		}
		else if(cmd_ptr[i].type == REG_BKGROUND){
			cmd_ptr[i].args[cmd_ptr[i].argc] = NULL;	//Remove the '&' symbol fomr arg list
			fork_exec(cmd_ptr[i]);
			//Dont wait for child process to exit
		}
		
		/* TODO...For piped commands and others.
		else if(){
		
		}
		*/
	}//end for
}

//Execution of inbuilt commands.
void execute_inbuilt(info_cmd *target){
	
}

#undef TOKENS
#undef PIPES
#undef BKGNDS
#undef COMMANDS
