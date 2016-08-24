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
 * inbuilt_table.h
 *
 * This file contains the hash(not quiet) table of inbuilt commands.
 */

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "shell_lib.h"

#ifndef INBUILT_TABLE_H_
#define INBUILT_TABLE_H_

// Command IDs
#define ID_INVALID  -1
#define ID_CHDIR    0
#define ID_TYPE     1
//TODO....Find out about all the inbuilt commands and fill up the macros above.

/*	Fixed-valued list of inbuilt commands with there valid arguments.
 * .cmd_name is the command name, .args is the collection of valid arguments that the command accepts to be able to run.
 * .type is just an indication that it is an inbuilt command type(Not so important). Just provides identity.
 */
static cmd_t inbuilt_cmds[] =
{
	{ .cmd_name="cd", .args={".", "..", "-", "~"}, .type=INBUILT_CMD}
};

/*  Verify whether '*cmd' satisfies all the requirements of a particular built-in command    */
int verify_cmd(cmd_t *cmd);

/*  Execute sub-routines for the given command 'cmd_ptr' and 'id' pair.*/
inline int exec_inbuild_cmd(cmd_t *cmd_ptr, int id);

#endif
