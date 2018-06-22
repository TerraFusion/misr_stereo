
#include "hdfField.h"
#include "hdfFile.h"
#include "hdfGrid.h"



// node functions //////////////////////////////////////////////////////////////



hdfFieldNode::hdfFieldNode( hdfGridNode * theParentGrid )
	:
parentGrid( theParentGrid ),
name(),
dimensionSize(),
dimensionName(),
dataType( hdfNone ),
dataSize( 0 ),
slice( -1 )
{
	memset( fillValue, 0, sizeof( fillValue ) );
}



hdfFieldNode::~hdfFieldNode()
{

}



ptr<hdfGridNode> hdfFieldNode::Grid() const
{
	return parentGrid;
}



std::string hdfFieldNode::Name() const
{
	return name;
}



int hdfFieldNode::Rank() const
{
	return int( dimensionName.size() );
}



int hdfFieldNode::DimensionSize( int index ) const
{
	return ( ( index >= 0 && index < Rank() ) ? dimensionSize[ index ] : 0 );
}



std::string hdfFieldNode::DimensionName( int index ) const
{
	return ( ( index >= 0 && index < Rank() ) ? dimensionName[ index ] : 0 );
}



std::list<std::stringlist> hdfFieldNode::OptionList() const
{
	std::list<std::stringlist> result;

	for ( int dim = 0 ; dim < Rank() ; ++dim )
	{
		if ( ( DimensionName( dim ) != "SOMBlockDim" )
				&& ( DimensionName( dim ) != "XDim" )
				&& ( DimensionName( dim ) != "YDim" ) )
		{
			std::stringlist itemList;
			std::string prettyName = DimensionName( dim );

			// pretty name : N{X}Dim --> {X}
			if ( StartsWith( prettyName, "N" ) && EndsWith( prettyName, "Dim" ) )
			{
				prettyName = prettyName.substr( 1, prettyName.length() - 4 );
			}

			// build list for a single option
			for ( int item = 1; item <= DimensionSize( dim ) ; ++item )
			{
				itemList.push_back( prettyName + ToString( item ) );
			}

			result.push_back( itemList );
		}
	}

	return result;
}



hdfDataType hdfFieldNode::DataType() const
{
	return dataType;
}



std::string hdfFieldNode::DataTypeString() const
{
	std::string dataTypeString = "<undefined>";

	switch ( DataType() )
	{
		case hdfChar8:    dataTypeString = "char8";    break;
		case hdfUChar8:   dataTypeString = "uchar8";   break;
		case hdfInt8:     dataTypeString = "int8";     break;
		case hdfUInt8:    dataTypeString = "uint8";    break;
		case hdfInt16:    dataTypeString = "int16";    break;
		case hdfUInt16:   dataTypeString = "uint16";   break;
		case hdfInt32:    dataTypeString = "int32";    break;
		case hdfUInt32:   dataTypeString = "uint32";   break;
		case hdfFloat32:  dataTypeString = "float32";  break;
		case hdfFloat64:  dataTypeString = "float64";  break;
		default: break;
	}

	return dataTypeString;
}



int hdfFieldNode::DataSize() const
{
	return dataSize;
}



int hdfFieldNode::BlockMemSize() const
{
	return ( DataSize() * YDim() * XDim() );
}



void hdfFieldNode::setSlice( int theSlice )
{
	slice = theSlice;
}



int hdfFieldNode::Slice() const
{
	return slice;
}



void *hdfFieldNode::FillValue() const
{
	return ( void * ) fillValue;
}



bool hdfFieldNode::Read( void * dest, int blockMin, int blockMax, int xMin, int xMax, int yMin, int yMax, const std::vector<int> & dims ) const
{
	int32 start[ 16 ];
	int32  edge[ 16 ];

	start[ 0 ] = blockMin - 1;
	edge[ 0 ] = blockMax - blockMin + 1;

	start[ 1 ] = xMin;
	edge[ 1 ] = xMax - xMin + 1;

	start[ 2 ] = yMin;
	edge[ 2 ] = yMax - yMin + 1;		

	for ( unsigned int i = 0 ; i < dims.size() ; ++i )
	{
		start[ i + 3 ] = dims[ i ];
		edge[ i + 3 ] = 1;
	}

	int32 status = GDreadfield( Grid()->ValidHandle(), (char*) Name().c_str(), start, NULL, edge, dest );

	if ( status == FAIL )
	{
		printf( "Failed to read field data from: %s\n", Name().c_str() );
		return false;
	}

	return true;
}



