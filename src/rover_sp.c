/**
 * rover_v1.c
 * 
 * Uses the pi-gpio library ( https://github.com/Milliways2/pi-gpio )
 */

#include <pi-gpio.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#include "mycutils.h"
#include "rover.h"

#define FRAMES_PER_SEC 10
#define NANOS_PER_FRAME NANOS_PER_SEC/FRAMES_PER_SEC


/**
 * Motor direction identities.
 */
//enum MotorDirection { UNKNOWN, FORWARDS, BACKWARDS, STOPPED };

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

int main()
{
    FILE* fstreamp;
    rpi_info info;  // Information about the raspberry pi
    struct timespec end_last_frame; // Framerate timer
    int dc; // The duty cycle
    char ch;
    
    // Setting up pi-gpio
    setup();
    
    // Getting Raspberry Pi information
    get_rpi_info( &info );
    
    // Printing Raspberry Pi info
    print_rpi_info( info );
    
    // Setting up the rover
    rover_init();
    
    start_timer( &end_last_frame );

    dc=100;
    
    while ( ch!='q' )
    {
        if ( check_timer( end_last_frame, NANOS_PER_FRAME  ) )
        {
            // Reading the current command
            fstreamp=open_file( "./current_command.txt", "r" );
            read_ch( fstreamp, &ch );
            close_file( fstreamp );
            
            // Clearing the current command from the file
            fstreamp=open_file( "./current_command.txt", "w" );
            close_file( fstreamp );

            // Printing the current command to stdout
            fprintf( stdout, "ch: %c\n", ch );

            // Responding to the command
            switch ( ch )
            {
                case 'w' :
                    forward( dc );
                    break;
                case 'a' :
                    left( dc );
                    break;
                case 's' :
                    backward( dc );
                    break;
                case 'd' :
                    right( dc );
                    break;
                case 'x' :
                    stop( 0 );
                    break;
            }
            start_timer( &end_last_frame );
        }
    }
   
    // Cleaning up the rover
    rover_free();
    
    // Cleaning up pi-gpio
    cleanup();
    
    return 0;
}
