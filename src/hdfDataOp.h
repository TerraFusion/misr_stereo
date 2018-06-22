#ifndef HDFDATAOP_H_INCLUDED
#define HDFDATAOP_H_INCLUDED

#include <math.h>
#include <sstream>

#include "hdfDataNode.h"
#include "utility.h"
#include "ptr.h"

//! A data node that performs operations that requires two arguments.
//! The template argument is a function object that specifies the operation.
//! This function object should have a function with the following signature:
//! \code hdfValue operator () ( const hdfValue& a, const hdfValue& b ) const \endcode
template <typename functor>
    class hdfOpBinary : public hdfDataNode
{

public:

	//! Constructs a binary operation.
	//! The inputs will be set to the default input.
	//! \param theOperation An instance of the functor that does the operation. Usually the default is fine, but some operations may have arguments.
    hdfOpBinary( functor theOperation = functor() )
    :
    hdfDataNode( 2 ),
    operation( theOperation )
    {

    }

	//! Constructs a binary operation.
	//! \param input0 the input for the operation's first argument
	//! \param input1 the input for the operation's second argument
	//! \param theOperation An instance of the functor that does the operation. Usually the default is fine, but some operations may have arguments.
    hdfOpBinary( ptr<hdfDataNode> input0, ptr<hdfDataNode> input1, functor theOperation = functor() )
    :
    hdfDataNode( 2 ),
    operation( theOperation )
    {
        setInput( 0, input0 );
        setInput( 1, input1 );
    }

	//! Destructor
    virtual ~hdfOpBinary()
    {

    }

	//! Returns the name of the operation.
	virtual std::string Name() const
	{
		return operation.Name();
	}

	//! Returns the result of the operation on the inputs at a location.
    virtual hdfValue Value( const hdfCoord & location )
    {
        return operation( Input( 0 )->Value( location ), Input( 1 )->Value( location ) );
    }

	//! Returns the result of the operation on the inputs at multiple locations.
	virtual std::vector<hdfValue> Values( const std::vector<hdfCoord> & location )
	{
		std::vector<hdfValue> input0 = Input( 0 )->Values( location );
		std::vector<hdfValue> input1 = Input( 1 )->Values( location );
		std::vector<hdfValue> output( location.size() );
		std::transform( input0.begin(), input0.end(), input1.begin(), output.begin(), operation );
		return output;
	}

protected:

    functor operation; //!< an instance of a binary operation function object
};

//! Function object for use with hdfOpBinary, computes a + b.
//! \see hdfOpBinary
class hdfOpBinaryAdd
{
public:

	//! "opBinaryAdd"
	std::string Name() const
	{
		return "opBinaryAdd";
	}

	//! Returns a + b.
	//! The returned coverage is the smaller of the two input coverages.
    hdfValue operator () ( const hdfValue& a, const hdfValue& b ) const
    {
        return hdfValue( a.x + b.x, min( a.y, b.y ) );
    }
};

//! Function object for use with hdfOpBinary, computes a - b.
//! \see hdfOpBinary
class hdfOpBinarySub
{
public:

	//! "opBinarySub"
	std::string Name() const
	{
		return "opBinarySub";
	}

	//! Returns a - b.
	//! The returned coverage is the smaller of the two input coverages.
    hdfValue operator () ( const hdfValue& a, const hdfValue& b ) const
    {
        return hdfValue( a.x - b.x, min( a.y, b.y ) );
    }
};

//! Function object for use with hdfOpBinary, computes a * b.
//! \see hdfOpBinary
class hdfOpBinaryMul
{
public:

	//! "opBinaryMul"
	std::string Name() const
	{
		return "opBinaryMul";
	}

	//! Returns a * b.
	//! The returned coverage is the smaller of the two input coverages.
    hdfValue operator () ( const hdfValue& a, const hdfValue& b ) const
    {
        return hdfValue( a.x * b.x, min( a.y, b.y ) );
    }
};

