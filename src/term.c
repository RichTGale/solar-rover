

#include "term.h"



struct term_data {
    coord2D origin;
    coord2D bounds;
};

void term_init( term* tp )
{
    // Allocating memory.
    *tp = ( term ) malloc( sizeof( struct term_data ) );
    
    // Initialising the origin of the terminal window.
    ( *tp )->origin.x = 0;
    ( *tp )->origin.y = 0;
    
    // Initialising the boundaries of the terminal window.
    term_res( tp );
}

void term_free( term* tp )
{
    free( *tp );
}

coord2D term_getbounds( term t )
{
    return t->bounds;
}

coord2D term_getorigin( term t )
{
    return t->origin;
}

void term_clear()
{
    system( "tput clear" );
}

void term_res( term* tp )
{
    FILE* rfp;
    FILE* cfp;
    char rbuf[ 5 ];
    char cbuf[ 5 ];
    char *end;

    system( "if [ ! -d temp/ ]; then\nmkdir temp/\nfi" );

    system( "tput lines >> temp/screen_rows.txt" );
    system( "tput cols >> temp/screen_cols.txt" );

    rfp = open_file( "temp/screen_rows.txt", "r" );
    cfp = open_file( "temp/screen_cols.txt", "r" );

    fgets( rbuf, sizeof( rbuf ), rfp );
    fgets( cbuf, sizeof( cbuf ), cfp );

    ( *tp )->bounds.x = strtol( cbuf, &end, 10 );
    ( *tp )->bounds.y = strtol( rbuf, &end, 10 );

    close_file( rfp );
    close_file( cfp );

    system( "rm temp/screen_rows.txt " );
    system( "rm temp/screen_cols.txt " );
}

void textmode( enum  textmodes m )
{
    switch ( m )
    {
        case BOLD       : system( "tput bold" ); break;
        case NORMAL     : system( "tput sgr0" ); break;
        case BLINK      : system( "tput blink" ); break;
        case REVERSE    : system( "tput smso" ); break;
        case UNDERLINE  : system( "tput smul" ); break;
    }
}

void bgcolour( enum termcolours c )
{
    char buf[15];

    sprintf( buf, "tput setab %d", c );
    system( buf );
}

void fgcolour( enum termcolours c )
{
    char buf[15];

    sprintf( buf, "tput setaf %d", c );
    system( buf );
}

void drawline( char* text, ssize_t numbytes, coord2D origin, coord2D bounds )
{
    char buf[20];
    int c;

    // Moving the cursor to the origin
    sprintf( buf, "tput cup %d %d", origin.y, origin.x );
    system( buf );
    
    for ( c = 0; c < numbytes && c < bounds.x; c++ )
    {
        if ( text[c] == '1' )
        {
            bgcolour( WHITE );
            system( "printf \" \"" );
        }
        else
        {
            system( "tput cuf1" );
        }
    }
    system( "printf \"\n\"" );
}

void drawfile( char* filepath, coord2D origin, coord2D bounds )
{
    FILE* fp = open_file( filepath, "r" );
    char* text = NULL;
    size_t len = 0;
    ssize_t numbytes;
    
    while ( ( numbytes = getline( &text, &len, fp ) ) != -1 ) {
        origin.y++;
        drawline( text, numbytes, origin, bounds );
    }

    close_file( fp );

    textmode( NORMAL );
}

void drawstr( char* str, coord2D origin, coord2D bounds )
{
    char filepath_buf[ 100 ];
    int c;

    for ( c = 0; c < strlen( str ); c++ )
    {
        sprintf( filepath_buf, "../../art/%c.txt", str[c] ); 
        drawfile(
                filepath_buf,
                origin,
                bounds 
                );
        origin.x += 8;
    }
}
