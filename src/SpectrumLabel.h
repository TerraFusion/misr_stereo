#ifndef SPECTRUMLABEL_H_INCLUDED
#define SPECTRUMLABEL_H_INCLUDED

#include <qlabel.h>

#include "colorMap.h"

class QPixmap;

//! Displays a color map with a number scale underneath.
class SpectrumLabel : public QLabel
{
    Q_OBJECT

public: 

    SpectrumLabel( const colorMap & theColorMap, QWidget *parent = 0 );

    virtual ~SpectrumLabel();

    void setMinMax( float theMin, float theMax );

protected:  

    void paintEvent( QPaintEvent *p );

    float min;
    float max;

    QPixmap *theSpectrumPixmap;
};

#endif // SPECTRUMLABEL_H_INCLUDED
