/**
 * subproc.c
 *
 * This file contains the internal data and procedure definitions for the 
 * subproc type.
 * The subproc type executes shell commands as a sub/child process.
 *
 * Author: Richard Gale
 * Version: 14th October, 2022
 */

#include "subproc.h"

/**
 * The internal data of the subproc data-structure.
 */
struct subproc_data {
    int fds[2]; // File descriptors.
    pid_t pid;  // Process Id
};

/**
 * Allocates memory to the provided subproc.
 */
void subproc_init(subproc* sp)
{
    *sp = (subproc) malloc( sizeof( struct subproc_data ) );
}

/**
 * Frees the memory allocated to the subproc.
 */
void subproc_free(subproc* sp)
{
    free(*sp);
}

/**
 * Calls dup2() using the provided file-descriptors for its actual parameters.
 * If there is an error, it is printed on stderr and the program exits.
 *
 * dup2() is used to specify the file descriptor being used for a pipe(),
 * file stream, etc...
 */
void duperr( int fdold, int fdnew )
{
    // Attempting to duplicate the file descriptor.
    if ( dup2( fdold, fdnew ) == -1 ) 
    {    
        // There was an error.
        fprintf( stderr, 
                "[ %s ] dup2 failed on fileno() %s\n", 
                timestamp(), strerror( errno ) );
        exit( EXIT_FAILURE );
    }
}

/**
 * Creates a file name from a directory path, a shell command,
 * and a file extension.
 */
void mkfname( char** ptr, char* dir, char* cmd, char* ext )
{
    // A copy of the body
    char* cmd_cpy;

    // Allocating memory for the copy
    cmd_cpy = ( char* ) malloc( sizeof( char ) * ( strlen( cmd ) + 1 ) );

    // Copying the command
    strcpy( cmd_cpy, cmd );

    // Removing unwanted characters from the copy
    rmchar( &cmd_cpy, '/' );
    rmchar( &cmd_cpy, '.' ); 

    sprintf( *ptr, "%s%s%s", dir, cmd_cpy, ext );

    // Unallocating memory for the copy
    free( cmd_cpy );
}

/**
 * Executes the provided command as a sub-process and returns its PID.
 */
void subproc_exec( subproc* sp, char* cmd )
{
    FILE* fout; // File for stdout
    FILE* ferr; // File for stderr
    char* fname_out; // Output file name for stdout.
    char* fname_err; // Output file name for stderr.
    // The relative path of the directory to save the output files in.
    char* fdir = "./output/";
    // The file extensions
    char* fext_out = "_out.txt";
    char* fext_err = "_err.txt";

    // Attempting to create a pipe to use for the child process.
    if ( pipe( ( *sp )->fds ) == -1 )
    {
        // There was an error creating the pipe so we are printing it on
        // and exiting the program.
        fprintf( stderr, "[ %s ] ERROR: In exec_child_process(): "
                            "pipe() - %s\n", 
                            timestamp(), strerror( errno ) );
        exit( EXIT_FAILURE );
    }

    // Attempting to create the child process.
    if ( ( ( *sp )->pid = fork() ) == -1 )
    {
        // There was an error creating the child process so we are printing
        // it and exiting the program.
        fprintf( stderr, "[ %s ] ERROR: In exec_child_process: "
                            "fork() - %s\n", 
                            timestamp(), strerror( errno ) );
        exit( EXIT_FAILURE );
    }
    else if ( ( *sp )->pid == 0 )    // The child process
    {
        // Printing status message.
        fprintf( stdout, "[ %s ] Creating sub-process...\n", timestamp() );

        // Allocating memory to the output file name strings.
        fname_out = ( char* ) malloc( sizeof( char ) * 
                ( strlen( fdir ) + strlen( cmd ) + strlen( fext_out ) + 1 ) );
        fname_err = ( char* ) malloc( sizeof( char ) * 
                ( strlen( fdir ) + strlen( cmd ) + strlen( fext_err ) + 1 ) );
        
        // Creating the file names for the output information
        mkfname( &fname_out, fdir, cmd, fext_out );
        mkfname( &fname_err, fdir, cmd, fext_err );

        // Setting the read descriptor of the child process to 
        // be stdin's descriptor.
        duperr( ( ( *sp )->fds )[0], STDIN_FILENO );
	    
        // Opening file streams.
        fout = open_file( fname_out, "w" );
        ferr = open_file( fname_err, "w" );
        
        // Setting the descriptor of the output file's stream to 
        // be stdout's descriptor.
        duperr( fileno( fout ), STDOUT_FILENO );

        // Setting the descriptor of the error file's stream to 
        // be stderr's descriptor.
        duperr( fileno( ferr ), STDERR_FILENO );

        // Now that the descriptors have been set we can close the 
        // file streams.
        close_file( fout );
        close_file( ferr );

        free( fname_out );
        free( fname_err );

        // Printing status message.
        timestamp( stdout );
        fprintf( stdout, 
                "[ %s ] Sub-process created... Executing command...\n",
                timestamp() );

        // Executing the command as the child process.
        execl ( "/bin/sh", "sh", "-c", cmd, NULL );
        
        // There was an error executing the command so we are printing it.
        fprintf( stderr, "[ %s ] execl failed with error - %s", 
                timestamp(), strerror( errno ) );
    }
}

/**
 * Requests for the provided sub-process to be terminated, waits for it to
 * exit, and reports its exit-status to stdout or stderr if there was an error.
 */
void subproc_term( subproc* sp )
{
    int status; // The exit status of the process.

    // Printing status message.
    fprintf( stdout, "[ %s ] Terminating sub-process...\n", timestamp() );

    // Terminating the process.
    kill( ( *sp )->pid, SIGTERM);

    // Waiting for the process to exit and then printing its status.
    do
    {
        if ( ( ( *sp )->pid = waitpid( ( *sp )->pid, &status, WNOHANG ) ) == -1 )
        {
            // There was an error waiting for the process to exit.
            fprintf( stderr, 
                    "[ %s ] ERROR: in term_child_process(): wait() error!\n",
                    timestamp() );
        }
        else if ( ( *sp )->pid == 0 )
        {
            // The process is still running.
            fprintf( stdout, "[ %s ] Waiting for process to terminate...\n",
                    timestamp() );
            sleep ( 1 );
        }
        else
        {
            // There is no longer a process with the pid so we'll look at
            // what happened to it.
            if ( WIFEXITED( status ) )
            {
                // The process exited normally so we are priting its
                // exit status.
                fprintf( stdout, "[ %s ] The process exited normally "
                        "with exit status %d.\n", 
                        timestamp(), WEXITSTATUS( status ) );
            }
            else if ( WIFSIGNALED( status ) )
            {
                // The process exited because of an uncaught signal.
                fprintf( stdout, 
                        "[ %s ] The process did not exit normally\n",
                        timestamp() );
            }
            else
            {
                // The process did not exit.
                fprintf( stdout, "[ %s ] The child process did not exit\n",
                        timestamp() );
            }
        }
    } while ( ( *sp )->pid == 0 );
}

