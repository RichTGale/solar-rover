

#ifndef TERM_H
#define TERM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mycutils.h"


enum termcolours { 
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    };

enum textmodes { 
    BOLD,
    NORMAL,
    BLINK,
    REVERSE,
    UNDERLINE
    };

typedef struct {
    int x;
    int y;
} coord2D;

typedef struct term_data* term;

void term_init( term* tp );

void term_free( term* tp );

coord2D term_getbounds( term t );

coord2D term_getorigin( term t );

void term_res( term* tp );

void textmode( enum textmodes m );

void bgcolour( enum termcolours c );

void fgcolour( enum termcolours c );

void drawfile( char* filepath, coord2D origin, coord2D bounds );

#endif // TERMUTIL_H
