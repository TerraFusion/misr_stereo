#ifndef HDFDATASOURCE_H_INCLUDED
#define HDFDATASOURCE_H_INCLUDED

#include <list>
#include <vector>

#include "matrix.h"
#include "hdfBase.h"
#include "hdfDataNode.h"
#include "hdfDataOp.h"
#include "hdfField.h"

//! data node that accesses a field in a hdf file.
//! Caches the data to speed up subsequent accesses.
//! This data node has no inputs.
class hdfDataSource : public hdfDataNode
{
public:

	//! Construct a new data source.
	//! Some fields may have extra dimensions, and data nodes can only access
	//! a single plane of data. Because of this, you must specify the indices
	//! that should be visible in for all extra dimensions. Each valid combination
	//! of dims is a slice of which only one at a time can be seen. This may be
	//! an empty vector if the field has no extra dimensions.
	//! \param theDataField the field to read data from
	//! \param dims visible slice in extra dimensions
	hdfDataSource( hdfField theDataField, const std::vector<int> & dims = std::vector<int>() );

	//! Destructor
	virtual ~hdfDataSource();

	//! "dataSource"
	virtual std::string Name() const;

	//! Prints the name of the data field.
	virtual void PrintAttributes( std::ostream& out, const std::string& prefix ) const;

	//! Sets the data field.
	void setDataField( hdfField theDataField );

	//! Returns the data field.
	const hdfField & DataField() const;

	//! Sets the visible slice in extra dimensions.
	void setDimensions( const std::vector<int> & dims = std::vector<int>() );

	//! visible slice in extra dimensions.
	const std::vector<int> & Dimensions() const;

	//! Loads the matching data field for a new orbit.
	virtual void setOrbit( int orbit );

	//! Returns true if data field is valid.
	virtual bool Check() const;

	//! Returns itself since this is a data source.
	virtual const hdfDataSource* FindDataSource() const;

	//! Samples the field using nearest neighbor sampling.
	//! If there is no data available at a location, returns 0 with a coverage of 0.
	virtual hdfValue Value( const hdfCoord & location );

	//! Same as Value(), but samples many points at once.
	virtual std::vector<hdfValue> Values( const std::vector<hdfCoord> & location );

	//! Returns the block regions as a list of polygons.
	virtual std::list<hdfPolygon> GetMask() const;

	//! Returns the data for an entire block
	matrix<hdfValue> GetBlock( int index );	

	//! Clears all block data. Usually done only when changing data field or dimensions.
	void Flush();

protected:

	//! Helper function used by Value() and Values() that actually samples data.
	hdfValue SampleAt( const hdfCoord & location );

	//! Makes sure that a block is in memory.
	//! \param blockIndex index of the block to load
	void LoadBlock( int blockIndex );

	//! Converts data from the field's data type to floating point.
	hdfScalar ConvertToCommon( void * data ) const;

	hdfField dataField; //!< field to read data from
	std::vector<int> dimensionList; //!< visible slice in extra dimensions
	hdfRect dataRect; //!< bounding rectangle for data in native projection
	std::vector<std::vector<unsigned char> > blockList; //!< stores data for individual blocks
	QString name; //!< the name of the field used for reading data
	QString cameraName; //!< the camera name of the field used for reading data
};

#endif // HDFDATASOURCE_H_INCLUDED
