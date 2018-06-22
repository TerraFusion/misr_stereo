#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

#include <list>
#include <vector>
#include <ostream>

#include "utility.h"
#include "point.h"
#include "polygon.h"

#ifdef QT_TYPE_CONVERSION
#include <qrect.h>
#endif

//! A rectangle.
template <typename dataType>
	class rect
{

public:

	typedef dataType elementType;
	typedef point<dataType> pointType;
	typedef polygon<point<dataType> > polygonType;

	typedef double localType;
	typedef point<localType> localPointType;    
	typedef polygon<localPointType> localPolygonType;

	rect()
	:
	minPoint( 0, 0 ),
	maxPoint( 1, 1 )
	{

	}

	rect( const rect & other )
	:
	minPoint( other.minPoint ),
	maxPoint( other.maxPoint )
	{

	}

	rect( const dataType & width, const dataType & height )
	:
	minPoint( 0, 0 ),
	maxPoint( width, height )
	{

	}

	rect( const point<dataType> & size )
	:
	minPoint( 0, 0 ),
	maxPoint( size )
	{

	}

	rect( const dataType & left, const dataType & top, const dataType & right, const dataType & bottom )
	:
	minPoint( left, top ),
	maxPoint( right, bottom )
	{

	}

	rect( const point<dataType> & topLeft, const point<dataType> & bottomRight )
	:
	minPoint( topLeft ),
	maxPoint( bottomRight )
	{

	}

	static rect CreateCentered( const dataType & centerX, const dataType & centerY, const dataType & width, const dataType & height )
	{
		return CreateCentered( point<dataType>( centerX, centerY ), point<dataType>( width, height ) );
	}

	static rect CreateCentered( const point<dataType> & center, const point<dataType> & size )
	{
		point<dataType> origin = center - size / 2;
		return rect( origin, origin + size );
	}

	template<typename newType>
		operator rect<newType> () const
	{
		return rect<newType>( point<newType>( minPoint ), point<newType>( maxPoint ) );
	}

	bool operator == ( const rect<dataType>& other ) const
	{
		return minPoint == other.minPoint && maxPoint == other.maxPoint;	
	}

	bool operator != ( const rect<dataType>& other ) const
	{
		return ! ( *this == other );
	}

	const dataType & Top() const
	{
		return minPoint.y;
	}

	const dataType & Bottom() const
	{
		return maxPoint.y;
	}

	const dataType & Left() const
	{
		return minPoint.x;
	}

	const dataType & Right() const
	{
		return maxPoint.x;
	}

	const dataType & MinX() const
	{
		return min( Left(), Right() );
	}

	const dataType & MaxX() const
	{
		return max( Left(), Right() );
	}

	const dataType & MinY() const
	{
		return min( Top(), Bottom() );
	}

	const dataType & MaxY() const
	{
		return max( Top(), Bottom() );
	}

	dataType Width() const
	{
		return ( Right() - Left() );
	}

	dataType WidthAbs() const
	{
		return ( Width() > 0 ) ? ( Width() ) : ( -Width() );
	}

	dataType Height() const
	{
		return ( Bottom() - Top() );
	}

	dataType HeightAbs() const
	{
		return ( Height() > 0 ) ? ( Height() ) : ( -Height() );
	}

	pointType Size() const
	{
		return ( maxPoint - minPoint );
	}

	pointType SizeAbs() const
	{
		return pointType( WidthAbs(), HeightAbs() );
	}

	pointType Center() const
	{
		return ( minPoint + maxPoint ) / 2;
	}

	pointType TopLeft() const
	{
		return pointType( minPoint.x, minPoint.y );
	}

	pointType TopRight() const
	{
		return pointType( maxPoint.x, minPoint.y );
	}

	pointType BottomLeft() const
	{
		return pointType( minPoint.x, maxPoint.y );
	}

	pointType BottomRight() const
	{
		return pointType( maxPoint.x, maxPoint.y );
	}

	void setTop( const dataType & theTop )
	{
		minPoint.y = theTop;
	}

	void setBottom( const dataType & theBottom )
	{
		maxPoint.y = theBottom;
	}

	void setLeft( const dataType & theLeft )
	{
		minPoint.x = theLeft;
	}

	void setRight( const dataType & theRight )
	{
		maxPoint.x = theRight;
	}

	void setWidth( const dataType & theWidth )
	{
		setSize( theWidth, Height() );
	}

	void setHeight( const dataType & theHeight )
	{
		setSize( Width(), theHeight );
	}

	void setSize( const dataType & theWidth, const dataType & theHeight )
	{
		setSize( pointType( theWidth, theHeight ) );
	}

