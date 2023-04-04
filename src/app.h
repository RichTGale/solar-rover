/**
 * app.h
 *
 * This file contains the publicly available data-structures and 
 * function for an app. 
 * An app is a type that contains everything that makes up a complete 
 * Command Line Interface (CLI) application.
 *
 * Version: 4th April, 2023.
 * Author(s): Richard Gale
 */

#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pi-gpio.h>

#include "mycutils.h"
#include "rpiutils.h"
#include "term.h"
#include "rover.h"

#define FRAMES_PER_SEC 2
#define NANOS_PER_FRAME NANOS_PER_SEC/FRAMES_PER_SEC

/**
 * The app data-type.
 */
typedef struct app_data* app;

/**
 * Prepares the provided app for use.
 */
void app_init( app* ap );

/**
 * Runs the provided app.
 */
void app_exec( app* ap );

/**
 * Cleans up the provided app.
 */
void app_term( app* ap );

#endif // APP_H
