/**
 * rpiutils.c
 *
 * This fle contains the function definitions for various utility
 * functions for the raspberry pi computer.
 *
 * Version: 1.0.0
 * Author(s): Richard Gale
 */

#include "rpiutils.h"

/**
 * Prints information about the Raspberry Pi this program is running on.
 */
void print_rpi_info(rpi_info info, vec2d origin)
{
    cursput(origin.x, origin.y);
    curscolf( MAGENTA );
    textmode( UNDERLINE );
    fprintf( stdout, "Raspberry Pi information:\n" );
    textmode( NORMAL); 

    // Printing P1Revision
    origin.y++;
    cursput(origin.x, origin.y);
    fprintf( stdout, " - p1_revision: " );
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
    origin.y++;
    cursput(origin.x, origin.y);
    fprintf( stdout, " - Revision: %s\n", info.revision );
    
    // Printing type
    origin.y++;
    cursput(origin.x, origin.y);
    fprintf( stdout, " - Type: %s\n", info.type );
    
    // Printing amount of ram
    origin.y++;
    cursput(origin.x, origin.y);
    fprintf( stdout, " - Ram: %s\n", info.ram );
    
    // Printing manufacturer
    origin.y++;
    cursput(origin.x, origin.y);
    fprintf( stdout, " - Manufacturer: %s\n", info.manufacturer );
    
    // Printing processor
    origin.y++;
    cursput(origin.x, origin.y);
    fprintf( stdout, " - Processor: %s\n", info.processor );
}
