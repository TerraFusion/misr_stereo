
#include <qstring.h>
#include <qlayout.h>
#include <qlabel.h>

#include "rccmThresholds.h"
#include "dataFloat.h"
#include "dataChar.h"
#include "SpectrumLabel.h"



rccmThresholds::rccmThresholds( QWidget *parent )
:
QWidget( parent )
{
    // layout
    QGridLayout *gridLayout = new QGridLayout( this );
	gridLayout->setSpacing( 4 );
    gridLayout->setColumnMinimumWidth( 7, 8 );



    // color map sfor threshold displays
    colorMap thresholdColorMap[ 2 ] = { redGradientMap, greenGradientMap };



    // threshold thumbnails
    for ( int i = 0 ; i < 2 ; i++ )
    {
        for ( int y = 0 ; y < 5 ; y++ )
        {
            for ( int x = 0 ; x < 3 ; x++ )
            {
                dataFloat *thresholdPtr = new dataFloat( thresholdColorMap[ i ], this );
                thresholdDisplay[ i ][ y ][ x ] = thresholdPtr;
                gridLayout->addWidget( thresholdPtr, y + 3, x + 1 + i * 7 );
                connect( thresholdPtr, SIGNAL( PixelClick( int, int ) ), this, SIGNAL( PixelClick( int, int ) ) );
                connect( thresholdPtr, SIGNAL( PixelHover( int, int ) ), this, SIGNAL( PixelHover( int, int ) ) );
                connect( this, SIGNAL( dataDisplayUpdate( int, int ) ), thresholdPtr, SLOT( dataDisplayUpdate( int, int ) ) );

                dataChar *updateFlagPtr = new dataChar( eightColorMap, this );
                updateFlagDisplay[ i ][ y ][ x ] = updateFlagPtr;
                gridLayout->addWidget( updateFlagPtr, y + 3, x + 4 + i * 7 );
                connect( updateFlagPtr, SIGNAL( PixelClick( int, int ) ), this, SIGNAL( PixelClick( int, int ) ) );
                connect( updateFlagPtr, SIGNAL( PixelHover( int, int ) ), this, SIGNAL( PixelHover( int, int ) ) );
                connect( this, SIGNAL( dataDisplayUpdate( int, int ) ), updateFlagPtr, SLOT( dataDisplayUpdate( int, int ) ) );
            }
        }
    }



    // spectrums
    for ( int i = 0 ; i < 2 ; i++ )
    {
        spectrum[ i ] = new SpectrumLabel( thresholdColorMap[ i ], this );
        gridLayout->addWidget( spectrum[ i ], 8, 1 + i * 7, 1, 6 );
    }   



    // coordinate display
    coordinates = new QLabel( "( 0, 0 )", this );
    gridLayout->addWidget( coordinates, 1, 0 );
    coordinates->setFixedWidth( coordinates->fontMetrics().width( "( 12, 12 )" ) );
	coordinates->setAlignment( Qt::AlignCenter );
    connect( this, SIGNAL( dataDisplayUpdate( int, int ) ), this, SLOT( coordinateDisplayUpdate( int, int ) ) );



    QLabel *newLabel;



    // observable titles
    newLabel = new QLabel( "First Observable", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 0, 1, 1, 6 );

    newLabel = new QLabel( "Second Observable", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 0, 8, 1, 6 );



    // threshold / update titles
    newLabel = new QLabel( "Threshold", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 1, 1, 1, 3 );

    newLabel = new QLabel( "Update Flag", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 1, 4, 1, 3 );

    newLabel = new QLabel( "Threshold", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 1, 8, 1, 3 );

    newLabel = new QLabel( "Update Flag", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 1, 11, 1, 3 );



    // column titles
    for ( int i = 0 ; i < 2 ; i++ )
    {
        for ( int x = 0 ; x < 6 ; x++ )
        {
            int tIndex = x % 3 + 1;

            newLabel = new QLabel( QString( "T" ) + QString::number( tIndex ), this );
			newLabel->setAlignment( Qt::AlignCenter );
			QPalette pal( newLabel->palette() );
			pal.setColor( QPalette::Foreground, ( tIndex == 1 ) ? Qt::darkRed : ( ( tIndex == 2 ) ? Qt::darkGreen : Qt::darkBlue ) );
			newLabel->setPalette( pal );
            gridLayout->addWidget( newLabel, 2, x + 1 + i * 7 );
        }
    }



    // row titles
    newLabel = new QLabel( "AN", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 3, 0 );

    newLabel = new QLabel( "A", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 4, 0 );

    newLabel = new QLabel( "B", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 5, 0 );

    newLabel = new QLabel( "C", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 6, 0 );

    newLabel = new QLabel( "D", this );
    newLabel->setAlignment( Qt::AlignCenter );
    gridLayout->addWidget( newLabel, 7, 0 );
}



rccmThresholds::~rccmThresholds()
{

}



bool rccmThresholds::LoadData( const QString & fileName, int index )
{
    if ( ( index < 0 ) || ( index > 4 ) ) return false;

    bool result = true;

    for ( int j = 0 ; j < 3 ; j++ )
    {
        result &=  thresholdDisplay[ 0 ][ index ][ j ]->LoadData( fileName, "First Observable Thresholds", j );
        result &= updateFlagDisplay[ 0 ][ index ][ j ]->LoadData( fileName, "First Observable Thresholds Update Flag", j );

        result &=  thresholdDisplay[ 1 ][ index ][ j ]->LoadData( fileName, "Second Observable Thresholds", j );
        result &= updateFlagDisplay[ 1 ][ index ][ j ]->LoadData( fileName, "Second Observable Thresholds Update Flag", j );
    }

    return result;
}



void rccmThresholds::csscUpdate( int newCsscIndex )
{
    for ( int i = 0 ; i < 2 ; i++ )
    {
        float min = thresholdDisplay[ i ][ 0 ][ 0 ]->Min( newCsscIndex );
        float max = thresholdDisplay[ i ][ 0 ][ 0 ]->Max( newCsscIndex );

        // find min / max for threshold data
        for ( int y = 0 ; y < 5 ; y++ )
        {
            for ( int x = 0 ; x < 3 ; x++ )
            {
                float localMin = thresholdDisplay[ i ][ y ][ x ]->Min( newCsscIndex );
                float localMax = thresholdDisplay[ i ][ y ][ x ]->Max( newCsscIndex );

                if ( localMin < min ) min = localMin;
                if ( localMax > max ) max = localMax;
            }
        }

        // set min / max, update thumbnails
        for ( int y = 0 ; y < 5 ; y++ )
        {
            for ( int x = 0 ; x < 3 ; x++ )
            {
                thresholdDisplay[ i ][ y ][ x ]->setMinMax( min, max );
                thresholdDisplay[ i ][ y ][ x ]->csscUpdate( newCsscIndex );
                updateFlagDisplay[ i ][ y ][ x ]->csscUpdate( newCsscIndex );
            }
        }

        spectrum[ i ]->setMinMax( min, max );
    }
}



void rccmThresholds::coordinateDisplayUpdate( int x, int y )
{
    coordinates->setText( QString( "( %1, %2 )" ).arg( x + 1 ).arg( y + 1 ) );  
}


