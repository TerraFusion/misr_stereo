#ifndef SOMPARAMETERS_H_INCLUDED
#define SOMPARAMETERS_H_INCLUDED

#include "database.h"
#include <vector>

//! Retrieves SOM Projection parameters without opening a hdf file.
class SOMParameters
{
public:
	//! Returns SOM parameters for a path.
	//! Retrieves parameters from sqlite database.
	std::vector<double> FromPath( int path );
	//! Returns SOM parameters for an orbit.
	std::vector<double> FromOrbit( int orbit );
	//! Retrieves parameters from sqlite database.
protected:
	database DB; //!< database containing SOM parameters.
};

//! global instance of SOMParams.
extern SOMParameters SOMParams;

#endif // SOMPARAMETERS_H_INCLUDED
