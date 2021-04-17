/*
(c) Nicolaus Anderson
*/

#include <irrlicht.h>

#include "UltraIcon.h"

#pragma once

namespace irr
{
namespace gui
{

//! Class Ultra List
/*
A list of Ultra Icon GUI elements.
>> It takes into account the sizes of the GUI elements and tries
to list them in such a way that they will fit within either
a specified width (or height) or the parent rectangle.
>> It also allows for searching through elements and displaying
only visible ones.
*/
class UltraList : public IGUIElement
{
protected:
	irr::s32 xSpacing;
	irr::s32 ySpacing;

public:
	UltraList(
		IGUIEnvironment* environment,
		IGUIElement* parent,
		irr::s32 id,
		irr::core::recti rectangle
		);

	~UltraList();

	/* Adds an Ultra Icon to the list.
	\param id - The ID of the element being added. */
	UltraIcon* addListItem( irr::s32 id );

	/* Remove an Ultra Icon.
	\param id - The ID of the element being added. */
	void removeListItem( irr::s32 id );

	// Spacing between icons
	void setHorizontalSpacing( irr::s32 gap );
	void setVerticalSpacing( irr::s32 gap );
	void setUniversalSpacing( irr::s32 gap ); // sets both horizontal and vertical

	// ------- Engine stuff ---------
	virtual void OnEvent( const irr::SEvent& event );
	virtual void draw();

	/* Hijacking this function - The given child is NOT added because
	the only elements wanted in this list are of the type UltraIcon,
	and the only way to add them is via addListItem(). */
	virtual void addChild( IGUIElement* child ) {}
};

}} // end namespace gui and irr