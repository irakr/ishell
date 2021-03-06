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
 
/* ************************************************************************************
 * environ.h
 *
 * This file contains shell environment variables just like we see any other shell.
 * ************************************************************************************
 */
 
#include "shell_lib.h"
//#include "flags.h"
#include "sh_limits.h"

#ifndef ENVIRON_H_
#define ENVIRON_H_

/*
 *	PS1 is the string that will be displayed as the prompt screen.
 *	By default PS1 = "user-name@machine-name:absolute-path-of-current-working-directory$"
 *	The ending character is '$' for a user and a '#' for root.
 */
char PS1[MAX_PS1_LENGTH];	//PS1 variable of the shell

extern char **environ;

/*
 ******************************
 * Function declarations      *
 ******************************
 */
//Displays all the environment variables in the variable 'extern char **environ', unless specified explicitly in 'char *name'
void print_environ(char *name);

#endif
