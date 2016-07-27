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
 * basic_utils.h
 *
 * This file contains some macros for very basic but important functionalities. For eg: Checking for a NULL value, etc...
 */

#if !defined(BASIC_UTILS_H)
#define BASIC_UTILS_H

#define is_null(ptr)	((ptr==NULL) || ((char)ptr==0))

//Input stream flusher
#define flush_stdin() 	int input = getchar();			\
			while(input!='\n' && input!=EOF) 	input=getchar();


/*
 ****************************
 * Function declarations	*
 ****************************
 */

void cleanup_all(info_cmd *);		//Cleanup all dynamically allocated resource taken up by the current program.
// [Try to make this function take variable argument list so that even if new data resources to be cleaned up is discovered in
// the future, we can simply pass it to this function without having to make separate function for it.]

#endif	//BASIC_UTILS_H
