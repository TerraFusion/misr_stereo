#ifndef HISTOGRAMLABEL_H
#define HISTOGRAMLABEL_H

#include <vector>

#include <qwidget.h>

class HistogramLabel : public QWidget
{
	Q_OBJECT

public:

	HistogramLabel( QWidget *parent = 0 );

	virtual ~HistogramLabel();

	void Clear();
	void InputData( const std::vector<unsigned int> & theData );

	double DomainMin() const;
	double DomainMax() const;
	double DomainSize() const;

	double RangeMin() const;
	double RangeMax() const;
	double RangeSize() const;

public slots:

	void setDomain();
	void setDomain( double theMin, double theMax );
	void setDomainMin( double theMin );
	void setDomainMax( double theMax );

	void setRange();
	void setRange( double theMin, double theMax );
	void setRangeMin( double theMin );
	void setRangeMax( double theMax );

	void thresholdUpdate( float newData, int index );

protected:

	void mousePressEvent( QMouseEvent *e );
	void resizeEvent( QResizeEvent *e );
	void paintEvent( QPaintEvent *e );

	static const int numThresholds = 3;
	float thresholds[ numThresholds ];

		class point { public: point( double x_, double y_ ) : x( x_ ), y( y_ ) {} double x, y; };
		class pointLessX { public: bool operator () ( const point & a, const point & b ) const { return ( a.x < b.x ); } };
		class pointLessY { public: bool operator () ( const point & a, const point & b ) const { return ( a.y < b.y ); } };
	std::vector<point> data;

	double domainMin;
	double domainMax;

	double rangeMin;
	double rangeMax;

	QRect graphRect;

	int xDividers;
	int yDividers;

	bool selected;
	double selectedX;
	double selectedY;

	// set graph display margins
	static const int leftMargin = 64;
	static const int topMargin = 16;
	static const int rightMargin = 64;
	static const int bottomMargin = 16;
};

#endif
