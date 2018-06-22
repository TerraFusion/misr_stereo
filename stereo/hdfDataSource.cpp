
#include <math.h>

#include "hdfDataSource.h"


hdfDataSource::hdfDataSource( hdfField theDataField, const std::vector<int> & dims )
:
hdfDataNode(),
dataField( theDataField ),
startBlock( dataField->StartBlock() ),
endBlock( dataField->EndBlock() ),
dimensionList( dims ),
dataRect( 0, 0, hdfScalar( dataField->XDim() ), hdfScalar( dataField->YDim() ) ),
blockLimit( 0x7FFFFFFF ),
blockList( dataField->NumBlocks(), ( unsigned char * ) 0 ),
validList()
{
	dataField->Open();
}



hdfDataSource::~hdfDataSource()
{
	BlockDestroyAll();
	blockList.clear();
}



hdfField hdfDataSource::DataField() const
{
	return dataField;
}



void hdfDataSource::setBlockLimit( int theBlockLimit )
{
	blockLimit = ( theBlockLimit > 1 ) ? ( theBlockLimit ) : ( 1 );
}



int hdfDataSource::BlockLimit() const
{
	return blockLimit;
}



void hdfDataSource::setBlockRange( int startBlockIndex, int endBlockIndex )
{
	setStartBlock( startBlockIndex );
	setEndBlock( endBlockIndex );
}



void hdfDataSource::setStartBlock( int blockIndex )
{
	startBlock = clamp( blockIndex, DataField()->StartBlock(), DataField()->EndBlock() );
}



int hdfDataSource::StartBlock() const
{
	return startBlock;
}



void hdfDataSource::setEndBlock( int blockIndex )
{
	endBlock = clamp( blockIndex, DataField()->StartBlock(), DataField()->EndBlock() );
}



int hdfDataSource::EndBlock() const
{
	return endBlock;
}



void hdfDataSource::setDimensions( const std::vector<int> & dims )
{
	dimensionList = dims;
}



const std::vector<int> & hdfDataSource::Dimensions() const
{
	return dimensionList;
}



hdfValue hdfDataSource::Value( const hdfCoord & location )
{
	hdfScalar metersPerBlock = fabs( dataField->BlockRect( dataField->StartBlock() ).Width() );

	int blockIndex = dataField->StartBlock() + int( ( location.x - dataField->BlockRect( dataField->StartBlock() ).Left() ) / metersPerBlock );

	if ( blockIndex >= dataField->StartBlock() && blockIndex <= dataField->EndBlock() )
	{
		if ( blockIndex >= StartBlock() && blockIndex <= EndBlock() )
		{
			if ( dataField->BlockRect( blockIndex ).Contains( location ) )
			{
				hdfCoord dataCoord(
						dataRect.ConvertGlobal(
							dataField->BlockRect( blockIndex ).ConvertLocal( location ) ) );

				int blockX = int( dataCoord.x );
				int blockY = int( dataCoord.y );

				if ( BlockValid( blockIndex ) == false )
				{
					BlockCreate( blockIndex );
				}

				unsigned char *dataPoint = blockList[ blockIndex ]
					+ ( blockX * dataField->YDim() + blockY ) * dataField->DataSize();

				if ( memcmp( dataPoint, dataField->FillValue(), dataField->DataSize() ) != 0 )
				{
					return hdfValue( ConvertToCommon( dataPoint ), 1 );
				}
			}
		}
	}

	return hdfValue( 0.0, 0.0 );
}



std::list<hdfPolygon> hdfDataSource::GetMask() const
{
	return dataField->BlockPolygonList();
}



matrix<hdfValue> hdfDataSource::GetBlock( int blockIndex )
{
	if ( ( blockIndex >= StartBlock() ) && ( blockIndex <= EndBlock() ) )
	{
		matrix<hdfValue> result( DataField()->YDim(), DataField()->XDim() );
		matrix<hdfValue>::iterator outData = result.begin();

		BlockCreate( blockIndex );
		unsigned char * rawData = blockList[ blockIndex ];
		const int rawDataSize = DataField()->DataSize();
		const void * rawFill = DataField()->FillValue();

		while ( outData != result.end() )
		{
			// check for fill value
			if ( memcmp( rawData, rawFill, rawDataSize ) == 0 )
			{
				*outData = hdfValue( 0, 0 );
			}
			else
			{
				*outData = hdfValue( ConvertToCommon( rawData ), 1 );
			}

			// move to next pixel
			++outData;
			rawData += rawDataSize;
		}

		return result;
	}
	else
	{
		return matrix<hdfValue>();
	}
}



