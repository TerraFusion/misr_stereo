#ifndef HDFDATANODE_H_INCLUDED
#define HDFDATANODE_H_INCLUDED

#include "hdfBase.h"
#include "ptr.h"
#include "utility.h"

class hdfDataSource;

//! Samples spatial data on the earth's surface.
//! Most of the time, you will use trees of classes derived from hdfDataNode to
//! display generic sources.
class hdfDataNode
{
public:

	//! Constructs a data node with no inputs.
	hdfDataNode();

	//! Constructs a data node and creates a list of default inputs.
	//! \param numInputs the number of inputs for this node.
	hdfDataNode( int numInputs );

	//! Destructor
	virtual ~hdfDataNode();

	//! Returns the name of this node for debugging purposes.
	//! \see Print
	virtual std::string Name() const;
	
	//! Prints debugging information about the tree.
	//! Also prints debugging information for inputs.
	//! \see Name
	//! \see PrintAttributes
	virtual void Print( std::ostream& out, int level ) const;
	
	//! Prints debugging for attributes on this node.
	//! Reimplement if a data node has extra data that should be printed.
	//! \see Print
	virtual void PrintAttributes( std::ostream& out, const std::string& prefix ) const;

	//! The number of attached inputs.
	int NumInputs() const;

	//! Returns an input.
	//! Results are undefined if index \< 0 or index \>= NumInputs().
	//! \param index the input number to return
	const ptr<hdfDataNode>& Input( int index ) const;

	//! Sets an input to a new data node.
	//! Results are undefined if index \< 0 or index \>= NumInputs().
	//! \param index the input number to set
	//! \param newInput the data node to attach, will be replaced with hdfDataNode if NULL.
	void setInput( int index, const ptr<hdfDataNode>& newInput );
	
	//! Sets the active orbit for the tree.
	//! This makes data sources return data for the new orbit.
	virtual void setOrbit( int orbit );

	//! Checks the tree for any missing data.
	//! \returns true if the tree can output useful data from Values().
	virtual bool Check() const;

	//! Returns the first data source that is a descendant of this node.
	//! This makes it possible for projectors to get parameters from data sources.
	virtual const hdfDataSource* FindDataSource() const;

	//! Returns the data value at a location.
	virtual hdfValue Value( const hdfCoord& location );
	
	//! Same as Value(), but samples many points at once.
	virtual std::vector<hdfValue> Values( const std::vector<hdfCoord>& location );

	//! Returns the mask, or a region that contains all points where the tree provides valid data.
	//! Used to skip areas that do not have data.
	virtual std::list<hdfPolygon> GetMask() const;

	//! Returns true if this node is a projector.
	virtual bool IsProjector() const;

	//! Returns true if another node is equal to this node.
	//! Currently implemented only for projectors, returns false for everything else even if equal.
	//! \todo Implement equality testing for non-projector items.
	virtual bool IsEqualTo( ptr<hdfDataNode> other ) const;
	
	std::vector<ptr<hdfDataNode> > inputs; //!< a list of inputs

	typedef std::vector<ptr<hdfDataNode> >::iterator inputIterator; //!< iterator type for input list

	//! Promotes projectors to the highest possible point in the tree, merging them if possible.
	//! The tree is passed by reference since the root may be modified.
	static void PromoteProjectors( ptr<hdfDataNode>& node );

protected:

	//! First, promote projectors on children. Projectors will now be immediate chidren of
	//! this node. Check children to see if they are projectors and they all have the same
	//! parameters. If so, we can remove these projectors and place a single one above us
	//! without altering the output of the tree.
	virtual void PromoteProjectorsTo( ptr<hdfDataNode>& parent, ptr<hdfDataNode>& dest );

	//! Allow hdfDataProjector to call PromoteProjectorsTo() on its inputs.
	friend class hdfDataProjector;
};

//! ostream output operator for printing hdfDataNode information.
//! \see hdfDataNode::Print()
std::ostream& operator << ( std::ostream& out, hdfDataNode* data );

//! ostream output operator for printing hdfDataNode information.
//! \see hdfDataNode::Print()
std::ostream& operator << ( std::ostream& out, const ptr<hdfDataNode>& data );

#endif // HDFDATANODE_H_INCLUDED
