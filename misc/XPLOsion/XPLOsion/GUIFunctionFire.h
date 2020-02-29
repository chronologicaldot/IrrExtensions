/*
Project name:
GUI Function Fire

Created by Nicolaus Anderson, 2012

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

Purpose:
Sometimes it is necessary to associate functions with certain GUI elements,
especially those that are interacted with by the user via clicking but do not fire.
This class is aimed handling clicking on seemingly non-responsive items such as
divisions and buttons by associating them with functions.
In essence, it acts as an event receiver for buttons and divisions.
*/

#include <irrlicht.h>
#include <LinusListix.h>
#include "IDedFunc.h"


#pragma once

class GUIFunctionFire : public irr::IReferenceCounted, public irr::IEventReceiver
{
	/* Pointer to the GUI environment in order to interact with the buttons. */
	irr::gui::IGUIEnvironment* environ;

	/* List containing all of the functions associated with GUI interaction. */
	Linus::LListix<IDedFunc*> funcs;

public:

	//! Constructor
	GUIFunctionFire(
		irr::gui::IGUIEnvironment* environment
		)
	{
		environ = environment;
	}

	//! Deconstructor
	~GUIFunctionFire()
	{
		funcs.clear();
	}

		//! Find function with name
	/* Returns the index of the function whose name matches the given.
	If no function is found, it returns -1. */
	irr::s32 findFunctionByName( irr::core::stringc funcName );

		//! Register function class
	/* Registers a class that responds to a function so that
	the function can be used by the GUI buttons. */
	void registerFunctionClass( IDedFunc* funcclass );

		//! Assign to function
	/* Assigns a function to a respondant. If the respondant isn't available,
	a dummy function will be made. */
	void giveToFunction( irr::core::stringc funcName, irr::s32 gui_elem_id );

		// Call function with respondant
	/* Attempts to find a function that is set to respond when the GUI element
	possessing the given ID is interacted with. If the function is found, it
	is called. 
	Returns true only if the function was found AND returns true itself.
	By not returning true, the function could be deferring processing to
	another function. */
	bool callFunctionWithRespondant( irr::s32 id );

		//! On event
	/* Responds to interaction with the GUI environment by the user. */
	virtual bool OnEvent( const irr::SEvent& event );
};