#ifndef IMAGESIZECHOOSER_H_INCLUDED
#define IMAGESIZECHOOSER_H_INCLUDED

class QLineEdit;

#include <qwidget.h>
#include <QSize>

//! An image size editor that enforces an aspect ratio constraint.
//! As the user edits width or height, the other is updated to maintain
//! the ratio of width / height.
class imageSizeChooser : public QWidget
{
	Q_OBJECT
	
public:

	//! Constructs imageSizeChooser, initializes aspect.
	imageSizeChooser( double aspect, QWidget* parent );

	//! Constructs imageSizeChooser, initializes aspect to size.width() / size.height().
	imageSizeChooser( const QSize & size, QWidget* parent );

	//! Constructs imageSizeChooser, initializes aspect to width / height.
	imageSizeChooser( int width, int height, QWidget* parent );
	
	//! Destructor.
	~imageSizeChooser();

	//! Returns the aspect ratio.
	double AspectRatio() const;

	//! Returns the current value for width.
	int Width() const;

	//! Returns the current value for height.
	int Height() const;

	//! Returns the current value for width and height.
	QSize Size() const;

public slots:

	//! Changes the aspect ratio constraint to size.width() / size.height().
	//! Resets width and height editors to size.width() and size.height().
	void setAspectRatio( const QSize& size );

	//! Changes the aspect ratio constraint to value.
	//! Updates width and height to enforce aspect ratio constraint.
	void setAspectRatio( double value );
	
protected slots:

	//! Called when the user has edited the width. Adjusts the height to
	//! newWidth / aspectRatio.
	void WidthChanged( const QString& newWidth );

	//! Called when the user has edited the width. Adjusts the width to
	//! newHeight * aspectRatio.
	void HeightChanged( const QString& newHeight );

protected:
	
	//! Helper function to create widgets and set up signals and slots in constructors.
	void Init( int initialWidth );

	double aspectRatio; //!< aspect ratio constraint ( width / height )
	QLineEdit* width; //!< width editor widget
	QLineEdit* height; //!< height editor widget
};

#endif // IMAGESIZECHOOSER_H_INCLUDED