bool hdfFieldNode::ReadBlocks( void * dest, int blockMin, int blockMax, const std::vector<int> & dims ) const
{
	return Read( dest, blockMin, blockMax, 0, XDim() - 1, 0, YDim() - 1, dims );
}



bool hdfFieldNode::ReadBlock( void * dest, int blockIndex, const std::vector<int> & dims ) const
{
	return ReadBlocks( dest, blockIndex, blockIndex, dims );
}



bool hdfFieldNode::Open()
{
	return parentGrid->Open();
}



bool hdfFieldNode::Close()
{
	return parentGrid->Close();
}



bool hdfFieldNode::Load( std::string fieldName )
{
	name = fieldName;	
	int32 status = FAIL;

	int32 fieldRank = FAIL;
	int32 dimensionSizeList[ 16 ];
	int32 fieldType = FAIL;
	char dimensionNameList[ 2048 ];

	// get field information
	status = GDfieldinfo( Grid()->Handle(), (char*)Name().c_str(), & fieldRank, dimensionSizeList, & fieldType, dimensionNameList );
	if ( status == FAIL )
	{
		printf( "couldn't read dimensions from field: %s\n", Name().c_str() );
		return false;
	}

	// setup field type
	switch ( fieldType & DFNT_MASK )
	{
		case DFNT_CHAR8:    dataType = hdfChar8;    dataSize = sizeof( char8   );  break;
		case DFNT_UCHAR8:   dataType = hdfUChar8;   dataSize = sizeof( uchar8  );  break;
		case DFNT_INT8:     dataType = hdfInt8;     dataSize = sizeof( int8    );  break;
		case DFNT_UINT8:    dataType = hdfUInt8;    dataSize = sizeof( uint8   );  break;
		case DFNT_INT16:    dataType = hdfInt16;    dataSize = sizeof( int16   );  break;
		case DFNT_UINT16:   dataType = hdfUInt16;   dataSize = sizeof( uint16  );  break;
		case DFNT_INT32:    dataType = hdfInt32;    dataSize = sizeof( int32   );  break;
		case DFNT_UINT32:   dataType = hdfUInt32;   dataSize = sizeof( uint32  );  break;
		case DFNT_FLOAT32:  dataType = hdfFloat32;  dataSize = sizeof( float32 );  break;
		case DFNT_FLOAT64:  dataType = hdfFloat64;  dataSize = sizeof( float64 );  break;

		default:
			printf( "bad data type for field: %s\n", Name().c_str() );
			break;
	}

	// setup dimension names
	dimensionName = split( dimensionNameList, "," );

	// setup dimension sizes
	dimensionSize.resize( Rank() );
	for ( int index = 0 ; index < Rank() ; index++ )
	{
		dimensionSize[ index ] = int( dimensionSizeList[ index ] );
	}	

	// get fill value
	status = GDgetfillvalue( Grid()->Handle(), (char*) Name().c_str(), fillValue );
	if ( status == FAIL )
	{
		printf( "couldn't get fill value from field: %s\n", Name().c_str() );
		return false;
	}

	return true;
}



void hdfFieldNode::Destroy()
{

}



void hdfFieldNode::Print( int indent ) const
{
	if ( indent > 32 ) indent = 32;
	char indentString[33];
	for ( int index = 0; index < 33; index++ )
	{
		indentString[ index ] = ( index < indent ) ? ' ' : '\0';
	}

	printf( "%sfield \"%s\"\n", indentString, Name().c_str() );
	printf( "%s+ type = %s ( %d bytes )\n", indentString, DataTypeString().c_str(), DataSize() );
	printf( "%s+ dimension[%d] = {", indentString, Rank() );

	for ( int index = 0 ; index < Rank() ; index++ )
	{
		printf( " %s(%d)", DimensionName( index ).c_str(), DimensionSize( index ) );
	}

	printf( " }\n" );
}
