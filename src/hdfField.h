#ifndef HDFFIELD_H_INCLUDED
#define HDFFIELD_H_INCLUDED

#include <hdf.h>
#include <HdfEosDef.h>

#include <qstring.h>
#include <qstringlist.h>

#include <vector>
#include <list>

#include "hdfBase.h"
#include "ptr.h"

//! storage types for data in hdf fields
typedef enum
{
	hdfNone,
	hdfChar8,
	hdfUChar8,
	hdfInt8,
	hdfUInt8,
	hdfInt16,
	hdfUInt16,
	hdfInt32,
	hdfUInt32,
	hdfFloat32,
	hdfFloat64

} hdfDataType;

#include "hdfFile.h"
#include "hdfGrid.h"

//! Provides access to attributes and data in a hdf field.
//! Although data may be stored using one of several data types, this class
//! does nothing to take that into account. All interaction with data is
//! using void*. Users of this class can access the fill value as a
//! void* to determine which values should be ignored and can check the date
//! type using DataType().
class hdfFieldNode
{
	friend class hdfFileNode;
	friend class hdfGridNode;

public:

	//! Constructs a grid node.
	//! \param theParentGrid the file node that this field is attached to.
	hdfFieldNode( hdfGridNode* theParentGrid );

	//! Destructor
	virtual ~hdfFieldNode();

	//! Returns the grid node that this field is attached to.
	ptr<hdfGridNode> Grid() const;

	//! Returns the name of this grid.
	QString Name() const;

	//! Returns the number of extra dimensions.
	int Rank() const;

	//! Returns the size of a dimension.
	//! \param index the dimension number ( 0 to Rank() - 1 )
	int DimensionSize( int index ) const;

	//! Returns the name of a dimension.
	//! \param index the dimension number ( 0 to Rank() - 1 )
	QString DimensionName( int index ) const;

	//! List of available options for all extra dimensions.
	std::list<QStringList> OptionList() const;

	//! Returns the data storage type.
	hdfDataType DataType() const;

	//! Returns the data storage type as a string.
	QString DataTypeString() const;

	//! Returns the size of the data type in bytes.
	int DataSize() const;

	//! Returns the size of an entire block in bytes.
	int BlockMemSize() const;

	//! Returns a void* to the fill value.
	//! This represents unavailable data.
	void *FillValue() const;

	//! Read any 3d cube of data across block, x, and y dimensions into a buffer
	//! \param dest destination address
	//! \param blockMin min value on block dimension
	//! \param blockMax max value on block dimension
	//! \param xMin min value on x dimension
	//! \param xMax max value on x dimension
	//! \param yMin min value on y dimension
	//! \param yMax max value on y dimension
	//! \param dims vector of values selecting from remaining dimensions
	bool Read( void* dest, int blockMin, int blockMax, int xMin, int xMax, int yMin, int yMax, const std::vector<int> & dims = std::vector<int>() ) const;

	//! Read a range of blocks into a buffer
	//! \param dest destination address
	//! \param blockMin first block to read
	//! \param blockMax last block to read
	//! \param dims vector of values selecting from remaining dimensions
	bool ReadBlocks( void* dest, int blockMin, int blockMax, const std::vector<int> & dims = std::vector<int>() ) const;

	//! Read a block into a buffer
	//! \param dest destination address
	//! \param blockIndex block number to read
	//! \param dims vector of values selecting from remaining dimensions
	bool ReadBlock( void* dest, int blockIndex, const std::vector<int> & dims = std::vector<int>() ) const;

	//! Makes sure the parent grid is open.
	bool Open();

	//! Closes the parent grid handle.
	bool Close();

	//! Loads information from a field.
	bool Load( QString fieldName );

	//! Does nothing, but included for uniformity with file/grid interface
	void Destroy();

	//! Prints field structure.
	void Print( int indent = 0 ) const;

	//! Returns the file node that this field is attached to.
	ptr<hdfFileNode> File() const { return parentGrid->File(); }

	//! Returns the name of this file.
	QString FileName() const { return parentGrid->Name(); }

	//! Returns the index of the first block where data exists.
	int StartBlock() const { return parentGrid->StartBlock(); }

	//! Returns the index of the last block where data exists.
	int EndBlock() const { return parentGrid->EndBlock(); }

	//! Returns the total number of blocks.
	//! This will not be the same as EndBlock() - StartBlock() + 1.
	int NumBlocks() const { return parentGrid->NumBlocks(); }

	//! Returns the bounding rectangle for all blocks.
	const hdfRect & BlockRect() const { return parentGrid->BlockRect(); }

	//! Returns the bounding rectangle for a block.
	//! \param blockIndex a block number from StartBlock() to EndBlock()
	const hdfRect & BlockRect( int blockIndex ) const { return parentGrid->BlockRect( blockIndex ); }

	//! Returns the bounding rectangle of a block converted to a polygon.
	//! \param blockIndex a block number from StartBlock() to EndBlock()
	hdfPolygon BlockPolygon( int blockIndex ) const { return parentGrid->BlockPolygon( blockIndex ); }

	//! Returns a list containing BlockPolygon() for each block.
	std::list<hdfPolygon> BlockPolygonList() const { return parentGrid->BlockPolygonList(); }

	//! Returns the path number ( 1 - 233 ).
	int PathNumber() const { return parentGrid->PathNumber(); }

	//! Returns the camera number ( 1 - 9 ).
	int CameraNumber() const { return parentGrid->CameraNumber(); }

	//! Returns the camera name associated with the camera number.
	QString CameraName() const { return parentGrid->CameraName(); }

	//! Returns the orbit number.
	int OrbitNumber() const { return parentGrid->OrbitNumber(); }

	//! Returns the name of this grid.
	QString GridName() const { return parentGrid->Name(); }

	//! Returns the size of the along track dimension.
	int XDim() const { return parentGrid->XDim(); }

	//! Returns the size of the across track dimension.
	int YDim() const { return parentGrid->YDim(); }

	//! Returns the size of the block dimension.
	int BlockDim() const { return parentGrid->BlockDim(); }

	//! Returns the SOM projection parameters ( 15 elements ).
	const std::vector<double>& ProjectionParameters() const { return parentGrid->ProjectionParameters(); }

	//! Returns the scaling factor attribute if present.
	double Scale() const { return parentGrid->Scale(); }

	//! Returns the solar irradiance attribute if present.
	double SolarIrradiance() const { return parentGrid->SolarIrradiance(); }

	//! Returns the distance to sun if present.
	double SolarDistance() const { return parentGrid->SolarDistance(); }

protected:

	hdfGridNode* parentGrid; //!< the grid containing this field
	QString name; //!< field name
	std::vector<int32> dimensionSize; //!< list of dimension sizes
	QStringList dimensionName; //!< list of dimension names
	hdfDataType dataType; //!< field storage type
	int dataSize; //!< size of data type in bytes
	unsigned char fillValue[ 8 ]; //!< buffer for fill value
};



class hdfFile;
class hdfGrid;

//! Wrapper around hdfFieldNode
class hdfField : public ptr<hdfFieldNode>
{
	friend class hdfFile;
	friend class hdfGrid;

public:

	//! Default constructor
	hdfField() : ptr<hdfFieldNode>() {}

	//! Constructor from raw hdfFieldNode
	hdfField( ptr<hdfFieldNode> data ) : ptr<hdfFieldNode>( data ) {}
};

#endif
