#include "imageSizeChooser.h"
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <QIntValidator>
#include <iostream>

imageSizeChooser::imageSizeChooser( double aspect, QWidget* parent )
:
QWidget( parent ),
aspectRatio( aspect )
{
	Init( 1024 );
}

imageSizeChooser::imageSizeChooser( const QSize & size, QWidget* parent )
:
QWidget( parent ),
aspectRatio( double( size.width() ) / double( size.height() ) )
{
	Init( size.width() );
}

imageSizeChooser::imageSizeChooser( int width, int height, QWidget* parent )
:
QWidget( parent ),
aspectRatio( double( width ) / double( height ) )
{
	Init( width );
}

imageSizeChooser::~imageSizeChooser()
{

}

double imageSizeChooser::AspectRatio() const
{
	return aspectRatio;
}

int imageSizeChooser::Width() const
{
	return width->text().toInt();
}

int imageSizeChooser::Height() const
{
	return height->text().toInt();
}

QSize imageSizeChooser::Size() const
{
	return QSize( Width(), Height() );
}

void imageSizeChooser::setAspectRatio( const QSize& size )
{
	width->setText( QString::number( size.width() ) );
	setAspectRatio( double( size.width() ) / double( size.height() ) );
}

void imageSizeChooser::setAspectRatio( double value )
{
	aspectRatio = value;
	if( aspectRatio >= 1 )
	{
		WidthChanged( width->text() );
	}
	else
	{
		HeightChanged( height->text() );
	}
}

void imageSizeChooser::WidthChanged( const QString& newWidth )
{
	height->setText( QString::number( int( newWidth.toInt() / aspectRatio ) ) );
}

void imageSizeChooser::HeightChanged( const QString& newHeight )
{
	width->setText( QString::number( int( newHeight.toInt() * aspectRatio ) ) );
}

void imageSizeChooser::Init( int initialWidth )
{
	QHBoxLayout* mainLayout = new QHBoxLayout( this );
	mainLayout->setSpacing( 2 );
	
	int inputWidth = 5 * fontMetrics().maxWidth();
	
	mainLayout->addWidget( width = new QLineEdit( QString::number( initialWidth ), this ) );
	mainLayout->addWidget( new QLabel( "x", this ) );
	mainLayout->addWidget( height = new QLineEdit( "0", this ) );      

	width->setValidator( new QIntValidator( 16, 32767, width ) );
	height->setValidator( new QIntValidator( 16, 32767, height ) );

	width->setFixedWidth( inputWidth );
	height->setFixedWidth( inputWidth );

	connect( width, SIGNAL( textEdited( const QString & ) ), this, SLOT( WidthChanged( const QString & ) ) );
	connect( height, SIGNAL( textEdited( const QString & ) ), this, SLOT( HeightChanged( const QString & ) ) );
	
	// Initialize to 1024xX
	WidthChanged( width->text() );
}
