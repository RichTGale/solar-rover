
#include <stdlib.h>

#include "rover.h"

int main()
{
    rover r;    /* The solar rover. */

    /* Initialise the rover. */
    fprintf(stdout, "Setting up the rover...\n");
    rover_init(&r);

    /* Run the rover. */
    fprintf(stdout, "Running the rover...\n");
    rover_exec(&r);

    /* Terminate the rover. */
    fprintf(stdout, "Terminating the rover...\n");
    rover_term(&r);

    /* Exit the program. */
    fprintf(stdout, "Exiting the program...\n");
    exit( EXIT_SUCCESS );
}
