#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

class dataImage;
class QLabel;
class SpectrumLabel;
class HistogramLabel;
class QLineEdit;
class QPushButton;
class QIntervalSlider;

#include <map>
#include <vector>

#include <qgroupbox.h>

#include <hdf.h>
#include <mfhdf.h>

class binInfo
{
public:
	binInfo() : binOffsetName(), binWidthName(), dimIndex( 0 ) {};

	binInfo( std::string offsetName, std::string widthName, int theDimIndex )
	: binOffsetName( offsetName ), binWidthName( widthName ), dimIndex( theDimIndex ) {}

	std::string binOffsetName;
	std::string binWidthName;
	int dimIndex;
};

class Histogram : public QGroupBox
{
	Q_OBJECT

public:

	Histogram( const QString & title, QWidget *parent = 0 );

	virtual ~Histogram();

	int NumBins() const;

	QStringList GetDataList( QString fileName );
	QStringList GetHistogramList( QString fileName );

	bool LoadData( QString fileName, QString dataName );

	int CombineBins() const;

signals:

	void PixelClick( int x, int y );

	void thresholdUpdate( float newData, int index );

public slots:

	void dataDisplayUpdate( int x, int y );

	void csscUpdate( int newCsscIndex );

	void ClearData();

	void setCombineBins( int num );

private:

	unsigned int & data( int cssc, int x, int y, int bin )
	{
		return rawData[ bin ].data[ cssc ][ x ][ y ];
	}

	void UpdateHistogram();
	void UpdateCountsLabel();
	void UpdateoobLabel();

	int csscIndex;
	int xCoord;
	int yCoord;
	int combineBins;

	dataImage      * countsLocation;
	QLabel         * countsLabel;
	QLabel         * oobLabel;
	SpectrumLabel  * countsSpectrum;

	HistogramLabel * histogramLocation;

	QIntervalSlider * domainChooser;
	QIntervalSlider * rangeChooser;

	float binOffset[ 1581 ];
	float binWidth[ 1581 ];

	unsigned long overflows[ 1581 ][ 12 ][ 10 ];
	unsigned long underflows[ 1581 ][ 12 ][ 10 ];

	class binData { public: unsigned int data[ 1581 ][ 12 ][ 10 ]; };
	std::vector<binData> rawData;

	double totalData[ 12 ][ 10 ];

	std::map<QString,binInfo> binInfoSource;
};

#endif
