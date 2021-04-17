/*
Created by Nicolaus Anderson, 2012

For license terms, see xplo.h
*/

#include <irrlicht.h>

#ifndef __INCLUDE_GUI_ELEM_ACCEPTOR__
#define __INCLUDE_GUI_ELEM_ACCEPTOR__

//! Class GUI Element Acceptor
/* In order to completely separate the GUI setup from functional classes
(that is, those whose function is independent of the way the GUI elements are
styled), this class should be inherited.
It is an interface by which a class can receive a GUI element and decide if
it wants to become dependent on / respond to the activity of that GUI element.

Reasoning:
In programming, it is hard to save enumeration data type data in a file,
whereas if the GUI was entirely built within the code, it would be simple
enough to use:
[code]
enum GUI_IDs { GI_1 = 1 };

IGUIElement* elem = envir->addButton();

elem->setID( GI_1 );
[/code]
... and inside of the functional class, associate activity with the GUI
element with the ID "GI_1".

When loading a GUI element from a file, we don't have this option, so
this class gives a psuedo-similar option, using a string (the "BindingHint")
instead of an enumeration.
*/
class GUIElemAcceptor
{
public:

	//! Receive and Accept
	/* Accepts the given GUI element and determines if the class (inheriting
	this one) should save a pointer to the GUI element or drop it.
	\param elem - The GUI element to be examined.
	\param BindingHints - Information to help the function decide if a pointer
	to the GUI element should be saved.
	\return - "true" if it accepted the GUI element and saved a pointer to it. */
	virtual bool bind( irr::gui::IGUIElement* elem, const irr::c8* BindingHint )=0;
};

#endif // #ifndef __INCLUDE_GUI_ELEM_ACCEPTOR__
