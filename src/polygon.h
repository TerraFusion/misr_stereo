#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <list>
#include <vector>

#ifdef QT_TYPE_CONVERSION
# include <qpolygon.h>
#endif // QT_TYPE_CONVERSION

//! Stores the vertices of a polygon. Provides functions to test the winding
//! of a polygon, which is an easy way to tell if any of the polygon edges
//! have been flipped.
template <typename _vertexType>
	class polygon : public std::vector<_vertexType>
{

public:

	typedef _vertexType vertexType;
	typedef typename std::vector<_vertexType>::iterator iterator;
	typedef typename std::vector<_vertexType>::const_iterator const_iterator;

    using std::vector<_vertexType>::size;
    using std::vector<_vertexType>::begin;
    using std::vector<_vertexType>::end;
    
	//! Constructs a polygon with a number of sides.
	//! The vertices must be set later.
	polygon( int numPoints = 0 )
	:
	std::vector<vertexType>( numPoints )
	{

	}

	//! Constructs a polygon from iterators to points.
	template<typename inputIterator>
	polygon( inputIterator first, inputIterator last )
	:
	std::vector<vertexType>( first, last )
	{

	}

	//! Returns the location of a vertex.
	//! \param index the vertex number ( 0 to NumVertices() - 1 ).
	vertexType & Vertex( int index )
	{
		return (*this)[ index ];
	}

	//! Returns the location of a vertex.
	//! \param index the vertex number ( 0 to NumVertices() - 1 ).
	const vertexType & Vertex( int index ) const
	{
		return (*this)[ index ];
	}   

	//! Returns the number of vertices.
	int NumVertices() const
	{
		return int( size() );
	}

	//! The 2D cross product of two vectors.
	static float cross( vertexType v1, vertexType v2 )
	{
		return ( v1.x * v2.y - v1.y * v2.x );
	}

	//! Returns true if all of the edges have a counterclockwise winding.
	//! This test works correctly only if the polygon is convex.
	bool IsCCW() const
	{
		for( int i = 2 ; i < NumVertices() ; ++i )
		{
			vertexType e1 = (*this)[ i-1 ] - (*this)[ i-2 ];
			vertexType e2 = (*this)[ i ] - (*this)[ i-1 ];
			if( cross( e1, e2 ) < 0 ) return false;
		}

		return true;
	}

	//! Returns true if all of the edges have a clockwise winding.
	//! This test works correctly only if the polygon is convex.
	bool IsCW() const
	{
		return ! IsCCW();
	}

#ifdef QT_TYPE_CONVERSION

	//! Converts this polygon to a QPolygon.
	QPolygon qPolygon() const
	{
		QPolygon output;
		output.reserve( size() );
		for( unsigned int i = 0; i < size(); i++ )
		{
			output.push_back( (*this)[ i ].qPoint() );
		}
		return output;
	}

	//! Converts this polygon to a QPolygonF.
	QPolygonF qPolygonF() const
	{
		QPolygonF output;
		output.reserve( size() );
		for( unsigned int i = 0; i < size(); i++ )
		{
			output.push_back( (*this)[ i ].qPointF() );
		}
		return output;
	}

#endif // QT_TYPE_CONVERSION

};


#ifdef QT_TYPE_CONVERSION

//! Converts a list of polygon to a list of QPolygon.
template <typename vertexType>
std::vector<QPolygon> qPolygonList( const std::list< polygon<vertexType> > & inputList )
{
	std::vector<QPolygon> outputList( inputList.size() );
	std::transform( inputList.begin(), inputList.end(), outputList.begin(),
		std::mem_fun_ref( & polygon<vertexType>::qPolygon ) );
	if( !inputList.empty() ) outputList.push_back( inputList.front().qPolygon() );
	return outputList;
}

//! Converts a list of polygon to a list of QPolygonF.
template <typename vertexType>
std::vector<QPolygonF> qPolygonFList( const std::list< polygon<vertexType> > & inputList )
{
	std::vector<QPolygonF> outputList( inputList.size() );
	std::transform( inputList.begin(), inputList.end(), outputList.begin(),
		std::mem_fun_ref( & polygon<vertexType>::qPolygonF ) );
	if( !inputList.empty() ) outputList.push_back( inputList.front().qPolygonF() );
	return outputList;
}

#endif // QT_TYPE_CONVERSION

#endif // POLYGON_H_INCLUDED
