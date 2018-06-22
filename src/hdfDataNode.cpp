#include "hdfDataNode.h"
#include <iostream>

hdfDataNode::hdfDataNode()
:
inputs()
{
	
}

hdfDataNode::hdfDataNode( int numInputs )
:
inputs( numInputs, new hdfDataNode )
{
	
}

hdfDataNode::~hdfDataNode()
{
	
}

std::string hdfDataNode::Name() const
{
	return "dataNode";
}

void hdfDataNode::Print( std::ostream& out, int level ) const
{
	std::string prefix( 2 * level, ' ' );
	out << prefix << Name() << '\n';
	PrintAttributes( out, prefix + "-" );
	for( unsigned int i = 0; i < inputs.size(); i++ )
	{
		inputs[ i ]->Print( out, level + 1 );
	}
}

void hdfDataNode::PrintAttributes( std::ostream&, const std::string& ) const
{

}

int hdfDataNode::NumInputs() const
{
	return int( inputs.size() );
}

const ptr<hdfDataNode>& hdfDataNode::Input( int index ) const
{
	return inputs[ index ];
}

void hdfDataNode::setInput( int index, const ptr<hdfDataNode> & newInput )
{
	if ( newInput.IsValid() )
	{
		inputs[ index ] = newInput;
	}
}

void hdfDataNode::setOrbit( int orbit )
{
	for( unsigned int i = 0; i < inputs.size(); i++ )
	{
		inputs[ i ]->setOrbit( orbit );
	}
}

bool hdfDataNode::Check() const
{
	for( unsigned int i = 0; i < inputs.size(); i++ )
	{
		if( !inputs[ i ]->Check() )
		{
			return false;
		}
	}

	return true;
}

const hdfDataSource* hdfDataNode::FindDataSource() const
{
	const hdfDataSource* ds = 0;
	for( unsigned int i = 0; i < inputs.size() && ds == 0; i++ )
	{
		ds = inputs[ i ]->FindDataSource();
	}
	return ds;
}

hdfValue hdfDataNode::Value( const hdfCoord & )
{
	return hdfValue( 0, 0 );
}

std::vector<hdfValue> hdfDataNode::Values( const std::vector<hdfCoord> & location )
{
	std::vector<hdfCoord> output( location.size() );
	for( unsigned int i = 0; i < location.size(); i++ )
	{
		output[ i ] = Value( location[ i ] );
	}
	return output;
}

std::list<hdfPolygon> hdfDataNode::GetMask() const
{
	std::list<hdfPolygon> combinedList;
	for( unsigned int i = 0; i < inputs.size(); i++ )
	{
		std::list<hdfPolygon> childList = inputs[ i ]->GetMask();
		combinedList.splice( combinedList.end(), childList );
	}
	return combinedList;
}

bool hdfDataNode::IsProjector() const
{
	return false;
}

bool hdfDataNode::IsEqualTo( ptr<hdfDataNode> ) const
{
	return false;
}

void hdfDataNode::PromoteProjectors( ptr<hdfDataNode>& node )
{
	node->PromoteProjectorsTo( node, node );
}

void hdfDataNode::PromoteProjectorsTo( ptr<hdfDataNode>&, ptr<hdfDataNode>& dest )
{
	if( inputs.size() > 1 )
	{
		// promote projectors from all children
		for( unsigned int i = 0; i < inputs.size() ; i++ )
		{
			inputs[ i ]->PromoteProjectorsTo( inputs[ i ], inputs[ i ] );
		}
		
		// quit if any child is not a projector or has different parameters
		for( unsigned int i = 0; i < inputs.size() ; i++ )
		{
			if( !inputs[i]->IsProjector() || !inputs[i]->IsEqualTo( inputs[0] ) )
			{
				return;
			}
		}

		// save a projector
		ptr<hdfDataNode> proj = inputs[0];
				
		// remove child projectors
		for( unsigned int i = 0; i < inputs.size() ; i++ )
		{
			ptr<hdfDataNode> old = inputs[ i ]->inputs[ 0 ];
			inputs[ i ] = old;
		}
		
		// insert projector above this node
		proj->inputs[ 0 ] = dest;
		dest = proj;
	}
	else if( inputs.size() == 1 )
	{
		inputs[ 0 ]->PromoteProjectorsTo( inputs[ 0 ], dest );
	}
}

std::ostream& operator << ( std::ostream& out, hdfDataNode* data )
{
	data->Print( out, 0 );
	return out;
}

std::ostream& operator << ( std::ostream& out, const ptr<hdfDataNode>& data )
{
	data->Print( out, 0 );
	return out;
}
