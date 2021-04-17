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
  
  Since this uses the irrlicht engine, see irrlicht.h for more details.
*/

#include <BigListInterface.h>

#pragma once

using namespace Linus;


template<class T>
class BigLinusHood : public BigListInterface<T>
{
	BigLListix<T> list;

public:
	BigLinusHood<T>() : BigListInterface<T>()
	{
	}

	~BigLinusHood<T>()
	{
		list.clear();
	}

	//! Get list size
	/* By convention, irrlicht objects return a u32 for size. */
	stdu64 size()
	{
		return (stdu64)list.Size();
	}
	
	//! Add to end
	/* Adds an object to the end of the list. */
	void push_back( T item )
	{
		list.push_back( item );
	}
	
	//! Add to front
	/* Adds an object to the front of the list. */
	void push_front( T item )
	{
		list.push_front( item );
	}
	
	//! Obtain item
	/* Returns the object. */
	T& operator[] ( stdu64 index )
	{
		return list[(stds64)index];
	}
	
	//! Obtain item
	/* Returns the object. */
	T& get( stdu64 index )
	{
		return list[(stds64)index];
	}
	
	//! Set item
	/* Sets the item at the given index. */
	void set( stdu64 index, T item )
	{
		list.replace( (stds64)index, item );
	}
	
	//! Insert
	/* Adds an item to the list at the given index. */
	void insert( stdu64 index, T item )
	{
		list.insert( item, (stds64)index );
	}

	//! Erase
	/* Removes an item from the list. */
	void erase( stdu64 index )
	{
		list.deleteAt( (stds64)index );
	}

	//! Clear
	/* Empties the list of all of its items. */
	void clear()
	{
		list.clear();
	}
	
	//! Sort list
	/* While irrlicht engine objects do not always have sort,
	lists in general have a sort function. */
	virtual void sort() {}

	//! Expose data
	/* Returns a void pointer to the actual data. */
	virtual void* exposeData()
	{
		return (void*)&list;
	}

	//! to string
	/* Returns the constant type of this object. Useful for type-checking. */
	const stdc8* toString() { return "LinusHood"; }
};
