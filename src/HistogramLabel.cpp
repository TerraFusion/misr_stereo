
#include <algorithm>

#include <math.h>

#include <qevent.h>
#include <qpixmap.h>
#include <qpainter.h>

#include "HistogramLabel.h"
#include "qutility.h"
#include "utility.h"

const float kNoThreshold = 1e-8;

const int kNoSelection = -1;
const int kDotRadius = 5;

HistogramLabel::HistogramLabel( QWidget *parent )
:
QWidget( parent ),
data(),
domainMin( 0 ),
domainMax( 128 ),
rangeMin( 0 ),
rangeMax( 1 ),
graphRect(),
xDividers( 8 ),
yDividers( 8 ),
selected( false ),
selectedX( 0 ),
selectedY( 0 )
{
	memset( thresholds, 0, sizeof( thresholds ) );
}



HistogramLabel::~HistogramLabel()
{

}



void HistogramLabel::Clear()
{
	InputData( std::vector<unsigned int>( 2, 0 ) );
}



void HistogramLabel::InputData( const std::vector<unsigned int> & theData )
{
	::mapper<unsigned int,double> indexToValue( createInterval( 0U, (unsigned int)theData.size() ), createInterval( DomainMin(), DomainMax() ) );

	data.resize( theData.size(), point( 0, 0 ) );
	for ( unsigned int i = 0 ; i < theData.size() ; ++i )
	{
		data[ i ] = point( indexToValue( i ), theData[ i ] );
	}

	setRange();

	for ( int i = 0 ; i < numThresholds ; i++ )
	{
		thresholdUpdate( kNoThreshold, i );
	}

	selected = false;

	update();    
}



double HistogramLabel::DomainMin() const
{
	return domainMin;
}



double HistogramLabel::DomainMax() const
{
	return domainMax;
}



double HistogramLabel::DomainSize() const
{
	return ( DomainMax() - DomainMin() );
}



double HistogramLabel::RangeMin() const
{
	return rangeMin;
}



double HistogramLabel::RangeMax() const
{
	return rangeMax;
}



double HistogramLabel::RangeSize() const
{
	return ( RangeMax() - RangeMin() );
}



void HistogramLabel::setDomain()
{
	if ( data.empty() )
	{
		setDomain( 0, 1 );
	}
	else
	{
		double theDomainMin = (*std::min_element( data.begin(), data.end(), pointLessX() )).x;
		double theDomainMax = (*std::max_element( data.begin(), data.end(), pointLessX() )).x;

		if ( theDomainMin == theDomainMax )
		{
			theDomainMax += 1.0;
		}

		setDomain( theDomainMin, theDomainMax );
	}
}



void HistogramLabel::setDomain( double theMin, double theMax )
{
	domainMin = theMin;
	domainMax = theMax;

	update();
}



void HistogramLabel::setDomainMin( double theMin )
{
	setDomain( theMin, DomainMax() );
}



void HistogramLabel::setDomainMax( double theMax )
{
	setDomain( DomainMin(), theMax );
}



void HistogramLabel::setRange()
{
	if ( data.empty() )
	{
		setRange( 0, 1 );
	}
	else
	{
		double theRangeMin = (*std::min_element( data.begin(), data.end(), pointLessY() )).y;
		double theRangeMax = (*std::max_element( data.begin(), data.end(), pointLessY() )).y;

		if ( theRangeMin == theRangeMax )
		{
			theRangeMax += 1.0;
		}

		setRange( theRangeMin, theRangeMax );
	}
}



void HistogramLabel::setRange( double theMin, double theMax )
{
	rangeMin = theMin;
	rangeMax = theMax;

	update();
}



void HistogramLabel::setRangeMin( double theMin )
{
	setRange( theMin, RangeMax() );
}



void HistogramLabel::setRangeMax( double theMax )
{
	setRange( RangeMin(), theMax );
}



void HistogramLabel::thresholdUpdate( float newData, int index )
{
	if ( index >= 0 && index < numThresholds )
	{
		thresholds[ index ] = newData;

		update();
	}
}



void HistogramLabel::mousePressEvent( QMouseEvent *e )
{
	// linear transformation from widget to data coordinates
	::mapper<double,double> dataToWidgetX( createInterval( DomainMin(), DomainMax() ),
		createInterval( double( graphRect.left() ), double( graphRect.right() ) ) );

	::mapper<double,double> dataToWidgetY( createInterval( RangeMax(), RangeMin() ),
		createInterval( double( graphRect.top() ), double( graphRect.bottom() ) ) );

	// calculate distance from each point to cursor
	std::vector<double> distance( data.size() );
	for ( unsigned int i = 0 ; i < data.size() ; ++i )
	{
		distance[ i ] = square( dataToWidgetX( data[ i ].x ) - e->x() )
			+ square( dataToWidgetY( data[ i ].y ) - e->y() );
	}

	// find shortest distance
	std::vector<double>::iterator minDistance = std::min_element( distance.begin(), distance.end() );

	// select point with shortest distance
	if ( selected = ( minDistance != distance.end() ) )
	{
		int index = minDistance - distance.begin();
		selectedX = data[ index ].x;
		selectedY = data[ index ].y;
	}

	update();
}



void HistogramLabel::resizeEvent( QResizeEvent * )
{	
	graphRect = QRect( leftMargin,
			topMargin,
			width() - leftMargin - rightMargin,
			height() - topMargin - bottomMargin );
}