//! Function object for use with hdfOpBinary, computes a / b.
//! \see hdfOpBinary
class hdfOpBinaryDiv
{
public:

	//! "opBinaryDiv"
	std::string Name() const
	{
		return "opBinaryDiv";
	}

	//! Returns a / b.
	//! The returned coverage is the smaller of the two input coverages.
    hdfValue operator () ( const hdfValue& a, const hdfValue& b ) const
    {
        return hdfValue( a.x / b.x, min( a.y, b.y ) );
    }
};

//! Function object for use with hdfOpBinary, computes if ( a == c ) b else 0.
//! c is a constant supplied to the constructor.
//! \see hdfOpBinary
class hdfOpBinaryConditional
{
public:

	//! Constructs a new conditional operation.
	//! \param c value of input a for which input b will be returned
    hdfOpBinaryConditional( const hdfScalar& c )
	:
	passValue( c )
	{
	
	}

	//! "opBinaryConditional( c = # )"
	std::string Name() const
	{
		std::ostringstream out;
		out << "opBinaryConditional( c = " << passValue << " )";
		return out.str();
	}

	//! if a is equal to passValue, returns b with the smaller of the two input coverages.
	//! otherwise, returns 0 with a coverage of 0.
    hdfValue operator () ( const hdfValue& a, const hdfValue& b ) const
    {
        return ( ( a.y > 0 ) && ( a.x == passValue ) ) ? b : hdfValue( 0, 0 );
    }

    hdfScalar passValue; //!< value of input a for which input b will be returned
};

//! A data node that performs operations that require one argument.
//! The template argument is a function object that specifies the operation.
//! This function object should have a function with the following signature:
//! \code hdfValue operator () ( const hdfValue& input ) const \endcode
template <typename functor>
class hdfOpUnary : public hdfDataNode
{
public:

	//! Constructs a binary operation.
	//! The input will be set to the default input.
	//! \param theOperation An instance of the functor that does the operation. Usually the default is fine, but some operations may have arguments.
    hdfOpUnary( functor theOperation = functor() )
    :
    hdfDataNode( 1 ),
    operation( theOperation )
    {

    }

	//! Constructs a binary operation.
	//! \param input0 the input for the operation's argument
	//! \param theOperation An instance of the functor that does the operation. Usually the default is fine, but some operations may have arguments.
    hdfOpUnary( ptr<hdfDataNode> input0, functor theOperation = functor() )
    :
    hdfDataNode( 1 ),
    operation( theOperation )
    {
        setInput( 0, input0 );
    }

	//! Destructor
    virtual ~hdfOpUnary()
    {

    }

	//! Returns the name of the operation.
	virtual std::string Name() const
	{
		return operation.Name();
	}

	//! Returns the result of the operation on the input at a location.
    virtual hdfValue Value( const hdfCoord & location )
    {
        return operation( Input( 0 )->Value( location ) );
    }

	//! Returns the result of the operation on the input at multiple locations.
	virtual std::vector<hdfValue> Values( const std::vector<hdfCoord> & location )
	{
		std::vector<hdfValue> input = Input( 0 )->Values( location );
		std::vector<hdfValue> output( location.size() );
		std::transform( input.begin(), input.end(), output.begin(), operation );
		return output;
	}

protected:

    functor operation; //!< an instance of a unary operation function object
};

//! Function object for use with hdfOpUnary, returns input.
//! \see hdfOpUnary
class hdfOpUnaryNone
{
public:

	//! "opUnaryNone"
	std::string Name() const
	{
		return "opUnaryNone";
	}

	//! returns input.
	//! The coverage is the same as the input coverage.
    hdfValue operator () ( const hdfValue& input ) const
    {
        return input;
    }
};

//! Function object for use with hdfOpUnary, computes cos( input ).
//! Assumes that the input is in degrees, not radians.
//! \see hdfOpUnary
class hdfOpUnaryCosDeg
{
public:

