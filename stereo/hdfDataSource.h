#ifndef HDFDATASOURCE_H_INCLUDED
#define HDFDATASOURCE_H_INCLUDED

#include <list>
#include <vector>

#include "matrix.h"
#include "hdfBase.h"
#include "hdfDataNode.h"
#include "hdfDataOp.h"
#include "hdfField.h"

// hdfDataSource
// + attaches to a field for on-demand reading of data blocks
class hdfDataSource : public hdfDataNode
{

public:

	hdfDataSource( hdfField theDataField, const std::vector<int> & dims = std::vector<int>() );

	virtual ~hdfDataSource();

	hdfField DataField() const;

	void setBlockLimit( int theBlockLimit );
	int BlockLimit() const;

	void setBlockRange( int startBlockIndex, int endBlockIndex );

	void setStartBlock( int blockIndex );
	int StartBlock() const;

	void setEndBlock( int blockIndex );
	int EndBlock() const;

	void setDimensions( const std::vector<int> & dims );
	const std::vector<int> & Dimensions() const;

	virtual hdfValue Value( const hdfCoord & location );

	virtual std::list<hdfPolygon> GetMask() const;

	matrix<hdfValue> GetBlock( int index );	

	void Flush();

protected:

	void BlockCreate( int blockIndex );

	void BlockDestroy( int blockIndex );

	void BlockDestroyAll();

	bool BlockValid( int blockIndex ) const;

	hdfScalar ConvertToCommon( void * data ) const;

	hdfField dataField;
	int startBlock;
	int endBlock;
	std::vector<int> dimensionList;

	hdfRect dataRect;	
	int blockLimit;
	std::vector<unsigned char *> blockList;
	std::list<int> validList;
};



// hdfRadianceSource
// + attaches to a field for on-demand reading of data blocks
class hdfRadianceSource : public hdfDataSource
{

public:

	hdfRadianceSource( hdfField theDataField );

	virtual ~hdfRadianceSource();

	virtual hdfValue Value( const hdfCoord & location );

protected:

	double scaleFactor;
};



class hdfBrfSource : public hdfOpBinary<hdfOpBinaryMul>
{
public:

	hdfBrfSource( ptr<hdfRadianceSource> dataRad, ptr<hdfDataSource> dataSZA );

	virtual ~hdfBrfSource();
};



class hdfBdasSource : public hdfOpBinary<hdfOpBinarySub>
{
public:

	hdfBdasSource( ptr<hdfBrfSource> red1, ptr<hdfBrfSource> blue1, ptr<hdfBrfSource> red2, ptr<hdfBrfSource> blue2 );

	virtual ~hdfBdasSource();
};



#endif
