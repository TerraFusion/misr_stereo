
#include <qevent.h>

#include "colorMap.h"
#include "dataImage.h"



dataImage::dataImage( const colorMap & theColorMap, QWidget *parent )
:
QLabel( parent ),
pixelHoverX( 0 ),
pixelHoverY( 0 ),
pixelClickX( 0 ),
pixelClickY( 0 ),
imageColorMap( theColorMap )
{
    // sunken frame
	setFrameStyle( QFrame::Panel | QFrame::Sunken );

    // get mouse motion events even if the button is not pressed
	setMouseTracking( true );

    // pixmap scales to fit label
	setScaledContents( true );

    // set initial size
	setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    // prevent unnecessary sizes
	setMinimumSize(  6 * 12,  6 * 10 );
	setMaximumSize( 16 * 12, 16 * 10 );

    // black until we get some data
	QPixmap blank( 1, 1 );
	blank.fill( Qt::black );
	setPixmap( blank );
}



dataImage::~dataImage()
{

}



int dataImage::PixelHoverX() const
{
	return pixelHoverX;
}



int dataImage::PixelHoverY() const
{
	return pixelHoverY;
}



int dataImage::PixelClickX() const
{
	return pixelClickX;
}



int dataImage::PixelClickY() const
{
	return pixelClickY;
}



void dataImage::mouseMoveEvent( QMouseEvent *e )
{
	int newPixelX = e->x() * 12 / width();
	int newPixelY = e->y() * 10 / height();

	if ( ( newPixelX >= 0 ) && ( newPixelX < 12 ) )
	{
		if ( ( newPixelY >= 0 ) && ( newPixelY < 10 ) )
		{
			if ( ( newPixelX != pixelHoverX ) || ( newPixelY != pixelHoverY ) )
			{
				pixelHoverX = newPixelX;
				pixelHoverY = newPixelY;

				emit PixelHover( pixelHoverX, pixelHoverY );
			}
		}
	}
}



void dataImage::mousePressEvent( QMouseEvent *e )
{
	int newPixelX = e->x() * 12 / width();
	int newPixelY = e->y() * 10 / height();

	if ( ( newPixelX >= 0 ) && ( newPixelX < 12 ) )
	{
		if ( ( newPixelY >= 0 ) && ( newPixelY < 10 ) )
		{
			pixelClickX = newPixelX;
			pixelClickY = newPixelY;

			emit PixelClick( pixelClickX, pixelClickY );
		}
	}
}
