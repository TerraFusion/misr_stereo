#ifndef HDFGRID_H_INCLUDED
#define HDFGRID_H_INCLUDED

#include <hdf.h>
#include <HdfEosDef.h>

#include <vector>

#include <qstring.h>

#include "ptr.h"
#include "hdfBase.h"
#include "hdfFile.h"

class hdfFieldNode;

//! Provides access to attributes and fields in a hdf grid.
class hdfGridNode
{
	friend class hdfFileNode;
	friend class hdfFieldNode;

public:

	//! Constructs a grid node.
	//! \param theParent the file node that this grid is attached to.
	hdfGridNode( hdfFileNode* theParent );

	//! Destructor. Destroys files.
	virtual ~hdfGridNode();

	//! Returns true if any fields are in use.
	bool CheckDependencies() const;

	//! Returns the file node that this grid is attached to.
	ptr<hdfFileNode> File() const;

	//! Returns the current grid handle. May be equal to FAIL if the grid is not open.
	int32 Handle() const;

	//! Returns a valid grid handle. Opens the grid if necessary.
	//! This may also require opening the parent file.
	int32 ValidHandle();

	//! Returns the name of this grid.
	QString Name() const;

	//! Returns the size of the along track dimension.
	int XDim() const;

	//! Returns the size of the across track dimension.
	int YDim() const;

	//! Returns the size of the block dimension.
	int BlockDim() const;

	//! Returns the BlockOffset() for all blocks (1-180).
	//! Note that the first block is block 1,
	//! but this is actually index 0 in the vector.
	std::vector<float32> BlockOffset() const;

	//! Returns the offset of the left edge of a block.
	//! \param block a block number from 1 to 180
	float32 BlockOffset( int block ) const;

	//! Returns the BlockOffset() for a range of blocks.
	//! \param begin the first block to return ( will be index 0 )
	//! \param end the last block to return ( will be index begin - end + 1 )
	std::vector<float32> BlockOffset( int begin, int end ) const;

	//! Returns the SOM projection parameters ( 15 elements ).
	const std::vector<double>& ProjectionParameters() const;

	//! Returns the scaling factor attribute if present.
	double Scale() const;

	//! Returns the solar irradiance attribute if present.
	double SolarIrradiance() const;

	//! Returns the distance to sun if present.
	double SolarDistance() const;

	//! Returns the number of fields that are available.
	int NumFields() const;

	//! Returns the field node with the requested index.
	//! \param index the field number, 0 to NumFields() - 1
	ptr<hdfFieldNode> Field( int index ) const;

	//! Given a field name, returns the index.
	//! Returns -1 if the field does not exist.
	int FieldIndex( QString fieldName ) const;

	//! Returns the field node with the requested name.
	//! May return an invalid field node if there is no field with the requested
	//! name, use IsValid() to check.
	ptr<hdfFieldNode> Field( QString fieldName ) const;

	//! Opens the grid, making Handle() valid.
	bool Open();

	//! Closes the grid, making Handle() invalid.
	bool Close();

	//! Loads information and field structure from a grid.
	bool Load( QString gridName );

	//! Destroys existing field structures.
	void Destroy();

	//! Prints grid structure.
	void Print( int indent = 0 ) const;

	//! Returns the name of this file.
	QString FileName() const { return parentFile->Name(); }

	//! Returns the index of the first block where data exists.
	int StartBlock() const { return parentFile->StartBlock(); }

	//! Returns the index of the last block where data exists.
	int EndBlock() const { return parentFile->EndBlock(); }

	//! Returns the total number of blocks.
	//! This will not be the same as EndBlock() - StartBlock() + 1.
	int NumBlocks() const { return parentFile->NumBlocks(); }

	//! Returns the bounding rectangle for all blocks.
	const hdfRect & BlockRect() const { return parentFile->BlockRect(); }

	//! Returns the bounding rectangle for a block.
	//! \param blockIndex a block number from StartBlock() to EndBlock()
	const hdfRect & BlockRect( int blockIndex ) const { return parentFile->BlockRect( blockIndex ); }

	//! Returns the bounding rectangle of a block converted to a polygon.
	//! \param blockIndex a block number from StartBlock() to EndBlock()
	hdfPolygon BlockPolygon( int blockIndex ) const { return parentFile->BlockPolygon( blockIndex ); }

	//! Returns a list containing BlockPolygon() for each block.
	std::list<hdfPolygon> BlockPolygonList() const { return parentFile->BlockPolygonList(); }

	//! Returns the path number ( 1 - 233 ).
	int PathNumber() const { return parentFile->PathNumber(); }

	//! Returns the camera number ( 1 - 9 ).
	int CameraNumber() const { return parentFile->CameraNumber(); }

	//! Returns the camera name associated with the camera number.
	QString CameraName() const { return parentFile->CameraName(); }

	//! Returns the orbit number.
	int OrbitNumber() const { return parentFile->OrbitNumber(); }

protected:

	hdfFileNode* parentFile; //!< the file containing this grid
	int32 handle; //!< the grid handle

	QString name; //!< grid name

	int32 xDim; //!< along track dimension
	int32 yDim; //!< across track dimension
	int32 blockDim; //!< block dimension

	std::vector<float32> blockOffset; //!< offset of left edge of blocks

	std::vector<double> projectionParameters; //!< SOM projection parameters ( 15 elements )

	double scale; //!< scaling factor attribute if present
	double solarIrradiance; //!< solar irradiance attribute if present
	double solarDistance; //!< distance to sun if present

	std::vector< ptr< hdfFieldNode > > fieldList; //!< list of field structures
};

class hdfFile;
class hdfField;

//! Wrapper around hdfGridNode
class hdfGrid : public ptr<hdfGridNode>
{
	friend class hdfFile;
	friend class hdfField;

public:

	//! Default constructor
	hdfGrid() : ptr<hdfGridNode>() {}

	//! Constructor from raw hdfGridNode
	hdfGrid( ptr<hdfGridNode> data ) : ptr<hdfGridNode>( data ) {}
};

#endif // HDFGRID_H_INCLUDED