	void setSize( const pointType & theSize )
	{
		maxPoint = minPoint + theSize;
	}

	void setSizeCentered( const dataType & theWidth, const dataType & theHeight )
	{
		setSizeCentered( pointType( theWidth, theHeight ) );
	}

	void setSizeCentered( const pointType & theSize )
	{
		pointType center = Center();

		minPoint = center - ( theSize / 2 );
		maxPoint = minPoint + theSize;
	}

	void setTopLeft( const pointType & theTopLeft )
	{
		minPoint.x = theTopLeft.x;
		minPoint.y = theTopLeft.y;
	}

	void setTopRight( const pointType & theTopRight )
	{
		maxPoint.x = theTopRight.x;
		minPoint.y = theTopRight.y;
	}

	void setBottomLeft( const pointType & theBottomLeft )
	{
		minPoint.x = theBottomLeft.x;
		maxPoint.y = theBottomLeft.y;
	}

	void setBottomRight( const pointType & theBottomRight )
	{
		maxPoint.x = theBottomRight.x;
		maxPoint.y = theBottomRight.y;      
	}

	void moveCenterX( const dataType & dest )
	{
		move( dest - Center().x, 0 );
	}

	void moveCenterY( const dataType & dest )
	{
		move( 0, dest - Center().y );
	}

	void moveTop( const dataType & dest )
	{
		move( 0, dest - Top() );
	}

	void moveBottom( const dataType & dest )
	{
		move( 0, dest - Bottom() );
	}

	void moveLeft( const dataType & dest )
	{
		move( dest - Left(), 0 );
	}

	void moveRight( const dataType & dest )
	{
		move( dest - Right(), 0 );
	}

	void moveMinX( const dataType & dest )
	{
		move( dest - MinX(), 0 );
	}

	void moveMaxX( const dataType & dest )
	{
		move( dest - MaxX(), 0 );
	}

	void moveMinY( const dataType & dest )
	{
		move( 0, dest - MinY() );
	}

	void moveMaxY( const dataType & dest )
	{
		move( 0, dest - MaxY() );
	}

	void move( const pointType & theDelta )
	{
		minPoint += theDelta;
		maxPoint += theDelta;
	}

	void move( const dataType & destX, const dataType & destY )
	{
		move( pointType( destX, destY ) );
	}

	void moveCenter( const pointType & destination )
	{
		move( destination - Center() );
	}

	void moveCenter( const dataType & destX, const dataType & destY )
	{
		moveCenter( pointType( destX, destY ) );
	}

	void moveTopLeft( const pointType & destination )
	{
		move( destination - TopLeft() );      
	}

	void moveTopLeft( const dataType & destX, const dataType & destY )
	{
		moveTopLeft( pointType( destX, destY ) );      
	}

	void moveBottomLeft( const pointType & destination )
	{
		move( destination - BottomLeft() );      
	}

	void moveBottomLeft( const dataType & destX, const dataType & destY )
	{
		moveBottomLeft( pointType( destX, destY ) );      
	}

	void moveTopRight( const pointType & destination )
	{
		move( destination - TopRight() );             
	}

	void moveTopRight( const dataType & destX, const dataType & destY )
	{
		moveTopRight( pointType( destX, destY ) );      
	}

	void moveBottomRight( const pointType & destination )
	{
		move( destination - BottomRight() );             
	}

	void moveBottomRight( const dataType & destX, const dataType & destY )
	{
		moveBottomRight( pointType( destX, destY ) );      
	}

	bool Contains( const pointType & thePoint ) const
	{
		localPointType local = ConvertLocal( thePoint );

		return ( ( local.x >= 0 ) && ( local.x <= 1 ) && ( local.y >= 0 ) && ( local.y <= 1 ) );
	}

	void Include( const rect<dataType> & theRect )
	{
		if ( theRect.Left()  < Left()  ) setLeft(  theRect.Left()  );
		if ( theRect.Right() > Right() ) setRight( theRect.Right() );

		if ( theRect.Top()    < Top()    ) setTop(    theRect.Top()    );
		if ( theRect.Bottom() > Bottom() ) setBottom( theRect.Bottom() );
	}

	rect<dataType> SwapX() const
	{
		return rect<dataType>( TopRight(), BottomLeft() );
	}

	rect<dataType> SwapY() const
	{
		return rect<dataType>( BottomLeft(), TopRight() );
	}

	rect<dataType> Swap() const
	{
		return rect<dataType>( TopLeft().Swap(), BottomRight().Swap() );
	}