void hdfDataSource::Flush()
{
	BlockDestroyAll();
}



void hdfDataSource::BlockCreate( int blockIndex )
{
	if ( ! BlockValid( blockIndex ) )
	{
		if ( int( validList.size() ) >= blockLimit )
		{
			BlockDestroy( validList.front() );
			validList.pop_front();
		}

		blockList[ blockIndex ] = new unsigned char [ DataField()->BlockMemSize() ];

		DataField()->ReadBlock( blockList[ blockIndex ], blockIndex, Dimensions() );

		validList.push_back( blockIndex );
	}
}



void hdfDataSource::BlockDestroy( int blockIndex )
{
	if ( BlockValid( blockIndex ) )
	{
		delete [] blockList[ blockIndex ];

		blockList[ blockIndex ] = 0;
	}
}



void hdfDataSource::BlockDestroyAll()
{
	std::list<int>::iterator current = validList.begin();

	while ( current != validList.end() )
	{
		BlockDestroy( *current++ );
	}

	validList.clear();
}



bool hdfDataSource::BlockValid( int blockIndex ) const
{
	return ( blockList[ blockIndex ] != 0 );
}



hdfScalar hdfDataSource::ConvertToCommon( void * data ) const
{
	hdfScalar retVal( 0 );

	switch ( dataField->DataType() )
	{
		case hdfChar8:    retVal = * ( char8   * ) data;   break;
		case hdfUChar8:   retVal = * ( uchar8  * ) data;   break;
		case hdfInt8:     retVal = * ( int8    * ) data;   break;
		case hdfUInt8:    retVal = * ( uint8   * ) data;   break;
		case hdfInt16:    retVal = * ( int16   * ) data;   break;
		case hdfUInt16:   retVal = * ( uint16  * ) data;   break;
		case hdfInt32:    retVal = * ( int32   * ) data;   break;
		case hdfUInt32:   retVal = * ( uint32  * ) data;   break;
		case hdfFloat32:  retVal = * ( float32 * ) data;   break;
		case hdfFloat64:  retVal = * ( float64 * ) data;   break;
		default: break;
	}

	return retVal;
}



hdfRadianceSource::hdfRadianceSource( hdfField theDataField )
	:
hdfDataSource( theDataField, std::vector<int>() )
{
	scaleFactor = dataField->Scale();
}



hdfRadianceSource::~hdfRadianceSource()
{

}



hdfValue hdfRadianceSource::Value( const hdfCoord & location )
{
	hdfValue ans( hdfDataSource::Value( location ) );

	return hdfValue( floor( ans.x / 4.0 ) * scaleFactor, ans.y );
}



hdfBrfSource::hdfBrfSource( ptr<hdfRadianceSource> dataRad, ptr<hdfDataSource> dataSZA )
:
hdfOpBinary<hdfOpBinaryMul>()
{        
	setInput( 0, new hdfOpUnary<hdfOpUnarySecDeg>( dataSZA ) );
	setInput( 1, new hdfOpUnary<hdfOpUnaryMul>( dataRad,
			hdfOpUnaryMul( pi * square( dataRad->DataField()->SolarDistance() )
				/ dataRad->DataField()->SolarIrradiance() ) ) );
}



hdfBrfSource::~hdfBrfSource()
{

}



hdfBdasSource::hdfBdasSource( ptr<hdfBrfSource> red1, ptr<hdfBrfSource> blue1, ptr<hdfBrfSource> red2, ptr<hdfBrfSource> blue2 )
:
hdfOpBinary<hdfOpBinarySub>()
{
	setInput( 0, new hdfOpBinary<hdfOpBinarySub>( red1, blue1 ) );
	setInput( 1, new hdfOpBinary<hdfOpBinarySub>( red2, blue2 ) );
}



hdfBdasSource::~hdfBdasSource()
{

}
