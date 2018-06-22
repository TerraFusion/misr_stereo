#include "SOMParameters.h"
#include <string>
#include "stringaux.h"
#include <iostream>
#include "qutility.h"
#include "orbitPathConverter.h"

SOMParameters SOMParams;

std::vector<double> SOMParameters::FromPath( int path )
{
	std::vector<double> result;

	if( DB.IsOpen() || DB.Open( getAppPath( "resource/app.db" ).toStdString() ) )
	{
		std::string sql = "SELECT * FROM SOM_Parameters WHERE PATH = " + ToString( path );
		query row = DB.Query( sql );
		if( row.Step() )
		{
			result = row.ValuesDouble( 1, 13 ); 
		}
	}
	else
	{
		std::cerr << "Failed to open database file\n";
	}

	return result;
}

std::vector<double> SOMParameters::FromOrbit( int orbit )
{
	static orbitPathConverter convert;
	return FromPath( convert.Path( orbit ) );
}
