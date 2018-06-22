
#include <math.h>

#include <qstring.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qsizepolicy.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qprogressdialog.h>
#include <qapplication.h>
#include <qslider.h>

#include "qutility.h"
#include "dataImage.h"
#include "SpectrumLabel.h"
#include "histogram.h"
#include "HistogramLabel.h"

#include "qintervalslider.h"


Histogram::Histogram( const QString & title, QWidget *parent )
:
QGroupBox( title, parent ),
csscIndex( 0 ),
xCoord( 0 ),
yCoord( 0 ),
combineBins( 1 ),
countsLocation( 0 ),
countsLabel( 0 ),
oobLabel( 0 ),
countsSpectrum( 0 ),
histogramLocation( 0 ),
domainChooser( 0 ),
rangeChooser( 0 )
{
	// bin info map entries
	binInfoSource[ "First Observable Histograms" ] = binInfo( "Histogram Lower Bounds", "Histogram Bin Widths", 0 );
	binInfoSource[ "Second Observable Histograms" ] = binInfo( "Histogram Lower Bounds", "Histogram Bin Widths", 1 );
	binInfoSource[ "Blue Band BRF Histograms" ] = binInfo( "BRF Histogram Lower Bounds", "BRF Histogram Bin Widths", 0 );
	binInfoSource[ "Green Band BRF Histograms" ] = binInfo( "BRF Histogram Lower Bounds", "BRF Histogram Bin Widths", 1 );
	binInfoSource[ "Red Band BRF Histograms" ] = binInfo( "BRF Histogram Lower Bounds", "BRF Histogram Bin Widths", 2 );
	binInfoSource[ "NearIR Band BRF Histograms" ] = binInfo( "BRF Histogram Lower Bounds", "BRF Histogram Bin Widths", 3 );	
	binInfoSource[ "NDVI Histograms" ] = binInfo( "NDVI Histogram Lower Bounds", "NDVI Histogram Bin Widths", 0 );

	ClearData();

	QGridLayout * mainLayout = new QGridLayout( this );
	mainLayout->setMargin( 16 );
	mainLayout->setSpacing( 8 );

	QVBoxLayout * pickerLayout = new QVBoxLayout;

	// create histogram picker
	countsLocation = new dataImage( monoGradientMap, this );
	pickerLayout->addWidget( countsLocation ); 
	countsLocation->setMinimumSize( countsLocation->maximumSize() );
	connect( countsLocation, SIGNAL( PixelClick( int, int ) ), this, SIGNAL( PixelClick( int, int ) ) );

	// create histogram coordinate display
	countsLabel = new QLabel( this );
	pickerLayout->addWidget( countsLabel );
	UpdateCountsLabel();

	oobLabel = new QLabel( "", this );
	pickerLayout->addWidget( oobLabel );
	UpdateoobLabel();

	QHBoxLayout * binCombineDisplayLayout = new QHBoxLayout();
	pickerLayout->addLayout( binCombineDisplayLayout );
	binCombineDisplayLayout->addWidget( new QLabel( "Bins Combined:", this ) );
	QLabel * binCombineDisplay = new QLabel( "1", this );
	binCombineDisplayLayout->addWidget( binCombineDisplay );

	mainLayout->addLayout( pickerLayout, 0, 0 );

	// create range slider
	rangeChooser = new QIntervalSlider( Qt::Vertical, this );
	mainLayout->addWidget( rangeChooser, 0, 1 );

	// create histogram display
	histogramLocation = new HistogramLabel( this );
	histogramLocation->setMinimumSize( 384, 256 );
	histogramLocation->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
	mainLayout->addWidget( histogramLocation, 0, 2 );
	connect( this, SIGNAL( thresholdUpdate( float, int ) ), histogramLocation, SLOT( thresholdUpdate( float, int ) ) );

	// create bin combination slider
	QSlider * binCombineSlider = new QSlider( Qt::Horizontal, this );
	binCombineSlider->setMinimum( 1 );
	binCombineSlider->setMaximum( 32 );
	binCombineSlider->setPageStep( 4 );
	binCombineSlider->setValue( 1 );
	binCombineSlider->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
	mainLayout->addWidget( binCombineSlider, 1, 0 );

	// create zoom button
	QPushButton * zoomButton = new QPushButton( "+", this );
	zoomButton->setFixedSize( 16, 16 );
	mainLayout->addWidget( zoomButton, 1, 1 );

	// create domain slider
	domainChooser = new QIntervalSlider( Qt::Horizontal, this );
	mainLayout->addWidget( domainChooser, 1, 2 );

	// create color scale display
	countsSpectrum = new SpectrumLabel( monoGradientMap, this );  
	countsSpectrum->setScaledContents( true );
	mainLayout->addWidget( countsSpectrum, 2, 0, 1, 3 );

	// zoom to full domain / range on click
	connect( zoomButton, SIGNAL( clicked() ), domainChooser, SLOT( setValuesFromLimits() ) );
	connect( zoomButton, SIGNAL( clicked() ), rangeChooser, SLOT( setValuesFromLimits() ) );

	// update domain on slider change
	connect( domainChooser, SIGNAL( ValuesChanging( double, double ) ), histogramLocation, SLOT( setDomain( double, double ) ) );
	connect( domainChooser, SIGNAL( ValuesChanged( double, double ) ), histogramLocation, SLOT( setDomain( double, double ) ) );	

	// update range on slider change
	connect( rangeChooser, SIGNAL( ValuesChanging( double, double ) ), histogramLocation, SLOT( setRange( double, double ) ) );
	connect( rangeChooser, SIGNAL( ValuesChanged( double, double ) ), histogramLocation, SLOT( setRange( double, double ) ) );

	// set combine bins on slider change
	connect( binCombineSlider, SIGNAL( valueChanged( int ) ), this, SLOT( setCombineBins( int ) ) );
	connect( binCombineSlider, SIGNAL( valueChanged( int ) ), binCombineDisplay, SLOT( setNum( int ) ) );
}



