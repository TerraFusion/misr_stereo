#include "qintervalslider.h"

#include <qpixmap.h>
#include <qpainter.h>
#include <qvector.h>
#include <QMouseEvent>

QIntervalSlider::QIntervalSlider( QWidget * parent )
:
QWidget( parent ),
movingValue( -1 ),
orientation( Qt::Horizontal ),
values( 0, 1 ),
limits( 0, 1 )
{
	Init();
}



QIntervalSlider::QIntervalSlider( Qt::Orientation theOrientation, QWidget * parent )
:
QWidget( parent ),
movingValue( -1 ),
orientation( theOrientation ),
values( 0, 1 ),
limits( 0, 1 )
{
	Init();
}



QIntervalSlider::QIntervalSlider( double minLimit, double maxLimit, Qt::Orientation theOrientation, QWidget * parent )
:
QWidget( parent ),
movingValue( -1 ),
orientation( theOrientation ),
values( minLimit, maxLimit ),
limits( minLimit, maxLimit )
{
	Init();
}



QIntervalSlider::QIntervalSlider( const orderedInterval<double> & theLimits, Qt::Orientation theOrientation, QWidget * parent )
:
QWidget( parent ),
movingValue( -1 ),
orientation( theOrientation ),
values( theLimits ),
limits( theLimits )
{
	Init();
}



QIntervalSlider::~QIntervalSlider()
{

}



const orderedInterval<double> & QIntervalSlider::Values() const
{
	return values;
}



const orderedInterval<double> & QIntervalSlider::Limits() const
{
	return limits;
}



void QIntervalSlider::setValues( double minValue, double maxValue )
{
	setValues( orderedInterval<double>( minValue, maxValue ) );
}



void QIntervalSlider::setValues( const orderedInterval<double> & theValues )
{
	values = theValues;

	emit ValueMinChanged( values.min );
	emit ValueMaxChanged( values.max );
	emit ValuesChanged( values.min, values.max );

	update();
}



void QIntervalSlider::setValuesFromLimits()
{
	setValues( Limits() );
}



void QIntervalSlider::setLimits( double minLimit, double maxLimit )
{
	setLimits( orderedInterval<double>( minLimit, maxLimit ) );
}



void QIntervalSlider::setLimits( const orderedInterval<double> & theLimits )
{
	limits = theLimits;
	updateMappers();

	emit LimitMinChanged( limits.min );
	emit LimitMaxChanged( limits.max );
	emit LimitsChanged( limits.min, limits.max );

	setValuesFromLimits();
}



void QIntervalSlider::setLimitsFromValues()
{
	setLimits( Values() );
}



void QIntervalSlider::Init()
{
	if ( orientation == Qt::Horizontal )
	{
		setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ) );
		setMinimumWidth( 64 );
		setFixedHeight( 8 );
	}
	else if ( orientation == Qt::Vertical )
	{
		setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );
		setFixedWidth( 8 );
		setMinimumHeight( 64 );
	}
}



void QIntervalSlider::updateMappers()
{
	if ( orientation == Qt::Horizontal )
	{
		mapValueToWidget = ::mapper<double,int>( Limits(), createInterval( 6, width() - 7 ) );
	}
	else if ( orientation == Qt::Vertical )
	{
		mapValueToWidget = ::mapper<double,int>( Limits(), createInterval( height() - 7, 6 ) );
	}

	mapWidgetToValue = mapValueToWidget.inverse();	
}



void QIntervalSlider::mousePressEvent( QMouseEvent * e )
{
	if ( orientation == Qt::Horizontal )
	{
		{
			int widgetValue = mapValueToWidget( values.min );
			orderedInterval<int> handle( widgetValue - 6, widgetValue + 6 );
			if ( handle.contains( e->x() ) )
			{
				moveLimits = clamper<int>( 6, mapValueToWidget( values.max ) - 1 );
				movingValue = 0;
			}
		}

		{
			int widgetValue = mapValueToWidget( values.max );
			orderedInterval<int> handle( widgetValue - 6, widgetValue + 6 );
			if ( handle.contains( e->x() ) )
			{
				moveLimits = clamper<int>( mapValueToWidget( values.min ) + 1, width() - 7 );
				movingValue = 1;
			}
		}
	}
	else if ( orientation == Qt::Vertical )
	{
		{
			int widgetValue = mapValueToWidget( values.min );
			orderedInterval<int> handle( widgetValue - 6, widgetValue + 6 );
			if ( handle.contains( e->y() ) )
			{
				moveLimits = clamper<int>( mapValueToWidget( values.max ) + 1, height() - 7 );
				movingValue = 0;
			}
		}

		{
			int widgetValue = mapValueToWidget( values.max );
			orderedInterval<int> handle( widgetValue - 6, widgetValue + 6 );
			if ( handle.contains( e->y() ) )
			{
				moveLimits = clamper<int>( 6, mapValueToWidget( values.min ) - 1 );
				movingValue = 1;
			}
		}
	}
}



