/**
 * rover_v1.c
 * 
 * Uses the pi-gpio library ( https://github.com/Milliways2/pi-gpio )
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#include "subproc.h"
#include "mycutils.h"

#define FRAMES_PER_SEC 6
#define NANOS_PER_FRAME NANOS_PER_SEC/FRAMES_PER_SEC

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

int main()
{
    FILE* fstreamp;
    struct timespec end_last_frame; // Framerate timer
    subproc sp;
    char ch;

    // Creating a new file in case there was a command in the old one.
    fstreamp=open_file( "./current_command.txt", "w" );
    close_file( fstreamp );
            
    subproc_init( &sp );
    subproc_exec( &sp, "./rover_sp" );
    
    start_timer( &end_last_frame );
    
    while ( ch!='q' )
    {
        if ( check_timer( end_last_frame, NANOS_PER_FRAME  ) )
        {
            // Waiting for user input
            ch=getch();

            // Printing user input
            fprintf( stdout, "Input: %c\n", ch );

            // Writing the input to a file
            fstreamp=open_file( "./current_command.txt", "w" );
            write_ch( fstreamp, ch );
            close_file( fstreamp );

            start_timer( &end_last_frame );
        }
    }
    
    subproc_term( &sp );
    subproc_free( &sp );

    return 0;
}
