/**
 * mycutils.h
 *
 * This file contains the public declarations for various
 * utility funtions.
 *
 * Author: Richard Gale
 * Version: 11th March, 2023
 */

#ifndef MYCUTILS_H
#define MYCUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h> // strerror()
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

#include "array.h"

#define NANOS_PER_SEC 1000000000

/********************************* Time **************************************/

/**
 * This function returns true if a number of nano-seconds equal to or greater
 * than wait_time has elapsed since start.
 */
bool check_timer(struct timespec ts_start, uint64_t wait_time);

/**
 * This function obtains the current time, storing it in the timespec
 * provided to it.
 */
void start_timer(struct timespec* ts);

/**
 * This function returns a string that represent the current time.
 */
char* timestamp();

/******************************** In/Out *************************************/

/**
 * This function asks the user to input a char in response to a prompt supplied
 * to it, then stores it in the supplied char pointer. 
 */
void read_userc(char* cptr, char* prompt);

/**
 * This function returns a char that was input by the user. It doesn't wait
 * for the user to press enter. (Not my code)
 */
char read_userc_nowait();

/**
 * Closes the provided file stream. If there is an error, it is printed on
 * stderr and the program will exit.
 */
void close_file(FILE* fp);

/**
 * This function opens a file that has a name that matches fname. It opens the
 * file in the mode specified by mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* open_file(char* fname, char* mode);

/**
 * This function assigns the next char in the file stream provided to it to
 * the buffer provided to it.
 */
bool read_filec(FILE* fstreamp, char* buf);

/**
 * This function assigns the next line in the file stream provided to it to
 * the string provided to it. It returns true if the line was read successfully
 * or false if EOF was reached. If an error occurs the program will exit.
 * Make sure to free() the buffer when you're finished with it.
 */
bool read_fileln(FILE* fstreamp, char** buf);

/**
 * This function writes the char provided to it to the file stream provided to
 * it.
 */
void write_filec(FILE* fstreamp, char ch);

/**
 * This function writes the string provided to it to the file steam provided
 * to it.
 */
void write_str(FILE* fstreamp, char* str);


/******************************** Strings ************************************/

/**
 * This function dynamically allocates only the needed amount of memory to a
 * string based on the argument list, then concatenates the argument list into 
 * the supplied format and stores it in the supplied string pointer.
 */
void stringf(char** sptr, char *fmt, ...);

/**
 * This function removes all cases of the provided char from the string at the
 * provided pointer.
 */
void stringrmc(char** str, char remove);

#endif // MYCUTILS_H
