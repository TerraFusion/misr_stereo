
#include "hdfFile.h"
#include "hdfGrid.h"
#include "hdfField.h"
#include "utility.h"

// node functions //////////////////////////////////////////////////////////////



hdfFileNode::hdfFileNode()
	:
handle( FAIL ),
name(),
description(),
startBlock( 0 ),
endBlock( 0 ),
numBlocks( 0 ),
pathNumber( -1 ),
cameraNumber( -1 ),
blockList(),
gridList()
{

}



hdfFileNode::~hdfFileNode()
{
	Close();
}



bool hdfFileNode::CheckDependencies() const
{
	std::vector< ptr< hdfGridNode > >::const_iterator currentGrid = gridList.begin();

	while ( currentGrid != gridList.end() )
	{
		if ( ( (*currentGrid).Count() > 1 ) || (*currentGrid)->CheckDependencies() )
		{
			return true;
		}

		++currentGrid;
	}

	return false;
}



int32 hdfFileNode::Handle() const
{
	return handle;
}



int32 hdfFileNode::ValidHandle()
{
	Open();

	return Handle();
}



std::string hdfFileNode::Name() const
{
	return name;
}



std::string hdfFileNode::Description() const
{
	return description;
}



int hdfFileNode::StartBlock() const
{
	return int( startBlock );
}



int hdfFileNode::EndBlock() const
{
	return int( endBlock );
}



int hdfFileNode::NumBlocks() const
{
	return int( numBlocks );
}



static const hdfRect hdfDefaultBlockRect( 0.0, 0.0, 0.0, 0.0 );

const hdfRect & hdfFileNode::BlockRect( int blockIndex ) const
{
	return ( ( blockIndex >= StartBlock() ) && ( blockIndex <= EndBlock() ) ) ? blockList[ blockIndex ].bounds : hdfDefaultBlockRect;
}



hdfPolygon hdfFileNode::BlockPolygon( int blockIndex ) const
{
	return CreatePolygon( BlockRect( blockIndex ) );
}



std::list<hdfPolygon> hdfFileNode::BlockPolygonList() const
{
	std::list<hdfPolygon> output;

	for ( int blockIndex = StartBlock() ; blockIndex <= EndBlock() ; blockIndex++ )
	{
		output.push_back( BlockPolygon( blockIndex ) );
	}

	return output;
}



int hdfFileNode::PathNumber() const
{
	return int( pathNumber );
}



int hdfFileNode::CameraNumber() const
{
	return int( cameraNumber );
}



std::string hdfFileNode::CameraName() const
{
	std::string retVal;

	switch ( CameraNumber() )
	{
		case 1: retVal = "DF"; break;
		case 2: retVal = "CF"; break;
		case 3: retVal = "BF"; break;
		case 4: retVal = "AF"; break;
		case 5: retVal = "AN"; break;
		case 6: retVal = "AA"; break;
		case 7: retVal = "BA"; break;
		case 8: retVal = "CA"; break;
		case 9: retVal = "DA"; break;
		default: retVal = "noCamera"; break;
	}

	return retVal;
}



int hdfFileNode::OrbitNumber() const
{
	return orbitNumber;
}



int hdfFileNode::NumGrids() const
{
	return int( gridList.size() );
}



ptr<hdfGridNode> hdfFileNode::Grid( int gridIndex ) const
{
	return ( ( gridIndex >= 0 ) && ( gridIndex < NumGrids() ) ) ? ( gridList[ gridIndex ] ) : ( 0 );
}



int hdfFileNode::GridIndex( std::string gridName ) const
{
	int gridIndex;

	for ( gridIndex = 0 ; gridIndex < NumGrids() ; gridIndex++ )
	{
		if ( gridName == Grid( gridIndex )->Name() )
		{
			break;
		}
	}

	if ( gridIndex >= NumGrids() )
	{
		gridIndex = -1;
	}

	return gridIndex;
}



ptr<hdfGridNode> hdfFileNode::Grid( std::string gridName ) const
{
	return Grid( GridIndex( gridName ) );
}



int hdfFileNode::NumFields() const
{
	int numFields = 0;

	for ( int gridIndex = 0 ; gridIndex < NumGrids() ; gridIndex++ )
	{
		numFields += Grid( gridIndex )->NumFields();
	}

	return numFields;
}



