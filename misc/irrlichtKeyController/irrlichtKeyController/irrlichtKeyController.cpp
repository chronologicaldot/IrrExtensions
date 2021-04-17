/*
(c) Nicolaus Anderson
Irrlicht engine (c) by Nikolaus Gebhardt

See irrlichtKeyController.h for copyright details.

Date created: April 26, 2012

Description: This is key controller class for handling when keys
are pressed. It allows for changes to the button configuration.
*/

#include <FunctionContainer.h>
#include <irrlicht.h>
#include "irrlichtKeyController.h"

#ifndef _IRR_KEY_CONTROLLER_C_
#define _IRR_KEY_CONTROLLER_C_


//******** MAIN CLASS FUNCTIONS ***********


//! Assign a function to a key
bool KeyController::assignToKey( bool newFunction(), EKEY_CODE key,
								bool keyState )
{
	if ( key < KEY_KEY_CODES_COUNT )
	{
		key_attr[ (s32)key ].set( newFunction, keyState );

		return true;
	} else
		return false;
} // end assignToKey


//*****************************************
//! Set active state (allow this to be used)
void KeyController::setActiveState( bool state )
{
	isActive = state;
} // end setActiveState


//*****************************************
//! Switch active state from previous
void KeyController::switchActiveState()
{
	// Is the flag "true"? - Then set it to false. Otherwise set it to true.
	isActive = isActive ? false : true;
} // end switchActiveState


//*****************************************
//! Respond to an event with the keyboard
bool KeyController::OnKeyChange( EKEY_CODE key, bool keyState )
{
	// Do something only if this is active
	if ( isActive )
	{
		return key_attr[ (s32) key ].use( keyState );
	}
} // end OnKeyChange

#endif