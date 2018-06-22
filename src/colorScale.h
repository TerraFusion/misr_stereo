#ifndef COLORSCALE_H_INCLUDED
#define COLORSCALE_H_INCLUDED

#include <map>
#include "color.h"

//! Provides an editable mapping from scalar keys to color values.
//! Once initialized, it is used like a function that accepts a number and returns a color.
//! Since this class uses a map, its behavior can be a little odd when editing
//! points. It is not possible to change the key values of points in a map, so the points must
//! be removed and added again. setDomain() and Move() both return iterators as a result
//! of this restriction.
class colorScale : public std::map<float,color>
{
public:

	//! create a color scale with a specified domain ( default is 0 to 1 )
	colorScale( float min = 0, float max = 1 );

	//! destructor
	~colorScale();

	//! Changes the color scale domain, transforming the points to maintain their
	//! relative spacing. All existing iterators to points on the color scale will become invalid.
	//! @param min the minimum value of the new domain
	//! @param max the maximum value of the new domain
	//! @param selected an optional iterator, returns an iterator to the same point after setting the domain ( the iterators will be different )
	colorScale::iterator setDomain( float min, float max, colorScale::iterator selected = NULL);

	//! Changes the minimum of the color scale domain, transforming the points to maintain their
	//! relative spacing. All existing iterators to points on the color scale will become invalid.
	void setDomainMin( float min );

	//! Changes the maximum of the color scale domain, transforming the points to maintain their
	//! relative spacing. All existing iterators to points on the color scale will become invalid.
	void setDomainMax( float max );

	//! Returns the minimum of the colorscale domain.
	float DomainMin() const;

	//! Returns the maximum of the colorscale domain.
	float DomainMax() const;

	//! Returns the size of the colorscale domain ( DomainMin() - DomainMin() ).
	float DomainSize() const;

	//! Adds a new point to the color scale.
	//! @returns an iterator for the new point.
	iterator Insert( float key, const color & value );

	//! Moves an existing point to a new position on the color scale.
	//! Any iterators for this point will become invalid, so this function is normally used as follows:
	//! @code
	//! point = colorMap.Move( point, key );
	//! @endcode
	//! @param point an iterator to the point that will be moved
	//! @param key the new value, which will be clamped to [ DomainMin(), DomainMax() ]
	//! @returns a valid iterator to the moved point
	iterator Move( iterator point, float key );

	//! Converts a value on the interval [ DomainMin(), DomainMax() ] to a color.
	//! Interpolates linearly between the closest smaller and larger points. If
	//! either point is missing the color of the closest color is used. If there are
	//! no points in the color scale this will always return white.
	color operator () ( float t ) const;

protected:

	//! Returns a value clamped to the interval [ DomainMin(), DomainMax() ]
	float clamp( float key ) const;

	float domainMin; //!< minimum of the colorscale domain
	float domainMax; //!< maximum of the colorscale domain
};

#endif // COLORSCALE_H_INCLUDED
