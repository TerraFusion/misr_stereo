#include "nodeMaker.h"
#include "hdfSpatialAverager.h"

QString nodeMaker::defaultOrbitSQL = "SELECT orbit FROM files WHERE 1";

nodeMaker::listType & nodeMaker::Builtins()
{
	static listType localList;
	return localList;
}

nodeMaker* nodeMaker::Register( nodeMaker* item )
{
	Builtins().push_back( item );
	return item;
}

nodeMaker::nodeMaker( QString theName )
:
name( theName )
{

}

nodeMaker::~nodeMaker()
{
	
}

QString nodeMaker::Name() const
{
	return name;
}

ptr<hdfDataNode> nodeMaker::CreateAveraged( QStringList values, int orbit, int radius ) const
{
	ptr<hdfDataNode> result = Create( values, orbit );
	if ( radius > 0 )
	{
		// average the output of the network
		result = new hdfSpatialAverager( result, radius, 17600 );
		// move the projector above the spatial averager
		hdfDataNode::PromoteProjectors( result );
	}
	return result;
}
