#ifndef NODEMAKERGROUP_H_INCLUDED
#define NODEMAKERGROUP_H_INCLUDED

#include <QList>
#include <QMap>
#include <QObject>
#include <QStringList>
#include "ptr.h"
#include "nodeMaker.h"

//! Stores a list of data sources. The list includes any automatically regiestered
//! sources, as well as those available from data indexed by fileHandler. Many
//! of these functions are also present on nodeMaker, but the versions here take
//! an extra argument of the input name, and return a suitable default if the input
//! does not actually exist.
class nodeMakerGroup : public QObject
{
	Q_OBJECT

signals:

	//! Sent when the list of available data sources has changed. Other classes
	//! may need to update themselves if inputs appear or disappear.
	void Refreshed();

public slots:

	//! Refreshes the list of available data sources when the available data changes.
	void Refresh();

public:

	//! Removes all data sources from the list.
	void Clear();

	//! Returns a list of the names of all data sources.
	QStringList Names() const;

	//! A list of options that can be selected to view different slices of data.
	QStringList Options( QString name ) const;

	//! Values associated with a particular option.
	//! This may return values that are not available on the current orbit.
	//! \param name name of the requested data source
	//! \param option one of the strings returned from Options()
	QStringList Values( QString name, QString option ) const;

	//! Values associated with a particular option that are available for a specified orbit.
	//! \param name name of the requested data source
	//! \param option one of the strings returned from Options()
	//! \param orbit filter the list of values to show only what is available on this orbit
	QStringList Values( QString name, QString option, int orbit ) const;

	//! Returns true if a data source can be used.
	bool HasData( QString name ) const;

	//! Creates a data node from selected options and orbit.
	ptr<hdfDataNode> Create( QString name, QStringList values, int orbit ) const;

	//! Creates a data node from selected options and orbit.
	ptr<hdfDataNode> CreateAveraged( QString name, QStringList values, int orbit, int radius ) const;

	//! Returns a SQL SELECT query for a single column containing the available orbits.
	QString OrbitSQL( QString name, QStringList values ) const;

	//! Returns a specific data source maker.
	nodeMaker* Get( QString name ) const;

protected:

	//! Adds a data source maker to the list of data sources.
	void Add( nodeMaker* maker );

	//! Adds an instance of dataSourceMaker for each available field.
	void AddDataSources();

	//! Adds registered classes derived from nodeMaker.
	void AddBuiltins();

	//! The available data sources indexed by name.
	QMap<QString,ptr<nodeMaker> > makers;
};

//! Global list of data sources that the user will select from.
extern nodeMakerGroup DataSources;

#endif // NODEMAKERGROUP_H_INCLUDED
