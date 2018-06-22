
#include <hdf.h>
#include <mfhdf.h>

#include <qlayout.h>
#include <qsizepolicy.h>

#include "dataChar.h"
#include "dataImage.h"



dataChar::dataChar( const colorMap & theColorMap, QWidget *parent )
:
QLabel( parent ),
csscIndex( 0 ),
min( -7 ),
max( 2 ),
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
    QVBoxLayout *layout = new QVBoxLayout( this );
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



dataChar::~dataChar()
{

}



int dataChar::CsscIndex() const
{
    return csscIndex;
}



void dataChar::setMinMax( char theMin, char theMax )
{
    min = theMin;
    max = theMax;
}



char dataChar::Min() const
{
    return min;
}



char dataChar::Max() const
{
    return max;
}



char dataChar::Min( const int newCsscIndex ) const
{
    char localMin = data[ newCsscIndex ][ 0 ][ 0 ];

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



char dataChar::Max( const int newCsscIndex ) const
{
    char localMax = data[ newCsscIndex ][ 0 ][ 0 ];

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



char dataChar::GlobalMin() const
{
    return globalMin;
}



char dataChar::GlobalMax() const
{
    return globalMax;
}



bool dataChar::LoadData( QString fileName, QString fieldName, int thresholdIndex )
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
                int32 start[ 4 ] = { thresholdIndex, 0, 0, 0 };
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

    CalcGlobalRange();

    return retVal;
}



void dataChar::dataDisplayUpdate( int x, int y )
{
    dataDisplay->setText( QString::number( data[ CsscIndex() ][ x ][ y ] ) );       
}




void dataChar::csscUpdate( int newCsscIndex )
{
    csscIndex = newCsscIndex;

    imageDisplay->InputData( & data[ CsscIndex() ][ 0 ][ 0 ], min, max );

    dataDisplay->setText( "" );
}



void dataChar::ClearData()
{
    memset( data, 0, sizeof( data ) );
}



void dataChar::CalcGlobalRange()
{
    globalMin = -7;
    globalMax = 2;
}