Histogram::~Histogram()
{

}



int Histogram::NumBins() const
{
	return int( rawData.size() );
}



QStringList Histogram::GetDataList( QString fileName )
{
	QStringList result;

	// open data file ( read only )
	int32 sdId = SDstart( fileName.toAscii(), DFACC_READ );

	if ( sdId != FAIL )
	{
		// get number of data sets in the file
		int32 numDataSets = 0, numAttributes = 0;
		if ( SDfileinfo( sdId, & numDataSets, & numAttributes ) != FAIL )
		{
			for ( int i = 0 ; i < numDataSets ; ++i )
			{
				// convert the index to a data set id
				int32 dataSetId = SDselect( sdId, i );

				if ( dataSetId != FAIL )
				{
					// read the data set info
					char dataSetName[64];
					int32 dataSetRank = 0;
					int32 dataSetDims[ 16 ];
					int32 dataSetType;
					int32 dataSetAttributes;
					int32 gotInfo = SDgetinfo( dataSetId, dataSetName, & dataSetRank, dataSetDims, & dataSetType, & dataSetAttributes );

					if ( gotInfo != FAIL )
					{
						// add the name to the list
						result.push_back( dataSetName );
					}

					// close data set
					SDendaccess( dataSetId );
				}
			}
		}

		// close data file
		SDend( sdId );
	}

	return result;
}



QStringList Histogram::GetHistogramList( QString fileName )
{
	// return only the results containing "Histograms"
	return GetDataList( fileName ).filter( "Histograms" );
}



