// By Nicolaus Anderson, 2018
// Based on code posted by Ethon for free on Irrlicht
// Thanks Ethon!

#include <type_traits>
#include <IReferenceCounted.h>

#ifndef __IRRPTR_H__
#define __IRRPTR_H__

// Uncomment to enable
//#define IRRPTR_CHECK_ACCESS

#ifdef IRRPTR_CHECK_ACCESS
class IrrPtrAccessNullException {};
#endif

namespace irr {
namespace tools {

template<typename ObjectType>
class irrptr : std::enable_if< std::is_base_of< irr::IReferenceCounted, ObjectType >::value >
{
public:
	typedef typename std::remove_pointer< ObjectType >::type  element_type;
	typedef element_type* pointer;
	typedef element_type const* const_pointer;

	irrptr()
		: dataptr(nullptr)
	{}

	irrptr( const irrptr<element_type>&  other )
		: dataptr( other.dataptr )
	{
		grab();
	}

	irrptr<element_type>&
	operator= ( irrptr<element_type>& other ) {
		set(other.dataptr);
		return *this;
	}

	~irrptr() {
		drop();
	}

	void
	set( pointer  data ) {
		if ( dataptr == data )
			return;
		drop();
		if ( data == 0 ) // Force nullptr usage
			dataptr = nullptr;
		else
			dataptr = data;
		grab();
	}

	void
	share( irrptr<element_type>& other ) {
		if ( dataptr == other.dataptr )
			return;
		drop();
		dataptr = other.dataptr;
		grab();
	}

	void
	swap( irrptr<element_type>& other ) {
		pointer  p = other.dataptr;
		other.dataptr = dataptr;
		dataptr = p;
	}

	void
	dump() {
		drop();
	}

	void
	quiet_dump() {
		// No drop
		dataptr = nullptr;
	}

	pointer
	get() {
		return dataptr;
	}

	const_pointer
	get() const
	{
		return dataptr;
	}

	element_type&
	access()
	{
#ifdef IRRPTR_CHECK_ACCESS
		if ( !dataptr )
			throw IrrPtrAccessNullException();
#endif
		return *dataptr;
	}

	element_type const&
	access() const
	{
#ifdef IRRPTR_CHECK_ACCESS
		if ( !dataptr )
			throw IrrPtrAccessNullException();
#endif
		return *dataptr;
	}

	explicit operator bool() const
	{
		return (dataptr != nullptr);
	}
	

protected:
	void grab() {
		if ( dataptr )
			dataptr->grab();
	}

	void drop() {
		if ( dataptr )
			dataptr->drop();
	}

private:
	pointer  dataptr;
};

}}
 
namespace std
{
   template<class ObjectType>
   void swap(  ::irr::tools::irrptr<ObjectType>& p1,
			   ::irr::tools::irrptr<ObjectType>& p2)
   {
	  p1.swap(p2);
   }
}
 
#endif // _IRRPTR_HPP__
