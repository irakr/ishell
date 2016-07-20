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
 *	flags.h
 *
 *	This file contains flag variables used by the shell to determine the characteristics
 *	of commands and behaviour of the shell itself according to different commands.
 */

#ifndef FLAGS_H_
#define FLAGS_H_

#include <stdint.h>

//Error flags. Default=NONE.
typedef enum{NOERROR=0, INVAL_CMD=100, _2LONG_CMD, INVAL_PARAM, NO_PARAM} error_t;

//Global error flag
error_t sh_error;

//Flags for indicating type of the input command(Shell specific command like: cd; Piped commands; Shell script syntaxes etc.; OR regular commands
// like: ls,pwd,fdisk,etc...)
typedef uint32_t cmdtype_t;

//Macro representing values used by cmdtype_t flags
#define REGULAR_CMD	0x00010000
#define INBUILT_CMD	0x00001000
#define SINGLE_CMD	0x00000100
#define PIPED_CMD	0x00000010
#define BKGROUND_CMD	0x00000001
#define NONE		0x00000000
#define IGNORE		0x10000000

/*
 * ---- Special macro values ----
 * PRE_PIPE -> A command before a '|' symbol
 * POST_PIPE -> A command after a '|' symbol
 * NOTE: These values are not to be used for global 'cmd_type' flag.
 */
#define PRE_PIPE	0x00000007
#define POST_PIPE	0x00000070

//Macros for composite flags instead of writing OR operations explicitly. This is especially helpful in deciding execution type for a command.
#define REG_SINGLE		(REGULAR_CMD | SINGLE_CMD)
#define REG_PIPED		(REGULAR_CMD | PIPED_CMD)
#define REG_BKGROUND		(REGULAR_CMD | BKGROUND_CMD)
#define INBUILT_SINGLE		(INBUILT | SINGLE_CMD)
#define INBUILT			(INBUILT_CMD)
#define PIPE_BK			(PIPED_CMD | BKGROUND_CMD)
#define REG_PREPIPE		(REGULAR_CMD | PRE_PIPE)
#define REG_POSTPIPE		(REGULAR_CMD | POST_PIPE)
#define REG_PREPOST_PIPE	(REGULAR_CMD | PRE_PIPE | POST_PIPE)
#define REG_BK_POSTPIPE		(REG_BKGROUND | POST_PIPE)

//For checking whether the flag determined by 'c' atleast holds any significant flags using masks
#define IS_ATLEAST_PIPE(c)		((c & 0x000000f0)==0x010)
#define IS_ATLEAST_BKGROUND(c)		((c & 0x0000000f)==0x001)
#define IS_PRE_PIPE(c)			((c & 0x0000000f)==0x007)
#define IS_POST_PIPE(c)			((c & 0x000000f0)==0x070)

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
//Global flag that determines a command type of the original command(not the broken down commands).
cmdtype_t cmd_type;

#define RESET_FLAG(flag_var)	if(flag_var == ##sh_error) flag_var = NOERROR;		\
				else if(flag_var == ##cmd_type) flag_var = NONE;
				
#define RESET_ALL_FLAGS()	sh_error = NOERROR; cmd_type = NONE;

#endif
