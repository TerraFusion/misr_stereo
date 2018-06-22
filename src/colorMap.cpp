
#include "colorMap.h"

colorMap defaultColorMap = colorMap().
    AddPoint( 0.00, color(   255,   170,   255 ), false ).
    AddPoint( 0.25, color(   170,   170,   255 ), false ).
    AddPoint( 0.50, color(   0,     255,   255 ), false ).
    AddPoint( 0.75, color(   85,    255,     0 ), false ).
    AddPoint( 1.00, color(   255,   255,   127 ), true  );


colorMap blueColorMap = colorMap().
    AddPoint( 0.0, color(   0,   0,   0 ), false ).
    AddPoint( 1.0, color( 255, 255, 255 ), true  );


colorMap eightColorMap = colorMap().
    AddPoint( 0.0f / 9.0f, color(    0,     0,     0 ), false ). //Black
    AddPoint( 1.0f / 9.0f, color(  185,    55,   183 ), false ). //Purple
    AddPoint( 2.0f / 9.0f, color(    8,     0,   167 ), false ). //Dk Blue
    AddPoint( 3.0f / 9.0f, color(  119,   146,   255 ), false ). //Lt Blue
    AddPoint( 4.0f / 9.0f, color(    1,   255,   230 ), false ). //Cyan
    AddPoint( 5.0f / 9.0f, color(   50,   255,    35 ), false ). //Green
    AddPoint( 6.0f / 9.0f, color(  255,   247,    11 ), false ). //Yellow
    AddPoint( 7.0f / 9.0f, color(    0,     0,     0 ), false ). //Black
    AddPoint( 8.0f / 9.0f, color(  255,   150,     3 ), false ). //Orange
    AddPoint( 9.0f / 9.0f, color(  255,     0,     0 ), true  ); //Red


colorMap fourteenColorMap = colorMap().
    AddPoint(  0.0f / 13.0f, color(    0,    0,    0 ), false ).
    AddPoint( 12.0f / 13.0f, color(  200,    0,    0 ), false ).
    AddPoint( 11.0f / 13.0f, color(  255,    0,    0 ), false ).
    AddPoint( 10.0f / 13.0f, color(  255,  100,    0 ), false ).
    AddPoint(  9.0f / 13.0f, color(  255,  200,    0 ), false ).
    AddPoint(  8.0f / 13.0f, color(  255,  255,    0 ), false ).
    AddPoint(  7.0f / 13.0f, color(   50,  255,    0 ), false ).
    AddPoint(  6.0f / 13.0f, color(    0,  200,   50 ), false ).
    AddPoint(  5.0f / 13.0f, color(    0,  255,  200 ), false ).
    AddPoint(  4.0f / 13.0f, color(    0,  255,  255 ), false ).
    AddPoint(  3.0f / 13.0f, color(    0,  200,  255 ), false ).
    AddPoint(  2.0f / 13.0f, color(    0,  100,  255 ), false ).
    AddPoint(  1.0f / 13.0f, color(    0,    0,  200 ), false ).
    AddPoint( 13.0f / 13.0f, color(   85,   55,   83 ), true  );

colorMap monoGradientMap = colorMap().
    AddPoint( 0.0f, color(   0,   0,   0 ), false ).
    AddPoint( 1.0f, color( 255, 255, 255 ), true  );

colorMap redGradientMap = colorMap().
    AddPoint( 0.0f, color(   0,   0,   0 ), false ).
    AddPoint( 1.0f, color( 255,   0,   0 ), true  );

colorMap greenGradientMap = colorMap().
    AddPoint( 0.0f, color(   0,   0,   0 ), false ).
    AddPoint( 1.0f, color(   0, 255,   0 ), true  );

colorMap blueGradientMap = colorMap().
    AddPoint( 0.0f, color(   0,   0,   0 ), false ).
    AddPoint( 1.0f, color(   0,   0, 255 ), true  );
