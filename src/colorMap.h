#ifndef COLORMAP_H_INCLUDED
#define COLORMAP_H_INCLUDED

#include <vector>

class color
{
public:

    color()
    {
        r = g = b = a = 0;
    }

    color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255 )
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    unsigned int Word() const
    {
        return ( ( r << 0 ) | ( g << 8 ) | ( b << 16 ) | ( a << 24 ) );
    }

    color operator * ( float c )
    {
        return color( ( unsigned char )( float( r ) * c ),
            ( unsigned char )( float( g ) * c ),
            ( unsigned char )( float( b ) * c ),
            ( unsigned char )( float( a ) * c )  );
    }

    color operator + ( color other )
    {
        return color( r + other.r, g + other.g, b + other.b, a + other.a );
    }

    unsigned char r,g,b,a;
};



#define COLOR_LOOKUP_SIZE 1024

class colorMap
{

public:

    class point
    {
    public:

        point() : Color()
        {
            Value = 0;
        }

        point( float theValue, color theColor )
        {
            Value = theValue;
            Color = theColor;
        }

        float Value;
        color Color;
    };

private:

	std::vector<point> points;

    unsigned int colorLookup[ COLOR_LOOKUP_SIZE + 1 ];

public:

    colorMap() : points()
    {
        for ( int index = 0; index <= COLOR_LOOKUP_SIZE; index++ )
        {
            colorLookup[ index ] = 0;
        }
    }

    int NumEntries() const
    {
        return points.size();
    }

    colorMap & AddPoint( float Value, color Color, bool recalc = true )
    {
        unsigned int index;

        for ( index = 0; index < points.size(); index++ )
        {
            if ( points[ index ].Value >= Value )
            {
                break;
            }
        }

        if ( index < points.size() )
        {
            points.insert( points.begin() + index, point( Value, Color ) );
        }
        else
        {
            points.push_back( point( Value, Color ) );
        }

        if ( recalc )
        {
            MakeLookupTable();
        }

        return *this;
    }

    void MakeLookupTable()
    {
        for ( int index = 0; index <= COLOR_LOOKUP_SIZE; index++ )
        {
            colorLookup[ index ] = Evaluate( float( index ) / float( COLOR_LOOKUP_SIZE ) );
        }
    }

    const unsigned int *GetImage() const
    {
        return colorLookup;
    }

    static int Size()
    {
        return COLOR_LOOKUP_SIZE;
    }

    unsigned int Evaluate( float t )
    {
        unsigned int index;

        if ( points.empty() )
        {
            return color( 0, 0, 0 ).Word();
        }

        for ( index = 0; index < points.size(); index++ )
        {
            if ( points[ index ].Value >= t )
            {
                while ( ( index < ( points.size() - 1 ) ) && ( points[ index + 1 ].Value == t ) )
                {
                    index++;
                }

                break;
            }
        }

        if ( index == 0 ) return points[ 0 ].Color.Word();

        if ( index == points.size() ) return points[ index - 1 ].Color.Word();

        return Blend( points[ index - 1 ].Color,
            points[ index ].Color,
            ( t - points[ index - 1 ].Value )
            / ( points[ index ].Value - points[ index - 1 ].Value ) );
    }

    unsigned int operator () ( float t )
    {
        int index = int( t * float( COLOR_LOOKUP_SIZE ) + 0.5 );

        if ( index < 0 ) index = 0;
        if ( index > COLOR_LOOKUP_SIZE ) index = COLOR_LOOKUP_SIZE;

        return colorLookup[ index ];
    }

    unsigned int Blend( color a, color b, float t )
    {
        return ( a * ( 1 - t ) + b * t ).Word();
    }

    point & Point( int index )
    {
        return points[ index ];
    }

    void Remove( int index )
    {
        points.erase( points.begin() + index );
    }

    int NumPoints()
    {
        return points.size();
    }

    void Clear()
    {
        points.clear();
    }
};

extern colorMap defaultColorMap;
extern colorMap blueColorMap;
extern colorMap eightColorMap;
extern colorMap fourteenColorMap;

extern colorMap monoGradientMap;
extern colorMap redGradientMap;
extern colorMap greenGradientMap;
extern colorMap blueGradientMap;

#endif
