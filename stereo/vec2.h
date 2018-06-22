#ifndef VEC2_H_INCLUDED
#define VEC2_H_INCLUDED

#include <iostream>
#include <math.h>
#include "utility.h"

template <typename T>
class vec2
{
protected:

    T e[2];

public:

    vec2()
    {

    }

    explicit vec2( T s )
    {
        *this = s;
    }

	vec2( T x, T y )
	{
		e[ 0 ] = x;
		e[ 1 ] = y;
	}

	template<typename U>
    vec2( const vec2<U> & v )
    {
        *this = v;
    }

    template<typename U>
    explicit vec2( const U v[2] )
    {
        for ( int i = 0 ; i < 2 ; ++i ) e[i] = v[i];
    }

    operator T * ()
    {
        return e;
    }

    operator const T * () const
    {
        return e;
    }

	T& operator [] ( unsigned int i )
	{
		return e[i];
	}

	const T& operator [] ( unsigned int i ) const
	{
		return e[i];
	}

    T& operator [] ( int i )
    {
        return e[i];
    }

    const T& operator [] ( int i ) const
    {
        return e[i];
    }

    bool operator == ( const vec2 & v ) const
    {
        for ( int i = 0 ; i < 2 ; ++i )
            if ( e[i] != v[i] )
                return false;
        return true;
    }

    bool operator != ( const vec2 & v ) const
    {
        return ! ( *this == v );
    }

    template<typename U>
        vec2 & operator = ( const vec2<U> & v )
    {
        for ( int i = 0 ; i < 2 ; ++i ) e[i] = T( v[i] );
        return *this;
    }

    vec2 & operator = ( T s )
    {
        for ( int i = 0 ; i < 2 ; ++i ) e[i] = s;
        return *this;
    }

    vec2 & operator *= ( T s )
    {
        for ( int i = 0 ; i < 2 ; ++i ) e[i] *= s;
        return *this;
    }

    vec2 & operator /= ( T s )
    {
        for ( int i = 0 ; i < 2 ; ++i ) e[i] /= s;
        return *this;
    }

    vec2 & operator += ( const vec2 & v )
    {
        for ( int i = 0 ; i < 2 ; ++i ) e[i] += v[i];
        return *this;
    }

    vec2 & operator -= ( const vec2 & v )
    {
        for ( int i = 0 ; i < 2 ; ++i ) e[i] -= v[i];
        return *this;
    }

    vec2 operator + ( const vec2 & v ) const
    {
        vec2 ans;
        for ( int i = 0 ; i < 2 ; ++i ) ans[i] = e[i] + v[i];
        return ans;
    }

    vec2 operator - ( const vec2 & v ) const
    {
        vec2 ans;
        for ( int i = 0 ; i < 2 ; ++i ) ans[i] = e[i] - v[i];
        return ans;
    }

    vec2 operator - () const
    {
        vec2 ans;
        for ( int i = 0 ; i < 2 ; ++i ) ans[i] = -e[i];
        return ans;
    }

    vec2 operator * ( T s ) const
    {
        vec2 ans;
        for ( int i = 0 ; i < 2 ; ++i ) ans[i] = e[i] * s;
        return ans;
    }   

    vec2 operator / ( T s ) const
    {
        vec2 ans;
        for ( int i = 0 ; i < 2 ; ++i ) ans[i] = e[i] / s;
        return ans;
    }

    vec2 operator * ( const vec2 & v ) const
    {
        vec2 ans;
        for ( int i = 0 ; i < 2 ; ++i ) ans[i] = e[i] * v[i];
        return ans; 
    }

    vec2 operator / ( const vec2 & v ) const
    {
        vec2 ans;
        for ( int i = 0 ; i < 2 ; ++i ) ans[i] = e[i] / v[i];
        return ans; 
    }

	static vec2 FromAngle( T angle )
	{
		return vec2( cos( angle ), sin( angle ) );
	}

	void setX( float x ) { e[0] = x; }
	void setY( float y ) { e[1] = y; }

	T & x() { return (*this)[0]; }
	T & y() { return (*this)[1]; }

	T x() const { return (*this)[0]; }
	T y() const { return (*this)[1]; }
};


template<typename T>
inline vec2<T> operator + ( T s, const vec2<T> & v )
{
	return vec2<T>( s ) + v;
}

template<typename T>
inline vec2<T> operator + ( const vec2<T> & v, T s )
{
	return v + vec2<T>( s );
}

template<typename T>
inline vec2<T> operator - ( T s, const vec2<T> & v )
{
	return vec2<T>( s ) - v;
}

