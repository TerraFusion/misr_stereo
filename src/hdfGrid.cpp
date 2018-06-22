
#include <qstringlist.h>

#include "hdfFile.h"
#include "hdfGrid.h"
#include "hdfField.h"



// node functions //////////////////////////////////////////////////////////////



hdfGridNode::hdfGridNode( hdfFileNode * theParentFile )
	:
parentFile( theParentFile ),
handle( FAIL ),
name(),
xDim( 0 ),
yDim( 0 ),
blockDim( 0 ),
projectionParameters( 15, 0 ),
scale( 1.0 ),
solarIrradiance( 0.0 ),
solarDistance( 0.0 ),
fieldList()
{

}



hdfGridNode::~hdfGridNode()
{
	Close();
}



bool hdfGridNode::CheckDependencies() const
{
	std::vector< ptr< hdfFieldNode > >::const_iterator currentField = fieldList.begin();

	while ( currentField != fieldList.end() )
	{
		if ( (*currentField).Count() > 1 )
		{
			return true;
		}

		++currentField;
	}

	return false;
}



ptr<hdfFileNode> hdfGridNode::File() const
{
	return parentFile;
}



int32 hdfGridNode::Handle() const
{
	return handle;
}



int32 hdfGridNode::ValidHandle()
{
	Open();

	return Handle();
}



QString hdfGridNode::Name() const
{
	return name;
}



int hdfGridNode::XDim() const
{
	return xDim;
}



int hdfGridNode::YDim() const
{
	return yDim;
}



int hdfGridNode::BlockDim() const
{
	return blockDim;
}



std::vector<float32> hdfGridNode::BlockOffset() const
{
	return blockOffset;
}



float32 hdfGridNode::BlockOffset( int block ) const
{
	return ( block >= 1 && block <= 180 ) ? blockOffset[ block - 1 ] : 0;
}



std::vector<float32> hdfGridNode::BlockOffset( int begin, int end ) const
{
	return std::vector<float32>( blockOffset.begin() + ( begin - 1 ), blockOffset.begin() + ( end - 1 ) );
}



const std::vector<double>& hdfGridNode::ProjectionParameters() const
{
	return projectionParameters;
}



double hdfGridNode::Scale() const
{
	return scale;
}



double hdfGridNode::SolarIrradiance() const
{
	return solarIrradiance;
}



double hdfGridNode::SolarDistance() const
{
	return solarDistance;
}



int hdfGridNode::NumFields() const
{
	return int( fieldList.size() );
}



ptr<hdfFieldNode> hdfGridNode::Field( int fieldIndex ) const
{
	return ( ( fieldIndex >= 0 ) && ( fieldIndex < NumFields() ) ) ? ( fieldList[ fieldIndex ] ) : ( 0 );
}



int hdfGridNode::FieldIndex( QString fieldName ) const
{
	int fieldIndex;

	for ( fieldIndex = 0 ; fieldIndex < NumFields() ; fieldIndex++ )
	{
		if ( fieldName == Field( fieldIndex )->Name() )
		{
			break;
		}
	}

	if ( fieldIndex >= NumFields() )
	{
		fieldIndex = -1;
	}

	return fieldIndex;
}



ptr<hdfFieldNode> hdfGridNode::Field( QString fieldName ) const
{
	return Field( FieldIndex( fieldName ) );
}



bool hdfGridNode::Open()
{
	if ( Handle() == FAIL )
	{
		handle = GDattach( File()->ValidHandle(), Name().toAscii().data() );

		if ( Handle() == FAIL )
		{
			printf( "failed to open grid: %s\n", Name().toAscii().data() );
			return false;
		}
	}

	return true;
}



bool hdfGridNode::Close()
{
	if ( Handle() != FAIL )
	{
		int32 status = GDdetach( Handle() );

		handle = FAIL;

		if ( status == FAIL )
		{
			printf( "failed to close grid: %s\n", Name().toAscii().data() );
			return false;
		}
	}

	return true;
}



bool hdfGridNode::Load( QString gridName )
{
	name = gridName;
	int32 status;

    // attach to the grid
	Open();



    // read the projection parameters
	{
		int32 projectionCode;

		status = GDprojinfo( Handle(), &projectionCode, NULL, NULL, &projectionParameters[0] );

		if ( status == FAIL )
		{
			printf( "GDprojinfo() failed\n" );
		}
	}



	int32 numFields;
	int32 fieldNamesLength;

    // read the number of fields 
	{
		numFields = GDnentries( Handle(), HDFE_NENTDFLD, & fieldNamesLength );

		if ( numFields == FAIL )
		{
			printf( "failed to read number of fields\n" );
			return false;
		}
	}



	QStringList fieldNameList;

    // read the name from each field
	{
		char *fieldNamesRaw = new char [ fieldNamesLength + 1 ];

		numFields = GDinqfields( Handle(), fieldNamesRaw, NULL, NULL );

		fieldNameList = QString( fieldNamesRaw ).split( ',' );

		delete [] fieldNamesRaw;

		if ( numFields == FAIL )
		{
			printf( "failed to read field information\n" );
			return false;
		}

		if ( numFields != int32( fieldNameList.size() ) )
		{
			printf( "error parsing field names" );
			return false;
		}
	}


	bool retVal = true;

    // create the fields one at a time
	for ( int fieldIndex = 0; fieldIndex < numFields; fieldIndex++ )
	{
		ptr<hdfFieldNode> theField = new hdfFieldNode( this );

		if ( theField->Load( fieldNameList[ fieldIndex ] ) )
		{
			fieldList.push_back( theField );
		}
		else
		{
			retVal = false;
		}
	}

    // close the grid
	Close();

	return retVal;
}



void hdfGridNode::Destroy()
{
	fieldList.clear();
}



void hdfGridNode::Print( int indent ) const
{
	if ( indent > 32 ) indent = 32;
	char indentString[33];
	for ( int index = 0; index < 33; index++ )
	{
		indentString[ index ] = ( index < indent ) ? ' ' : '\0';
	}

	printf( "%sgrid \"%s\"\n", indentString, Name().toAscii().data() );
	printf( "%s+ size = %dx%dx%d\n", indentString, YDim(), XDim(), BlockDim() );

	if ( Scale() != 1.0 )
	{
		printf( "%s+ scale = %f\n", indentString, Scale() );
	}

	if ( SolarIrradiance() != 0.0 )
	{
		printf( "%s+ solar irradiance = %f\n", indentString, SolarIrradiance() );
	}

	if ( SolarDistance() != 0.0 )
	{
		printf( "%s+ solar distance = %f\n", indentString, SolarDistance() );
	}


	for ( int fieldIndex = 0 ; fieldIndex < NumFields() ; fieldIndex++ )
	{
		Field( fieldIndex )->Print( indent + 4 );
	}
}
