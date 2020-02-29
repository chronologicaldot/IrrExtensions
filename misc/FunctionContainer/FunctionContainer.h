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

#pragma once

//! FunctionContainer
/* Holds the function to be used when a key is pressed. */
class FunctionContainer
{
	//! Function to use
	bool (*function) ();

	//! If the function is available
	bool hasFunc;

public:
	//! Constructor
	FunctionContainer()
	{
		hasFunc = false;
	}

	//! Constructor with passed function
	FunctionContainer( bool newFunction() )
	{
		// Save a pointer to the passed function
		function = newFunction;

		// Indicate that a function can be used
		hasFunc = true;
	}

	//! If there is a function that can be used
	bool isUsable()
	{
		return hasFunc;
	}

	//! Assign a new function
	void set( bool newFunction() )
	{
		// Save a pointer to the passed function
		function = newFunction;

		// Indicate that a function can be used
		hasFunc = true;
	}

	//! Use the assigned function
	bool use()
	{
		if ( hasFunc )
			return function();
		else return false;
	}
}; // end FunctionContainer