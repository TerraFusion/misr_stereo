#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <functional>
#include <math.h>
#include <ostream>

#ifdef QT_TYPE_CONVERSION
# include <qpoint.h>
#endif // QT_TYPE_CONVERSION

//! A point.
template <typename T>
	class point
{
public:

	typedef T elementType;

	static point FromAngle( const T & angle )
	{
		return( point( cos( angle ), sin( angle ) ) );
	}

	point()
		:
	x( 0 ),
	y( 0 )
	{

	}

	point( const T & xValue, const T & yValue )
	:
	x( xValue ),
	y( yValue )
	{

	}

	template<typename otherT>
		operator point<otherT> () const
	{
		return point<otherT>( otherT( x ), otherT( y ) );
	}

#ifdef QT_TYPE_CONVERSION

	point( const QPoint& other )
	:
	x( other.x() ),
	y( other.y() )
	{
			
	}

	point( const QPointF& other )
	:
	x( other.x() ),
	y( other.y() )
	{
			
	}

	QPoint qPoint() const
	{
		return QPoint( int( x ), int( y ) );
	}
	
	QPointF qPointF() const
	{
		return QPointF( x, y );
	}
	
#endif // QT_TYPE_CONVERSION

	T * Ptr() const
	{
		return( ( T * ) this );
	}

	const T * ConstPtr() const
	{
		return( ( const T * ) this );
	}

	bool operator == ( const point & other ) const
	{
		return ( ( x == other.x ) && ( y == other.y ) );
	}

	bool operator != ( const point & other ) const
	{
		return ( ( x != other.x ) || ( y != other.y ) );
	}

	const point & operator += ( const point & other )
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	const point & operator -= ( const point & other )
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	const point & operator *= ( const T & c )
	{
		x *= c;
		y *= c;
		return *this;
	}

	const point & operator *= ( const point & other )
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	const point & operator /= ( const point & other )
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	point operator + ( const point & other ) const
	{
		return point( x + other.x, y + other.y );
	}

	point operator - ( const point & other ) const
	{
		return point( x - other.x, y - other.y );
	}

	point operator * ( const point & other ) const
	{
		return point( x * other.x, y * other.y );
	}

	point operator / ( const point & other ) const
	{
		return point( x / other.x, y / other.y );
	}

	T aTan() const
	{
		return atan2( y, x );
	}

	T MagSquared() const
	{
		return ( x * x + y * y );
	}

	T Mag() const
	{
		return sqrt( MagSquared() );
	}

	point Swap() const
	{
		return point( y, x );
	}

	class functionGetX : public std::unary_function< const point &, T >
	{
	public:
		T operator () ( const point & thePoint ) const
		{
			return thePoint.x;
		}
	};

	class functionGetY : public std::unary_function< const point &, T >
	{
	public:
		T operator () ( const point & thePoint ) const
		{
			return thePoint.y;
		}
	};

	class functionLessX : public std::binary_function< const point &, const point &, bool >
	{
	public:
		bool operator () ( const point & left, const point & right ) const
		{
			return ( left.x < right.x );
		}
	};

	class functionLessY : public std::binary_function< const point &, const point &, bool >
	{
	public:
		bool operator () ( const point & left, const point & right ) const
		{
			return ( left.y < right.y );
		}
	};

	class functionCreate : public std::binary_function< T, T, point >
	{
	public:
		point operator () ( T x, T y ) const
		{
			return point( x, y );
		}
	};

	T x;
	T y;
};



template <typename T, typename S>
	inline point<T> operator + ( const point<T> & p, const S & c )
{
	return point<T>( T( p.x + c ), T( p.y + c ) );
}

template <typename T, typename S>
	inline point<T> operator - ( const point<T> & p, const S & c )
{
	return point<T>( T( p.x - c ), T( p.y - c ) );
}

template <typename T, typename S>
	inline point<T> operator * ( const point<T> & p, const S & c )
{
	return point<T>( T( p.x * c ), T( p.y * c ) );
}

template <typename T, typename S>
	inline point<T> operator / ( const point<T> & p, const S & c )
{
	return point<T>( T( p.x / c ), T( p.y / c ) );
}



template <typename S, typename T>
	inline point<T> operator + ( const S & c, const point<T> & p )
{
	return point<T>( T( c + p.x ), T( c + p.y ) );
}

template <typename S, typename T>
	inline point<T> operator - ( const S & c, const point<T> & p )
{
	return point<T>( T( c - p.x ), T( c - p.y ) );
}

template <typename S, typename T>
	inline point<T> operator * ( const S & c, const point<T> & p )
{
	return point<T>( T( c * p.x ), T( c * p.y ) );
}

template <typename S, typename T>
	inline point<T> operator / ( const S & c, const point<T> & p )
{
	return point<T>( T( c / p.x ), T( c / p.y ) );
}



template <typename T>
	inline point<T> perp( const point<T> & v )
{
	return point<T>( v.y, -v.x );
}

template <typename T>
	inline T cross( const point<T> & u, const point<T> & v )
{
	return ( u.x * v.y - u.y * v.x );
}

template <typename T>
	inline T dot( const point<T> & u, const point<T> & v )
{
	return ( u.x * v.x + u.y * v.y );
}

template <typename T>
	inline T lengthSquared( const point<T> & p )
{
	return dot( p, p );
}

template <typename T>
	inline T length( const point<T> & p )
{
	return sqrt( lengthSquared( p ) );
}

template <typename T>
	inline point<T> normalize( const point<T> & v )
{
	return v / length( v );
}


template <typename T>
	inline bool HasNAN( const point<T> & p )
{
	return p.x != p.x || p.y != p.y;
}



template <typename T>
inline std::ostream & operator << ( std::ostream & out, const point<T> & p )
{
	return out << "point( " << p.x << ", " << p.y << " )";
}

#endif // POINT_H_INCLUDED
