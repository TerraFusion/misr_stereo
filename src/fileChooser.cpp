#include <qlineedit.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qlayout.h>

#include "fileChooser.h"

fileChooser::fileChooser( QWidget *parent )
:
QWidget( parent ),
md( File )
{
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setMargin( 0 );

    lineEdit = new QLineEdit( this );
    layout->addWidget( lineEdit );

    connect( lineEdit, SIGNAL( textChanged( const QString & ) ),
        this, SIGNAL( fileNameChanged( const QString & ) ) );

    button = new QPushButton( "...", this );
    button->setFixedWidth( button->fontMetrics().width( " ... " ) );
    layout->addWidget( button );

    connect( button, SIGNAL( clicked() ),
        this, SLOT( chooseFile() ) );

    setFocusProxy( lineEdit );
}



fileChooser::~fileChooser()
{

}



void fileChooser::setMode( Mode m )
{
    md = m;
}



fileChooser::Mode fileChooser::mode() const
{
    return md;
}



void fileChooser::setFileName( const QString &fn )
{
    lineEdit->setText( fn );
}



QString fileChooser::fileName() const
{
    return lineEdit->text();
}



void fileChooser::chooseFile()
{
    QString fn;

    if ( mode() == File )
        fn = QFileDialog::getOpenFileName( this, QString(), lineEdit->text(), "HDF Files ( *.hdf )" );
    else
        fn = QFileDialog::getExistingDirectory( this, QString(), lineEdit->text() );

    if ( !fn.isEmpty() ) {
        lineEdit->setText( fn );
        emit fileNameChanged( fn );
    }
}



