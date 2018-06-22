#include "hdfDataProjector.h"
#include "projector/som.h"
#include "hdfDataSource.h"
#include <iostream>

hdfDataProjector::hdfDataProjector( ptr<projector> theProjector )
:
hdfDataNode( 1 ),
proj( theProjector )
{

}

hdfDataProjector::hdfDataProjector( ptr<hdfDataNode> input0, ptr<projector> theProjector )
:
hdfDataNode( 1 ),
proj( theProjector )
{
    setInput( 0, input0 );
}

hdfDataProjector::~hdfDataProjector()
{

}

std::string hdfDataProjector::Name() const
{
	return "dataProjector";
}

void hdfDataProjector::PrintAttributes( std::ostream& out, const std::string& prefix ) const
{
	out << prefix << "projector = " << proj->Name() << '\n';
}

ptr<projector> hdfDataProjector::Projector() const
{
    return proj;
}

void hdfDataProjector::setProjector( ptr<projector> theProjector )
{
    proj = theProjector;
}

void hdfDataProjector::setOrbit( int orbit )
{
	hdfDataNode::setOrbit( orbit );
	const hdfDataSource* ds = FindDataSource();
	if( ds )
	{
		hdfField field = ds->DataField();
		if( field.IsValid() )
		{
			setProjector( new projectorSom( field->ProjectionParameters(), field->BlockRect() ) );
		}
		else
		{
			setProjector( 0 );
		}
	}
}

bool hdfDataProjector::Check() const
{
	return proj.IsValid() && hdfDataNode::Check();
}

hdfValue hdfDataProjector::Value( const hdfCoord & location )
{
    return inputs[ 0 ]->Value( proj->Project( location ) );
}

std::vector<hdfValue> hdfDataProjector::Values( const std::vector<hdfCoord> & location )
{
	std::vector<hdfCoord> projected( location.size() );
	for( unsigned int i = 0; i < location.size(); i++ )
	{
		projected[ i ] = proj->Project( location[ i ] );
	}
	return inputs[ 0 ]->Values( projected );
}

std::list<hdfPolygon> hdfDataProjector::GetMask() const
{
    return proj->UnProject( inputs[ 0 ]->GetMask() );
}

bool hdfDataProjector::IsProjector() const
{
	return true;
}

bool hdfDataProjector::IsEqualTo( ptr<hdfDataNode> other ) const
{
	hdfDataProjector* casted = dynamic_cast<hdfDataProjector*>( other.Ptr() );
	return proj->IsEqualTo( casted->proj.Ptr() );
}

void hdfDataProjector::PromoteProjectorsTo( ptr<hdfDataNode>& parent, ptr<hdfDataNode>& dest )
{
	if( parent != dest )
	{
		ptr<hdfDataNode> old = inputs[0];
		inputs[0] = dest;
		dest = this;
		parent = old;
		inputs[0]->PromoteProjectorsTo( inputs[0], inputs[0] );
	}
	inputs[0]->PromoteProjectorsTo( inputs[0], inputs[0] );
}
