#include <stdio.h>
#include <algorithm>
#include "ptr.h"
#include "singleton.h"

using namespace std;

singletonHolder::singletonHolder( std::string theName )
	:
name( theName ),
predecessors(),
successors()
{

}

singletonHolder::~singletonHolder()
{
	for_each( predecessors.begin(), predecessors.end(), bind2nd( mem_fun( & singletonHolder::RemoveSuccessor ), this ) );
	for_each( successors.begin(), successors.end(), bind2nd( mem_fun( & singletonHolder::RemovePredecessor ), this ) );
}

bool singletonHolder::HasPredecessor( singletonHolder * other ) const
{
	return count( predecessors.begin(), predecessors.end(), other ) > 0;
}

bool singletonHolder::HasSuccessor( singletonHolder * other ) const
{
	return count( successors.begin(), successors.end(), other ) > 0;
}

bool singletonHolder::HasNoPredecessors() const
{
	return predecessors.empty();
}

void singletonHolder::RemovePredecessor( singletonHolder * predecessor )
{
	if ( predecessor != this )
	{
		predecessors.remove( predecessor );
	}
}

void singletonHolder::RemoveSuccessor( singletonHolder * successor )
{
	if ( successor != this )
	{
		successors.remove( successor );
	}
}

void singletonHolder::DependsOn( singletonHolder * instance )
{
	if ( instance == 0 )
	{
		printf( "[singleton] ERROR: can't make %s depend on NULL\n", Name().c_str() );
	}
	else if ( instance == this )
	{
		printf( "[singleton] ERROR: can't make %s depend on itself\n", Name().c_str() );
	}
	else
	{
		//printf( "[singleton] %s depends on %s\n", Name().c_str(), instance->Name().c_str() );
		successors.push_back( instance );
		instance->predecessors.push_back( this );
	}
}

void singletonHolder::Print() const
{
	printf( "[singleton]  %s\n", Name().c_str() );
	for ( const_iterator pred = predecessors.begin() ; pred != predecessors.end() ; pred++ )
	{
		printf( "[singleton]   pred: %s\n", (*pred)->Name().c_str() );
	}
	for ( const_iterator succ = successors.begin() ; succ != successors.end() ; succ++ )
	{
		printf( "[singleton]   succ: %s\n", (*succ)->Name().c_str() );
	}
}

void singletonManager::Print()
{
	printf( "[singleton] list of active singletons:\n" );
	for ( const_iterator instance = Active().begin() ; instance != Active().end() ; instance++ )
	{
		instance->second->Print();
	}
}

void singletonManager::Destroy()
{
	while ( ! Active().empty() )
	{
		// find a singleton with no predecessors
		iterator instance = Active().begin();
		while ( instance != Active().end() )
		{
			if ( instance->second->HasNoPredecessors() ) break;
			instance++;
		}

		// if no such singleton exists, there is a cycle
		if ( instance == Active().end() )
		{
			printf( "[singleton] ERROR: cyclic dependency found\n" );
			Print();
			break;
		}
		else // destroy the singleton
		{
			//Print();
			//printf( "[singleton] destroying %s\n", instance->second->Name().c_str() );
			delete instance->second;
			Active().erase( instance );
		}
	}
}

singletonManager::tableType & singletonManager::Active()
{
	static tableType active;
	return active;
}
