/*
(c) Nicolaus Anderson
*/

#include <irrlicht.h>

#pragma once

//! Class UltraIcon
/*
This is like a desktop icon.
It should handle the displaying of its text and icon based on the area it is allotted.
*/
class UltraIcon : public IGUIElement, public Searchable
{
public:
	UltraIcon(
		IGUIEnvironment* environment,
		IGUIElement* parent,
		irr::s32 id,
		irr::core::recti rectangle
		);

	~UltraIcon();

	void setFontSize( irr::s32 size );

	void setTextArea( irr::core::dimension2du area );

	void setIconArea( irr::core::dimension2du area );

	void setText( const wchar_t* text );

	void setIconSprite( irr::video::ITexture* sprite );

	/* Sets whether the text should be visible or not.
	If the text is not visible, the size of the sprite
	should be used to determine the bounding box of the element. */
	void setTextVisible( bool setting );

	/* Sets whether the icon should be visible or not.
	If the icon is not visible, the text area should be used
	to determine the bounding box of the element. */
	void setIconVisible( bool setting );

	//! Should show
	/* Indicates whether this element should even show up in lists. */
	virtual void shouldShow( bool setting );

	// ------- Engine stuff ---------
	virtual void OnEvent( const irr::SEvent& event );
	virtual void draw();
};
