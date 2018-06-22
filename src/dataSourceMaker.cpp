#include "dataSourceMaker.h"
#include "fileHandler.h"

dataSourceMaker::dataSourceMaker( QString theName )
:
nodeMaker( theName )
{

}

bool dataSourceMaker::HasData() const
{
	QString sql = QString(
		"SELECT COUNT(*) FROM files, fields "
		"WHERE files.name == fields.file "
		"AND fields.name == '%1';" )
		.arg( Name() );

	return Data->Query( sql ).SingleRow().ValueInt() > 0;
}

QString dataSourceMaker::OrbitSQL( QStringList values ) const
{
	return QString(
		"SELECT orbit FROM files, fields "
		"WHERE files.name == fields.file "
		"AND fields.name == '%1' "
		"%2"
	).arg( Name(), OrbitOptionSQL( values ) );
}

QStringList dataSourceMaker::CameraValues() const
{
	QString sql = QString(
		"SELECT camera "
		"FROM files, fields, cameras "
		"WHERE files.name == fields.file "
		"AND fields.name == '%1' "
		"AND files.camera == cameras.name "
		"GROUP BY camera "
		"ORDER BY cameras.id;" )
		.arg( Name() );

	return Data->QueryStringList( sql );
}

QStringList dataSourceMaker::CameraValues( int orbit ) const
{
	QString sql = QString(
		"SELECT camera "
		"FROM files, fields, cameras "
		"WHERE files.name == fields.file "
		"AND fields.name == '%1' "
		"AND files.orbit == %2 "
		"AND files.camera == cameras.name "
		"GROUP BY camera "
		"ORDER BY cameras.id;" )
		.arg( Name() ).arg( orbit );

	return Data->QueryStringList( sql );
}

QStringList dataSourceMaker::DimensionOptions() const
{
	QString sql = QString(
		"SELECT name "
		"FROM dimensions "
		"WHERE field == '%1' "
		"GROUP BY name "
		"ORDER BY id;" )
		.arg( Name() );

	return Data->QueryStringList( sql );
}

QStringList dataSourceMaker::DimensionValues( QString option ) const
{
	QString sql = QString(
		"SELECT valueName "
		"FROM dimensions "
		"WHERE field == '%1' "
		"AND name == '%2' "
		"ORDER BY valueId;" )
		.arg( Name(), option );

	return Data->QueryStringList( sql );
}

int dataSourceMaker::DimensionValueID( QString option, QString value ) const
{
	QString sql = QString(
		"SELECT valueID "
		"FROM dimensions "
		"WHERE field == '%1' "
		"AND name == '%2' "
		"AND valueName == '%3';" )
		.arg( Name(), option, value );

	return Data->Query( sql ).SingleRow().ValueInt();
}

std::vector<int> dataSourceMaker::Dimensions( QStringList values ) const
{
	QStringList options = DimensionOptions();
	std::vector<int> dimensions( options.size() );
	for( unsigned int i = 0; i < dimensions.size(); i++ )
	{
		dimensions[ i ] = DimensionValueID( options[ i ], values[ i ] );
	}
	return dimensions;
}



dataSourceMakerCamera::dataSourceMakerCamera( QString theName )
:
dataSourceMaker( theName )
{

}

QStringList dataSourceMakerCamera::Options() const
{
	return QStringList( "Camera" ) + DimensionOptions();
}

QStringList dataSourceMakerCamera::Values( QString option ) const
{
	if( option == "Camera" )
	{
		return CameraValues();
	}
	else
	{
		return DimensionValues( option );
	}
}

QStringList dataSourceMakerCamera::Values( QString option, int orbit ) const
{
	if( option == "Camera" )
	{
		return CameraValues( orbit );
	}
	else
	{
		return DimensionValues( option );
	}
}

ptr<hdfDataNode> dataSourceMakerCamera::Create( QStringList values, int orbit ) const
{
	QString camera = values.takeFirst();
	return Data->Create( Name(), camera, Dimensions( values ), orbit ).Ptr();
}

QString dataSourceMakerCamera::OrbitOptionSQL( QStringList values ) const
{
	return QString( "AND camera == \"%1\" " ).arg( values[ 0 ] );
}



dataSourceMakerNoCamera::dataSourceMakerNoCamera( QString theName )
:
dataSourceMaker( theName )
{

}

QStringList dataSourceMakerNoCamera::Options() const
{
	return DimensionOptions();
}

QStringList dataSourceMakerNoCamera::Values( QString option ) const
{
	return DimensionValues( option );
}

QStringList dataSourceMakerNoCamera::Values( QString option, int ) const
{
	return DimensionValues( option );
}

ptr<hdfDataNode> dataSourceMakerNoCamera::Create( QStringList values, int orbit ) const
{
	return Data->Create( Name(), Dimensions( values ), orbit ).Ptr();
}

QString dataSourceMakerNoCamera::OrbitOptionSQL( QStringList ) const
{
	return QString();
}
