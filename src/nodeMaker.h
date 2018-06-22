#ifndef NODEMAKER_H_INCLUDED
#define NODEMAKER_H_INCLUDED

#include <vector>
#include <list>
#include <QString>
#include <QStringList>

#include "ptr.h"
#include "hdfDataNode.h"

//! Manages the creation of arbitrary data nodes.
//! Provides a standard interface for querying available orbits, as well as the number of available options and their values.
//! Creates a data source given a set of option values and an orbit, and can also make a data source that performs a spatial average on its output.
class nodeMaker
{
public:

	typedef std::vector<ptr<nodeMaker> > listType; //!< list of builtin nodeMakers

	//! Returns the list of builtin nodeMakers.
	static listType & Builtins();

	//! Registers the nodeMaker in a list.
	//! \see Builtins()
	static nodeMaker* Register( nodeMaker* item );

	//! Creates a new node maker.
	nodeMaker( QString theName );

	//! destructor.
	virtual ~nodeMaker();

	//! Name of this item as presented to the user.
	QString Name() const;

	//! A list of options that can be selected to view different slices of data.
	virtual QStringList Options() const = 0;

	//! Values associated with a particular option.
	//! This may return values that are not available on the current orbit.
	//! \param option one of the strings returned from Options()
	virtual QStringList Values( QString option ) const = 0;

	//! Values associated with a particular option that are available for a specified orbit.
	//! \param option one of the strings returned from Options()
	//! \param orbit filter the list of values to show only what is available on this orbit
	virtual QStringList Values( QString option, int orbit ) const = 0;

	//! Returns true if this item can create any data sources.
	//! Subclasses should return false if there are no orbits for which they can create data sources.
	virtual bool HasData() const = 0;

	//! Creates a data node from selected options and orbit.
	//! Subclasses should return hdfDataNode() if the data node is unavailable.
	virtual ptr<hdfDataNode> Create( QStringList values, int orbit ) const = 0;

	//! Creates a data node from selected options and orbit.
	ptr<hdfDataNode> CreateAveraged( QStringList values, int orbit, int radius ) const;

	//! Returns a SQL SELECT query for a single column containing the available orbits.
	//! The query should NOT be terminated with a semicolon.
	virtual QString OrbitSQL( QStringList values ) const = 0;

protected:

	QString name; //!< name of this item.

	static QString defaultOrbitSQL; //!< SQL query for all orbits
};

//! Registers a node maker in the list of builtins.
#define REGISTER_NODE_MAKER( CLASS, NAME ) \
static nodeMaker* register_##CLASS = nodeMaker::Register( new CLASS( NAME ) );

#endif // NODEMAKER_H_INCLUDED
