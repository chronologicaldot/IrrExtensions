/*
(c) 2012 Nicolaus Anderson

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

*/

#include <stdtypes.h>

#pragma once


/*
There are many situations in which lists and arrays are used. The choice of
which is used may be dependent on the implementation within the cpp file and
not the header. Rather than force a specific implementation within the header,
it may be better to provide a partially abstract class header that allows the
person who creates the final implementation to decide which is best.

This class provides a constant interface for adding, removing, and accessing
items in a list.

While this class is intended to be inherited from by a class that provides a
list implementation, it can be used as a container of a list or array class,
especially if the names of the storage class do not follow the general convention
of the program.
*/

template<class T>
class BigListInterface
{
public:
	
	//! Get list size
	/* For completeness, we return a u64 for size. */
	virtual stdu64 size()=0;
	
	//! Add to end
	/* Adds an object to the end of the list. */
	virtual void push_back( T item )=0;
	
	//! Add to front
	/* Adds an object to the front of the list. */
	virtual void push_front( T item )=0;
	
	//! Obtain item
	/* Returns the object. */
	virtual T& operator[] ( stdu64 index )=0;
	
	//! Obtain item
	/* Returns the object. */
	virtual T& get( stdu64 index )=0;
	
	//! Set item
	/* Sets the item at the given index. */
	virtual void set( stdu64 index, T item )=0;
	
	//! Insert
	/* Adds an item to the list at the given index. */
	virtual void insert( stdu64 index, T item )=0;

	//! Erase
	/* Removes an item from the list. */
	virtual void erase( stdu64 index )=0;

	//! Clear
	/* Empties the list of all of its items. */
	virtual void clear()=0;
	
	//! Sort list
	/* While irrlicht engine objects do not always have sort,
	lists in general have a sort function. */
	virtual void sort()=0;

	//! Expose data
	/* Returns a void pointer to the actual data. */
	virtual void* exposeData()=0;

	//! to string
	/* Returns the constant type of this object. Useful for type-checking. */
	virtual const stdc8* toString() { return "ListInterface"; }
};