#ifndef PTR_H_INCLUDED
#define PTR_H_INCLUDED

#include <map>

//! Implements common functionality for all types of smart pointers.
//! Since the smart pointer is a template object, each instantiation
//! would have its own static variable if the pointer lookup table
//! was placed in the template class. ptrBase avoids this problem by
//! placing the lookup table in a common base class.
class ptrBase
{
public:

	typedef std::map< const void *, int > ptrMapType;

	//! Returns pointer lookup table.
	//! Stored as a static local so it is created on demand.
	static ptrMapType & PointerTable()
	{
		static ptrMapType ptrMap;
		return ptrMap;
	}
};


//! Provides an implementation of a smart pointer.
//! The smart pointer owns the memory once it is constructed. As soon as data
//! is no longer referenced by a smart pointer, the memory will be released.
//! This behavior can be dangerous when mixing smart pointers with standard
//! pointers. Use Ptr() only to pass data to old functions that cannot be
//! updated. Storing the address of a smart pointer in a standard pointer will
//! cause problems if the smart pointer is freed and the standard pointer still
//! needs the data.
template<class dataType>
	class ptr : private ptrBase
{

public:

	//! Constructs a new smart pointer from a standard pointer.
	//! Creates shared reference counter with initial value 1.
	ptr( dataType * init = 0 ) : ptrBase(), data( init )
	{
		attach( init );
	}

	//! Creates a new smart pointer from an existing smart pointer.
	//! Maintains reference count and ownership.
	ptr( const ptr & other ) : ptrBase()
	{
		attach( other.data );
	}

	//! Assigns one smart pointer to another.
	//! Maintains reference count and ownership.
	const ptr & operator = ( const ptr & other )
	{
		if ( this != & other )
		{
			detach();
			attach( other.data );
		}

		return( *this );
	}

	//! Destroys a smart pointer.
	//! Maintains reference count and ownership.
	~ptr()
	{
		detach();
	}

	//! Converts a smart pointer to a smart pointer of a different type.
	//! Works for any two compatible pointer types.
	//! Allows conversion along an inheritance heirarchy.
	template<typename otherType>
		operator ptr<otherType>() const
	{
		return( ptr<otherType>( ( otherType * ) data ) );
	}

	//! Returns true if two smart pointers have the same memory address.
	bool operator == ( const ptr & other ) const
	{
		return( data == other.data );
	}

	//! Returns true if two smart pointers have different memory addresses.
	bool operator != ( const ptr & other ) const
	{
		return( data != other.data );
	}

	//! Returns true if this smart pointer's address is less than other's.
	bool operator < ( const ptr & other ) const
	{
		return( data < other.data );
	}

	//! Returns true if the smart pointer points to null.
	bool IsNull() const
	{
		return( data == 0 );
	}

	//! Returns true if the smart pointer does not point to null.
	bool IsValid() const
	{
		return( data != 0 );
	}

	//! Returns a standard pointer to the memory address of the smart pointer.
	dataType * Ptr() const
	{
		return( this->data );
	}

	//! Returns the number of smart pointers referencing the shared data.
	int Count() const
	{
		return( refCount() );
	}

	//! Smart pointer dereference.
	dataType & operator * () const
	{
		return( * data );
	}

	//! Smart pointer member access.
	dataType * operator -> () const
	{
		return( data );
	}

protected:

	dataType * data; //!< encapsulated standard pointer

	//! Returns a reference to the smart pointer's reference count.
	//! Creates a counter set to 0 if one does not exist.
	int & refCount() const
	{
		// search for pointer in lookup table
		ptrMapType::iterator ptrMatch = PointerTable().find( ( const void * ) data );

		// if no pointer has this address
		if ( ptrMatch == PointerTable().end() )
		{
			// create entry, set count to 0
			ptrMatch = PointerTable().insert( std::make_pair( ( const void * ) data, 0 ) ).first;
		}

		// return reference to count
		return (*ptrMatch).second;
	}

	//! Smart pointer shares data with other.
	void attach( dataType * other )
	{
		data = other;
		++refCount();
	}

	//! Smart pointer stops sharing data.
	//! Frees memory if no longer referenced.
	void detach()
	{
		if ( refCount() > 1 )
		{
			// decrement shared reference count
			--refCount();
		}
		else
		{
			// remove from pointer lookup table
			PointerTable().erase( ( const void * ) data );

			// free memory if last reference
			if ( data ) delete data;
		}

		// prevent accidental use of old data
		data = 0;
	}
};

#endif // PTR_H_INCLUDED
