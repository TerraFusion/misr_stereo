#ifndef QINTERVALSLIDER_H_INCLUDED
#define QINTERVALSLIDER_H_INCLUDED

#include <qwidget.h>
#include "utility.h"

class QIntervalSlider : public QWidget
{
	Q_OBJECT

public:

	QIntervalSlider( QWidget * parent = 0 );

	QIntervalSlider( Qt::Orientation theOrientation, QWidget * parent = 0 );

	QIntervalSlider( double minLimit, double maxLimit, Qt::Orientation theOrientation, QWidget * parent = 0 );

	QIntervalSlider( const orderedInterval<double> & theLimits, Qt::Orientation theOrientation, QWidget * parent = 0 );

	virtual ~QIntervalSlider();

	const orderedInterval<double> & Values() const;
	const orderedInterval<double> & Limits() const;

signals:

	void ValueMinChanging( double value );
	void ValueMaxChanging( double value );
	void ValuesChanging( double minValue, double maxValue );

	void ValueMinChanged( double value );
	void ValueMaxChanged( double value );
	void ValuesChanged( double minValue, double maxValue );

	void LimitMinChanged( double limit );
	void LimitMaxChanged( double limit );
	void LimitsChanged( double minLimit, double maxLimit );

public slots:

	void setValues( double minValue, double maxValue );
	void setValues( const orderedInterval<double> & theValues );
	void setValuesFromLimits();

	void setLimits( double minLimit, double maxLimit );
	void setLimits( const orderedInterval<double> & theLimits );
	void setLimitsFromValues();

protected:

	void Init();
	void updateMappers();

	void mousePressEvent( QMouseEvent * e );
	void mouseMoveEvent( QMouseEvent * e );
	void mouseReleaseEvent( QMouseEvent * e );

	void resizeEvent( QResizeEvent * e );
	void paintEvent( QPaintEvent * e );

private:

	int movingValue;
	Qt::Orientation orientation;

	orderedInterval<double> values;
	orderedInterval<double> limits;

	::mapper<double,int> mapValueToWidget;
	::mapper<int,double> mapWidgetToValue;

	clamper<int> moveLimits;
};

#endif // QINTERVALSLIDER_H_INCLUDED