void QIntervalSlider::mouseMoveEvent( QMouseEvent * e )
{
	if ( orientation == Qt::Horizontal )
	{
		if ( movingValue == 0 )
		{
			values.min = mapWidgetToValue( moveLimits( e->x() ) );
			emit ValueMinChanging( values.min );
			emit ValuesChanging( values.min, values.max );
			update();
		}
		else if ( movingValue == 1 )
		{
			values.max = mapWidgetToValue( moveLimits( e->x() ) );
			emit ValueMaxChanging( values.max );
			emit ValuesChanging( values.min, values.max );
			update();
		}
	}
	else if ( orientation == Qt::Vertical )
	{
		if ( movingValue == 0 )
		{
			values.min = mapWidgetToValue( moveLimits( e->y() ) );
			emit ValueMinChanging( values.min );
			emit ValuesChanging( values.min, values.max );
			update();
		}
		else if ( movingValue == 1 )
		{
			values.max = mapWidgetToValue( moveLimits( e->y() ) );
			emit ValueMaxChanging( values.max );
			emit ValuesChanging( values.min, values.max );
			update();
		}
	}
}



void QIntervalSlider::mouseReleaseEvent( QMouseEvent * e )
{
	mouseMoveEvent( e );

	if ( movingValue != -1 )
	{
		emit ValueMinChanged( values.min );
		emit ValueMaxChanged( values.max );
		emit ValuesChanged( values.min, values.max );
		movingValue = -1;
	}
}



void QIntervalSlider::resizeEvent( QResizeEvent * )
{
	updateMappers();
}



void QIntervalSlider::paintEvent( QPaintEvent * )
{
	QPainter paint( this );
	{
		paint.setPen( Qt::black );

		if ( orientation == Qt::Horizontal )
		{
			// draw line
			paint.drawLine( 6, 0, width() - 7, 0 );

			// draw tick marks

			// draw min handle
			{
				QVector<QPoint> Triangle( 3 );
				Triangle[ 0 ] = QPoint( mapValueToWidget( values.min ), 1 );
				Triangle[ 1 ] = Triangle[ 0 ] + QPoint( 6, 6 );
				Triangle[ 2 ] = Triangle[ 0 ] + QPoint( -6, 6 );
				paint.setBrush( ( movingValue == 0 ) ? Qt::yellow : palette().background() );
				paint.drawPolygon( Triangle );
			}

			// draw max handle
			{
				QVector<QPoint> Triangle( 3 );
				Triangle[ 0 ] = QPoint( mapValueToWidget( values.max ), 1 );
				Triangle[ 1 ] = Triangle[ 0 ] + QPoint( 6, 6 );
				Triangle[ 2 ] = Triangle[ 0 ] + QPoint( -6, 6 );
				paint.setBrush( ( movingValue == 1 ) ? Qt::yellow : palette().background() );
				paint.drawPolygon( Triangle );
			}
		}
		else if ( orientation == Qt::Vertical )
		{
			// draw line
			paint.drawLine( width() - 1, 6, width() - 1, height() - 7 );

			// draw tick marks

			// draw min handle
			{
				QVector<QPoint> Triangle( 3 );
				Triangle[ 0 ] = QPoint( width() - 2, mapValueToWidget( values.min ) );
				Triangle[ 1 ] = Triangle[ 0 ] + QPoint( -6, 6 );
				Triangle[ 2 ] = Triangle[ 0 ] + QPoint( -6, -6 );
				paint.setBrush( ( movingValue == 0 ) ? Qt::yellow : palette().background() );
				paint.drawPolygon( Triangle );
			}

			// draw max handle
			{
				QVector<QPoint> Triangle( 3 );
				Triangle[ 0 ] = QPoint( width() - 2, mapValueToWidget( values.max ) );
				Triangle[ 1 ] = Triangle[ 0 ] + QPoint( -6, 6 );
				Triangle[ 2 ] = Triangle[ 0 ] + QPoint( -6, -6 );
				paint.setBrush( ( movingValue == 1 ) ? Qt::yellow : palette().background() );
				paint.drawPolygon( Triangle );
			}
		}
	}
}


