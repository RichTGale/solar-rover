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
    rpi_info rpiinfo;                   /* Information about the raspberry pi. */
    rover r;                            /* The rover. */
    bool is_running;                    /* Whether the app is running. */
    bool start_screen_on;               /* Whether the starting screen is on. */
    bool manual_drive_screen_on;        /* Whether the control screen is on. */
};

/**
 * This function initialises the app provided to it.
 */
void app_init( app* ap )
{
    /* Allocating memory for the app. */
    *ap = (app) malloc( sizeof( struct app_data ) );
    
    /* Setting up pi-gpio. */
    fprintf( stdout, " - Setting up pi-gpio..\n" );
    setup();

    /* Getting information about the raspberry pi that this
     * program is running on. */
    fprintf( stdout, " - Getting rpi info..\n" );
    get_rpi_info( &(*ap)->rpiinfo );

    /* Preparing the rover. */
    fprintf( stdout, " - Setting up the rover..\n" );
    rover_init( &(*ap)->r );

    /* Initialising whether the app should continue running. */
    (*ap)->is_running = true;

    /* Initialising which cli screen to draw. */
    (*ap)->start_screen_on = true;
    (*ap)->manual_drive_screen_on = false;
}

/**
 * This function terminates the app provided to it.
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
 * This function draws the app start-screen.
 */
void draw_start_screen(app a)
{
    vec2d origin;   /* The cursor position. */
    vec2d bounds;   /* The size of the terminal. */
    char* message;  /* A message. */

    /* Cursor starts in the top left corner. */
    origin.x = 1;
    origin.y = 1;

    /* Getting the bounds of the terminal. */
    bounds = termres();

    /* Setting the message. */
    message = "<Press any key or 'q' to quit>";

    /* Drawing the program title. */
    textmode(NORMAL);
    termprintfs("../../art/title.txt", &origin, WHITE, NORMAL);

    /* Printing information about the raspberry pi this program
     * is running on. */
    cursput(origin.x, origin.y);
    print_rpi_info(a->rpiinfo);

    /* Printing the message. */
    origin.x = 42/2 - (strlen(message) / 2);
    origin.y = 24;
    textmode(BOLD);
    termprint(message, origin);
    textmode(NORMAL);
}

/**
 * This function draws the manual drive screen.
 */
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
    switch ( rover_get_motor_direction( a->r ) )
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

/**
 * This function updates the app provided to it based on the user input
 * that is also provided to it.
 */
void update( app* ap, char user_in )
{
    /* Checking if the start-screen is on. */
    if ( (*ap)->start_screen_on )
    {
        /* Checking what the user input. */
        switch ( user_in )
        {
            /* Terminate the app. */
	        case 'q' :  (*ap)->start_screen_on = false;
                        (*ap)->is_running = false;
                        break;

            /* Show the manual drive screen. */
            default  :  (*ap)->start_screen_on = false;
                        (*ap)->manual_drive_screen_on = true;
                        break;
        }
    }

    /* Checking if the manual drive screen is on. */
    else if ( (*ap)->manual_drive_screen_on )
    {
        /* Checking what the user input. */
        switch ( user_in )
        {
            /* Accelerate the rover. */
            case 'w' :  rover_change_direction( &(*ap)->r, FORWARDS );
                        break;

            /* Turn the rover left. */
            case 'a' :  rover_change_direction( &(*ap)->r, LEFT );
                        break;

            /* Decellerate the rover. */
            case 's' :  rover_change_direction( &(*ap)->r, BACKWARDS );
                        break;

            /* Turn the rover right. */
            case 'd' :  rover_change_direction( &(*ap)->r, RIGHT );
                        break;

            /* Stop the rover. */
            case 'x' :  rover_change_direction( &(*ap)->r, STOP );
                        break;

            /* Rotate the rover's solar rack clockwise. */
            case 'i' :  rover_step_x_1degree( &(*ap)->r, CLOCKWISE );
                        break;

            /* Rotate the rover's solar rack anticlockwise. */
            case 'k' :  rover_step_x_1degree( &(*ap)->r, ANTICLOCK );
                        break;

            /* Rotate the rover's solar rack clockwise. */
            case 'j' :  rover_step_z_1degree( &(*ap)->r, ANTICLOCK );
                        break;

            /* Rotate the rover's solar rack anticlockwise. */
            case 'l' :  rover_step_z_1degree( &(*ap)->r, CLOCKWISE );
                        break;

            /* Turn on the start screen. */
            case 'q' :  (*ap)->start_screen_on = true;
                        (*ap)->manual_drive_screen_on = false;
                        rover_change_direction( &(*ap)->r, STOP );
                        break;
        }
    }
}

/**
 * This function draws the app to the screen.
 */
void draw( app a )
{
    /* Clearing the terminal. */
    termclear();

    /* Drawing a screen. */
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
 * This function runs the app provided to it.
 */
void app_exec( app* ap )
{
    struct timespec end_last_frame; /* Framerate timer. */
    char user_in;                   /* User input. */

    /* Starting the timer. */
    start_timer( &end_last_frame );
 
    /* Drawing the first screen. */
    draw( *ap );

    /* Checking if the app is running. */
    while ( (*ap)->is_running )
    {
        /* Checking if it's time to run a frame. */
	    if ( check_timer( end_last_frame, NANOS_PER_FRAME ) )
	    {
            /* Getting user input. */
	        user_in = scanc_nowait(); 
 
            /* Processing user input. */
	        update( ap, user_in );

            /* Drawing the app. */
            draw( *ap );

            /* Restarting the framerate timer. */
            start_timer( &end_last_frame );
	    }
    }
}

