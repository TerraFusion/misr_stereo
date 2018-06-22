#include "orbitPathConverter.h"

orbitPathConverter::orbitPathConverter()
	:
paths( 233, 193 )
{
	for( unsigned int i = 1; i < paths.size(); i++ )
	{
		paths[ i ] = ( ( paths[ i - 1 ] - 1 + 16 ) % paths.size() ) + 1;
	}
}

int orbitPathConverter::Path( int orbit ) const
{
	return paths[ ( orbit - 1 ) % paths.size() ];
}
