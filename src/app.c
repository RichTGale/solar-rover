/**
 * app.c
 *
 * This file contains the data-structure and function definitions
 * for an app.
 * An app is a type that contains everything that makes up a complete 
 * Command Line Interface (CLI) application.
 *
 * Version: 3rd April, 2023.
 * Author(s): Richard Gale
 */

#include "app.h"

/**
 * The data contained within the app data-type.
 */
struct app_data {
    rpi_info info;  // Information about the raspberry pi
    term t; // The terminal
//    rover r;    // The rover
//    struct timespec end_last_frame; // Framerate timer.
};

/**
 * Prepares the provided app for use.
 */
void app_init( app* ap )
{
    *ap = (app) malloc( sizeof( struct app_data ) );
    term_init( &(*ap)->t );
}

/**
 * Runs the provided app.
 */
void app_exec( app* ap )
{
    system( "tput clear" );
    drawfile(
           "../../art/test.txt",
           ( term_getorigin( (*ap)->t ) ),
           ( term_getbounds( (*ap)->t ) )
           );

}

/**
 * Ends the provided app.
 */
void app_term( app* ap )
{
    term_free( &(*ap)->t );
    free ( *ap );
}
