/*
 *	flags.h
 *
 *	This file contains flag variables used by the shell to determine the characteristics
 *	of commands and behaviour of the shell itself according to different commands.
 */

#ifndef FLAGS_H_
#define FLAGS_H_

#include <stdint.h>

//Error flags. Default=NONE.
typedef enum{NOERROR=0,INVAL_CMD=100,_2LONG_CMD,INVAL_PARAM,NO_PARAM} error_t;

//Global error variable
error_t sh_error;

//Flags for indicating type of the input command(Shell specific command like: cd, Piped commands, Shell script syntaxes, etc. OR regular command
// like: ls,pwd,fdisk,etc...)
typedef uint32_t cmdtype_t;

#define REGULAR_CMD	0x00010000
#define INBUILT_CMD	0x00001000
#define SINGLE_CMD	0x00000100
#define PIPED_CMD	0x00000010
#define BKGROUND_CMD	0x00000001
#define NONE		0x00000000
//Macros for composite flags instead of writing OR operations explicitly. This is especially helpful in deciding execution type for a command.
#define REG_SINGLE	(REGULAR_CMD | SINGLE_CMD)
#define REG_PIPED	(REGULAR_CMD | PIPED_CMD)
#define REG_BKGROUND	(REGULAR_CMD | BKGROUND_CMD)
#define INBUILT		(INBUILT_CMD)

/*
 *	USAGE OF THIS FLAG
 *	(Similar to linux file permission flags, except here we use hexadecimal conventions)
 *	Here, different flag values can be bitwise ORed using the operator '|' to combine more than one behaviour of a command.
 *	Unfortunately, all combinations are not allowed here.
 *	Possible combinations of flag values are:
 *	 -> (REGULAR_CMD | SINGLE_CMD): Indicates that the command is a regular type(eg: ls,pwd,fdisk,sudo,etc.) and no pipe is applied.
 *	 -> (REGULAR_CMD | PIPED_CMD): Indicates that the command is regular and piped to more than one command.
 *	 -> (REGULAR_CMD | BKGROUND_CMD): Indicates that the command is regular and will run in background without letting the terminal wait.
 *	 				This can also simply be considered as BKGROUND_CMD only.
 *	 -> (INBUILT_CMD): Indicates inbuilt command like: exit,cd,etc.
 *	Any other combinations(or even single values) other then mentioned above will have no effect, i.e, the command will be simply neglected.
 */
//All of the above flag will be handled by this variable.
cmdtype_t cmd_type;

#define RESET_FLAG(flag_var)	if(flag_var == ##sh_error) flag_var = NOERROR;		\
				else if(flag_var == ##cmd_type) flag_var = NONE;
				
#define RESET_ALL_FLAGS()	sh_error = NOERROR; cmd_type = NONE;

#endif
