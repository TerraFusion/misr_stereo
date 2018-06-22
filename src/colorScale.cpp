#include "colorScale.h"
#include "utility.h"

// have it hold a min and max range and have whole color scale mapped to that range

colorScale::colorScale( float min, float max )
:
std::map<float,color>(),
domainMin( min ),
domainMax( max )
{

}



colorScale::~colorScale()
{

}



colorScale::iterator colorScale::setDomain( float min, float max, colorScale::iterator selected )
{
	if ( min != DomainMin() || max != DomainMax() )
	{
		colorScale oldScale(*this);
		domainMin = min;
		domainMax = max;
		clear();
		for ( iterator scaleIt = oldScale.begin() ; scaleIt != oldScale.end() ; scaleIt++ )
		{
			if (scaleIt == selected)
			{
				selected = Insert( remap( (*scaleIt).first, oldScale.DomainMin(), oldScale.DomainMax(), DomainMin(), DomainMax() ), (*scaleIt).second );
			}	
			else
			{
				Insert( remap( (*scaleIt).first, oldScale.DomainMin(), oldScale.DomainMax(), DomainMin(), DomainMax() ), (*scaleIt).second );
			}	
		}
	}
	return selected;
}



void colorScale::setDomainMin( float min )
{
	setDomain(min, domainMax);
}



void colorScale::setDomainMax( float max )
{
	setDomain(domainMin, max);
}



float colorScale::DomainMin() const
{
	return domainMin;
}



float colorScale::DomainMax() const
{
	return domainMax;
}



float colorScale::DomainSize() const
{
	return ( DomainMax() - DomainMin() );
}



colorScale::iterator colorScale::Insert( float key, const color & value )
{
	return insert( value_type( clamp( key ), value ) ).first;
}



colorScale::iterator colorScale::Move( iterator point, float key )
{
	key = clamp( key );
	if ( point != end() && find( key ) == end() )
	{
		color tempColor( point->second );
		erase( point );
		return insert( value_type( key, tempColor ) ).first;
	}
	else
	{
		return point;
	}
}



color colorScale::operator () ( float t ) const
{
	// return default color if no points specified
	if ( empty() ) return color();

	// get upper bound of interpolation interval
	const_iterator next = upper_bound( t );

	// check if key is after last map point
	if ( next == end() ) return rbegin()->second;

	// check if key is before first map point
	if ( next == begin() ) return begin()->second;

	// get lower bound
	const_iterator prev = next;
	prev--;

	// calculate distances and check for divide by 0 conditions
	const float a = t - prev->first;
	if ( a <= 0 ) return prev->second;

	const float b = next->first - t;
	if ( b <= 0 ) return next->second;

	// calculate interpolated color value
	return ( b * prev->second + a * next->second ) / ( a + b );
}



float colorScale::clamp( float key ) const
{
	return ( key < DomainMin() ) ? DomainMin()
		: ( key > DomainMax() ) ? DomainMax()
		: key;
}
