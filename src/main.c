/**
 * main.c
 * 
 * Controls a remote-control vehicle.
 * Uses the pi-gpio library ( https://github.com/Milliways2/pi-gpio ).
 *
 * Author: Richard Gale.
 * Version: April 2nd, 2023.
 */

#include <stdlib.h>

#include "app.h"

#define FRAMES_PER_SEC 10
#define NANOS_PER_FRAME NANOS_PER_SEC/FRAMES_PER_SEC

int main( int argc, char* argv[] )
{
    // The application
    app a;

    // Preparing the app
    app_init( &a );

    // Running the app
    app_exec( &a );

    // Cleaning up the app
    app_term( &a );

    // Returning from the program
    exit( EXIT_SUCCESS );
}

//int main()
//{
//    rpi_info info;  // Information about the raspberry pi.
//    subproc motion;
//    rover r;    // The rover.
//    struct timespec end_last_frame; // Framerate timer.
//    char command;   // The command read from user input. 
//    
//    // Setting up pi-gpio.
//    setup();
//    
//    // Getting Raspberry Pi information.
//    get_rpi_info( &info );
//    
//    // Printing Raspberry Pi info.
//    print_rpi_info( info );
//    
//    // Initialising the motion sub-process.
//    subproc_init( &motion );
//
//    // Starting motion.
//    subproc_exec( &motion, "sudo motion" ); 
//
//    // Setting up the rover.
//    rover_init( &r );
//    
//    // Starting the timer.
//    start_timer( &end_last_frame );
//    
//    // Reading user input  and responding to it.
//    while ( command!='q' )
//    {
//        if ( check_timer( end_last_frame, NANOS_PER_FRAME  ) )
//        {
//            // Reading user input.
//            command=getch();
//
//            // Responding to the user input.
//            switch ( command )
//            {
//                case 'w' :  rover_change_vel( &r, FORWARDS );
//                            break;
//                case 'a' :  rover_change_vel( &r, LEFT );
//                            break;
//                case 's' :  rover_change_vel( &r, BACKWARDS );
//                            break;
//                case 'd' :  rover_change_vel( &r, RIGHT );
//                            break;
//                case 'x' :  rover_change_vel( &r, STOP );
//                            break;
//            }
//
//            // Printing the current command and information about the rover
//            fprintf( stdout, "command: %c ", command );
//            rover_print( r );
//            fprintf( stdout, "\n" );
//
//            // Restarting the timer.
//            start_timer( &end_last_frame );
//        }
//    }
//
//    // Ending the motion sub-process.
//    subproc_term( &motion );
//
//    // Cleaning up the motion sub-process.
//    subproc_free( &motion );
//   
//    // Cleaning up the rover.
//    rover_free( &r );
//    
//    // Cleaning up pi-gpio.
//    cleanup();
//
//    // Exiting the program.
//    exit( EXIT_SUCCESS );
//}

