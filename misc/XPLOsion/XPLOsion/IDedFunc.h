/*
Created by Nicolaus Anderson, 2012

For license terms, see GUIFunctionFire.h
*/

#include <irrlicht.h>
#include <FunctionContainer.h>

#pragma once

//! Function class
/*
This class is designed for housing a function that is to respond to a GUI
operation.
It has two special functions:
matches() - Called to see if the ID matches this one's ID string.
call() - Called when the function is used by a GUI object.
*/
class IDedFunc
{
protected:
	//irr::s32 ID;

	irr::core::stringc my_name;

	/* GUI elements that are to respond by using this function */
	irr::core::array<irr::s32> respondants;

public:
	IDedFunc()
	{}

	IDedFunc( irr::core::stringc name )
	{
		my_name = name;
	}

	~IDedFunc()
	{
		respondants.clear();
	}

	/*
	void setID( irr::s32 id )
	{
		ID = id;
	}

	irr::s32 getID()
	{
		return ID;
	}

	bool matches( irr::s32 id )
	{
		return ID == id;
	}
	*/

	void setName( irr::core::stringc name )
	{
		my_name = name;
	}

	irr::core::stringc getName()
	{
		return my_name;
	}

	bool matches( irr::core::stringc name )
	{
		return my_name == name;
	}

	bool hasRespondant( irr::s32 id )
	{
		return (respondants.linear_search( id ) != -1);
	}

	void addRespondant( irr::s32 id )
	{
		// Don't add if already in the list
		if (respondants.linear_search( id ) != -1)
			respondants.push_back( id );
	}

	void removeRespondant( irr::s32 id )
	{
		irr::s32 index;

		index = respondants.linear_search( id );

		// Don't try to remove if it isn't even there
		if ( index != -1 )
			respondants.erase( index );
	}

	//! Call function
	/* This is called when the class in to respond to a GUI
	event. */
	virtual bool call() { return false; }
};


//! Class Simple Func Class
/* Simple implementation of IdedFunc that allows the storage of a
function. */
class SimpleIdedFunc : public FunctionContainer, IDedFunc
{
public:
	virtual bool call()
	{
		return this->use();
	}
};