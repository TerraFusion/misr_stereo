
#include <QString>
#include <QRegExp>

#include "hdfMain.h"

hdfField LoadField( hdfFile file, const QString & fieldName )
{
	hdfField result;

	if ( file.IsValid() )
	{
		result = file->Field( fieldName );
	}

	return result;
}

hdfField LoadField( hdfFile file, const QString & gridName, const QString & fieldName )
{
	hdfField result;

	if ( file.IsValid() )
	{
		hdfGrid grid = file->Grid( gridName );

		if ( grid.IsValid() )
		{
			result = grid->Field( fieldName );
		}
	}

	return result;
}

const QString& CameraName( int cameraNumber )
{
	static const QString noName;
	static const QStringList cameraNames = QString( "NULL,DF,CF,BF,AF,AN,AA,BA,CA,DA" ).split( ',' );
	if( cameraNumber >= 0 && cameraNumber < cameraNames.size() )
	{
		return cameraNames[ cameraNumber ];
	}
	else
	{
		return noName;
	}
}

int OrbitNumber( QString fileName )
{
	int orbitIndex = fileName.indexOf( QRegExp( "_O[0-9]*_" ) );
	QString orbitNumberString = fileName.mid( orbitIndex + 2, 6 );
	return orbitNumberString.toInt();
}
