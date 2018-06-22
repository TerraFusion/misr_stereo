#ifndef CSSCNAME_H_INCLUDED
#define CSSCNAME_H_INCLUDED

#include <QLabel>

//! A QLabel that displays the Olsen class name for CSSC classes.
class csscName : public QLabel
{
    Q_OBJECT

public:

    //! Constructor
    csscName( QWidget *parent = 0 );

    //! destructor
    virtual ~csscName();

    //! Initializes CSSC to Olsen mapping and list of Olsen class names.
    static void Init();

    //! Returns the Olsen class of a CSSC class.
    static int CsscToOlsen( int csscIndex );

    //! Returns the name of an Olsen class.
    static QString OlsenToName( int olsenIndex );


public slots:

    //! Updates the label to display the Olsen class name of a new CSSC class.
    void CsscUpdate( int newCsscIndex );

protected:

	//! Sets the Olsen class for a range of CSSC classes. Called only from Init().
	//! \param first the first CSSC class in the range
	//! \param last the last CSSC class in the range
	//! \param olsen the Olsen class for the range of CSSC classes.
	static void SetOlsenClass( int first, int last, int olsen );

    static int csscOlsen[ 1581 ]; //!< olsen class for each CSSC class

    static QString olsenName[ 74 ]; //!< olsen class names
};

#endif // CSSCNAME_H_INCLUDED
