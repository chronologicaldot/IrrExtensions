// (c) 2014 Nicolaus Anderson

/* Color selection dialog. Notably, it allows selecting and returning
an SColor specifically (as opposed to other color types, such as SColorf.
This element should be able to be embedded in a window but does not need to be. */

#ifndef IGUISCOLORSELECT_H
#define IGUISCOLORSELECT_H

#include <SColor.h>
#include <IGUIElement.h>

namespace irr {

using core::recti;
using video::SColor;

namespace gui {

// For the window containing the color select dialog
enum EGUISColorSelectEvent
{
	EGSCSE_None,
	EGSCSE_ColorChanged,
	EGSCSE_COUNT
};

class IGUISColorSelect : public IGUIElement
{
public:
	IGUISColorSelect( IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id=-1 )
		: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	{}

	//! Get the color
	/* Returns the ARGB color of this dialog, regardless of if it stores HSV natively. */
	virtual SColor getColor()=0;

	//! Get event
	/* Returns the event that occurs if, for example, the user interacts with it. */
	virtual EGUISColorSelectEvent getEvent()=0;
};

}
}

#endif // #ifndef IGUISCOLORSELECT_H