void HistogramLabel::paintEvent( QPaintEvent * )
{	
	const int tickSize = 4;

	// linear transformation from data to widget coordinates
	::mapper<double,double> dataToWidgetX( createInterval( DomainMin(), DomainMax() ),
		createInterval( double( graphRect.left() ), double( graphRect.right() ) ) );

	::mapper<double,double> dataToWidgetY( createInterval( RangeMin(), RangeMax() ),
		createInterval( double( graphRect.bottom() ), double( graphRect.top() ) ) );

    // begin painting
	QPainter paint( this );
	{
	    // draw graph checker background
		paint.save();
		{
			paint.setPen( Qt::NoPen );
			paint.setWindow( 0, 0, xDividers, yDividers );
			paint.setViewport( graphRect );

			const QColor darkGray = QColor( 180, 180, 180 );
			const QColor lightGray = QColor( 200, 200, 200 );

			for ( int y = 0 ; y < yDividers ; y++ )
			{
				for ( int x = 0 ; x < xDividers ; x++ )
				{
					paint.setBrush( ( ( x + y ) & 1 ) ? darkGray : lightGray );
					paint.drawRect( QRect( QPoint( x, y ), QSize( 1, 1 ) ) );
				}
			}
		}
		paint.restore();

		// draw graph border
		paint.setPen( Qt::black );
		paint.setBrush( Qt::NoBrush );
		paint.drawLine( graphRect.left(), graphRect.top(),        graphRect.left(),  graphRect.bottom() + 1 );
		paint.drawLine( graphRect.left(), graphRect.bottom() + 1, graphRect.right(), graphRect.bottom() + 1 );

		// set font for graph scales
		QFont theFont( paint.font() );
		theFont.setPointSize( 8 );
		paint.setFont( theFont );

		// draw x scale along bottom
		int yPos = graphRect.bottom() + 1;
		int textWidth = graphRect.width() / xDividers / 2;
		for ( int x = 0 ; x <= xDividers ; x++ )
		{
			int xPos = graphRect.left() + x * graphRect.width() / xDividers;
			paint.drawLine( xPos, yPos, xPos, yPos + tickSize );

			paint.drawText( QRect( QPoint( xPos - textWidth, yPos + tickSize + 1 ), QPoint( xPos + textWidth, height() ) ),
				Qt::AlignHCenter | Qt::AlignTop,
				QString::number( x * DomainSize() / xDividers + DomainMin(), 'f', 3 ) );
		}

    	// draw y scale along left
		int xPos = graphRect.left();
		for ( int y = 0 ; y <= yDividers ; y++ )
		{
			int yPos = graphRect.bottom() + 1 - y * graphRect.height() / yDividers;
			paint.drawLine( xPos - tickSize + 1, yPos, xPos, yPos );

			paint.drawText( QRect( QPoint( 0, yPos - 8 ), QPoint( xPos - tickSize - 3, yPos + 8 ) ),
				Qt::AlignRight | Qt::AlignVCenter,
				CommaInt( y * RangeSize() / yDividers + RangeMin() ) );
		}

    	// draw data
		if ( data.size() > 1 )
		{
			paint.save();
			{
				paint.setClipRect( graphRect );
				paint.setPen( Qt::darkBlue );

				QPointF prev( dataToWidgetX( data[ 0 ].x ), dataToWidgetY( data[ 0 ].y ) );
				for ( unsigned int i = 1 ; i < data.size() ; ++i )
				{
					QPointF current( dataToWidgetX( data[ i ].x ), dataToWidgetY( data[ i ].y ) );
					paint.drawLine( prev, current );
					prev = current;
				}
			}
			paint.restore();
		}

    	// draw thresholds
		QColor thresholdColors[ 3 ] = { Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow };
		for ( int i = 0 ; i < 3 ; i++ )
		{
			if ( thresholds[ i ] != kNoThreshold )
			{
				paint.setPen( thresholdColors[ i ] );

				int xPos = int( dataToWidgetX( thresholds[ i ] ) );

				if ( xPos < graphRect.left() )      xPos = graphRect.left();
				if ( xPos > graphRect.right() + 1 ) xPos = graphRect.right() + 1;

				paint.drawLine( xPos, graphRect.top(), xPos, graphRect.bottom() + 1 );
			}
		}

    	// show selected point with crosshairs
		if ( selected )
		{
			if ( createOrderedInterval( DomainMin(), DomainMax() ).contains( selectedX ) )
			{
				if ( createOrderedInterval( RangeMin(), RangeMax() ).contains( selectedY ) )
				{
					paint.setPen( Qt::darkRed );

					// get position in local coordinates
					QPoint pos = QPoint( int( dataToWidgetX( selectedX ) ), int( dataToWidgetY( selectedY ) ) );

					paint.drawEllipse( QRect( pos - QPoint( kDotRadius, kDotRadius ), QSize( 2 * kDotRadius + 1, 2 * kDotRadius + 1 ) ) );
					paint.drawLine( graphRect.left(), pos.y(), graphRect.right() + 1, pos.y() );
					paint.drawLine( pos.x(), graphRect.top(), pos.x(), graphRect.bottom() + 1 );

					paint.drawText( graphRect.right() + 1 + kDotRadius, pos.y() - 8, 62, 16,
						Qt::AlignLeft | Qt::AlignVCenter,
						CommaInt( selectedY ) );

					paint.drawText( pos.x() - 32, 0, 64, topMargin - kDotRadius,
						Qt::AlignHCenter | Qt::AlignBottom,
						QString::number( selectedX ) );
				}
			}
		}	
	}
}
