#ifndef LINE_H_
#define LINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct line{
    char *str;
    struct line *next;
};

/* This function takes a file pointer 'fp' and read the file in the "r" access mode
 * using the "struct line" as data structure to keep every line of the file
 *
 * Returns the 'head' of the data structure where head->next is the 
 * first line from read file
 */
struct line *read_file(FILE *fp, unsigned int buf_size);


/* Write the file 'from' pointer to the the file 'to' pointer. The file pointer 'to'
 * has to be open in a write mode.
 */
void write_file(FILE *from, FILE *to, unsigned int buf_size);


/* Rotate the file 'from' pointer to the file 'from' pointer. E.g.:
 *
 *
 * File opened in the 'from' pointer:
 * 
 * |123 328  51 64 \n
 * | 45 64  387 23 \n
 * |  6 98  215 314\n
 * |*   +   *   +  \n
 * 
 * File written in the 'to' pointer:
 * 
 * |1  *\n
 * |24  \n
 * |356 \n
 * | ....
 */
void rotate_file(FILE *from, FILE *to, unsigned int buf_size);

#endif // LINE_H_