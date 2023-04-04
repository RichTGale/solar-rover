
#include "rpiutils.h"

/**
 * Prints information about the Raspberry Pi this program is running on.
 */
void print_rpi_info( rpi_info info )
{
    fgcolour( MAGENTA );
    textmode( UNDERLINE );
    fprintf( stdout, "Raspberry Pi information:\n" );
    textmode( NORMAL );

    // Printing P1Revision
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
    fprintf( stdout, " - Revision: %s\n", info.revision );
    
    // Printing type
    fprintf( stdout, " - Type: %s\n", info.type );
    
    // Printing amount of ram
    fprintf( stdout, " - Ram: %s\n", info.ram );
    
    // Printing manufacturer
    fprintf( stdout, " - Manufacturer: %s\n", info.manufacturer );
    
    // Printing processor
    fprintf( stdout, " - Processor: %s\n", info.processor );
}
