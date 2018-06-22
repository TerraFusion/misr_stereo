#ifndef HDFSPATIALAVERAGER_H_INCLUDED
#define HDFSPATIALAVERAGER_H_INCLUDED

#include <iostream>

#include "ptr.h"
#include "hdfDataNode.h"

//! A data node that computes the spatial average of an input.
//! Computes the mean of all samples within a specified radius.
//! \todo Specify radius in meters, automatically figure out how many samples to take, and sample in a circle instead of a box.
class hdfSpatialAverager : public hdfDataNode
{
public:

	//! Constructs a spatial average node.
	//! \param input the data source to sample
	//! \param theRadius the sampling radius in pixels. 0 means only the original sample point
	//! \param theSpacing the spacing between samples in pixels
    hdfSpatialAverager( ptr<hdfDataNode> input, int theRadius, hdfScalar theSpacing )
    :
    hdfDataNode( 1 ),
    radius( theRadius ),
    spacing( theSpacing )
    {
        setInput( 0, input );
    }

	//! Destructor
    virtual ~hdfSpatialAverager()
    {

    }

	//! Returns the standard deviation of the input sampled in a box around the requested location.
	//! The total number of samples is ( 2 * radius + 1 ) * ( 2 * radius + 1 ).
    virtual hdfValue Value( const hdfCoord & location )
    {
        hdfValue mean( 0, 0 );

        for ( int y = -radius ; y <= radius ; ++y )
        {
            for ( int x = -radius ; x <= radius ; ++x )
            {
                hdfValue value = Input( 0 )->Value( location + spacing * hdfCoord( x, y ) );
                mean.x += value.y * value.x;
                mean.y += value.y;
            }
        }

        if ( mean.y > 0 )
        {
            return hdfValue( mean.x / mean.y, mean.y / square( 2 * radius + 1 ) );
        }
        else
        {
            return hdfValue( 0, 0 );
        }
    }

protected:

    const int radius; //!< sampling radius in pixels
    const hdfScalar spacing; //!< size of sample pixels
};

#endif // HDFSPATIALAVERAGER_H_INCLUDED
