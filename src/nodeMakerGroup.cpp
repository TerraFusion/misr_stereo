#include "dataSourceMaker.h"
#include "fileHandler.h"
#include "nodeMaker.h"
#include "nodeMakerGroup.h"

nodeMakerGroup DataSources;

void nodeMakerGroup::Refresh()
{
	Clear();
	AddDataSources();
	AddBuiltins();
	emit Refreshed();
}

void nodeMakerGroup::Clear()
{
	makers.clear();
}

QStringList nodeMakerGroup::Names() const
{
	return makers.keys();
}

QStringList nodeMakerGroup::Options( QString name ) const
{
	nodeMaker* maker = Get( name );
	return maker ? maker->Options() : QStringList();
}

QStringList nodeMakerGroup::Values( QString name, QString option ) const
{
	nodeMaker* maker = Get( name );
	return maker ? maker->Values( option ) : QStringList();
}

QStringList nodeMakerGroup::Values( QString name, QString option, int orbit ) const
{
	nodeMaker* maker = Get( name );
	return maker ? maker->Values( option, orbit ) : QStringList();
}

bool nodeMakerGroup::HasData( QString name ) const
{
	nodeMaker* maker = Get( name );
	return maker ? maker->HasData() : false;
}

ptr<hdfDataNode> nodeMakerGroup::Create( QString name, QStringList values, int orbit ) const
{
	nodeMaker* maker = Get( name );
	return maker ? maker->Create( values, orbit ) : new hdfDataNode();
}

ptr<hdfDataNode> nodeMakerGroup::CreateAveraged( QString name, QStringList values, int orbit, int radius ) const
{
	nodeMaker* maker = Get( name );
	return maker ? maker->CreateAveraged( values, orbit, radius ) : new hdfDataNode();
}

QString nodeMakerGroup::OrbitSQL( QString name, QStringList values ) const
{
	nodeMaker* maker = Get( name );
	return maker ? maker->OrbitSQL( values )
				 : QString( "SELECT orbit FROM files" );
}

nodeMaker* nodeMakerGroup::Get( QString name ) const
{
	return makers.value( name, 0 ).Ptr();
}

void nodeMakerGroup::Add( nodeMaker* maker )
{
	makers.insert( maker->Name(), maker );
}

void nodeMakerGroup::AddDataSources()
{
	// get all field names, with one of the camera names for each
	query row = Data->Query(
		"SELECT fields.name, MAX( camera ) "
		"FROM fields, files "
		"WHERE fields.file = files.name "
		"GROUP BY fields.name;" );

	while( row.Step() )
	{
		const char* name = row.ValueCString( 0 );
		std::string camera = row.ValueString( 1 );
		if( camera.empty() )
		{
			Add( new dataSourceMakerNoCamera( name ) );
		}
		else
		{
			Add( new dataSourceMakerCamera( name ) );
		}
	}
}

void nodeMakerGroup::AddBuiltins()
{
	nodeMaker::listType::const_iterator maker = nodeMaker::Builtins().begin();
	while( maker != nodeMaker::Builtins().end() )
	{
		if( (*maker)->HasData() )
		{
			makers.insert( (*maker)->Name(), *maker );
		}
		maker++;
	}
}
