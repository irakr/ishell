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
 * inbuilt_table.c
 *
 * This file defines all the functions for dealing with the inbuilt command table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_lib.h"
#include "inbuilt_table.h"
#include "basic_utils.h"

/* Verify whether '*cmd' is an inbuilt command and also satisfies all the requirements of a particular built-in command.
 * Returns the index number of the inbuilt command table.
*/
int verify_cmd(cmd_t *cmd){     //TODO...
    if(is_null(cmd))
        return;

    int i, n = sizeof(inbuilt_cmds) / sizeof(cmd_t);   //Compute the no of elements in the inbuilt_cmds struct.
    //printf("n =  %d\n", n);
    //Check command names in 'cmd' against all entries in 'inbuilt_cmds' struct
    for(i=0; i<n; i++){
        if(!strcmp(cmd->cmd_name, inbuilt_cmds[i].cmd_name)){
            printf("Shell Info [%s:%d]: Valid inbuilt command '%s'\n", __FILE__, __LINE__, cmd->cmd_name);
            break;
        }
    }
    if(i >= n){
        fprintf(stderr, "Shell Info [%s:%d]: Invalid inbuilt command '%s'\n", __FILE__, __LINE__, cmd->cmd_name);
        return ID_INVALID;
    }
    return i;
}

/*  Execute sub-routines for the given command 'cmd_ptr' and 'id' pair.*/
inline int exec_inbuild_cmd(cmd_t *cmd_ptr, int id){
    //TODO...
    switch (id){
        case ID_CHDIR:  //cd
            if(chdir(cmd_ptr->args[1]) == -1){
                fprintf(stderr, "Shell Info [%s:%d]: \n", __FILE__, __LINE__, cmd_ptr->cmd_name);
                perror("chdir");
            }
            break;
        //Also include codes for other inbuilt commands here...
        default:
            fprintf(stderr,"Shell Info [%s:%d]: The inbuilt command '%s' is not yet functional\n", __FILE__, __LINE__, cmd_ptr->cmd_name);
    }
    return;
}
