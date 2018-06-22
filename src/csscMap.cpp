
#include <hdf.h>
#include <mfhdf.h>

#include <qdatastream.h>
#include <qfile.h>
#include <qimage.h>
#include <qpixmap.h>
#include <QMouseEvent>

#include "csscMap.h"
#include "csscName.h"

// csscMap
// + constructor    
csscMap::csscMap( QWidget *parent )
:
QLabel( parent ),
csscIndex( 0 )
{
    memset( mapData, 0, sizeof( mapData ) );

    setMinimumSize( 432, 216 );
    setMaximumSize( mapWidth, mapHeight );

    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ); 
}



// ~csscMap
// + destructor
csscMap::~csscMap()
{

}



// loadData
// + load the raw data from a file
// + data is floating point, convert to unsigned short
bool csscMap::loadData( QString fileName )
{
    QFile csscMapFile( fileName );

	if ( ! csscMapFile.open( QIODevice::ReadOnly ) ) return false;

    QDataStream csscStream( & csscMapFile );

    for ( int y = 0 ; y < mapHeight ; y++ )
    {
        for ( int x = 0 ; x < mapWidth ; x++ )
        {
            csscStream >> mapData[ y ][ x ];
        }
    }

    return true;
}



// csscUpdate( csscIndex )
// + call whenever the viewed cssc changes
void csscMap::csscUpdate( int newCsscIndex )
{
    if ( ( unsigned short ) ( newCsscIndex ) != csscIndex )
    {
        csscIndex = ( unsigned short ) ( newCsscIndex );

        makeImageFromData();
    }
}



// heightForWidth
// + returns the height of the window
int heightForWidth( int width )
{
    return ( width >> 1 );
}



// mousePressEvent
// + on a mouse press, look up the pixel value for the cssc and emit
//   the changed signal
void csscMap::mousePressEvent( QMouseEvent *e )
{
    int x = e->x() * mapWidth  /  width();
    int y = e->y() * mapHeight / height();

    unsigned short newCsscIndex = mapData[ y ][ x ];

    if ( newCsscIndex != csscIndex )
    {
        emit csscChanged( ( int ) newCsscIndex );
    }
}



// resizeEvent
// + resize the pixmap when the widget resizes
void csscMap::resizeEvent( QResizeEvent* )
{
    makeImageFromData();
}



// updates the pixmap displaying the image data
void csscMap::makeImageFromData()
{
    unsigned short data;

    const int imageWidth( width() );
    const int imageHeight( height() );

    QImage temp( imageWidth, imageHeight, QImage::Format_RGB32 ); 

    QRgb *linePtr = 0;

    QRgb colorSelected = qRgb( 0, 255, 0 );
    QRgb colorMatching = qRgb( 255, 0, 0 );
    QRgb colorTable[ 1581 ];

    int olsenIndex = csscName::CsscToOlsen( csscIndex );

    // build color table
    for ( int i = 0 ; i <= 1580 ; i++ )
    {
        unsigned char color( i * 255 / 1580 );            
        colorTable[ i ] = qRgb( color, color, color );
    }

    // build pixmap
    for ( int y = 0, v = 0, errorY = imageHeight >> 2 ; y < imageHeight ; y++ )
    {
        linePtr = ( QRgb * ) temp.scanLine( y );

        for ( int x = 0, u = 0, errorX = imageWidth >> 2 ; x < imageWidth ; x++, linePtr++ )
        {
            data = mapData[ v ][ u ];

            *linePtr = colorTable[ data ];

            errorX += mapWidth;
            while ( errorX >= imageWidth )
            {
                if ( mapData[ v ][ u ] == csscIndex )
                {
                    *linePtr = colorSelected;
                }
                else if ( csscName::CsscToOlsen( mapData[ v ][ u ] ) == olsenIndex )
                {
                    *linePtr = colorMatching;
                }

                errorX -= imageWidth;
                u++;
            }
        }

        errorY += mapHeight;
        while ( errorY >= imageHeight )
        {
            errorY -= imageHeight;
            v++;
        }
    }

    setPixmap( QPixmap::fromImage( temp ) );
}
