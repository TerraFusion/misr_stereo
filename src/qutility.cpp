
#include <iostream>
#include <cmath>
#include <stdlib.h>

#include <qapplication.h>
#include <qdir.h>
#include <qcolor.h>
#include <qpainter.h>
#include <QRectF>

#include "qutility.h"
#include "utility.h"

using namespace std;

QString appPath = ".";

void initAppPath( const char * environment_variable )
{
    // check the environment variable
	char *result = getenv( environment_variable );

	if ( result == NULL )
	{
        // the environment variable does not exist
        // print an error message

		cout << "ERROR: The environment variable "
			<< environment_variable
			<< " is not defined\n";

		cout << "The program will be unable to open some necessary files\n"
			<< "To fix this, define "
			<< environment_variable
			<< " or run the program from its own directory\n";

		appPath = ".";
	}
	else
	{
		appPath = QString( result );
	}
}

QString getAppPath( QString subPath )
{
	return ( subPath.isNull() ) ? ( appPath ) : ( appPath + '/' + subPath );
}

unsigned int getQRgba( unsigned int rgba )
{
	return qRgba(
		( rgba & 0x000000FF ) >> 0,
		( rgba & 0x0000FF00 ) >> 8,
		( rgba & 0x00FF0000 ) >> 16,
		( rgba & 0xFF000000 ) >> 24 );
}

QString CommaInt( double input )
{
	QString outString = QString::number( floor( input ), 'f', 0 );

	for ( int index = ( int ) outString.length() - 3 ; index > 0 ; index -= 3 )
	{
		outString.insert( index, ',' );
	}

	return outString;
}

QString ToString( double input, int digits )
{
	if ( input >= 10000 ) return CommaInt( input );

	int power = 0;
	if( input != 0 ) power = int( log10( input ) );
	int precision = digits - power;

	return QString::number( input, 'f', clamp( precision, 0, digits ) );
}

QStringList GlueStringList( const QStringList source, QString before, QString after )
{
	QStringList destination;

	QStringList::const_iterator current = source.begin();

	while ( current != source.end() )
	{
		destination << ( before + (*current++) + after );
	}

	return destination;
}

std::list<QFileInfo> getFileInfoList( QStringList path, QString pattern, int levels )
{
	std::list<QFileInfo> fileInfoList;

	QStringList::Iterator currentPath = path.begin();

	while ( currentPath != path.end() )
	{
		// search directories in path one at a time
		QDir directory( *currentPath );

		if ( levels > 0 ) // prevent infinite loops due to symlinks
		{
			// get subdirectory list
			QStringList subDirNameList = directory.entryList( QDir::Dirs, QDir::Name );

			// do not traverse . or ..
			subDirNameList.removeAll( "."  );
			subDirNameList.removeAll( ".." );

			// get information from matching files in subdirectories
			std::list<QFileInfo> newFileInfo = getFileInfoList( GlueStringList( subDirNameList, *currentPath + '/' ), pattern, levels - 1 );
			fileInfoList.insert( fileInfoList.end(), newFileInfo.begin(), newFileInfo.end() );
		}			

		// get matching files in this directory
		QFileInfoList fileInfo = directory.entryInfoList( QStringList( pattern ), QDir::Files, QDir::Name );

		// append files in this directory to list
		QList<QFileInfo>::const_iterator current = fileInfo.begin();
		while ( current != fileInfo.end() )
		{
			fileInfoList.push_back( *current );
			++current;
		}

		// examine next directory
		*currentPath++;
	}

	return fileInfoList;
}

std::list<QStringList> SplitSplit( QString delimiter1, QString delimiter2, const QString & theString )
{
	std::list<QStringList> result;

	QStringList splitStringList = theString.split( delimiter1 );

	QStringList::iterator currentString = splitStringList.begin();

	while ( currentString != splitStringList.end() )
	{
		result.push_back( currentString->split( delimiter2 ) );

		++currentString;
	}

	return result;
}

QString JoinJoin( QString delimiter1, QString delimiter2, const std::list<QStringList> & theListList )
{
	QStringList result;

	std::list<QStringList>::const_iterator currentStringList = theListList.begin();

	while ( currentStringList != theListList.end() )
	{
		result.push_back( (*currentStringList).join( delimiter2 ) );

		++currentStringList;
	}

	return result.join( delimiter1 );
}

void SetPainterWindow( QPainter & paint, const QRectF & window )
{
	paint.scale( paint.device()->width() / window.width(), paint.device()->height() / window.height() );
	paint.translate( -window.left(), -window.top() );
}
