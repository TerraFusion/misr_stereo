
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qregexp.h>

#include "multiFileChooser.h"
#include "fileChooser.h"

#include <iostream>
using namespace std;


multiFileChooser::multiFileChooser( const QStringList & theCodeNames, QWidget * parent )
:
QDialog( parent )
{
    codeNames = theCodeNames;

    QVBoxLayout *verticalLayout = new QVBoxLayout( this );
    verticalLayout->setMargin( 8 );
    verticalLayout->setSpacing( 8 );

    QPushButton * SmartSearch = new QPushButton( "Smart Search For File Set", this );
    verticalLayout->addWidget( SmartSearch );
    connect( SmartSearch, SIGNAL( clicked() ), this, SLOT( SmartSearch_Click() ) );

    QGridLayout * chooserLayout = new QGridLayout();
	chooserLayout->setMargin( 2 );
	chooserLayout->setSpacing( 8 );
	verticalLayout->addLayout( chooserLayout );

    chooser.resize( codeNames.count() );

    for ( unsigned int i = 0 ; i < codeNames.count() ; i++ )
    {
        QLabel *rowLabel = new QLabel( codeNames[ i ], this );
		rowLabel->setAlignment( Qt::AlignCenter );
        chooserLayout->addWidget( rowLabel, i, 0 );

        chooser[ i ] = new fileChooser( this );
        chooser[ i ]->setMinimumSize( 640, 8 );
        chooserLayout->addWidget( chooser[ i ], i, 1 );
    }

    QHBoxLayout * buttonLayout = new QHBoxLayout();
	verticalLayout->addLayout( buttonLayout );

    QPushButton * chooserOpen = new QPushButton( "Open", this );
    buttonLayout->addWidget( chooserOpen );
    connect( chooserOpen, SIGNAL( clicked() ), this, SLOT( accept() ));
    chooserOpen->setDefault( true );

    QPushButton * chooserCancel = new QPushButton( "Cancel", this );
    buttonLayout->addWidget( chooserCancel );
    connect( chooserCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}



multiFileChooser::~multiFileChooser()
{

}



QString multiFileChooser::fileName( int index ) const
{
    return chooser[ index ]->fileName();
}



void multiFileChooser::SmartSearch_Click()
{
    QString inputFile = QFileDialog::getOpenFileName(
			this,
			"Select one of the files in the data set",
			QString(),
            "HDF Files ( *.hdf )" );

    if ( !inputFile.isNull() && !inputFile.isEmpty() )
    {
        QDir inputDir( inputFile );
        inputDir.cdUp();

        // get a list of all hdf files in the current directory
		QStringList allFiles = inputDir.entryList( QStringList( "*.hdf" ), QDir::Files );

        // add the path onto the filenames
        for ( QStringList::Iterator it = allFiles.begin() ; it != allFiles.end() ; ++it )
        {
            *it = inputDir.path() + '/' + *it;
        }

        // position of camera code in input file name
        int position = -1;

        // count the number of files found
        int numFiles = 0;

        // create regular expression for code
        QRegExp codeExpression = QRegExp( codeNames.join( "|" ) );

        do
        {
            // find camera code in input file name
            position = inputFile.indexOf( codeExpression, position + 1 );

            // done if no match
            if ( position < 0 ) break;

            // strip the code from the file name
            QString fileStripped( inputFile );
            fileStripped.remove( position, codeExpression.cap().length() );

            // reset file counter
            numFiles = 0;

            // search for matching files
            for ( unsigned int index = 0; index < codeNames.count(); index++ )
            {
                // construct file name to search for
                QString fileSearch( fileStripped );
                fileSearch.insert( position, codeNames[ index ] );

                // get matching files in directory
				QStringList matches = allFiles.filter( fileSearch, Qt::CaseInsensitive );

                // do any files match the name?
                if ( matches.count() > 0 )
                {
                    // yes, store file name
                    chooser[ index ]->setFileName( matches[ 0 ] );
                    numFiles++;
                }
                else
                {
                    // no, set file name blank
                    chooser[ index ]->setFileName( "" );
                }
            }
        }
        while ( numFiles <= 1 );
        // if only one file is found, there might be another match on the camera code
        //   later in the input file name.
        // it can't hurt to try again since inputFile will always be one of the file names
    }   
}

