#ifndef DATACHAR_H_INCLUDED
#define DATACHAR_H_INCLUDED

class dataImage;

#include <qlabel.h>

#include "colorMap.h"

class dataChar : public QLabel
{
	Q_OBJECT

public:

	dataChar( const colorMap & theColorMap, QWidget *parent = 0 );

	virtual ~dataChar();

	int CsscIndex() const;

	void setMinMax( char theMin, char theMax );
	char Min() const;
	char Max() const;

	char Min( const int newCsscIndex ) const;
	char Max( const int newCsscIndex ) const;

	char GlobalMin() const;
	char GlobalMax() const;

	bool LoadData( QString fileName, QString fieldName, int thresholdIndex );

signals:

	void PixelHover( int x, int y );

	void PixelClick( int x, int y );

public slots:

	void dataDisplayUpdate( int x, int y );

	void csscUpdate( int newCsscIndex );

	void ClearData();

protected:

	void CalcGlobalRange();

	int csscIndex;

	char min;
	char max;   

	char globalMin;
	char globalMax;

	QLabel *dataDisplay;

	dataImage *imageDisplay;    

	char data[ 1581 ][ 12 ][ 10 ];
};

#endif // DATACHAR_H_INCLUDED
