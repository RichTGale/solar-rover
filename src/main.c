
#include <stdlib.h>

#include "rover.h"

int main()
{
    rover r;    /* The solar rover. */

    /* Initialise the rover. */
    rover_init(&r);

    /* Run the rover. */
    rover_exec(&r);

    /* Terminate the rover. */
    rover_term(&r);

    /* Exit the program. */
    exit( EXIT_SUCCESS );
}
