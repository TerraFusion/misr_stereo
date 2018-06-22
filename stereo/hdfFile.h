#ifndef HDFFILE_H_INCLUDED
#define HDFFILE_H_INCLUDED

#include <hdf.h>
#include <HdfEosDef.h>

#include <list>
#include <vector>
#include <string>

#include "hdfBase.h"
#include "ptr.h"
#include "stringaux.h"

class hdfBlockData
{
public:

	uint8   oceanFlag;
	hdfRect bounds;
	uint8   dataFlag;       
};



class hdfGridNode;
class hdfFieldNode;

class hdfFileNode
{
	friend class hdfGridNode;
	friend class hdfFieldNode;

public:

	hdfFileNode();
	virtual ~hdfFileNode();

	bool CheckDependencies() const;

	int32 Handle() const;
	int32 ValidHandle();

	std::string Name() const;
	std::string Description() const;

	int StartBlock() const;
	int EndBlock() const;
	int NumBlocks() const;

	const hdfRect & BlockRect() const;
	const hdfRect & BlockRect( int blockIndex ) const;

	hdfPolygon BlockPolygon( int blockIndex ) const;
	std::list<hdfPolygon> BlockPolygonList() const;

	int PathNumber() const;

	int CameraNumber() const;
	std::string CameraName() const;

	int OrbitNumber() const;

	int NumGrids() const;
	ptr<hdfGridNode> Grid( int gridIndex ) const;

	int GridIndex( std::string gridName ) const;
	ptr<hdfGridNode> Grid( std::string gridName ) const;

	int NumFields() const;
	ptr<hdfFieldNode> Field( int fieldIndex ) const;
	std::stringlist FieldNameList() const;

	int FieldIndex( std::string fieldName ) const;
	ptr<hdfFieldNode> Field( std::string fieldName ) const;

	bool Open();
	bool Close();

	bool Load( std::string fileName );
	void Destroy();

	void Print( int indent = 0 ) const;

protected:

	ptr<hdfGridNode> CreateGridNode( std::string gridName );

	int32 handle;

	std::string name;
	std::string description;

	int32 startBlock;
	int32 endBlock;
	int32 numBlocks;
	int32 pathNumber;
	int32 cameraNumber;
	int32 orbitNumber;

	std::vector< hdfBlockData > blockList;
	hdfRect blockRect;

	std::vector< ptr< hdfGridNode > > gridList;
};



class hdfFileList;
class hdfGrid;
class hdfField;

class hdfFile : public ptr<hdfFileNode>
{
	friend class hdfDictionary;
	friend class hdfFileList;
	friend class hdfGrid;
	friend class hdfField;

public:

	hdfFile() : ptr<hdfFileNode>() {}

	hdfFile( ptr<hdfFileNode> data ) : ptr<hdfFileNode>( data ) {}

	hdfFile( std::string fileName ) : ptr<hdfFileNode>( new hdfFileNode )
	{
		if ( ! (*this)->Load( std::string( fileName ) ) )
		{
			(*this) = hdfFile();
		}
	}
};



#endif
