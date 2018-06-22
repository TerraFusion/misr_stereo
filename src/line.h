#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "point.h"

//! A line.
template<typename T>
class line
{
public:

	struct noIntersection {};

	line()
		:
	normal( 0 ),
	distance( 0 )
	{

	}

	line( const point<T> & a, const point<T> & b )
	:
	normal( normalize( perp( b - a ) ) ),
	distance( dot( normal, a ) )
	{

	}

	line( const point<T> & n, T d )
	:
	normal( n ),
	distance( d )
	{

	}

	T operator () ( const point<T> & p ) const
	{
		return ( dot( normal, p ) - distance ); 
	}

	point<T> normal;
	T distance;
};

typedef line<float> linef;
typedef line<double> lined;

#endif // LINE_H_INCLUDED
