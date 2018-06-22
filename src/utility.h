#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <algorithm>
#include <functional>
#include <string>
#include <sstream>

#ifdef min
# undef min
#endif // min
#ifdef max
# undef max
#endif // max

//! Returns x * x for an argument x.
//! Useful for squaring complicated expressions without using a temporary variable.
template< typename T > T square( const T & x )
{
    return x * x;
}

using std::min;
using std::max;

//! Returns the smallest of its arguments.
template< typename T >
const T & min( const T & a, const T & b, const T & c )
{
    return min( min( a, b ), c );
}

//! Returns the smallest of its arguments.
template< typename T >
const T & min( const T & a, const T & b, const T & c, const T & d )
{
    return min( min( a, b ), min( c, d ) );
}

//! Returns the largest of its arguments.
template< typename T >
const T & max( const T & a, const T & b, const T & c )
{
    return max( max( a, b ), c );
}

//! Returns the largest of its arguments.
template< typename T >
const T & max( const T & a, const T & b, const T & c, const T & d )
{
    return max( max( a, b ), max( c, d ) );
}

//! Returns true if a value is within an interval
//! \param value the value to test
//! \param minLimit the lower bound of the interval
//! \param maxLimit the upper bound of the interval
template < typename T >
bool inRange( const T & value, const T & minLimit, const T & maxLimit )
{
    return ( ( minLimit <= value ) && ( value <= maxLimit ) );
}

//! Clamps a value so it is within an interval
//! \returns the original value if inside of the interval, otherwise the closest point that is in the interval
//! \param value the value to clamp
//! \param minLimit the lower bound of the interval
//! \param maxLimit the upper bound of the interval
template < typename T >
T clamp( const T & value, const T & minLimit, const T & maxLimit )
{
    return min( max( value, minLimit ), maxLimit );
}

//! Maps a value from one interval onto another.
//! This is conceptually the same as transforming from the domain to the unit interval [0,1] to the range.
//! \param value the value to map
//! \param domainMin lower bound of the domain
//! \param domainMax upper bound of the domain
//! \param rangeMin lower bound of the range
//! \param rangeMax upper bound of the range
template < typename inputType, typename outputType >
outputType remap( const inputType & value, const inputType & domainMin, const inputType & domainMax, const outputType & rangeMin, const outputType & rangeMax )
{
    return clamp( ( value - domainMin ) * ( rangeMax - rangeMin ) / ( domainMax - domainMin ) + rangeMin, rangeMin, rangeMax );
}

//! Stores an interval, allows min \> max.
template < typename T >
class interval
{
public:

	//! Constructs a new interval [ 0, 1 ]
    interval()
    : min( 0 ), max( 1 ) {}

	//! Constructs a new interval [ theMin, theMax ]
	//! \param theMin lower bound
	//! \param theMax upper bound
    template < typename other >
        interval( const other & theMin, const other & theMax )
    : min( theMin ), max( theMax ) {}

	//! Returns the size of the interval
	//! The size is the difference between the upper bound and the lower bound.
    T size() const { return ( max - min ); }

	//! Returns a new interval with the upper and lower bounds swapped.
    interval reversed() const { return interval( max, min ); }

    T min; //!< the lower bound
	T max; //!< the upper bound
};

//! Create intervals without specifying a template argument
template < typename T >
interval<T> createInterval( const T & theMin, const T & theMax )
{
    return interval<T>( theMin, theMax );
}

//! Stores an interval, enforces min \< max.
template < typename T >
class orderedInterval : public interval<T>
{
public:

    using interval<T>::min; //!< use lower bound of interval
    using interval<T>::max; //!< use upper bound of interval

 	//! Constructs a new ordered interval [ 0, 1 ]
	orderedInterval()
    : interval<T>( 0, 1 ) {}

	//! Constructs a new ordered interval from an interval.
    orderedInterval( const interval<T> & input )
    : interval<T>( std::min( input.min, input.max ), std::max( input.min, input.max ) ) {}

	//! Constructs a new ordered interval from an interval.
	//! The upper and lower bound may be swapped to enforce lower \< upper
	//! \param theMin lower bound
	//! \param theMax upper bound
    orderedInterval( const T & theMin, const T & theMax )
    : interval<T>( std::min( theMin, theMax ), std::max( theMin, theMax ) ) {}

	//! Returns true if value is on the interval [ min, max ]
    bool contains( const T & value ) const
	{
		return ( ( value >= min ) && ( value <= max ) );
	}
};

//! Create ordered intervals without specifying a template argument
template < typename T >
    orderedInterval<T> createOrderedInterval( const T & theMin, const T & theMax )
{
    return orderedInterval<T>( theMin, theMax );
}

//! A function object that tests if a value is within an interval.
//! Similar to inRange(), but with the interval specified in advance.
template < typename T >
class intervalTester
{
public:

	//! Default constructor
    intervalTester()
    : domain() {}

	//! Constructs an interval tester from an ordered interval.
    intervalTester( const orderedInterval<T> & theDomain )
    : domain( theDomain ) {}

	//! Constructs an interval tester.
	//! \param theMin lower bound
	//! \param theMax upper bound
    intervalTester( const T & theMin, const T & theMax )
    : domain( theMin, theMax ) {}

	//! Returns true if value is in the interval [ domain.min, domain.max ]
    bool operator () ( const T & value ) const
    {
        return domain.contains( value );
    }

	//! Stores the interval used for testing.
    orderedInterval<T> domain;
};

//! A function object that maps values from one interval to another.
//! Similar to remap(), but with the intervals specified in advance.
template < typename inType, typename outType >
class mapper
{
public:

