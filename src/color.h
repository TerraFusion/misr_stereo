#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#ifdef QT_TYPE_CONVERSION
# include <qcolor.h>
# include "utility.h"
#endif

//! Stores color information as RGBA floats ranging from 0 to 1.
//! Supports artimetic and conversion to and from QColor.
class color
{
public:

	float r,g,b,a;

	//! Initializes the color to opaque white.
	color();
	
	//! Creates a new color with optional opacity.
	color( float R, float G, float B, float A = 1.0f );

	//! Tests for equality.
	//! @returns true if all components are equal.
	bool operator == ( const color & other ) const;

	//! Tests for inequality.
	//! @returns true if any components are not equal.
	bool operator != ( const color & other ) const;

	//! Casts to a float*
	//! Useful for passing a color to opengl: glColor4fv( color )
	operator float * ();

	//! Casts to a const float*
	//! Useful for passing a color to opengl: glColor4fv( color )
	operator const float * () const;

	//! Multiplies each component by a constant.
	color operator *= ( float c );

	//! Multiplies each component by the matching component in another color.
	color operator * ( const color & other ) const;

	//! Multiplies each component by the matching component in another color.
	color & operator *= ( const color & other );

	//! Adds each component to the matching component in another color.
	color operator + ( const color & other ) const;

	//! Adds each component to the matching component in another color.
	color & operator += ( const color & other );

	//! Subtracts each component from the matching component in another color.
	color operator - ( const color & other ) const;

	//! Subtracts each component from the matching component in another color.
	color & operator -= ( const color & other );

#ifdef QT_TYPE_CONVERSION

	//! Returns the QColor closest to this color.
	QColor getQColor()
	{
		return QColor(int(r*255),int(g*255),int(b*255));
	}

	//! Creates a color from an existing QColor.
	color( QColor qcolor )
	{
		r = qcolor.red()   / 255.0;
		g = qcolor.green() / 255.0;
		b = qcolor.blue()  / 255.0;
		a = 1.0f;
	}
	
	//! Returns the color converted to a qRgb, but with a specified opacity.
	QRgb qRgb( int a = 255 ) const
	{
		return ::qRgba( int(r*255), int(g*255), int(b*255), a );
	}
		
	//! Returns the color converted to a qRgb.
	QRgb qRgba() const
	{
		return ::qRgba( int(r*255), int(g*255), int(b*255), int(a*255) );
	}

#endif // QT_TYPE_CONVERSION

};

//! Returns the color multiplied by a constant.
color operator * ( const color & theColor, float c );

//! Returns the color multiplied by a constant.
color operator * ( float c, const color & theColor );

//! Returns the color with each component divided by the constant.
color operator / ( const color & theColor, float c );

#endif // COLOR_H_INCLUDED
