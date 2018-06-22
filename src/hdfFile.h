#ifndef HDFFILE_H_INCLUDED
#define HDFFILE_H_INCLUDED

#include <hdf.h>
#include <HdfEosDef.h>

#include <QString>
#include <QStringList>

#include <list>
#include <vector>

#include "hdfBase.h"
#include "ptr.h"

//! per-block data stored in an hdf file
class hdfBlockData
{
public:
	uint8   oceanFlag;
	hdfRect bounds; //!< block boundaries in SOM coordinates
	uint8   dataFlag;
};

class hdfGridNode;
class hdfFieldNode;

//! Provides access to attributes, grids, and fields in a hdf file.
class hdfFileNode
{
	friend class hdfGridNode;
	friend class hdfFieldNode;

public:

	//! Constructs a file node that does not refer to a particular file.
	hdfFileNode();

	//! Destructor. Destroys grids and fields.
	virtual ~hdfFileNode();

	//! Returns true if any grids or fields are in use.
	bool CheckDependencies() const;

	//! Returns the current file handle. May be equal to FAIL if the file is not open.
	int32 Handle() const;

	//! Returns a valid file handle. Opens the file if necessary.
	int32 ValidHandle();

	//! Returns the name of this file.
	QString Name() const;

	//! Returns the index of the first block where data exists.
	int StartBlock() const;

	//! Returns the index of the last block where data exists.
	int EndBlock() const;

	//! Returns the total number of blocks that could be present.
	//! This will not be the same as EndBlock() - StartBlock() + 1.
	int NumBlocks() const;

	//! Returns the bounding rectangle for all blocks.
	const hdfRect & BlockRect() const;

	//! Returns the bounding rectangle for a block.
	//! \param blockIndex a block number from StartBlock() to EndBlock()
	const hdfRect & BlockRect( int blockIndex ) const;

	//! Returns the bounding rectangle of a block converted to a polygon.
	//! \param blockIndex a block number from StartBlock() to EndBlock()
	hdfPolygon BlockPolygon( int blockIndex ) const;

	//! Returns a list containing BlockPolygon() for each block.
	std::list<hdfPolygon> BlockPolygonList() const;

	//! Returns the path number ( 1 - 233 ).
	int PathNumber() const;

	//! Returns the camera number ( 1 - 9 ).
	int CameraNumber() const;

	//! Returns the camera name associated with the camera number.
	QString CameraName() const;

	//! Returns the orbit number.
	int OrbitNumber() const;

	//! Returns the number of grids that are available.
	int NumGrids() const;

	//! Returns the grid node with the requested index.
	//! \param gridIndex the grid number, 0 to NumGrids() - 1
	ptr<hdfGridNode> Grid( int gridIndex ) const;

	//! Given a grid name, returns the index.
	//! Returns -1 if the grid does not exist.
	int GridIndex( QString gridName ) const;

	//! Returns the grid node with the requested name.
	//! May return an invalid grid node if there is no grid with the requested
	//! name, use IsValid() to check.
	ptr<hdfGridNode> Grid( QString gridName ) const;

	//! Returns the number of fields that are available.
	int NumFields() const;

	//! Returns the field node with the requested index.
	//! \param fieldIndex the field number, 0 to NumFields() - 1
	ptr<hdfFieldNode> Field( int fieldIndex ) const;

	//! Returns a list of all field names.
	QStringList FieldNameList() const;

	//! Given a field name, returns the index.
	//! Returns -1 if the field does not exist.
	int FieldIndex( QString fieldName ) const;

	//! Returns the field node with the requested name.
	//! May return an invalid field node if there is no field with the requested
	//! name, use IsValid() to check.
	ptr<hdfFieldNode> Field( QString fieldName ) const;

	//! Opens the file, making Handle() valid.
	bool Open();

	//! Closes the file, making Handle() invalid.
	bool Close();

	//! Loads information and grid and field structure from a file.
	bool Load( QString fileName );

	//! Destroys existing grid and field structures.
	void Destroy();

	//! Prints file structure.
	void Print( int indent = 0 ) const;

protected:

	//! Adds a grids structure to the list of grids.
	//! \param gridName the name of the new grid
	ptr<hdfGridNode> CreateGridNode( QString gridName );

	int32 handle; //!< hdf file handle

	QString name; //!< name of the open file

	int32 startBlock; //!< first valid block
	int32 endBlock; //!< last valid block
	int32 numBlocks; //!< total number of blocks
	int32 pathNumber; //!< path number (1-233)
	int32 cameraNumber; //!< camera number (1-9) or -1 if none
	int32 orbitNumber; //!< orbit number (995-)

	std::vector< hdfBlockData > blockList; //!< list of block data structures
	hdfRect blockRect; //!< bounding rectangle around all blocks

	std::vector< ptr< hdfGridNode > > gridList; //!< list of grid structures in file
};

class hdfGrid;
class hdfField;

//! Wrapper around hdfFileNode to allow constructing with a file name
class hdfFile : public ptr<hdfFileNode>
{
	friend class hdfGrid;
	friend class hdfField;

public:

	//! Default constructor
	hdfFile() : ptr<hdfFileNode>() {}

	//! Constructor from raw hdfFileNode
	hdfFile( ptr<hdfFileNode> data ) : ptr<hdfFileNode>( data ) {}

	//! Constructs and loads data from a file.
	//! If loading fails, IsValid() will return false and no functions
	//! on the hdfFileNode should be called.
	hdfFile( std::string fileName ) : ptr<hdfFileNode>( new hdfFileNode )
	{
		if ( ! (*this)->Load( QString::fromStdString( fileName ) ) )
		{
			(*this) = hdfFile();
		}
	}
};

#endif // HDFFILE_H_INCLUDED
