#ifndef DATAFLOAT_H_INCLUDED
#define DATAFLOAT_H_INCLUDED

class dataImage;

#include <qlabel.h>

#include "colorMap.h"

class dataFloat : public QLabel
{
	Q_OBJECT

public:

	dataFloat( const colorMap & theColorMap, QWidget *parent = 0 );

	virtual ~dataFloat();

	int CsscIndex() const;

	void setMinMax( float theMin, float theMax );
	float Min() const;
	float Max() const;

	float Min( const int newCsscIndex ) const;
	float Max( const int newCsscIndex ) const;

	float GlobalMin() const;
	float GlobalMax() const;

	bool LoadData( QString fileName, QString fieldName, int thresholdIndex );

signals:

	void PixelHover( int x, int y );

	void PixelClick( int x, int y );

	void dataChanged( float newData );

	void dataChanged( float newData, int index );

public slots:

	void dataDisplayUpdate( int x, int y );

	void csscUpdate( int newCsscIndex );

	void ClearData();

protected:

	void CalcGlobalRange();

	int csscIndex;

	int thresholdIndex;

	float min;
	float max;   

	float globalMin;
	float globalMax;

	QLabel *dataDisplay;

	dataImage *imageDisplay;    

	float data[ 1581 ][ 12 ][ 10 ];
};

#endif // DATAFLOAT_H_INCLUDED