ptr<hdfFieldNode> hdfFileNode::Field( int fieldIndex ) const
{
	ptr<hdfFieldNode> field;

	for ( int gridIndex = 0 ; gridIndex < NumGrids() ; gridIndex++ )
	{
		if ( fieldIndex < Grid( gridIndex )->NumFields() )
		{
			field = Grid( gridIndex )->Field( fieldIndex );
			break;
		}
		else
		{
			fieldIndex -= Grid( gridIndex )->NumFields();
		}
	}

	return field;
}



std::stringlist hdfFileNode::FieldNameList() const
{
	std::stringlist fieldNameList;

	for ( int fieldIndex = 0 ; fieldIndex < NumFields() ; ++fieldIndex )
	{
		fieldNameList.push_back( Field( fieldIndex )->Name() );
	}

	return fieldNameList;
}



int hdfFileNode::FieldIndex( std::string fieldName ) const
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



ptr<hdfFieldNode> hdfFileNode::Field( std::string fieldName ) const
{
	return Field( FieldIndex( fieldName ) );
}



bool hdfFileNode::Open()
{
	if ( Handle() == FAIL )
	{
		handle = GDopen( (char*) Name().c_str(), DFACC_READ );

		if ( Handle() == FAIL )
		{
			printf( "failed to open file: %s\n", Name().c_str() );
			return false;
		}
	}

	return true;
}



bool hdfFileNode::Close()
{
	if ( Handle() != FAIL )
	{
		for ( int index = 0 ; index < NumGrids() ; index++ )
		{
			gridList[ index ]->Close();
		}

		int32 status = GDclose( Handle() );

		handle = FAIL;

		if ( status == FAIL )
		{
			printf( "failed to close file: %s\n", Name().c_str() );
			return false;
		}
	}

	return true;
}

int GetOrbitNumber( std::string fileName )
{
	size_t first = fileName.find( "_O" ) + 2;
	while( first < fileName.length() )
	{
		size_t last = fileName.find_first_not_of( "0123456789", first );
		if( fileName[ last ] == '_' && last - first > 0 )
		{
			return FromString<int>( fileName.substr( first, last - first ) );
		}
		first = fileName.find( "_O", last ) + 2;
	}

	return 0;
}

