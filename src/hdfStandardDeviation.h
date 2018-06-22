#ifndef HDFSTANDARDDEVIATION_H_INCLUDED
#define HDFSTANDARDDEVIATION_H_INCLUDED

#include "ptr.h"
#include "hdfSpatialAverager.h"

//! A data node that computes the standard deviation of an input.
//! Computes the standard deviation of all samples within a specified radius.
//! \todo Specify radius in meters, automatically figure out how many samples to take, and sample in a circle instead of a box.
class hdfStandardDeviation : public hdfSpatialAverager
{
public:

	//! Constructs a standard deviation node.
	//! \param input the data source to sample
	//! \param theRadius the sampling radius in pixels. 0 means only the original sample point
	//! \param theSpacing the spacing between samples in pixels
	hdfStandardDeviation( ptr<hdfDataNode> input, int theRadius, hdfScalar theSpacing )
    :
    hdfSpatialAverager( input, theRadius, theSpacing )
    {

    }

	//! Destructor
    virtual ~hdfStandardDeviation()
    {

    }

	//! Returns the standard deviation of the input sampled in a box around the requested location.
	//! The total number of samples is ( 2 * radius + 1 ) * ( 2 * radius + 1 ).
    virtual hdfValue Value( const hdfCoord & location )
    {
        hdfValue mean = hdfSpatialAverager::Value( location );
        hdfValue sum( 0, 0 );
        for ( int y = -radius ; y <= radius ; ++y )
        {
            for ( int x = - radius ; x <= radius ; ++x )
            {
                hdfValue value = Input( 0 )->Value( location + spacing * hdfCoord( x, y ) );
                sum.x += value.y * square( value.x - mean.x );
                sum.y += value.y;
            }
        }

        if ( sum.y <= 1 ) return hdfValue( 0, mean.y );
        return hdfValue( sqrt( sum.x / ( sum.y - 1 ) ), mean.y );
    }
};

#endif // HDFSTANDARDDEVIATION_H_INCLUDED
