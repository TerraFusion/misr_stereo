#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

#include <new>
#include <map>
#include <list>
#include <string>
#include <typeinfo>

//! Records a singleton's dependencies for use in order of destruction control.
class singletonHolder
{
public:
	
	//! Constructor
	//! \param theName a name used for debugging purposes.
	singletonHolder( std::string theName );

	//! Destructor
    virtual ~singletonHolder();

	//! Returns the name of the singleton.
    std::string Name() const { return name; }

	//! Records that this singleton depends on another.
	//! This singleton will be destroyed before its dependencies.
    void DependsOn( singletonHolder * other );

	//! Prints information about this singleton's dependencies.
	void Print() const;
    
	//! Returns true if no singletons depend on this singleton.
	//! If true, this singleton can be safely destroyed.
	bool HasNoPredecessors() const;

	//! Returns true if another singleton depends on this one.
	bool HasPredecessor( singletonHolder* other ) const;

	//! Returns true if this singleton depends on another one.
	bool HasSuccessor( singletonHolder* other ) const;

    typedef std::list<singletonHolder*>::iterator iterator; //!< dependency list iterator type
    typedef std::list<singletonHolder*>::const_iterator const_iterator; //!< dependency list constant iterator type

protected:

    std::string name; //!< name for debugging purposes
    std::list<singletonHolder*> predecessors; //!< list of singletons that depend on this one
    std::list<singletonHolder*> successors; //!< list of singletons that this one depends on

	//! Remove dependency connection from another singleton on this singleton.
	void RemovePredecessor( singletonHolder * predecessor );

	//! Remove dependency connection from this singleton on another singleton.
	void RemoveSuccessor( singletonHolder * successor );
};

//! Provides a method of accessing a singleton like it is stored in a pointer.
//! operator -\> may be used to access a singleton's members. The singleton
//! type will be created on demand.
template <typename T>
class singleton
{
public:

	//! Returns a pointer to the managed singleton.
	T* operator * () { return Holder()->Instance(); }

	//! Overloaded member access operator.
	T* operator -> () { return Holder()->Instance(); }
	
	//! Implements type specific construction and destruction.
	class holder : public singletonHolder
	{
	public:
		
		holder()
		:
		singletonHolder( typeid( T ).name() ),
		instance( 0 )
		{

		}
		
		virtual ~holder()
		{
			Destroy();
		}
		
		T* Instance()
		{
			if ( instance == 0 )
			{
				Construct( instance = Allocate() );
			}
			return instance;
		}
		
	protected:

		static T* Allocate()
		{
			return ( T* ) operator new( sizeof( T ) );
		}
		
		static void Construct( T* allocated )
		{
			new( allocated ) T();
		}
				
		void Destroy()
		{
			instance->~T();
			operator delete( dynamic_cast<void*>( instance ) );
			instance = 0;
		}

		T* instance;
	};
	
	//! Returns the singleton holder for this type.
	static holder* Holder();

	//! Returns a pointer to the singleton instance.
	static T* Instance() { return Holder()->Instance(); }
	
	friend class singletonManager;

private:

	//! Specifies dependencies on other types.
	//! \see Dependencies
	template<typename S>
	static void DependsOn();

	//! Must be specialized for every type stored in a singleton, according to
	//! the following example. You must include a DependsOn<>() for each
	//! OTHER_TYPE that must not be destroyed until after this singleton
	//! is destroyed.
	//! \code
	//! template<>
	//!void singleton<TYPE>::Dependencies()
	//!{
	//!    DependsOn<OTHER_TYPE>();
	//!}
	//! \endcode
	static void Dependencies();
};

//! Keeps track of singletons and controls order of destruction.
//! Call Destroy() before the program exits to use order of destruction control.
//! Singletons will be destroyed in a order that satisfies their dependencies.
class singletonManager
{
public:
	
	//! Returns a pointer to an instance of T.
	//! Creates one if it does not already exist.
	template<typename T>
	static typename singleton<T>::holder * Get();

	//! Prints active singletons and their dependencies
	static void Print();
	
	//! Destroys all singletons, taking dependencies into account.
	static void Destroy();
	
protected:
	
	typedef std::map<void *,singletonHolder*> tableType; //!< active singletons table type
	typedef tableType::iterator iterator; //!< active singletons iterator type
	typedef tableType::const_iterator const_iterator; //!< active singletons constant iterator type

	//! Returns the table of active singletons
	static tableType & Active();
};

template<typename T>
typename singleton<T>::holder* singletonManager::Get()
{
	iterator result = Active().find( ( void * ) & typeid( T ) );
	if ( result == Active().end() )
	{
		typename singleton<T>::holder * newHolder = new typename singleton<T>::holder();
		Active()[ ( void * ) & typeid( T ) ] = newHolder;
		singleton<T>::Dependencies();
		return newHolder;
	}
	else
	{
		return dynamic_cast<typename singleton<T>::holder *>( result->second );
	}
}

template<typename T>
inline typename singleton<T>::holder* singleton<T>::Holder()
{
	return singletonManager::Get<T>();
}

template<typename T>
template<typename S>
inline void singleton<T>::DependsOn()
{
	singletonManager::Get<T>()->DependsOn( singletonManager::Get<S>() );
}

#endif // SINGLETON_H_INCLUDED
