#ifndef CSSCMAP_H_INCLUDED
#define CSSCMAP_H_INCLUDED

#include <qlabel.h>

//! Displays a map of CSSC classes on a label.
//! The selected CSSC value is displayed in green, CSSC values with the same Olsen class are optionally displayed in red.
class csscMap : public QLabel
{
    Q_OBJECT

public:

    //! Creates a new CSSC map label.
    csscMap( QWidget *parent = 0 );

    //! destructor
    virtual ~csscMap();

    //! Loads binary data from a file.
	//! The file is interpreted as a uint16 matrix in row major order with dimensions mapHeight x mapWidth.
    bool loadData( QString fileName );

    static const int mapWidth  = 2160; //!< width of the CSSC data matrix
    static const int mapHeight = 1080; //!< height of the CSSC data matrix

signals:    

    //! Signal sent when selected cssc index changes.
    void csscChanged( int csscIndex );

public slots:

    //! Selects a new CSSC value.
    void csscUpdate( int newCsscIndex );

protected:

    //! Returns the height for a specified width.
	//! Used by Qt to maintain the label's aspect ratio.
    int heightForWidth( int width );

    //! When the mouse is clicked, select the CSSC value of the clicked pixel.
    void mousePressEvent( QMouseEvent *e );

	//! When the widget is resized, update the CSSC display.
    void resizeEvent( QResizeEvent *e );

    //! Updates the label with the image data scaled to the label's size.
    void makeImageFromData();

    bool showOlsen; //!< if true, draw matching olsen classes

    unsigned short csscIndex; //!< the selected cssc value

    unsigned short mapData[ mapHeight ][ mapWidth ]; //!< raw cssc data
};

#endif // CSSCMAP_H_INCLUDED
