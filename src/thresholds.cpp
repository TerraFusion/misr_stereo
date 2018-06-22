
#include <qlayout.h>
#include <qlabel.h>
#include <qpixmap.h>

#include "thresholds.h"
#include "dataChar.h"
#include "dataFloat.h"
#include "SpectrumLabel.h"
#include "qsizepolicy.h"

#include "qutility.h"



Thresholds::Thresholds( const QString & title, QWidget *parent )
:
QGroupBox( title, parent ),
gridLayout( 0 ),
spectrum( 0 )
{	
    // Grid Layout
	gridLayout = new QGridLayout( this );
	gridLayout->setMargin( 16 );
	gridLayout->setSpacing( 8 );

    // Labels
	QLabel *label( 0 );

	gridLayout->addWidget( label = new QLabel( "T1", this ), 1, 0 );
	label->setAlignment( Qt::AlignCenter );

	QPalette pal( label->palette() );
	pal.setColor( QPalette::Foreground, Qt::darkCyan );
	label->setPalette( pal );

	gridLayout->addWidget( label = new QLabel( "T2", this ), 2, 0 );
	label->setAlignment( Qt::AlignCenter );
	pal.setColor( QPalette::Foreground, Qt::darkMagenta );
	label->setPalette( pal );

	gridLayout->addWidget( label = new QLabel( "T3", this ), 3, 0 );
	label->setAlignment( Qt::AlignCenter );
	pal.setColor( QPalette::Foreground, Qt::darkYellow );
	label->setPalette( pal );

	gridLayout->addWidget( label = new QLabel( "Thresholds", this ), 0, 1 );
	label->setAlignment( Qt::AlignCenter );

	gridLayout->addWidget( label = new QLabel( "Flags", this ), 0, 2 );
	label->setAlignment( Qt::AlignCenter );


    // Threshold Displays
	for ( int i = 0 ; i < 3 ; i++ )
	{
		thresholdDisplay[ i ] = new dataFloat( monoGradientMap, this );
		gridLayout->addWidget( thresholdDisplay[ i ], i + 1, 1 );

		connect( thresholdDisplay[ i ], SIGNAL( PixelClick( int, int ) ), this, SIGNAL( PixelClick( int, int ) ) );

		connect( this, SIGNAL( dataDisplayUpdate( int, int ) ), thresholdDisplay[ i ], SLOT( dataDisplayUpdate( int, int ) ) );

		connect( thresholdDisplay[ i ], SIGNAL( dataChanged( float, int ) ), this, SIGNAL( dataChanged( float, int ) ) );
	}


    // Update Flag Displays
	for ( int i = 0 ; i < 3 ; i++ )
	{
		updateFlagDisplay[ i ] = new dataChar( eightColorMap, this );
		gridLayout->addWidget( updateFlagDisplay[ i ], i + 1, 2 );

		connect( updateFlagDisplay[ i ], SIGNAL( PixelClick( int, int ) ), this, SIGNAL( PixelClick( int, int ) ) );

		connect( this, SIGNAL( dataDisplayUpdate( int, int ) ), updateFlagDisplay[ i ], SLOT( dataDisplayUpdate( int, int ) ) );
	}


    // Color Scale
	spectrum = new SpectrumLabel( monoGradientMap, this );
	gridLayout->addWidget( spectrum, 4, 0, 1, 3 );
}



Thresholds::~Thresholds()
{

}



bool Thresholds::LoadData( QString fileName, QString fieldName )
{
	bool result = true;

	result &= thresholdDisplay[ 0 ]->LoadData( fileName, fieldName + " Thresholds", 0 );
	result &= thresholdDisplay[ 1 ]->LoadData( fileName, fieldName + " Thresholds", 1 );
	result &= thresholdDisplay[ 2 ]->LoadData( fileName, fieldName + " Thresholds", 2 );

	float combinedMin = thresholdDisplay[ 0 ]->Min();
	if ( thresholdDisplay[ 1 ]->Min() < combinedMin ) combinedMin = thresholdDisplay[ 1 ]->Min();
	if ( thresholdDisplay[ 2 ]->Min() < combinedMin ) combinedMin = thresholdDisplay[ 2 ]->Min();

	float combinedMax = thresholdDisplay[ 0 ]->Max();
	if ( thresholdDisplay[ 1 ]->Max() > combinedMax ) combinedMax = thresholdDisplay[ 1 ]->Max();
	if ( thresholdDisplay[ 2 ]->Max() > combinedMax ) combinedMax = thresholdDisplay[ 2 ]->Max();

	thresholdDisplay[ 0 ]->setMinMax( combinedMin, combinedMax );
	thresholdDisplay[ 1 ]->setMinMax( combinedMin, combinedMax );
	thresholdDisplay[ 2 ]->setMinMax( combinedMin, combinedMax );

	spectrum->setMinMax( combinedMin, combinedMax );

	result &= updateFlagDisplay[ 0 ]->LoadData( fileName, fieldName + " Thresholds Update Flag", 0 );
	result &= updateFlagDisplay[ 1 ]->LoadData( fileName, fieldName + " Thresholds Update Flag", 1 );
	result &= updateFlagDisplay[ 2 ]->LoadData( fileName, fieldName + " Thresholds Update Flag", 2 );

	if ( result )
	{
		setTitle( fieldName );
	}
	else
	{
		ClearData();
	}

	return result;
}



void Thresholds::csscUpdate( int newCsscIndex )
{
	float min = thresholdDisplay[ 0 ]->Min( newCsscIndex );
	float max = thresholdDisplay[ 0 ]->Max( newCsscIndex );

    // find min / max for threshold data
	for ( int x = 0 ; x < 3 ; x++ )
	{
		float localMin = thresholdDisplay[ x ]->Min( newCsscIndex );
		float localMax = thresholdDisplay[ x ]->Max( newCsscIndex );

		if ( localMin < min ) min = localMin;
		if ( localMax > max ) max = localMax;
	}

    // set min / max, update thumbnails
	for ( int x = 0 ; x < 3 ; x++ )
	{
		thresholdDisplay[ x ]->setMinMax( min, max );
		thresholdDisplay[ x ]->csscUpdate( newCsscIndex );
		updateFlagDisplay[ x ]->csscUpdate( newCsscIndex );
	}

	spectrum->setMinMax( min, max );
}



void Thresholds::ClearData()
{
	setTitle( "No Data" );

	for ( int x = 0 ; x < 3 ; x++ )
	{
		thresholdDisplay[ x ]->ClearData();
		updateFlagDisplay[ x ]->ClearData();
	}
}



