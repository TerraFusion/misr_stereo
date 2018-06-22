#ifndef HDFFIELD_H_INCLUDED
#define HDFFIELD_H_INCLUDED

#include <hdf.h>
#include <HdfEosDef.h>

#include <vector>
#include <list>

#include "hdfBase.h"
#include "ptr.h"

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



// hdfField
// + provides an interface to a field
// + fields can store data of any type
// + will provide access to data only as void *




#include "hdfFile.h"
#include "hdfGrid.h"

class hdfFieldNode
{
	friend class hdfFileNode;
	friend class hdfGridNode;

public:

	hdfFieldNode( hdfGridNode * theParentGrid );
	virtual ~hdfFieldNode();

	ptr<hdfGridNode> Grid() const;
	std::string Name() const;

	int Rank() const;
	int DimensionSize( int index ) const;
	std::string DimensionName( int index ) const;

	std::list<std::stringlist> OptionList() const;

	hdfDataType DataType() const;
	std::string DataTypeString() const;
	int DataSize() const;

	int BlockMemSize() const;

	void setSlice( int theSlice );
	int Slice() const;

	void *FillValue() const;

	bool Read( void * dest, int blockMin, int blockMax, int xMin, int xMax, int yMin, int yMax, const std::vector<int> & dims = std::vector<int>() ) const;
	bool ReadBlocks( void * dest, int blockMin, int blockMax, const std::vector<int> & dims = std::vector<int>() ) const;
	bool ReadBlock( void * dest, int blockIndex, const std::vector<int> & dims = std::vector<int>() ) const;

	bool Open();
	bool Close();

	bool Load( std::string fieldName );
	void Destroy();

	void Print( int indent = 0 ) const;

	// ancestor access functions ( file )

	ptr<hdfFileNode> File() const { return Grid()->File(); }

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

	// ancestor access functions ( grid )

	std::string GridName() const { return Grid()->Name(); }

	int XDim() const { return Grid()->XDim(); }
	int YDim() const { return Grid()->YDim(); }
	int BlockDim() const { return Grid()->BlockDim(); }

	const hdfRect & BoundRect() const { return Grid()->BoundRect(); }

	const double *ProjectionParameters() const { return Grid()->ProjectionParameters(); }

	double Scale() const { return Grid()->Scale(); }
	double SolarIrradiance() const { return Grid()->SolarIrradiance(); }
	double SolarDistance() const { return Grid()->SolarDistance(); }

protected:

	hdfGridNode * parentGrid;
	std::string name;
	std::vector<int32> dimensionSize;
	std::stringlist dimensionName;
	hdfDataType dataType;
	int dataSize;
	int slice;

	unsigned char fillValue[ 8 ];
};



class hdfFile;
class hdfGrid;

class hdfField : public ptr<hdfFieldNode>
{
	friend class hdfFile;
	friend class hdfGrid;

public:

	hdfField() : ptr<hdfFieldNode>() {}

	hdfField( ptr<hdfFieldNode> data ) : ptr<hdfFieldNode>( data ) {}
};

#endif
