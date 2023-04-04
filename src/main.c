/**
 * main.c
 * 
 * Controls a remote-control vehicle.
 * Uses the pi-gpio library ( https://github.com/Milliways2/pi-gpio ).
 *
 * Version: April 4th, 2023.
 * Author: Richard Gale.
 */

#include <stdlib.h>

#include "app.h"

int main( int argc, char* argv[] )
{
    // The application
    app a;

    // Preparing the app
    fprintf( stdout, "Setting up..\n" );
    app_init( &a );
    fprintf( stdout, "Done\n" );

    // Running the app
    fprintf( stdout, "Running the app..\n" );
    app_exec( &a );

    // Cleaning up the app
    fprintf( stdout, "Cleaning up..\n" );
    app_term( &a );

    // Exiting the program
    fprintf( stdout, "Exiting the program..\n" );
    exit( EXIT_SUCCESS );
}