	rect<dataType> Normalized() const
	{
		rect<dataType> normalized = *this;
		if( Width() < 0 ) normalized = normalized.SwapX();
		if( Height() < 0 ) normalized = normalized.SwapY();
		return normalized;
	}

	localPointType ConvertLocal( const pointType & thePoint ) const
	{
		return ( localPointType( thePoint - TopLeft() ) / localPointType( Size() ) );
	}

	pointType ConvertGlobal( const localPointType & thePoint ) const
	{
		return ( thePoint * localPointType( Size() ) + localPointType( TopLeft() ) );
	}

	rect<localType> ConvertLocal( const rect<dataType> & theRect ) const
	{
		return rect<localType>( ConvertLocal( theRect.TopLeft() ), ConvertLocal( theRect.BottomRight() ) );
	}

	rect<dataType> ConvertGlobal( const rect<localType> & theRect ) const
	{
		return rect<dataType>( ConvertGlobal( theRect.TopLeft() ), ConvertGlobal( theRect.BottomRight() ) );
	}

	polygon<localPointType> ConvertLocal( const polygon<pointType> & thePolygon ) const
	{
		typename polygon<pointType>::const_iterator input = thePolygon.begin();
		polygon<localPointType> output;

		while ( input != thePolygon.end() )
		{
			output.push_back( ConvertLocal( *input++ ) );
		}

		return output;
	}

	polygon<pointType> ConvertGlobal( const polygon<localPointType> & thePolygon ) const
	{
		polygon<localPointType>::const_iterator input( thePolygon.begin() );        
		polygon<pointType> output;

		while ( input != thePolygon.end() )
		{
			output.push_back( ConvertGlobal( *input++ ) );
		}

		return output;
	}

	std::list<localPointType> ConvertLocal( const std::list<pointType> & pointList ) const
	{
		typename std::list<pointType>::const_iterator input( pointList.begin() );
		std::list<localPointType> output;

		while ( input != pointList.end() )
		{
			output.push_back( ConvertLocal( *input++ ) );
		}

		return output;
	}

	std::list<pointType> ConvertGlobal( const std::list<localPointType> & pointList ) const
	{
		std::list<localPointType>::const_iterator input( pointList.begin() );
		std::list<pointType> output;

		while ( input != pointList.end() )
		{
			output.push_back( ConvertGlobal( *input++ ) );
		}

		return output;
	}

	std::vector<localPointType> ConvertLocal( const std::vector<pointType> & pointList ) const
	{
		typename std::vector<pointType>::const_iterator input( pointList.begin() );
		std::vector<localPointType> output;

		while ( input != pointList.end() )
		{
			output.push_back( ConvertLocal( *input++ ) );
		}

		return output;
	}

	std::vector<pointType> ConvertGlobal( const std::vector<localPointType> & pointList ) const
	{
		std::vector<localPointType>::const_iterator input( pointList.begin() );
		std::vector<pointType> output;

		while ( input != pointList.end() )
		{
			output.push_back( ConvertGlobal( *input++ ) );
		}

		return output;
	}

	std::list<std::vector<localPointType> > ConvertLocal( const std::list<std::vector<pointType> > & pointList ) const
	{
		typename std::list<std::vector<pointType> >::const_iterator input( pointList.begin() );
		std::list<std::vector<localPointType> > output;

		while ( input != pointList.end() )
		{
			output.push_back( ConvertLocal( *input++ ) );
		}

		return output;
	}

	std::list<std::vector<pointType> > ConvertGlobal( const std::list<std::vector<localPointType> > & pointList ) const
	{
		std::list<std::vector<localPointType> >::const_iterator input( pointList.begin() );
		std::list<std::vector<pointType> > output;

		while ( input != pointList.end() )
		{
			output.push_back( ConvertGlobal( *input++ ) );
		}

		return output;
	}

	std::list<localPolygonType> ConvertLocal( const std::list<polygonType> & polygonList ) const
	{
		typename std::list<polygonType>::const_iterator input( polygonList.begin() );
		std::list<localPolygonType> output;

		while ( input != polygonList.end() )
		{
			output.push_back( ConvertLocal( *input++ ) );
		}

		return output;
	}

	std::list<polygonType> ConvertGlobal( const std::list<localPolygonType> & polygonList ) const
	{
		std::list<localPolygonType>::const_iterator input( polygonList.begin() );
		std::list<polygonType> output;

		while ( input != polygonList.end() )
		{
			output.push_back( ConvertGlobal( *input++ ) );
		}

		return output;
	}

