
#include <math.h>

#include "hdfDataSource.h"
#include "fileHandler.h"

hdfDataSource::hdfDataSource( hdfField theDataField, const std::vector<int> & dims )
:
hdfDataNode(),
dimensionList( dims )
{
	setDataField( theDataField );
}

hdfDataSource::~hdfDataSource()
{

}

std::string hdfDataSource::Name() const
{
	return "dataSource";
}

void hdfDataSource::PrintAttributes( std::ostream& out, const std::string& prefix ) const
{
	out << prefix << "source = " << name.toStdString() << '\n';
}

void hdfDataSource::setDataField( hdfField theDataField )
{
	if( theDataField != dataField )
	{
		dataField = theDataField;
		blockList.clear();
		if( dataField.IsValid() )
		{
			name = dataField->Name();
			cameraName = dataField->CameraName();
			dataField->Open();
			blockList.resize( dataField->NumBlocks() );
		}
	}
}

const hdfField & hdfDataSource::DataField() const
{
	return dataField;
}

void hdfDataSource::setDimensions( const std::vector<int> & dims )
{
	if( dims != dimensionList )
	{
		dimensionList = dims;
		blockList.clear();
		blockList.resize( dataField->NumBlocks() );
	}
}

const std::vector<int> & hdfDataSource::Dimensions() const
{
	return dimensionList;
}

void hdfDataSource::setOrbit( int orbit )
{
	setDataField( Data->OpenField( name, cameraName, orbit ) );
}

bool hdfDataSource::Check() const
{
	return dataField.IsValid();
}

const hdfDataSource* hdfDataSource::FindDataSource() const
{
	return this;
}

hdfValue hdfDataSource::SampleAt( const hdfCoord & location )
{
	hdfScalar metersPerBlock = fabs( dataField->BlockRect( dataField->StartBlock() ).Width() );

	int blockIndex = dataField->StartBlock() + int( ( location.x - dataField->BlockRect( dataField->StartBlock() ).Left() ) / metersPerBlock );

	if ( blockIndex >= dataField->StartBlock() && blockIndex <= dataField->EndBlock() )
	{
		hdfCoord dataCoord = dataField->BlockRect( blockIndex ).ConvertLocal( location );

		if ( dataCoord.x >= 0 && dataCoord.x < 1 && dataCoord.y >= 0 && dataCoord.y < 1 )
		{
			LoadBlock( blockIndex );

			int blockX = int( dataField->XDim() * dataCoord.x );
			int blockY = int( dataField->YDim() * dataCoord.y );

			unsigned char *dataPoint = (unsigned char *)&blockList[ blockIndex ][ 0 ]
				+ ( blockX * dataField->YDim() + blockY ) * dataField->DataSize();

			if ( memcmp( dataPoint, dataField->FillValue(), dataField->DataSize() ) != 0 )
			{
				return hdfValue( ConvertToCommon( dataPoint ), 1 );
			}
		}
	}

	return hdfValue( 0.0, 0.0 );
}

hdfValue hdfDataSource::Value( const hdfCoord & location )
{
	return SampleAt( location );
}

std::vector<hdfValue> hdfDataSource::Values( const std::vector<hdfCoord> & location )
{
	std::vector<hdfValue> output( location.size() );
	for( unsigned int i = 0; i < location.size(); i++ )
	{
		output[ i ] = SampleAt( location[ i ] );
	}
	return output;
}

std::list<hdfPolygon> hdfDataSource::GetMask() const
{
	return dataField->BlockPolygonList();
}

matrix<hdfValue> hdfDataSource::GetBlock( int blockIndex )
{
	if ( ( blockIndex >= DataField()->StartBlock() ) && ( blockIndex <= DataField()->EndBlock() ) )
	{
		matrix<hdfValue> result( DataField()->YDim(), DataField()->XDim() );
		matrix<hdfValue>::iterator outData = result.begin();

		LoadBlock( blockIndex );
		unsigned char * rawData = & blockList[ blockIndex ][ 0 ];
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
	blockList.clear();
}

void hdfDataSource::LoadBlock( int blockIndex )
{
	if ( blockList[ blockIndex ].empty() )
	{
		blockList[ blockIndex ].resize( DataField()->BlockMemSize() );
		DataField()->ReadBlock( & blockList[ blockIndex ][ 0 ], blockIndex, Dimensions() );
	}
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