bool Histogram::LoadData( QString fileName, QString dataName )
{
	bool retVal = true;

	int32 SDid = SDstart( fileName.toAscii(), DFACC_READ ); 

	if ( SDid != FAIL )
	{
		int32 dataIndex = SDnametoindex( SDid, dataName.toAscii() );

		if ( dataIndex != FAIL )
		{
			int32 setId = SDselect( SDid, dataIndex );

			if ( setId != FAIL )
			{
				// get number of bins
				int32 dataType, numAttrs, numBins = 0, binDimId = SDgetdimid( setId, 0 );
				retVal &= ( binDimId != FAIL ) && ( SDdiminfo( binDimId, NULL, & numBins, & dataType, & numAttrs ) != FAIL );

				// resize data array to correct number of bins
				rawData.resize( numBins );

				// set bounds to read all data of one bin
				int32 start[ 4 ] = { 0, 0, 0, 0 };
				int32 edge[ 4 ] = { 1, 1581, 12, 10 };

				// display progress to user
				QProgressDialog progress( QString( "Loading %1 (%2 bins)" ).arg( dataName ).arg( numBins ),
					"Cancel", 0, numBins - 1, this );
				progress.setMinimumDuration( 2 );

				// load data one bin at a time
				for ( start[ 0 ] = 0 ; start[ 0 ] < numBins ; start[ 0 ]++ )
				{
					progress.setValue( ( int ) start[ 0 ] );
					qApp->processEvents();

					retVal &= !progress.wasCanceled();
					if ( retVal == false ) break;

					retVal &= ( SDreaddata( setId, start, NULL, edge, rawData[ start[ 0 ] ].data ) != FAIL );
				}

				SDendaccess( setId );
			}
		}


        // read bin offsets
		dataIndex = SDnametoindex( SDid, binInfoSource[ dataName ].binOffsetName.c_str() );

		if ( dataIndex != FAIL )
		{
			int32 setId = SDselect( SDid, dataIndex );

			if ( setId != FAIL )
			{
				int32 start[ 4 ] = { binInfoSource[ dataName ].dimIndex, 0, 0, 0 };
				int32 edge[ 4 ] = { 1, 1581, 1, 1 };

				retVal &= ( SDreaddata( setId, start, NULL, edge, binOffset ) != FAIL );
				SDendaccess( setId );
			}			
		}


        // read bin widths
		dataIndex = SDnametoindex( SDid, binInfoSource[ dataName ].binWidthName.c_str() );

		if ( dataIndex != FAIL )
		{
			int32 setId = SDselect( SDid, dataIndex );

			if ( setId != FAIL )
			{
				int32 start[ 4 ] = { binInfoSource[ dataName ].dimIndex, 0, 0, 0 };
				int32 edge[ 4 ] = { 1, 1581, 1, 1 };

				retVal &= ( SDreaddata( setId, start, NULL, edge, binWidth ) != FAIL );
				SDendaccess( setId );
			}			
		}


		// read overflows
		QString underflowName( dataName );
		underflowName.replace( "Histograms", "Underflows" );

		dataIndex = SDnametoindex( SDid, underflowName.toAscii() );

		if ( dataIndex != FAIL )
		{
			int32 setId = SDselect( SDid, dataIndex );

			if ( setId != FAIL )
			{
				int32 start[ 3 ] = { 0, 0, 0 };
				int32 edge[ 3 ] = { 1581, 12, 10 };

				retVal &= ( SDreaddata( setId, start, NULL, edge, underflows ) != FAIL );
				SDendaccess( setId );
			}			
		}


		// read overflows
		QString overflowName( dataName );
		overflowName.replace( "Histograms", "Overflows" );

		dataIndex = SDnametoindex( SDid, overflowName.toAscii() );

		if ( dataIndex != FAIL )
		{
			int32 setId = SDselect( SDid, dataIndex );

			if ( setId != FAIL )
			{
				int32 start[ 3 ] = { 0, 0, 0 };
				int32 edge[ 3 ] = { 1581, 12, 10 };

				retVal &= ( SDreaddata( setId, start, NULL, edge, overflows ) != FAIL );
				SDendaccess( setId );
			}			
		}


		SDend( SDid );
	}

	if ( retVal )
	{
		setTitle( dataName );
	}
	else
	{
		ClearData();
	}

	return retVal;
}



int Histogram::CombineBins() const
{
	return combineBins;
}



void Histogram::dataDisplayUpdate( int x, int y )
{
	if ( ( x >= 0 ) && ( x < 12 ) )
	{
		if ( ( y >= 0 ) && ( y < 10 ) )
		{
			xCoord = x;
			yCoord = y;

			UpdateoobLabel();
			UpdateCountsLabel();
			UpdateHistogram();
		}
	}
}



void Histogram::csscUpdate( int newCsscIndex )
{
	csscIndex = newCsscIndex;

    // Calculate the total histogram counts
	for ( int y = 0 ; y < 10 ; y++ )
	{
		for ( int x = 0 ; x < 12 ; x++ )
		{
			totalData[ x ][ y ] = 0.0;

			for ( int bin = 0 ; bin < NumBins() ; bin++ )
			{
				totalData[ x ][ y ] += ( double ) data( csscIndex, x, y, bin );
			}
		}
	}

    // Find the min/max of the data
	double min = totalData[ 0 ][ 0 ];
	double max = totalData[ 0 ][ 0 ];

	for ( int y = 0 ; y < 10 ; y++ )
	{
		for ( int x = 0 ; x < 12 ; x++ )
		{
			if ( totalData[ x ][ y ] < min ) min = totalData[ x ][ y ];
			if ( totalData[ x ][ y ] > max ) max = totalData[ x ][ y ];
		}
	}

    // update the color spectrum
	countsSpectrum->setMinMax( min, max );

    // update the image
	countsLocation->InputData( & totalData[ 0 ][ 0 ], min, max );
	dataDisplayUpdate( xCoord, yCoord );

    // Update the histogram
	UpdateHistogram();
}



