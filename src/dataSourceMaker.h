#ifndef DATASOURCEMAKER_H_INCLUDED
#define DATASOURCEMAKER_H_INCLUDED

#include "nodeMaker.h"

//! Abstract base class for a maker that creates data sources from fields in hdf files.
//! Provides helper functions for its derived classes.
class dataSourceMaker : public nodeMaker
{
public:

	dataSourceMaker( QString theName );

	//! Returns true if this item has any available data.
	virtual bool HasData() const;

	//! A list of options that can be selected to view different slices of data.
	virtual QStringList Options() const = 0;

	//! Values associated with a particular option.
	virtual QStringList Values( QString option ) const = 0;

	//! Values associated with a particular option and orbit.
	virtual QStringList Values( QString option, int orbit ) const = 0;

	//! Creates a data node from user selected options.
	virtual ptr<hdfDataNode> Create( QStringList values, int orbit ) const = 0;

	//! Returns a SQL SELECT query for the orbits available to this item.
	virtual QString OrbitSQL( QStringList values ) const;

protected:

	//! Returns the list of available cameras.
	QStringList CameraValues() const;

	//! Returns the list of available cameras for this orbit.
	QStringList CameraValues( int orbit ) const;

	//! Returns the list of available dimensions.
	QStringList DimensionOptions() const;

	//! Returns the list of values for a dimension.
	QStringList DimensionValues( QString option ) const;

	//! Returns the id of a value for a dimension.
	int DimensionValueID( QString option, QString value ) const;

	//! Returns the integer values for the dimension value selections.
	std::vector<int> Dimensions( QStringList values ) const;

	//! Returns additional constraints to place on the list of returned orbits.
	virtual QString OrbitOptionSQL( QStringList values ) const = 0;
};

//! Creates data sources from fields in hdf files that have an associated camera.
//! The camera is used to provide an extra dimension/option in the fieldSelector.
class dataSourceMakerCamera : public dataSourceMaker
{
public:

	dataSourceMakerCamera( QString theName );

	//! A list of options that can be selected to view different slices of data.
	virtual QStringList Options() const;

	//! Values associated with a particular option.
	virtual QStringList Values( QString option ) const;

	//! Values associated with a particular option and orbit.
	virtual QStringList Values( QString option, int orbit ) const;

	//! Creates a data node from user selected options.
	virtual ptr<hdfDataNode> Create( QStringList values, int orbit ) const;

	//! Returns additional constraints to place on the list of returned orbits.
	virtual QString OrbitOptionSQL( QStringList values ) const;
};

//! Creates data sources from fields in hdf files that have no associated camera.
class dataSourceMakerNoCamera : public dataSourceMaker
{
public:

	dataSourceMakerNoCamera( QString theName );

	//! A list of options that can be selected to view different slices of data.
	virtual QStringList Options() const;

	//! Values associated with a particular option.
	virtual QStringList Values( QString option ) const;

	//! Values associated with a particular option and orbit.
	virtual QStringList Values( QString option, int orbit ) const;

	//! Creates a data node from user selected options.
	virtual ptr<hdfDataNode> Create( QStringList values, int orbit ) const;

	//! Returns additional constraints to place on the list of returned orbits.
	virtual QString OrbitOptionSQL( QStringList values ) const;
};

#endif // DATASOURCEMAKER_H_INCLUDED
