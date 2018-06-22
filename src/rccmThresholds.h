#ifndef RCCMTHRESHOLDS_H_INCLUDED
#define RCCMTHRESHOLDS_H_INCLUDED

#include <qwidget.h>

class QString;
class gridLayout;
class QLabel;
class dataFloat;
class dataChar;
class SpectrumLabel;



class rccmThresholds : public QWidget
{
    Q_OBJECT

public:

    rccmThresholds( QWidget *parent = 0 );

    virtual ~rccmThresholds();

    bool LoadData( const QString & fileName, int index );

signals:

    void PixelClick( int x, int y );

    void PixelHover( int x, int y );

    void dataDisplayUpdate( int x, int y );

public slots:

    void csscUpdate( int newCsscIndex );

private slots:

    void coordinateDisplayUpdate( int x, int y );

private:

    QLabel    *coordinates;

    dataFloat * thresholdDisplay[ 2 ][ 5 ][ 3 ];
    dataChar  *updateFlagDisplay[ 2 ][ 5 ][ 3 ];

    SpectrumLabel *spectrum[ 2 ];
};



#endif // RCCMTHRESHOLDS_H_INCLUDED
