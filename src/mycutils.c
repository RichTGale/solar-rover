/**
 * mycutils.c
 *
 * This file contains the definitions of various utility functions.
 *
 * Author: Richard Gale
 * Version: 14th October, 2022
 */

#include "mycutils.h"

/**
 * Prints information about the Raspberry Pi this program is running on.
 */
void print_rpi_info( rpi_info info )
{
    // Printing P1Revision
    fprintf( stdout, "p1_revision: " );
    switch ( info.p1_revision )
    {
    	case 0 :
            fprintf( stdout, "None" );
            break;
        case 1 :
            fprintf( stdout, "Pi B" );
            break;
        case 2 :
            fprintf( stdout, "Pi B v2" );
            break;
        case 3 :
            fprintf( stdout, "40 pin" );
            break;
        
    }
    fprintf( stdout, "\n" );

    // Printing revision
    fprintf( stdout, "Revision: %s\n", info.revision );
    
    // Printing type
    fprintf( stdout, "Type: %s\n", info.type );
    
    // Printing amount of ram
    fprintf( stdout, "Ram: %s\n", info.ram );
    
    // Printing manufacturer
    fprintf( stdout, "Manufacturer: %s\n", info.manufacturer );
    
    // Printing processor
    fprintf( stdout, "Processor: %s\n", info.processor );
}

/*
 * Returns a char that was input by the user.
 * Dowsn't wait for the user to press enter.
 */
char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

/**
 * Obtains the current time, storing it in a timespec.
 */
void start_timer( struct timespec* ts )
{
    // Obtaining the current time.
    if ( (clock_gettime( CLOCK_REALTIME, ts) ) == -1 )
    {
        fprintf( stderr, 
                "[ %s ] ERROR: in function start_timer(): clock_gettime() "
                "returned an error: %s",
                timestamp(), strerror( errno ) );
    }
}

/**
 * Returns true if the provided wait-time has elapsed since the
 * provided timespec was given a time.
 */
bool check_timer( struct timespec start, uint64_t wait_time )
{
    struct timespec current; // The current time.
    struct timespec elapsed; // The elapsed time
    bool wait_has_elapsed = false; // Whether the designated time has elapsed.

    // Obtaining the current time.
    clock_gettime( CLOCK_REALTIME, &current );

    // Calculating the elapsed time.
    elapsed.tv_sec = current.tv_sec - start.tv_sec;
    elapsed.tv_nsec = current.tv_nsec - start.tv_nsec;

    // Determining whether the disignated time has elapsed.
    if ( ( elapsed.tv_sec * NANOS_PER_SEC ) + elapsed.tv_nsec >= wait_time )
    {
        wait_has_elapsed = true; // The designated time has elapsed.
    }

    // Retuning whether the designated time has elapsed.
    return wait_has_elapsed;
}

/**
 * Removes all cases of the provided char from the string at the
 * provided pointer.
 */
void rmchar( char** str, char remove )
{
    int len = strlen( *str );   // The original length of the string.
    int total_chars = strlen( *str );   // The current length of the string.
    int i; // An indexer
    char* src; // The address of where to start moving the memory.
    char* dst; // The address of where to move the memory to.

    // Overwriting the unwanted character.
    for ( i = 0; i < len; i++ )
    {
        if ( ( *str )[i] == remove )
        {
            // Setting the source and destinations points for moving.
            src = &( ( *str )[i + 1] );
            dst = &( ( *str )[i] );

            // Overwriting an unwanted character.
            memmove(dst, src, ( sizeof(char) * strlen( *str ) ) - 
                                    ( sizeof( char ) * i ) );

            // Decrementing the index so we will check the 
            // replacement character.
            i--;

            // Recording the new length of the string.
            total_chars--;
        }
    }

    // Designating the end of the string.
    ( *str )[total_chars] = '\0';
}

/**
 * Returns a timestamp.
 */
char* timestamp()
{
    time_t current_time;
    char* c_time_string;

    // Obtaining the current time.
    current_time = time( NULL );
    if ( current_time == ( ( time_t ) - 1 ) )
    {
        // There was an error obtaining the time so we're printing 
        // a message and exiting.
        fprintf( stderr, "Failure to obtain the current time.\n" );
        exit( EXIT_FAILURE );
    }

    // Converting time to local time format.
    c_time_string = ctime(&current_time);
    if ( c_time_string == NULL )
    {
        // There was an error converting the time to a string so we're
        // printing a message and exiting.
        fprintf( stderr, "Failure to convert the current time.\n" );
        exit( EXIT_FAILURE );
    }

    // Removing the newline character that was added by ctime().
    rmchar( &c_time_string, '\n' );

    return c_time_string;
}

/**
 * Opens the file with the provided file name in the provided mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* open_file( char* fname, char* mode )
{
    FILE* fp;   // The pointer to the file stream.

    // Attempting to open the file.
    if ( ( fp = fopen( fname, mode ) ) == NULL )
    {
        // There was an error opening the file.
        fprintf( stderr, "[ %s ] ERROR: In open_file(): "
                    "Could not open file %s\n", timestamp(), fname );
        exit( EXIT_FAILURE );
    }

    // Returning the pointer to the file stream.
    return fp;
}

/**
 * Writes the provided string to the provided file stream.
 */
void write_ch( FILE* fstreamp, char ch )
{
    fprintf( fstreamp, "%c", ch ); 
}

/**
 * Returns the contents of the provided file stream as a string.
 */
void read_ch( FILE* fstreamp, char* ch )
{
    if ( ( *ch=fgetc( fstreamp ) ) == EOF )
    {
        fprintf( stderr, "File read error.\n" );
    }
}

/**
 * Closes the provided file stream. If there is an error, it is printed on
 * stderr and the program will exit.
 */
void close_file( FILE* fp )
{
    // Attempting to close the file stream.
    if ( fclose( fp ) != 0 )
    {
        // There was an error closing the file stream.
        fprintf( stderr, "[ %s ] Error closing file\n", timestamp() );
        exit( EXIT_FAILURE );
    }
}