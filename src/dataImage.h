#ifndef DATAIMAGE_H_INCLUDED
#define DATAIMAGE_H_INCLUDED

#include <qlabel.h>
#include <qpixmap.h>
#include <qimage.h>

#include "colorMap.h"
#include "qutility.h"



class dataImage : public QLabel
{
    Q_OBJECT

public:

    dataImage( const colorMap & theColorMap, QWidget *parent );

    virtual ~dataImage();

    int PixelHoverX() const;
    int PixelHoverY() const;

    int PixelClickX() const;
    int PixelClickY() const;

    template <typename dataType>
        void InputData( const dataType * data, const dataType min, const dataType max )
    {
        QImage imageTemp( 12, 10, QImage::Format_ARGB32_Premultiplied );

        if ( min < max )
        {
            float oneOverSize = 1.0f / float( max - min );

            for ( int x = 0 ; x < 12 ; x++ )
            {
                for ( int y = 0 ; y < 10 ; y++ )
                {
                    imageTemp.setPixel( x, y, getQRgba( imageColorMap( float( data[ x * 10 + y ] - min ) * oneOverSize ) ) );
                }
            }           
        }
        else
        {
            imageTemp.fill( 0 );
        }

		setPixmap( QPixmap::fromImage( imageTemp, Qt::ColorOnly | Qt::ThresholdDither ) );

        update();       
    }

signals:

    void PixelHover( int x, int y );

    void PixelClick( int x, int y );

protected:

    void mouseMoveEvent( QMouseEvent *e );

    void mousePressEvent( QMouseEvent *e );

    int pixelHoverX;
    int pixelHoverY;

    int pixelClickX;
    int pixelClickY;

    colorMap imageColorMap;
};

#endif // DATAIMAGE_H_INCLUDED
