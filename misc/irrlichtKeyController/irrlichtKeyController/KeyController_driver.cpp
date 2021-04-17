/*
Driver for irrlichtKeyController class
(c) 2012 Nicolaus Anderson

Current progress:
irrlichtKeyController functions as expected.

Limitations:
You cannot pass functions from classes directly to the key
controller (since accessing member functions in classes in C++
is illegal). Thus, you must assign an independent function to
a key's function and have the independent function call the
class function you wish to use.
*/

#include <stdio.h>
#include <irrlicht.h>
#include "irrlichtKeyController.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;
using namespace core;
using namespace video;


// the number of the key being pressed
int key;

bool pressed()
{ printf( "\nKey pressed (%i)", key ); return true; }

bool released()
{ printf ( "\nKey released (%i)", key ); return true; }


class MyReceiver : public IEventReceiver
{
public:
	KeyController key_controller;

	MyReceiver()
	{
		key_controller.isActive = true;

		for ( s32 k = 0; k < KEY_KEY_CODES_COUNT; k++ )
		{
			key_controller.assignToKey( pressed, (EKEY_CODE)k, true );
			key_controller.assignToKey( released, (EKEY_CODE)k, false );
		}
	}

	virtual bool OnEvent( const SEvent& event )
	{
		if ( event.EventType == EET_KEY_INPUT_EVENT )
		{
			key = (int) event.KeyInput.Key;
		} else key = 0;

		return this->key_controller.OnEvent( event );
	}
};

void main()
{
	// Create a receiver
	MyReceiver receiver;

	// Create an engine
	IrrlichtDevice * dev;

	dev = createDevice(
		EDT_BURNINGSVIDEO, dimension2d<u32> (640,480), 16, false, false, false,
		&receiver // using test receiver
		);

	// Test presence of all functions for pressed keys
	printf( "\nDry run:\n" );
	for ( s32 k = 0; k < KEY_KEY_CODES_COUNT; k++ )
	{
		receiver.key_controller.OnKeyChange( (EKEY_CODE)k, true );
		receiver.key_controller.OnKeyChange( (EKEY_CODE)k, false );
	}

	printf( "\nDry run completed.\n\n" );

	// Loop
	while ( dev->run() );
}
