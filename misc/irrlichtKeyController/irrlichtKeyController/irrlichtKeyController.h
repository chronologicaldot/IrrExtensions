/*
(c) Nicolaus Anderson
Irrlicht engine (c) by Nikolaus Gebhardt

Date created: April 26, 2012

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

Description: This is key controller for handling when keys
are pressed. It allows for changes to the button configuration.
*/

#include <FunctionContainer.h>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace gui;

#ifndef _IRR_KEY_CONTROLLER_H_
#define _IRR_KEY_CONTROLLER_H_

// total keys:
// KEY_KEY_CODES_COUNT

// Constants
#define ISKEY_PRESSED true
#define ISKEY_RELEASED false


//******** SUPPORTING CLASSES ***********


//! Key Functions Container
/* Class that holds the pair of functions associated with
the pressed and released states of a key. */
class KeyFunctionsContainer
{
	// Function for being pressed
	FunctionContainer Pressed;
	FunctionContainer Released;

public:
	//! Assign a function to a key state
	void set( bool newFunction(), bool keyState )
	{
		if ( keyState == ISKEY_PRESSED )
			Pressed.set( newFunction );
		else
			Released.set( newFunction );
	}

	//! See if a function can be used
	bool isUsable( bool keyState )
	{
		if ( keyState == ISKEY_PRESSED )
			return Pressed.isUsable();
		else
			return Released.isUsable();
	}

	//! Use the function associated with the key
	bool use( bool keyState )
	{
		if ( keyState == ISKEY_PRESSED )
			return Pressed.use();
		else
			return Released.use();
	}

}; // end KeyFunctionsContainer


//*************** MAIN CLASS ******************

class KeyController
{
private:
	// Identification tag of this instance
	s32 ID;

	// List of attributes assigned to keys, including their functions
	KeyFunctionsContainer key_attr[ KEY_KEY_CODES_COUNT ];

public:
	//! is active
	/* Determines if the key controller should respond when keys
	are pressed. Set this to false when you wish to use either
	another key controller or are having the user change the functions
	of the keys during runtime. */
	bool isActive;


	//! Constructor
	KeyController( s32 id=0 )
	{
		ID = id;
		isActive = false; // assumer the programmer doesn't want this active at first
	}

	//! Get the ID of this instance
	s32 getID() const
	{
		return ID;
	}

	//! Set the ID of this instance
	void setID( s32 newID )
	{
		ID = newID;
	}

	//! Assign a function to a key
	/* Assigns a function to respond to key input. The type of input
	(pressed down or released) is determined by keyState. Note that a key
	can have a response to either being pressed down or being released
	OR BOTH.
	\param newFunction - The function to assign.
	\param key - The key to assign (of type EKEY_CODE, which can be casted to
	from s32 since it's just an integer.
	\param keyState - The key state, either pressed down (true) or released (false)
	that this function responds to. */
	bool assignToKey( bool newFunction(), EKEY_CODE key, bool keyState=ISKEY_PRESSED );

	//! Set active state (allow the functions in this class to be used)
	void setActiveState( bool state );

	//! Switch active state from previous
	void switchActiveState();

	//! Respond to an event with the keyboard
	/* Call this function inside your class that inherits IEventReceiver
	if you already have the key code ( type == irr::EKEY_CODE ) and know
	the state of the key. */
	bool OnKeyChange( EKEY_CODE key, bool keyState );

	//! Respond to user interaction
	/* Reacts to a key input event. Returns true if it reacted. */
	bool OnEvent( const SEvent & event )
	{
		// Respond only if this is a key input event
		if ( event.EventType == EET_KEY_INPUT_EVENT )
		{
			return OnKeyChange( event.KeyInput.Key, event.KeyInput.PressedDown );
		} else
			return false;
	}
};

#endif