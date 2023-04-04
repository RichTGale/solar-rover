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
}

/**
 * Draws the app starting screen.
 */
void app_startscreen( app a )
{
    coord2D origin;
    coord2D bounds;
    char* title = "solar";
    char filepath_buf[ 100 ];
    int c;

    origin = term_getorigin( a->t );
    bounds = term_getbounds( a->t );

    // Clearing the terminal
    term_clear();

    // Drawing the program title
    drawstr( "solar", origin, bounds );
    origin.y += 8;
    drawstr( "rover", origin, bounds );

    /* Printing information about the raspberry pi this program
     * is running on. */
    print_rpi_info( a->rpiinfo );
}

/**
 * Runs the provided app.
 */
void app_exec( app* ap )
{
    struct timespec end_last_frame; // Framerate timer.
    bool first;
    char usrin;

    first = true;

    // Starting the timer.
    start_timer( &end_last_frame );
    
    while ( usrin != 'q' )
    {
	    if ( check_timer( end_last_frame, NANOS_PER_FRAME ) )
	    {
	        term_clear();
	
	        if ( first )
	        {
	            app_startscreen( *ap );
	            first = false;
	        }
	
	        // TODO: Draw CLI

	        
	        // Getting user input
	        usrin = getch();
	
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
                case 'b' :  first = true;;
                            break;
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
