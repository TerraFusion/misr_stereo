#include <stdio.h>
#include <stdarg.h>

#include "glutaux.h"

void glutDrawText( const vec2d & pos, const char *text, ... )
{
    if ( text != NULL )
    {
        static void *font = GLUT_BITMAP_9_BY_15;

        static char buffer[ 1024 ];
        va_list arguments;
        va_start( arguments, text );
        vsprintf( buffer, text, arguments );
        va_end( arguments );

        char *output = buffer;

        glRasterPos2dv( pos + vec2d( 0, 15 ) );

        while ( *output )
        {
            glutBitmapCharacter( font, *output++ );
        }
    }
}



void glutDrawTextCentered( const vec2d & pos, const char *text, ... )
{
    if ( text != NULL )
    {
        static void *font = GLUT_BITMAP_9_BY_15;

        static char buffer[ 1024 ];
        va_list arguments;
        va_start( arguments, text );
        double length = double( vsprintf( buffer, text, arguments ) );
        va_end( arguments );

        char *output = buffer;

        glRasterPos2dv( pos + vec2d( - length * 9 / 2, 8 ) );

        while ( *output )
        {
            glutBitmapCharacter( font, *output++ );
        }
    }
}
