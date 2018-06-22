
#include <qpainter.h>
#include <qimage.h>
#include <math.h>
#include <qpixmap.h>
#include <qcolor.h>

#include "SpectrumLabel.h"
#include "qutility.h"



SpectrumLabel::SpectrumLabel( const colorMap & theColorMap, QWidget *parent )
:
QLabel( parent ),
min( 0.0f ),
max( 0.0f )
{
	setMinimumWidth( 256 );
	setFixedHeight( 24 );

	theSpectrumPixmap = new QPixmap();

	std::vector<QRgb> colorBuffer( colorMap::Size() );
	const unsigned int *sourceData = theColorMap.GetImage();

	for ( int index = 0 ; index < colorMap::Size() ; index++ )
	{
		colorBuffer[ index ] = getQRgba( sourceData[ index ] );
	}

	*theSpectrumPixmap = QPixmap::fromImage(
		QImage( ( unsigned char* ) &colorBuffer[0],
			colorBuffer.size(),
			1,
			QImage::Format_RGB32 ) );
}



SpectrumLabel::~SpectrumLabel()
{
	delete theSpectrumPixmap;
}



void SpectrumLabel::setMinMax( float theMin, float theMax )
{
	min = theMin;
	max = theMax;

	update();   
}



void SpectrumLabel::paintEvent( QPaintEvent *p  )
{
	QPainter paint( this );

	const int numDivisions = 8;

    // fill in the background
	paint.fillRect( QRect( QPoint( 0, 0 ), size() ), palette().background() );

    // draw the spectrum image
	const int barLeft  = width() * ( 2 * 0 + 1 ) / ( 2 * numDivisions );
	const int barRight = width() * ( 2 * numDivisions - 1 ) / ( 2 * numDivisions );
	paint.drawPixmap( QRect( QPoint( barLeft, 0 ), QPoint( barRight, 8 ) ), *theSpectrumPixmap );

    // draw text and lines in black
	paint.setPen( palette().foreground().color() );

    // make font smaller
	QFont theFont( paint.font() );
	theFont.setPointSize( 8 );
	paint.setFont( theFont );

    // calculate output precision
	int precision = 0;
	if( ( max - min ) > 0 ) precision = int( 3 - log10( max - min ) );
	if( precision < 0 ) precision = 0;

    // draw vertical lines and numbers
	for ( int i = 0 ; i < numDivisions ; i++ )
	{
		int xStart = width() * ( 2 * i + 0 ) / ( 2 * numDivisions );
		int xMid   = width() * ( 2 * i + 1 ) / ( 2 * numDivisions );
		int xEnd   = width() * ( 2 * i + 2 ) / ( 2 * numDivisions );

		paint.drawLine( xMid, height() - 13, xMid, height() - 20 );

		paint.drawText( QRect( QPoint( xStart, height() - 9 ), QPoint( xEnd, height() ) ),
			Qt::AlignCenter,
			QString::number( ( max - min ) * float( i ) / numDivisions + min, 'f', precision ) );      
	}
}
