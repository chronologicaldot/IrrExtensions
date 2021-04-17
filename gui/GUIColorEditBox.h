// (c) 2014 Nicolaus Anderson

#include <irrMath.h>
#include <IGUIElement.h>
#include <IGUIEnvironment.h>

#ifndef GUI_COLOR_EDITBOX_H
#define GUI_COLOR_EDITBOX_H

/* Class GUI Color Edit Box

A GUI Element that accepts only certain user input
(e.g. the character range '0'-'9', 'a'-'f', as well as some
useful editing keys).
The box always contains a valid, HTML-style color, and the
user input is such that it is overwriting, and that way,
8 characters are always present in the edit box.
*/

namespace irr
{
namespace gui
{

	using irr::video::SColor;
	using irr::core::recti;
	using irr::core::vector2di;
	using irr::core::dimension2du;

class GUIColorEditBox : public IGUIElement
{
protected:
	SColor color, cursorColor;
	s32 cursorIdx; // index of the cursor in the text
	recti cursorRect;
	u32 focusTime;

public:
	GUIColorEditBox( IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id=-1 );
	~GUIColorEditBox();

	SColor getColor();
	void setColor( SColor pColor, bool notifyParent=true );

	// Set the numeric text
	virtual void setText(const wchar_t* text);

	/* Handles user events.
	Because of how this element refreshes, the data/color should NOT
	be taken from this element until this element has lost focus,
	upon which it will call its parent. */
	virtual bool OnEvent( const SEvent& event );

	// Draws to the screen
	virtual void draw();

	// Post rendering (for cursor time)
	virtual void OnPostRender(u32 timeMs);

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "colorEditbox"; }

	// For completeness, we allow both setText and setColor to set the color
	virtual void serializeAttributes(
		irr::io::IAttributes* out,
		irr::io::SAttributeReadWriteOptions* options=0
		);

	virtual void deserializeAttributes(
		irr::io::IAttributes* in,
		irr::io::SAttributeReadWriteOptions* options=0
		);

protected:
	// Attempts to set the cursor position - Returns whether or not it could
	bool setCursorIndexFromPosition( vector2di pPos );
	
	// Sets the cursor rectangle to the cursor position
	void updateCursorRect();

	// Attempts to insert a character
	bool insertChar( EKEY_CODE pKeyCode, wchar_t pKey );

	// Internal conversion functions
	void convertTextToColor();
	void convertColorToText();

	// Erase data
	void clear();
};

}
}

#endif