bool hdfFileNode::Load( std::string fileName )
{
	int32 status = FAIL;
	name = fileName;


    // open the file
	Open(); 



    // orbit number ////////////////////////////////////////////////////////////

	orbitNumber = GetOrbitNumber( fileName );




    // data ////////////////////////////////////////////////////////////////////

	int32 sdID;
	int32 vID;
	status = EHidinfo( handle, &vID, &sdID );
	if ( status == FAIL )
	{
		printf( "failed to get vdata file handle\n" );
		return false;
	}



	status = Vstart( vID );
	if ( status == FAIL )
	{
		printf( "failed to create vdata interface\n" );
		return false;
	}



	int32 dataRef = -1;
	int32 dataID = -1;
	int32 NumRecords = -1;
	int32 InterlaceMode = -1;
	int32 DataSize = -1;

	char dataFieldNames[ MAX_FIELD_SIZE ];
	char dataNameRaw[ VSNAMELENMAX ];
	std::string dataName;
	std::string gridName;

	int scaleCounter = 0;
	int solarIrradianceCounter = 0;
	int solarDistanceCounter = 0;

	while ( ( dataRef = VSgetid( vID, dataRef ) ) != FAIL )
	{
        // attach to data chunk
		dataID = VSattach( vID, dataRef, "r" );
		if ( dataID == FAIL )
		{
			printf( "couldn't create data interface\n" );
			return false;
		}



        // get data chunk info
		status = VSinquire( dataID, &NumRecords, &InterlaceMode, dataFieldNames, &DataSize, dataNameRaw );
		if ( status == FAIL )
		{
			printf( "couldn't get data chunk description\n" );
			return false;
		}



        // seek to beginning of data
		status = VSseek( dataID, 0 );
		if ( status == FAIL )
		{
			printf( "couldn't seek to beginning of data\n" );
			return false;
		}



        // fill in data / grid name
		std::string qDataNameRaw( dataNameRaw );
		size_t colonIndex = qDataNameRaw.find( ':' );
		if ( colonIndex < qDataNameRaw.size() )
		{
			dataName = qDataNameRaw.substr( 0, colonIndex );
			gridName = qDataNameRaw.substr( colonIndex + 1, qDataNameRaw.length() - ( colonIndex + 1 ) );
		}
		else
		{
			dataName = qDataNameRaw;
			gridName = "";
		}



        // do chunk-specific operations
		if ( dataName == "PerBlockMetadataCommon" )
		{
			blockList.clear();



			status = VSsetfields( dataID, dataFieldNames );
			if ( status == FAIL )
			{
				printf( "couldn't set fields to read from data chunk\n" );
				return false;
			}



			uint8 blockDataBuffer[ sizeof( uint32 ) + sizeof( uint8 ) + 4 * sizeof( float64 ) + sizeof( uint8 ) ];
			for ( int index = 0 ; index < NumRecords ; index++ )
			{
				status = VSread( dataID, blockDataBuffer, 1, InterlaceMode );
				if ( status == FAIL )
				{
					printf( "couldn't read block data\n" );
					return false;
				}

				uint8 *readData = blockDataBuffer;
				double coord;

                // block index
				int32 blockIndex;
				memcpy( &blockIndex, readData, sizeof( uint32 ) );
				readData += sizeof( uint32 );

                // make room for new blocks
				if ( int( blockList.size() ) <= blockIndex )
				{
					blockList.resize( blockIndex + 1 );
				}

                // ocean flag
				memcpy( &blockList[ blockIndex ].oceanFlag, readData, sizeof( uint8 ) );
				readData += sizeof( uint8 );

                // block left
				memcpy( &coord, readData, sizeof( float64 ) );
				readData += sizeof( float64 );
				blockList[ blockIndex ].bounds.setLeft( coord );

                // block top
				memcpy( &coord, readData, sizeof( float64 ) );
				readData += sizeof( float64 );
				blockList[ blockIndex ].bounds.setTop( coord );

                // block right
				memcpy( &coord, readData, sizeof( float64 ) );
				readData += sizeof( float64 );
				blockList[ blockIndex ].bounds.setRight( coord );

                // block bottom
				memcpy( &coord, readData, sizeof( float64 ) );
				readData += sizeof( float64 );
				blockList[ blockIndex ].bounds.setBottom( coord );

                // data flag
				memcpy( &blockList[ blockIndex ].dataFlag, readData, sizeof( uint8 ) );
				readData += sizeof( uint8 );
			}

			blockRect = blockList[ StartBlock() ].bounds;
			for ( int i = StartBlock() + 1 ; i <= EndBlock() ; ++i )
			{
				blockRect.Include( blockList[ i ].bounds );
			}
		}
		else if ( dataName == "_BLKSOM" )
		{
			ptr<hdfGridNode> gridNode = CreateGridNode( gridName );

			gridNode->blockOffset.clear();
			gridNode->blockOffset.resize( 180, 0 );

			status = VSsetfields( dataID, dataFieldNames );
			if ( status == FAIL )
			{
				printf( "couldn't set fields to read from data chunk\n" );
				return false;
			}

			status = VSread( dataID, (unsigned char *) &gridNode->blockOffset[1], 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block offsets\n" );
				return false;
			}

		}
		else if ( dataName == "SOMBlockDim" )
		{
			int32 blockDim;
			status = VSread( dataID, ( uint8 * ) &blockDim, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}

			ptr<hdfGridNode> theGrid = CreateGridNode( gridName );
			if ( theGrid.IsValid() ) theGrid->blockDim = blockDim;
		}
		else if ( dataName == "XDim" )
		{
			int32 xDim;
			status = VSread( dataID, ( uint8 * ) &xDim, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}

			ptr<hdfGridNode> theGrid = CreateGridNode( gridName );
			if ( theGrid.IsValid() ) theGrid->xDim = xDim;
		}
		else if ( dataName == "YDim" )
		{
			int32 yDim;
			status = VSread( dataID, ( uint8 * ) &yDim, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}   

			ptr<hdfGridNode> theGrid = CreateGridNode( gridName );
			if ( theGrid.IsValid() ) theGrid->yDim = yDim;
		}
		else if ( dataName == "Scale factor" )
		{
			if ( scaleCounter < NumGrids() )
			{
				ptr<hdfGridNode> gridNode = Grid( scaleCounter );

				if ( gridNode.IsValid() )
				{
					status = VSread( dataID, ( uint8 * ) & ( gridNode->scale ), 1, InterlaceMode );

					if ( status == FAIL )
					{
						printf( "couldn't read scaling factor\n" );
						return false;
					}               
				}

				scaleCounter++;
			}
		}
		else if ( dataName == "std_solar_wgted_height" )
		{
			if ( solarIrradianceCounter < NumGrids() )
			{
				ptr<hdfGridNode> gridNode = Grid( solarIrradianceCounter );

				if ( gridNode.IsValid() )
				{
					float offset = 0;

					status = VSread( dataID, ( uint8 * ) & ( offset ), 1, InterlaceMode );

					if ( status == FAIL )
					{
						printf( "couldn't read solar irradiance\n" );
						return false;
					}
					else
					{
						gridNode->solarIrradiance = double( offset );
					}
				}

				solarIrradianceCounter++;
			}
		}
		else if ( dataName == "SunDistanceAU" )
		{
			if ( solarDistanceCounter < NumGrids() )
			{
				ptr<hdfGridNode> gridNode = Grid( solarDistanceCounter );

				if ( gridNode.IsValid() )
				{
					status = VSread( dataID, ( uint8 * ) & ( gridNode->solarDistance ), 1, InterlaceMode );

					if ( status == FAIL )
					{
						printf( "couldn't read solar distance\n" );
						return false;
					}               
				}

				solarDistanceCounter++;
			}
		}
		else if ( dataName == "HDFEOSVersion" )
		{

		}
		else if ( dataName == "StructMetadata.0" )
		{

		}
		else if ( dataName == "Start_block" )
		{
			status = VSread( dataID, (uint8 *)&startBlock, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}
		}
		else if ( dataName == "End block" )
		{
			status = VSread( dataID, (uint8 *)&endBlock, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}
		}
		else if ( dataName == "Number_blocks" )
		{
			status = VSread( dataID, (uint8 *)&numBlocks, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}
		}
		else if ( dataName == "Path_number" )
		{
			status = VSread( dataID, (uint8 *)&pathNumber, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}
		}
		else if ( dataName == "Camera" )
		{
			status = VSread( dataID, (uint8 *)&cameraNumber, 1, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read block data\n" );
				return false;
			}
		}/*
		else if ( dataName == "coremetadata" )
		{
			printf( "saving core metadata ( size = %d )\n", int( DataSize ) );

			uint8 * buffer = new uint8 [ DataSize ];

			status = VSread( dataID, buffer, NumRecords, InterlaceMode );
			if ( status == FAIL )
			{
				printf( "couldn't read core metadata\n" );
				delete buffer;
				return false;
			}

			QFile outFile( std::string::number( OrbitNumber() ) + ".dat" );
			outFile.open( IO_WriteOnly );
			outFile.writeBlock( ( const char * ) buffer,
				( unsigned long int ) DataSize );
			outFile.close();

			delete [] buffer;
		}*/
		else
		{
			/*
			if ( gridName.isNull() || gridName.isEmpty() )
			{
				printf( "data chunk: \"%s\"", dataNameRaw );
		}
		else
		{
			printf( "data chunk: \"%s\" ( %s )", dataName.c_str(), gridName.c_str() );
		}

		printf( "\n" );
		*/
		}



        // detach from data chunk
		status = VSdetach( dataID );
		if ( status == FAIL )
		{
			printf( "couldn't detach from data chunk\n" );
			return false;
		}
	}



    // close data interface
	status = Vend( vID );
	if ( status == FAIL )
	{
		printf( "failed to close data interface\n" );
		return false;
	}   



    // close the file
	Close();



	return true;
}