void Histogram::ClearData( void )
{
	setTitle( "No Data" );

	for ( unsigned int binIndex = 0 ; binIndex < rawData.size() ; ++binIndex )
	{
		memset( & rawData[ binIndex ], 0, sizeof( rawData[ binIndex ] ) );
	}

	for ( int csscIndex = 0 ; csscIndex <= 1580 ; csscIndex++ )
	{
		binOffset[ csscIndex ] = 0;
		binWidth[ csscIndex ] = 1;
	}

	memset( overflows, 0, sizeof( overflows ) );
	memset( underflows, 0, sizeof( underflows ) );
	memset( totalData, 0, sizeof( totalData ) );
}



void Histogram::setCombineBins( int num )
{
	if ( ( num >= 0 ) && ( num <= NumBins() ) )
	{
		combineBins = num;
		UpdateHistogram();
	}
}



void Histogram::UpdateHistogram()
{
	if ( NumBins() > 0 )
	{
		std::vector<unsigned int> histogramData;
		histogramData.reserve( NumBins() / CombineBins() );

		int combineCounter = 0;
		for ( int bin = 0 ; bin < NumBins() ; ++bin )
		{
			if ( --combineCounter <= 0 )
			{
				histogramData.push_back( data( csscIndex, xCoord, yCoord, bin ) );
				combineCounter = CombineBins();
			}
			else
			{
				histogramData.back() += data( csscIndex, xCoord, yCoord, bin );
			}
		}

		domainChooser->setLimits( binOffset[ csscIndex ], binOffset[ csscIndex ] + binWidth[ csscIndex ] * CombineBins() * histogramData.size() );

		histogramLocation->InputData( histogramData );

		rangeChooser->setLimits( histogramLocation->RangeMin(), histogramLocation->RangeMax() );
	}
	else
	{
		domainChooser->setLimits( 0, 1 );
		histogramLocation->InputData( std::vector<unsigned int>() );
		rangeChooser->setLimits( 0, 1 );
	}
}



void Histogram::UpdateCountsLabel()
{	
	int relAzimuth[ 12 ] = { 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180 };
	double cosSZA[ 10 ] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };

	countsLabel->setText( QString( "x = %1, rel azimuth = %2\ny = %3, cos SZA = %4\ncounts = %5" )
			.arg( xCoord + 1 )
			.arg( ( ( xCoord >= 0 ) && ( xCoord < 12 ) ) ? relAzimuth[ xCoord ] : 0 )
			.arg( yCoord + 1 )
			.arg( ( ( yCoord >= 0 ) && ( yCoord < 12 ) ) ? cosSZA[ yCoord ] : 0, 0, 'f', 1 )
		.arg( CommaInt( totalData[ xCoord ][ yCoord ] ) ) );
}



void Histogram::UpdateoobLabel()
{
	double numUnderflows = double( underflows[ csscIndex ][ xCoord ][ yCoord ] );
	double numOverflows = double( overflows[ csscIndex ][ xCoord ][ yCoord ] );

	double total = totalData[ xCoord ][ yCoord ] + numUnderflows + numOverflows;
	double percentUnderflow = ( total > 0 ) ? ( 100.0 * numUnderflows / total ) : ( 0.0 );
	double percentOverflow = ( total > 0 ) ? ( 100.0 * numOverflows / total ) : ( 0.0 );

	oobLabel->setText( QString( "Underflows: %1 (%2%)\nOverflows: %3 (%4%)" )
		.arg( CommaInt( numUnderflows ) )
		.arg( percentUnderflow, 0, 'f', 2 )
		.arg( CommaInt( numOverflows ) )
		.arg( percentOverflow, 0, 'f', 2 ) );
}

