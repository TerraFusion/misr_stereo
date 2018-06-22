#ifndef HDFGRID_H_INCLUDED
#define HDFGRID_H_INCLUDED

#include <hdf.h>
#include <HdfEosDef.h>

#include <vector>

#include "ptr.h"
#include "hdfBase.h"
#include "hdfFile.h"

class hdfFieldNode;

class hdfGridNode
{
	friend class hdfFileNode;
	friend class hdfFieldNode;

public:

	hdfGridNode( hdfFileNode * theParent );
	virtual ~hdfGridNode();

	bool CheckDependencies() const;

	ptr<hdfFileNode> File() const;

	int32 Handle() const;
	int32 ValidHandle();

	std::string Name() const;

	int XDim() const;
	int YDim() const;
	int BlockDim() const;

	const hdfRect & BoundRect() const;

	std::vector<float32> BlockOffset() const;
	float32 BlockOffset( int block ) const;
	std::vector<float32> BlockOffset( int begin, int end ) const;

	const double *ProjectionParameters() const;

	double Scale() const;
	double SolarIrradiance() const;
	double SolarDistance() const;

	int NumFields() const;
	ptr<hdfFieldNode> Field( int index ) const;

	int FieldIndex( std::string fieldName ) const;
	ptr<hdfFieldNode> Field( std::string fieldName ) const;

	bool Open();
	bool Close();

	bool Load( std::string gridName );
	void Destroy();

	void Print( int indent = 0 ) const;

	// ancestor access functions ( file )

	std::string FileName() const { return File()->Name(); }

	int StartBlock() const { return File()->StartBlock(); }
	int EndBlock() const { return File()->EndBlock(); }
	int NumBlocks() const { return File()->NumBlocks(); }

	const hdfRect & BlockRect( int blockIndex ) const { return File()->BlockRect( blockIndex ); }

	hdfPolygon BlockPolygon( int blockIndex ) const { return File()->BlockPolygon( blockIndex ); }
	std::list<hdfPolygon> BlockPolygonList() const { return File()->BlockPolygonList(); }

	int PathNumber() const { return File()->PathNumber(); }

	int CameraNumber() const { return File()->CameraNumber(); }
	std::string CameraName() const { return File()->CameraName(); }

	int OrbitNumber() const { return File()->OrbitNumber(); }

protected:

	hdfFileNode * parentFile;
	int32 handle;

	std::string name;

	int32 xDim;
	int32 yDim;
	int32 blockDim;

	hdfRect bounds;

	std::vector<float32> blockOffset;

	double projectionParameters[ 13 ];

	double scale;
	double solarIrradiance;
	double solarDistance;

	std::vector< ptr< hdfFieldNode > > fieldList;
};




class hdfFile;
class hdfField;

class hdfGrid : public ptr<hdfGridNode>
{
	friend class hdfFile;
	friend class hdfField;

public:

	hdfGrid() : ptr<hdfGridNode>() {}

	hdfGrid( ptr<hdfGridNode> data ) : ptr<hdfGridNode>( data ) {}
};

#endif