void hdfFileNode::Destroy()
{
	for ( int gridIndex = 0 ; gridIndex < NumGrids() ; gridIndex++ )
	{
		gridList[ gridIndex ]->Destroy();
	}

	gridList.clear();
}



void hdfFileNode::Print( int indent ) const
{
	if ( indent > 32 ) indent = 32;
	char indentString[33];
	for ( int index = 0; index < 33; index++ )
	{
		indentString[ index ] = ( index < indent ) ? ' ' : '\0';
	}

	printf( "%sfile \"%s\"\n", indentString, Name().c_str() );
	printf( "%s+ orbit number = %d\n", indentString, OrbitNumber() );
	printf( "%s+ path number = %d\n", indentString, PathNumber() );
	printf( "%s+ camera = %s\n", indentString, CameraName().c_str() );
	printf( "%s+ start block = %d\n", indentString, StartBlock() );
	printf( "%s+ end block = %d\n", indentString, EndBlock() );
	printf( "%s+ numBlocks = %d\n", indentString, NumBlocks() );

	for ( int gridIndex = 0 ; gridIndex < NumGrids() ; gridIndex++ )
	{
		Grid( gridIndex )->Print( indent + 4 );
	}
}



ptr<hdfGridNode> hdfFileNode::CreateGridNode( std::string gridName )
{
	ptr<hdfGridNode> theGrid = Grid( gridName );

	if ( theGrid.IsNull() )
	{
		theGrid = new hdfGridNode( this );

		if ( theGrid->Load( gridName ) )
		{
			gridList.push_back( theGrid );
		}
	}

	return theGrid;
}
