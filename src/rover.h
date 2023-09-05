/**
 * rover.h
 *
 * This file contains the public data-structure and function prototype
 * declarations for the rover type.
 *
 * Version: 0.1.0
 * Author(s): Richard Gale
 */

#ifndef rover_h
#define rover_h

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "interface.h"
#include "drive.h"
#include "rack.h"
#include "mycutils.h"

#define FRAMES_PER_SEC 2
#define NANOS_PER_FRAME NANOS_PER_SEC / FRAMES_PER_SEC

/**
 * This is the rover data-type.
 */
typedef struct rover_data* rover;

/**
 * This function initialises the rover supplied to it.
 */
void rover_init(rover* rp);

/**
 * This function terminates the rover supplied to it.
 */
void rover_term(rover* rp);

/**
 * This function runs the rover supplied to it.
 */
void rover_exec(rover* rp);

#endif
