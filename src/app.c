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
    rpi_info rpiinfo;                   // Information about the raspberry pi
    rover r;                            // The rover
    enum MotorDirection new_direction;  // The new motor direction
    bool is_running;                    // Whether the app is running
    bool start_screen_on;               // Whether the starting screen is on
    bool manual_drive_screen_on;        // Whether the control screen is on
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
    (*ap)->is_running = true;

    // Initialising which cli screen to draw
    (*ap)->start_screen_on = true;
    (*ap)->manual_drive_screen_on = false;

    (*ap)->new_direction = STOP;
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
void draw_start_screen(app a)
{
    vec2d origin;
    vec2d bounds;
    vec2d message_origin;
    char* message = "<Press any key or 'q' to quit>";

    origin.x = 1;
    origin.y = 1;
    bounds = termres();

    // Drawing the program title
    textmode(NORMAL);
    termprintfs("../../art/title.txt", &origin, WHITE, NORMAL);

    /* Printing information about the raspberry pi this program
     * is running on. */
    cursput(origin.x, origin.y);
    print_rpi_info(a->rpiinfo);

    /* Printing the message. */
    message_origin.x = 42/2 - (strlen(message) / 2);
    message_origin.y = 24;
    textmode(BOLD);
    termprint(message, message_origin);
    textmode(NORMAL);
}

void draw_manual_drive_screen( app a )
{
    vec2d uarrow_origin;    /* Origin of the up arrow image. */
    vec2d darrow_origin;    /* Origin of the down arrow image. */
    vec2d larrow_origin;    /* Origin of the left arrow image. */
    vec2d rarrow_origin;    /* Origin of the right arrow image. */

    /* Initialise arrow image origins. */
    uarrow_origin.x = 9;
    uarrow_origin.y = 2;
    darrow_origin.x = uarrow_origin.x;
    darrow_origin.y = uarrow_origin.y + 4;
    larrow_origin.x = uarrow_origin.x - 5;
    larrow_origin.y = uarrow_origin.y + 3;
    rarrow_origin.x = uarrow_origin.x + 2;
    rarrow_origin.y = uarrow_origin.y + 3;

    /* Draw a coloured arrow for the direction that was last pressed. Draw
     * white arrows for the rest. */    
    switch ( a->new_direction )
    {
        case FORWARDS: 
            termprintfs("../../art/up_arrow.txt", &uarrow_origin, GREEN, BOLD);
            termprintfs("../../art/down_arrow.txt", &darrow_origin, WHITE, NORMAL);
            termprintfs("../../art/left_arrow.txt", &larrow_origin, WHITE, NORMAL);
            termprintfs("../../art/right_arrow.txt", &rarrow_origin, WHITE, NORMAL);
            break;
        case BACKWARDS: 
            termprintfs("../../art/up_arrow.txt", &uarrow_origin, WHITE, NORMAL);
            termprintfs("../../art/down_arrow.txt", &darrow_origin, GREEN, BOLD);
            termprintfs("../../art/left_arrow.txt", &larrow_origin, WHITE, NORMAL);
            termprintfs("../../art/right_arrow.txt", &rarrow_origin, WHITE, NORMAL);
            break;
        case LEFT:
            termprintfs("../../art/up_arrow.txt", &uarrow_origin, WHITE, NORMAL);
            termprintfs("../../art/down_arrow.txt", &darrow_origin, WHITE, NORMAL);
            termprintfs("../../art/left_arrow.txt", &larrow_origin, GREEN, BOLD);
            termprintfs("../../art/right_arrow.txt", &rarrow_origin, WHITE, NORMAL);
            break;
        case RIGHT:
            termprintfs("../../art/up_arrow.txt", &uarrow_origin, WHITE, NORMAL);
            termprintfs("../../art/down_arrow.txt", &darrow_origin, WHITE, NORMAL);
            termprintfs("../../art/left_arrow.txt", &larrow_origin, WHITE, NORMAL);
            termprintfs("../../art/right_arrow.txt", &rarrow_origin, GREEN, BOLD);
            break;
        case STOP:
            termprintfs("../../art/up_arrow.txt", &uarrow_origin, WHITE, NORMAL);
            termprintfs("../../art/down_arrow.txt", &darrow_origin, WHITE, NORMAL);
            termprintfs("../../art/left_arrow.txt", &larrow_origin, WHITE, NORMAL);
            termprintfs("../../art/right_arrow.txt", &rarrow_origin, WHITE, NORMAL);
    }
}

void update( app* ap, char user_in )
{
    if ( (*ap)->start_screen_on )
    {
        switch ( user_in )
        {
	        case 'q' :  (*ap)->start_screen_on = false;
                        (*ap)->is_running = false;
                        break;
            default  :  (*ap)->start_screen_on = false;
                        (*ap)->manual_drive_screen_on = true;
                        break;
        }
    }
    else if ( (*ap)->manual_drive_screen_on )
    {
        switch ( user_in )
        {
            case 'w' :  rover_change_direction( &(*ap)->r, FORWARDS );
                        (*ap)->new_direction = FORWARDS;
                        break;
            case 'a' :  rover_change_direction( &(*ap)->r, LEFT );
                        (*ap)->new_direction = LEFT;
                        break;
            case 's' :  rover_change_direction( &(*ap)->r, BACKWARDS );
                        (*ap)->new_direction = BACKWARDS;
                        break;
            case 'd' :  rover_change_direction( &(*ap)->r, RIGHT );
                        (*ap)->new_direction = RIGHT;
                        break;
            case 'x' :  rover_change_direction( &(*ap)->r, STOP );
                        (*ap)->new_direction = STOP;
                        break;
            case 'j' :  rover_rotate_zaxis( &(*ap)->r, CLOCKWISE );
                        break;
            case 'l' :  rover_rotate_zaxis( &(*ap)->r, ANTICLOCKWISE );
                        break;
            case 'q' :  (*ap)->start_screen_on = true;
                        (*ap)->manual_drive_screen_on = false;
                        rover_change_direction( &(*ap)->r, STOP );
                        (*ap)->new_direction = STOP;
                        break;
        }
    }
}

void draw( app a )
{
    termclear();

    if ( a->start_screen_on )
    {
        draw_start_screen( a );
    }
    else if ( a->manual_drive_screen_on )
    {
        draw_manual_drive_screen( a );
    }
}

/**
 * Runs the provided app.
 */
void app_exec( app* ap )
{
    struct timespec end_last_frame; // Framerate timer.
    char user_in;

    // Starting the timer.
    start_timer( &end_last_frame );
 
    draw( *ap );

    while ( (*ap)->is_running )
    {
	    if ( check_timer( end_last_frame, NANOS_PER_FRAME ) )
	    {
            /* Getting user input. */
	        user_in = scanc_nowait(); 
 
            /* Processing user input. */
	        update( ap, user_in );

            draw( *ap );

            /* Restarting the framerate timer. */
            start_timer( &end_last_frame );
	    }
    }
}