template<typename T>
inline vec2<T> operator - ( const vec2<T> & v, T s )
{
	return v - vec2<T>( s );
}

template<typename T>
inline vec2<T> operator * ( T s, const vec2<T> & v )
{
    return v * s;
}

template<typename T>
inline std::ostream & operator << ( std::ostream & stream, const vec2<T> & v )
{
	stream << "vec2( " << v[ 0 ];
    for ( int i = 1 ; i < 2 ; ++i ) stream << ", " << v[ i ];
	stream << " )";
    return stream; 
}

template<typename T>
inline std::istream & operator >> ( std::istream & stream, vec2<T> & v )
{
    for ( int i = 0 ; i < 2 ; ++i ) stream >> v[ i ];
    return stream; 
}

template<typename T>
inline T dot( const vec2<T> & u, const vec2<T> & v )
{
    T sum = 0;
    for ( int i = 0 ; i < 2 ; i++ )
        sum += u[i] * v[i];

    return sum;
}

template<typename T>
inline T lengthSquared( const vec2<T> & v )
{
    return dot( v, v );
}

template<typename T>
inline T length( const vec2<T> & v )
{
    return sqrt( lengthSquared( v ) );
}

template<typename T>
inline T distanceSquared( const vec2<T> & u, const vec2<T> & v )
{
    return lengthSquared( u - v );
}

template<typename T>
inline T distance( const vec2<T> & u, const vec2<T> & v )
{
    return length( u - v );
}

template<typename T>
inline vec2<T> normalize( const vec2<T> & v )
{
    return v / length( v );
}

template<typename T>
inline vec2<T> truncate( const vec2<T> & v, T maxLen )
{
	T len = length( v );
	return ( len > maxLen ) ? v * ( maxLen / len ) : v;
}

template<typename T>
inline vec2<T> reflect( const vec2<T> & ray, const vec2<T> & normal )
{
    return ( ray - 2 * dot( normal, ray ) * normal );
}

template<typename T>
inline vec2<T> mix( const vec2<T> & u, const vec2<T> & v, T weight )
{
    return ( ( 1 - weight ) * u + weight * v );
}

template<typename T>
vec2<T> sin( const vec2<T> & v );

template<>
inline vec2<float> sin( const vec2<float> & v )
{
	vec2<float> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = sinf( v[i] );
	return ans;
}

template<>
inline vec2<double> sin( const vec2<double> & v )
{
	vec2<double> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = sin( v[i] );
	return ans;
}

template<typename T>
vec2<T> cos( const vec2<T> & v );

template<>
inline vec2<float> cos( const vec2<float> & v )
{
	vec2<float> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = cosf( v[i] );
	return ans;
}

template<>
inline vec2<double> cos( const vec2<double> & v )
{
	vec2<double> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = cos( v[i] );
	return ans;
}

template<typename T>
vec2<T> tan( const vec2<T> & v );

template<>
inline vec2<float> tan( const vec2<float> & v )
{
	vec2<float> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = tanf( v[i] );
	return ans;
}

template<>
inline vec2<double> tan( const vec2<double> & v )
{
	vec2<double> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = tan( v[i] );
	return ans;
}

template<typename T>
inline T min( const vec2<T> & v )
{
	return min( v[0], v[1] );
}

template<typename T>
inline T max( const vec2<T> & v )
{
	return max( v[0], v[1] );
}

template<typename T>
inline vec2<T> min( const vec2<T> & u, const vec2<T> & v )
{
	vec2<T> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = min( u[i], v[i] );
	return ans;
}

template<typename T>
inline vec2<T> max( const vec2<T> & u, const vec2<T> & v )
{
	vec2<T> ans;
	for ( int i = 0 ; i < 2 ; ++i ) ans[i] = max( u[i], v[i] );
	return ans;
}

template <typename T>
inline vec2<T> perp( const vec2<T> & v )
{
	return vec2<T>( v[1], -v[0] );
}

template <typename T>
inline T cross( const vec2<T> & u, const vec2<T> & v )
{
	return ( u[0] * v[1] - u[1] * v[0] );
}

template <typename T>
inline vec2<T> project( const vec2<T> & u, const vec2<T> & v )
{
	return ( dot( u, v ) / lengthSquared( v ) ) * v;
}

template <typename T>
inline float cos( const vec2<T> & u, const vec2<T> & v )
{
	return dot( u, v ) / ( length( u ) * length( v ) );
}

typedef vec2<short>  vec2s;
typedef vec2<long>   vec2i;
typedef vec2<float>  vec2f;
typedef vec2<double> vec2d;

#endif // VEC2_H_INCLUDED
