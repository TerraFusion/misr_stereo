
#include <hdf.h>
#include <mfhdf.h>

#include <qlayout.h>
#include <qsizepolicy.h>

#include <math.h>

#include "dataFloat.h"
#include "dataImage.h"



dataFloat::dataFloat( const colorMap & theColorMap, QWidget *parent )
:
QLabel( parent ),
csscIndex( 0 ),
thresholdIndex( -1 ),
min( 0 ),
max( 1 ),
globalMin( 0 ),
globalMax( 0 ),
dataDisplay( 0 ),
imageDisplay( 0 )
{
	const int margin = 2;
	const int spacing = 2;

	ClearData();

    // raised frame
	setFrameStyle( QFrame::Panel | QFrame::Raised );

    // vertical layout
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( margin );
	layout->setSpacing( spacing );

    // image display
	imageDisplay = new dataImage( theColorMap, this );
	layout->addWidget( imageDisplay );

    // relay signals from image
	connect( imageDisplay, SIGNAL( PixelHover( int, int ) ), this, SIGNAL( PixelHover( int, int ) ) );
	connect( imageDisplay, SIGNAL( PixelClick( int, int ) ), this, SIGNAL( PixelClick( int, int ) ) );

    // data display label
	dataDisplay = new QLabel( this );
	dataDisplay->setMinimumWidth( 48 );
	dataDisplay->setFixedHeight( 16 );
	dataDisplay->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
	dataDisplay->setAlignment( Qt::AlignCenter );
	layout->addWidget( dataDisplay );

    // set our size constraints
	setMinimumSize( imageDisplay->minimumWidth() + 2 * margin, imageDisplay->minimumHeight() + 16 + 2 * margin + spacing );
	setMaximumSize( imageDisplay->maximumWidth() + 2 * margin, imageDisplay->maximumHeight() + 16 + 2 * margin + spacing );
}



dataFloat::~dataFloat()
{

}



int dataFloat::CsscIndex() const
{
	return csscIndex;
}



void dataFloat::setMinMax( float theMin, float theMax )
{
	min = theMin;
	max = theMax;
}



float dataFloat::Min() const
{
	return min;
}



float dataFloat::Max() const
{
	return max;
}



float dataFloat::Min( const int newCsscIndex ) const
{
	float localMin = data[ newCsscIndex ][ 0 ][ 0 ];

	for ( int x = 0 ; x < 12 ; x++ )
	{
		for ( int y = 0 ; y < 10 ; y++ )
		{
			if ( data[ newCsscIndex ][ x ][ y ] < localMin )
			{
				localMin = data[ newCsscIndex ][ x ][ y ];
			}
		}
	}

	return localMin;
}



float dataFloat::Max( const int newCsscIndex ) const
{
	float localMax = data[ newCsscIndex ][ 0 ][ 0 ];

	for ( int x = 0 ; x < 12 ; x++ )
	{
		for ( int y = 0 ; y < 10 ; y++ )
		{
			if ( data[ newCsscIndex ][ x ][ y ] > localMax )
			{
				localMax = data[ newCsscIndex ][ x ][ y ];
			}
		}
	}

	return localMax;
}



float dataFloat::GlobalMin() const
{
	return globalMin;
}



float dataFloat::GlobalMax() const
{
	return globalMax;
}



bool dataFloat::LoadData( QString fileName, QString fieldName, int theThresholdIndex )
{
	ClearData();

	bool retVal = false;

	int32 SDid = SDstart( fileName.toAscii(), DFACC_READ ); 

	if ( SDid != FAIL )
	{
		int32 dataIndex = SDnametoindex( SDid, fieldName.toAscii() );

		if ( dataIndex != FAIL )
		{
			int32 setId = SDselect( SDid, dataIndex );

			if ( setId != FAIL )
			{
				int32 start[ 4 ] = { theThresholdIndex, 0, 0, 0 };
				int32 edge[ 4 ] = { 1, 1581, 12, 10 };

				int32 result = SDreaddata( setId, start, NULL, edge, data );

				if ( result != FAIL )
				{
					retVal = true;
				}
			}

			SDendaccess( setId );
		}
	}

	SDend( SDid );



	if ( retVal == true )
	{
		thresholdIndex = theThresholdIndex;

		CalcGlobalRange();
	}
	else
	{
		thresholdIndex = -1;

		min = 0;
		max = 1;
	}

	return retVal;
}



void dataFloat::dataDisplayUpdate( int x, int y )
{
	if ( ( x >= 0 ) && ( x < 12 ) && ( y >= 0 ) && ( y < 10 ) )
	{
        // get data
		float theData = data[ CsscIndex() ][ x ][ y ];

        // calculate output precision
		int precision = 0;
		if( theData != 0 ) precision = int( 4 - log10( theData ) );
		if( precision < 0 ) precision = 0;

        // set label text
		dataDisplay->setText( QString::number( theData, 'f', precision ) );

        // notify others of data change
		emit dataChanged( theData );

		if ( thresholdIndex >= 0 )
		{
			emit dataChanged( theData, thresholdIndex );
		}
	}
}



void dataFloat::csscUpdate( int newCsscIndex )
{
	csscIndex = newCsscIndex;

	imageDisplay->InputData( & data[ CsscIndex() ][ 0 ][ 0 ], min, max );

	dataDisplay->setText( "" );
}



void dataFloat::ClearData()
{
	memset( data, 0, sizeof( data ) );
}



void dataFloat::CalcGlobalRange()
{
	min = data[ 0 ][ 0 ][ 0 ];
	max = data[ 0 ][ 0 ][ 0 ];

	for ( int c = 0 ; c <= 1580 ; c++ )
	{
		for ( int x = 0 ; x < 12 ; x++ )
		{
			for ( int y = 0 ; y < 10 ; y++ )
			{
				if ( data[ c ][ x ][ y ] < min ) min = data[ c ][ x ][ y ];
				if ( data[ c ][ x ][ y ] > max ) max = data[ c ][ x ][ y ];
			}
		}
	}

	if ( min < 0 ) min = 0;
}
