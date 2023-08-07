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

    // Preparing the rover
    fprintf( stdout, " - Setting up the rover..\n" );
    rover_init( &(*ap)->r );

    // Initialising whether the app should continue running
    (*ap)->isrunning = true;

    // Initialising whether the app should draw the start page
    (*ap)->drawstart = true;
}

/**
 * Ends the provided app.
 */
void app_term( app* ap )
{
    // Cleaning up the rover
    rover_free( &(*ap)->r );

    // Cleaning up pi-gpio
    cleanup();
    
    // Unallocating the app's memory
    free ( *ap );
}

/**
 * Draws the app starting screen.
 */
void app_startscreen(app a)
{
    vec2d origin;
    vec2d bounds;
    vec2d message_origin;
    char* message = "<Press any key>";

    origin.x = 1;
    origin.y = 1;
    bounds = termres();

    // Clearing the terminal
    termclear();

    // Drawing the program title
    textmode(NORMAL);
    termprintfs("../../art/title.txt", &origin);

    /* Printing information about the raspberry pi this program
     * is running on. */
    cursput(origin.x, origin.y);
    print_rpi_info(a->rpiinfo);

    /* Printing the message. */
    message_origin.x = 42/2 - (strlen(message) / 2);
    message_origin.y = 24;
    textmode(BLINK);
    termprint(message, message_origin);
    textmode(NORMAL);
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
	        termclear();

            if ( !(*ap)->drawstart )
            {
	            // TODO: Draw CLI
                
                // Getting user input
	            usrin = scanc_nowait();
                
                // Processing user input
                app_processusrin( ap, usrin );
            }
            else if ( (*ap)->drawstart )
	        {
	            app_startscreen( *ap );
                (*ap)->drawstart = false;
	        
                // Waiting for the user to press any key
	            usrin = scanc_nowait();
	        }
            start_timer( &end_last_frame );
	    }
    }
}