	//! "opUnaryCosDeg"
	std::string Name() const
	{
		return "opUnaryCosDeg";
	}

	//! Returns cos( input ).
	//! The coverage is the same as the input coverage.
    hdfValue operator () ( const hdfValue& input ) const
    {
        return hdfValue( cos( ( pi / 180 ) * input.x ), input.y );
    }
};

//! Function object for use with hdfOpUnary, computes 1 / cos( input ).
//! Assumes that the input is in degrees, not radians.
//! \see hdfOpUnary
class hdfOpUnarySecDeg
{
public:

	//! "opUnarySecDeg"
	std::string Name() const
	{
		return "opUnarySecDeg";
	}

	//! Returns cos( input ).
	//! The coverage is the same as the input coverage.
    hdfValue operator () ( const hdfValue& input ) const
    {
        double result = 1.0 / cos( ( pi / 180 ) * input.x );

        return ( result > 0.01 ) ? hdfValue( result, input.y ) : hdfValue( 0, 0 );
    }
};

//! Function object for use with hdfOpUnary, computes c * input.
//! c is a constant supplied to the constructor.
//! \see hdfOpUnary
class hdfOpUnaryMul
{
public:

	//! Constructs a new unary multiplication operation.
	//! \param c number to multiply by input
    hdfOpUnaryMul( const hdfScalar& c = 1.0 )
	:
	multiplier( c )
	{
	
	}

	//! "opUnaryMul( # * input )"
	std::string Name() const
	{
		std::ostringstream out;
		out << "opUnaryMul( " << multiplier << " * input )";
		return out.str();
	}

	//! Returns c * input.
	//! The coverage is the same as the input coverage.
    hdfValue operator () ( const hdfValue& input ) const
    {
        return hdfValue( multiplier * input.x, input.y );
    }

    const hdfScalar multiplier; //!< constant
};

//! Function object for use with hdfOpUnary, computes c / input.
//! c is a constant supplied to the constructor.
//! \see hdfOpUnary
class hdfOpUnaryDiv
{
public:

	//! Constructs a new unary division operation.
	//! \param c number to divide by input
	hdfOpUnaryDiv( const hdfScalar & c = 1.0 )
	:
	multiplier( c )
	{
	
	}

	//! "opUnaryDiv( # / input )"
	std::string Name() const
	{
		std::ostringstream out;
		out << "opUnaryDiv( " << multiplier << " / input )";
		return out.str();
	}

	//! Returns c / input.
	//! The coverage is the same as the input coverage.
    hdfValue operator () ( const hdfValue& input ) const
    {
        return hdfValue( multiplier / input.x, input.y );
    }

    const hdfScalar multiplier; //!< constant
};

//! Function object for use with hdfOpUnary, returns input if input is in a specified interval.
//! Filters input value so it is ingored if outside the interval.
//! \see hdfOpUnary
class hdfOpUnaryBandPass
{
public:

	//! Constructs a new band pass operation.
	//! \param theMin lower bound of the allowed interval
	//! \param theMax upper bound of the allowed interval
	hdfOpUnaryBandPass( const hdfScalar& theMin, const hdfScalar& theMax )
    : minVal( theMin ), maxVal( theMax ) {}

	//! "opUnaryBandPass( min = #, max = # )"
	std::string Name() const
	{
		std::ostringstream out;
		out << "opUnaryBandPass( min = " << minVal << ", max = " << maxVal << " )";
		return out.str();
	}

	//! if a \> minVal and a \< maxVal, returns a.
	//! otherwise, returns 0 with a coverage of 0.
    hdfValue operator () ( const hdfValue& input ) const
    {
        return ( ( input.x >= minVal ) && ( input.x <= maxVal ) ) ? input : hdfValue( 0, 0 );
    }

    hdfScalar minVal; //!< lower bound of the allowed interval
	hdfScalar maxVal; //!< upper bound of the allowed interval
};

#endif // HDFDATAOP_H_INCLUDED
