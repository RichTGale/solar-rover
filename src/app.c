/**
 * app.c
 *
 * This file contains the data-structure and function definitions
 * for an app.
 * An app is a type that contains everything that makes up a complete 
 * Command Line Interface (CLI) application.
 *
 * Version: 4th April, 2023.
 * Author(s): Richard Gale
 */

#include "app.h"

/**
 * The data contained within the app data-type.
 */
struct app_data {
    rpi_info rpiinfo;   // Information about the raspberry pi
    term t;             // The terminal
    rover r;            // The rover
    bool isrunning;     // Whether the app should continue running
    bool drawstart;     // Whether the app should draw the start page
};

/**
 * Prepares the provided app for use.
 */
void app_init( app* ap )
{
    // Allocating memory for the app
    *ap = (app) malloc( sizeof( struct app_data ) );
    
    // Setting up pi-gpio
    fprintf( stdout, " - Setting up pi-gpio..\n" );
    setup();

    /* Getting information about the raspberry pi that this
     * program is running on. */
    fprintf( stdout, " - Getting rpi info..\n" );
    get_rpi_info( &(*ap)->rpiinfo );

    // Preparing the CLI utility
    fprintf( stdout, " - Setting up the terminal utility..\n" );
    term_init( &(*ap)->t );

    // Preparing the rover
    fprintf( stdout, " - Setting up the rover..\n" );
    rover_init( &(*ap)->r );

    // Initialising whether the app should continue running
    (*ap)->isrunning = true;

    // Initialising whether the app should draw the start page
    (*ap)->drawstart = true;
}

/**
 * Draws the app starting screen.
 */
void app_startscreen( app a )
{
    coord2D origin;
    coord2D bounds;
    coord2D message_origin;
    char* title = "solar";
    char filepath_buf[ 100 ];
    char* message = "<Press any key>";
    int c;

    origin.x = 1;
    origin.y = 1;
    bounds = term_getbounds( a->t );

    // Clearing the terminal
    term_clear();

    // Drawing the program title
    drawstr( "solar", origin, bounds );
    origin.y += 8;
    drawstr( "rover", origin, bounds );

    /* Printing information about the raspberry pi this program
     * is running on. */
    placecursor( 1, 16);
    print_rpi_info( a->rpiinfo );

    message_origin.x = 42/2 - ( strlen( message ) / 2 );
    message_origin.y = 24;

    textmode( BLINK );
    printstr( message, message_origin );
    textmode( NORMAL );
}


void app_processusrin( app* ap, char usrin )
{
    // Processing user input and sending it to the rover.
    switch ( usrin )
    {
        case 'w' :  rover_change_vel( &(*ap)->r, FORWARDS );
                    break;
        case 'a' :  rover_change_vel( &(*ap)->r, LEFT );
                    break;
        case 's' :  rover_change_vel( &(*ap)->r, BACKWARDS );
                    break;
        case 'd' :  rover_change_vel( &(*ap)->r, RIGHT );
                    break;
        case 'x' :  rover_change_vel( &(*ap)->r, STOP );
                    break;
        case 'b' :  (*ap)->drawstart = true;
                    break;
        case 'q' :  (*ap)->isrunning = false;
                    break;
    }
}

/**
 * Runs the provided app.
 */
void app_exec( app* ap )
{
    struct timespec end_last_frame; // Framerate timer.
    char usrin;

    // Starting the timer.
    start_timer( &end_last_frame );
    
    while ( (*ap)->isrunning )
    {
	    if ( check_timer( end_last_frame, NANOS_PER_FRAME ) )
	    {
	        term_clear();

            if ( !(*ap)->drawstart )
            {
	            // TODO: Draw CLI
                
                // Getting user input
	            usrin = getch();
                
                // Processing user input
                app_processusrin( ap, usrin );

            }
            else if ( (*ap)->drawstart )
	        {
	            app_startscreen( *ap );
                (*ap)->drawstart = false;
	        
                // Waiting for the user to press any key
	            usrin = getch();
	        }
            start_timer( &end_last_frame );
	    }
    }
}

/**
 * Ends the provided app.
 */
void app_term( app* ap )
{
    // Cleaning up the rover
    rover_free( &(*ap)->r );

    // Cleaning up the CLI utility
    term_free( &(*ap)->t );

    // Cleaning up pi-gpio
    cleanup();
    
    // Unallocating the app's memory
    free ( *ap );
}
