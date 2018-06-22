#ifndef ORBITPATHCONVERTER_H_INCLUDED
#define ORBITPATHCONVERTER_H_INCLUDED

#include <vector>

//! Converts orbit numbers to path numbers.
class orbitPathConverter
{
public:
	//! Builds a lookup table for easy mapping from orbit to path.
	orbitPathConverter();

	//! Returns the path number of an orbit.
	int Path( int orbit ) const;

	//! orbit to path lookup table.
	std::vector<int> paths;
};

#endif // ORBITPATHCONVERTER_H_INCLUDED
