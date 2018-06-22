#include "color.h"



color::color()
{
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;
}

color::color( float R, float G, float B, float A )
{
	r = R;
	g = G;
	b = B;
	a = A;
}

bool color::operator == ( const color & other ) const
{
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool color::operator != ( const color & other ) const
{
	return !((*this)==other);
}

color::operator float * ()
{
	return ( float * ) this;
}

color::operator const float * () const
{
	return ( const float * ) this;
}

color color::operator *= ( float c )
{
	r *= c;
	g *= c;
	b *= c;
	a *= c;

	return *this;
}

color color::operator * ( const color & other ) const
{
	return color( r * other.r, g * other.g, b * other.b, a * other.a );
}

color & color::operator *= ( const color & other )
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	a *= other.a;

	return *this;
}

color color::operator + ( const color & other ) const
{
	return color( r + other.r, g + other.g, b + other.b, a + other.a );
}

color & color::operator += ( const color & other )
{
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;

	return *this;
}

color color::operator - ( const color & other ) const
{
	return color( r - other.r, g - other.g, b - other.b, a - other.a );
}

color & color::operator -= ( const color & other )
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	a -= other.a;

	return *this;
}

color operator * ( const color & theColor, float c )
{
	return color( theColor.r * c, theColor.g * c, theColor.b * c, theColor.a * c );
}

color operator * ( float c, const color & theColor )
{
	return color( theColor.r * c, theColor.g * c, theColor.b * c, theColor.a * c );
}

color operator / ( const color & theColor, float c )
{
	return color( theColor.r / c, theColor.g / c, theColor.b / c, theColor.a / c );
}