	//! Default constructor
    mapper()
    : domain(), range() {}

	//! Constructs a mapper from one interval to another.
	//! \param theDomain input interval
	//! \param theRange output interval
    mapper( const interval<inType> & theDomain, const interval<outType> & theRange )
    : domain( theDomain ), range( theRange ) {}

	//! Constructs a mapper from one interval to another.
	//! \param domainMin input interval lower bound
	//! \param domainMax input interval upper bound
	//! \param rangeMin output interval lower bound
	//! \param rangeMax output interval upper bound
    mapper( const inType & domainMin, const inType & domainMax, const outType & rangeMin, const outType & rangeMax )
    : domain( domainMin, domainMax ), range( rangeMin, rangeMax ) {}

	//! Returns value mapped from [ domain.min, domain.max ] to [ range.min, range.max ].
    outType operator () ( const inType & value ) const
    {
        return outType( ( value - domain.min ) * range.size() / domain.size() + range.min );
    }

	//! Returns a mapper with the domain and range swapped
	//! Note that this mapper may have a different type if the domain and range were different types.
    mapper<outType,inType> inverse() const
    {
        return mapper<outType,inType>( range, domain );
    }

    interval<inType> domain; //!< input interval
    interval<outType> range; //!< output interval
};

//! A function object that clamps values to an interval.
//! Similar to clamp(), but with the interval specified in advance.
template < typename T >
    class clamper
{
public:

	//! Default constructor
    clamper()
    : limits() {}

	//! Constructs a clamper from an interval.
    clamper( const interval<T> & theLimits )
    : limits( theLimits ) {}

	//! Constructs a clamper.
	//! \param theMinLimit lower bound
	//! \param theMaxLimit upper bound
    clamper( const T & theMinLimit, const T & theMaxLimit )
    : limits( theMinLimit, theMaxLimit ) {}

	//! Returns the value clamped to [ limits.min, limits.max ].
    T operator () ( const T & value ) const
    {
        return clamp( value, limits.min, limits.max );
    }

	//! Stores the interval used for clamping.
    orderedInterval<T> limits;
};

//! Computes a weighted average of two values.
//! \param a the value if w = 0
//! \param b the value if w = 1
//! \param w weight on [0,1] that controls the relative weight of a and b
template <typename T, typename S>
T mix( T a, T b, S w )
{
	return ( 1 - w ) * a + w * b;
}

//! Writes a sequence of numbers to an iterator.
//! \param begin the first element in the sequence.
//! \param increment the difference between elements in the sequence.
//! \param end the last element in the sequence.
//! \param output an iterator specifying where the output will be written.
template <typename dataType, typename InsertIterator>
void generate_sequence( dataType begin, dataType increment, dataType end, InsertIterator output )
{
    for ( dataType x = begin ; x < end ; x += increment, ++output )
    {
        *output = x;
    }

    *output = end;
}

//! Calls a function on each element of a container.
template <typename inputContainer,typename unaryFunction>
unaryFunction for_each_in( inputContainer container, unaryFunction f )
{
	return std::for_each( container.begin(), container.end(), f );
}

//! Stores a generic set of three values. Uses std::pair to sort triples by
//! their three values. Triples will be sorted by first, then by second if
//! first is the same, then by third if second is the same. This should not
//! be used if you will need to access the individual members later, use a
//! struct or class instead.
template < typename firstType, typename secondType, typename thirdType >
class triple : public std::pair< firstType, std::pair< secondType, thirdType > >
{
public:
    triple( const firstType & theFirst, const secondType & theSecond, const thirdType & theThird )
    :
    std::pair< firstType, std::pair< secondType, thirdType > > ( theFirst,
        std::pair< secondType, thirdType > ( theSecond, theThird ) )
    {}
};

//! A function object that extracts and returns the member named first from a structure.
template < typename T >
    struct firstFromPair : public std::unary_function<const T &,typename T::first_type>
{
    typename T::first_type operator () ( const T & p ) const { return p.first; }
};

//! A function object that extracts and returns the member named second from a structure.
template < typename T >
    struct secondFromPair : public std::unary_function<const T &,typename T::second_type>
{
    typename T::second_type operator () ( const T & p ) const { return p.second; }
};

//! Returns a random number on [0,1]
inline float frand()
{
    return float( rand() ) / float( RAND_MAX );
}

//! Returns a random number on [min,max]
inline float frand( float min, float max )
{
    return frand() * ( max - min ) + min;
}

//! A function object that generates random numbers on an interval
class functionRandom
{
public:

	//! Constructs a random number generator with a specified interval.
	//! The default interval is [0,1]
    functionRandom( float min = 0, float max = 1 )
    :
    outSize( max - min ),
    outOffset( min )
    {

    }

	//! Returns a random number.
    float operator () () const
    {
        return frand() * outSize + outOffset;
    }

protected:

    const float outSize; //!< the size of the random interval.
    const float outOffset; //!< the offset of the random interval
};

//! A function object used to delete pointers allocated with new.
//! The template argument is the type of pointer to delete.
//! In the following example, any objects stored in values would be deleted:
//! \code
//! std::vector<object*> values;
//! std::foreach( values.begin(), values.end(), del_fun<object>() )
//! \endcode
//! It is recommended to follow this with:
//! \code values.clear(); \endcode
//! or:
//! \code std::fill( values.begin(), values.end(), NULL ); \endcode
//! to avoid using pointers that have been deleted.
template<typename T>
struct del_fun
{
	//! Calls delete on its argument.
    void operator () ( T* pointer ) const
    {
        delete pointer;
    }
};

#endif // UTILITY_H_INCLUDED
