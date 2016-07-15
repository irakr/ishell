#define MAX_CMD_LENGTH 512	//characters(bytes)
#define MAX_PS1_LENGTH 512	//Max length of PS1 variable
#define ENV_PATH_SIZE 2048	//Assigned size for PATH environment variable

/*	Max tokens in a command(i.e., No of components in a command allowed)
 *	Eg: For a command: $ls -l | grep .txt$
 *	Here, tokenss are: 'ls', '-l', '|', 'grep', '.txt$'
 */
#define MAX_TOKENS 50

//Maximum no of arguments to a command
#define MAX_ARGS 10

//Maximum no. of combined or piped commands
//Eg of combined commands: $ sudo fdisk -l;	where 'sudo' and 'fdisk' are separate commands.
//Eg of piped commands:	$ fdisk -l | tee fdisk_info.txt;	where 'fdisk' is the first command and 'tee' is the command after the pipe operator.
#define MAX_CMDS	15

//Max path length
#if !defined(PATH_MAX)
#define PATH_MAX 512
#endif 

//Maximum no of pipes that can be created for multiple commands
#define MAX_PIPE 10