	std::list< rect<dataType> > DivideRect( const dataType & w, const dataType & h )
	{
		std::vector<dataType> xCoord;
		generate_sequence( Left(), w, Right(), back_inserter( xCoord ) );

		std::vector<dataType> yCoord;
		generate_sequence( Top(), h, Bottom(), back_inserter( yCoord ) );

		std::list< rect<dataType> > subRegions;

		for ( unsigned int y = 1 ; y < yCoord.size() ; ++y )
		{
			for ( unsigned int x = 1 ; x < xCoord.size() ; ++x )
			{
				subRegions.push_back( rect<dataType>(
							xCoord[ x - 1 ], yCoord[ y - 1 ],
						xCoord[ x ], yCoord[ y ] ) );
			}
		}

		return subRegions;
	}

	rect<dataType> ContainRect(rect<dataType> contained) const;

	rect<dataType> FitInside( const rect<dataType> & theRect ) const
	{
		localType scale = min( localType( HeightAbs() ) / theRect.HeightAbs(), localType( WidthAbs() ) / theRect.WidthAbs() );
		return CreateCentered( Center(), scale * theRect.Size() );
	}

	rect<dataType> FitOutside( const rect<dataType> & theRect ) const
	{
		localType scale = max( localType( HeightAbs() ) / theRect.HeightAbs(), localType( WidthAbs() ) / theRect.WidthAbs() );
		return CreateCentered( Center(), scale * theRect.Size() );
	}

#ifdef QT_TYPE_CONVERSION

	rect( const QRect & oldRect )
		:
	minPoint( dataType( oldRect.left()  ), dataType( oldRect.top()    ) ),
	maxPoint( dataType( oldRect.right() ), dataType( oldRect.bottom() ) )
	{

	}

	QRect qRect() const
	{
		return QRect( int( Left() ), int( Top() ), int( Width() ), int( Height() ) );
	}

	QRectF qRectF() const
	{
		return QRectF( Left(), Top(), Width(), Height() );
	}
	
#endif

protected:

	pointType minPoint;
	pointType maxPoint;
};



template <typename dataType>
	polygon< point<dataType> > CreatePolygon( const rect<dataType> & theRect )
{
	polygon< point<dataType> > result( 4 );

	result[ 0 ] = theRect.TopLeft();
	result[ 1 ] = theRect.TopRight();
	result[ 2 ] = theRect.BottomRight();
	result[ 3 ] = theRect.BottomLeft();

	return result;
}



template <typename scalarType, typename dataType>
	inline rect<dataType> operator * ( const scalarType & c, const rect<dataType> & r )
{
	return rect<dataType>( c * r.TopLeft(), c * r.BottomRight() );
}


template <typename dataType>
	inline rect<dataType> operator + ( const point<dataType> & p, const rect<dataType> & r )
{
	return rect<dataType>( p + r.TopLeft(), p + r.BottomRight() );
}

template <typename dataType, typename scalarType>
	inline rect<dataType> operator * ( const rect<dataType> & r, const scalarType & c )
{
	return rect<dataType>( r.TopLeft() * c, r.BottomRight() * c );
}

template <typename dataType>
	inline rect<dataType> operator + ( const rect<dataType> & r, const point<dataType> & p )
{
	return rect<dataType>( r.TopLeft() + p, r.BottomRight() + p );
}

template <typename dataType>
	inline rect<dataType> operator - ( const rect<dataType> & r, const point<dataType> & p )
{
	return rect<dataType>( r.TopLeft() - p, r.BottomRight() - p );
}

// takes in rect and keeps it inside of this rect. if rect fits in this rect 
// containRect will move it inside of this rect if it is not already.  If the 
//incoming rect is too big containRect centers the dimensions that are too large.
template <typename dataType>
	rect<dataType> rect<dataType>::ContainRect( rect<dataType> theRect ) const
{   
	if ( theRect.WidthAbs() < WidthAbs() )
	{
		if ( theRect.MaxX() > MaxX() ) theRect.moveMaxX( MaxX() );
		if ( theRect.MinX() < MinX() ) theRect.moveMinX( MinX() );
	}
	else
	{
		theRect.moveCenterX( Center().x );
	}

	if ( theRect.HeightAbs() < HeightAbs() )
	{
		if ( theRect.MinY() < MinY() ) theRect.moveMinY( MinY() );
		if ( theRect.MaxY() > MaxY() ) theRect.moveMaxY( MaxY() );
	}
	else
	{
		theRect.moveCenterY( Center().y );
	}

	return theRect;
}

template<typename T>
inline std::ostream & operator << ( std::ostream & out, const rect<T>& r )
{
	return out << "rect( " << r.TopLeft() << ", " << r.BottomRight() << " )";
}

#endif // RECT_H_INCLUDED
