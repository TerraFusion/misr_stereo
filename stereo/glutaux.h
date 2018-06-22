#ifndef GLUTAUX_H_INCLUDED
#define GLUTAUX_H_INCLUDED

#include "vec2.h"

#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
# ifdef WIN32
#  pragma comment( lib, "opengl32" )
#  pragma comment( lib, "glu32" )
#  ifdef min
#   undef min
#  endif
#  ifdef max
#   undef max
#  endif
# endif
#endif

void glutDrawText( const vec2d & pos, const char *text, ... );

void glutDrawTextCentered( const vec2d & pos, const char *text, ... );

#endif // GLUTAUX_H_INCLUDED
