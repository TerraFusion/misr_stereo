#ifndef THRESHOLDS_H_INCLUDED
#define THRESHOLDS_H_INCLUDED

class QGridLayout;
class SpectrumLabel;
class QLabel;
class dataFloat;
class dataChar;
class QPixmap;

#include <qgroupbox.h>



class Thresholds : public QGroupBox
{
	Q_OBJECT

public:

	Thresholds( const QString & title, QWidget *parent = 0 );

	virtual ~Thresholds();

	bool LoadData( QString fileName, QString fieldName );

signals:

	void PixelClick( int x, int y );

	void dataDisplayUpdate( int x, int y );

	void dataChanged( float newData, int index );

public slots:   

	void csscUpdate( int newCsscIndex );

	void ClearData();

private:

	QGridLayout *gridLayout;
	SpectrumLabel *spectrum;

	dataFloat *thresholdDisplay[ 3 ];
	dataChar  *updateFlagDisplay[ 3 ];
};

#endif
