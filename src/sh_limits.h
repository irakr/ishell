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

