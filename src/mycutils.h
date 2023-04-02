/**
 * mycutils.h
 *
 * This file contains the public declarations for various
 * utility funtions.
 *
 * Author: Richard Gale
 * Version: 14th October, 2022
 */

#ifndef MYCUTILS_H
#define MYCUTILS_H

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

#define NANOS_PER_SEC 1000000000

/**
 * Prints information about the Raspberry Pi this program is running on.
 */
void print_rpi_info( rpi_info info );

/*
 * Returns a char that was input by the user.
 * Dowsn't wait for the user to press enter.
 */
char getch(); 

/**
 * Obtains the current time, storing it in a timespec.
 */
void start_timer( struct timespec* ts );

/**
 * Returns true if the provided wait-time has elapsed since the
 * provided timespec was given a time.
 */
bool check_timer( struct timespec ts_start, uint64_t wait_time );

/**
 * Removes all cases of the provided char from the string at the
 * provided pointer.
 */
void rmchar( char** str, char remove );

/**
 * Returns a timestamp.
 */
char* timestamp();

/**
 * Opens the file with the provided file name in the provided mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* open_file( char* fname, char* mode );

/**
 * Writes the provided string to the provided file stream.
 */
void write_ch( FILE* fstreamp, char ch );

/**
 * Returns the contents of the provided file stream as a string.
 */
void read_ch( FILE* fstreamp, char* ch );

/**
 * Closes the provided file stream. If there is an error, it is printed on
 * stderr and the program will exit.
 */
void close_file( FILE* fp );

#endif //MYCUTILS
