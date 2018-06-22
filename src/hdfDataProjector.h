#ifndef HDFDATAPROJECTOR_H_INCLUDED
#define HDFDATAPROJECTOR_H_INCLUDED

#include "hdfDataNode.h"
#include "projector.h"
#include "ptr.h"

//! A data node that converts the location from one projection to another.
//! This is commonly used to convert MISR data from its native SOM
//! projection to latitude, longitude coordinates.
class hdfDataProjector : public hdfDataNode
{
public:

	//! Creates a new data projector.
	//! The input data will be specified later.
	//! \param theProjector projects locations into this projection 
    hdfDataProjector( ptr<projector> theProjector );

	//! Creates a new data projector.
	//! \param input0 the input data to be sampled
	//! \param theProjector projects locations into this projection 
    hdfDataProjector( ptr<hdfDataNode> input0, ptr<projector> theProjector );

	//! Destructor
    virtual ~hdfDataProjector();

	//! "dataProjector"
	virtual std::string Name() const;

	//! Prints extra information about the projector.
	virtual void PrintAttributes( std::ostream& out, const std::string& prefix ) const;

	//! Returns the projector used to transform locations.
    ptr<projector> Projector() const;

	//! Sets the projector used to transform locations.
	void setProjector( ptr<projector> theProjector );

	//! Sets the active orbit, updating projection parameters.
	virtual void setOrbit( int orbit );

	//! Checks the tree for any missing data.
	//! \returns true if the projector is valid and the tree can output useful data from Values().
	virtual bool Check() const;

	//! Returns the data value at a projected location.
	virtual hdfValue Value( const hdfCoord & location );

	//! Same as Value(), but samples many points at once.
	virtual std::vector<hdfValue> Values( const std::vector<hdfCoord> & location );

	//! Returns the input mask unprojected to latitude, longitude.
    virtual std::list<hdfPolygon> GetMask() const;

	//! Returns true since this is a projector.
	virtual bool IsProjector() const;

	//! Returns true if other is a projector and has all of the same parameters.
	virtual bool IsEqualTo( ptr<hdfDataNode> other ) const;

protected:

	//! Since this node is a projector, move it so that dest is its new parent. Dest will
	//! be as many nodes upwards as the projector can move without encountering a node with
	//! multiple children.
	virtual void PromoteProjectorsTo( ptr<hdfDataNode>& parent, ptr<hdfDataNode>& dest );

	ptr<projector> proj; //!< the projector used to transform locations
};

#endif // HDFDATAPROJECTOR_H_INCLUDED
