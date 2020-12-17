// (c) 2014 Nicolaus Anderson

#ifndef GUISCOLORSELECT_H
#define GUISCOLORSELECT_H

#include "IGUISColorSelect.h"
#include "../video/Colors.h"

/*

NOTE TO SELF:

If you create an alpha-bar, it should be either to the right of the
color field or directly below the hue bar.

I could have made this of sub-elements
- one for the color field, which
would be merely a selection field returning values between 0 and 1 along
whatever horizontal, vertical, or diagonal you wanted
- two sliders for the hue and alpha bars, noting that the background of a
slider need not be drawn (so I could draw a hue gradient below it),
in which the sliders limit the selected range as well as both respond to
the middle mouse movement (if in focus)
(While it might annoy the user, the middle mouse button + direction in
the same direction as the orientation of the slider would matter, allowing
the user to control both hue and alpha (if on different bars), but not
if both bars are oriented the same. This could be very problematic.)

This level of abstraction would allow me to more easily control how
all of the pieces are placed on the screen.

*/

namespace irr {
namespace gui {

using core::vector2di;
using core::recti;
using video::SColor;
using video::SColorf;
using video::SColorHSL;

class GUISColorSelect : public IGUISColorSelect
{
protected:
	SColorHSL color;
	u8 fieldAlpha;

	const f32 HUE_RANGE;// = 360.f; // 60 between each color and 1st and last red
	const f32 LUMINANCE_RANGE;// = 100.f;
	const f32 SATURATION_RANGE;// = 100.f;

	EGUISColorSelectEvent lastEvent;

	u8 mouseStateFlags;
	static const u8 MOUSE_LEFT_PRESSED = 1;
	static const u8 MOUSE_MIDDLE_PRESSED = 2;
	static const u8 MOUSE_RIGHT_PRESSED = 4;
				/* Indicates the mouse at least WAS in
				the color fields upon the start of the
				click, even if not now. */
	static const u8 MOUSE_IN_COLOR_FIELD = 8;
	static const u8 MOUSE_IN_HUE_FIELD = 16;

private:
	static const s32 padding = 5; // padding for the color field rectangles
	recti colorFieldRect;
	recti hueFieldRect;
	vector2di firstMousePos;
	f32 startHue;
	vector2di lastColorFieldMousePos;

	// color rectangles for the hue field
	recti redYellowRect,
		yellowGreenRect,
		greenTealRect,
		tealBlueRect,
		bluePurpleRect,
		purpleRedRect;


public:
	GUISColorSelect( IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id=-1 );

	virtual SColor getColor();
	SColor getHueAsColor();

	void setColor( SColor pColor, bool notifyParent=true );
	void setHue( SColor pColor, bool notifyParent=true );
	void setAlpha( u8 pAlpha );

	/* To do...
	Add overrides for updating the Relative rectangle.
	This can be done by overridding only updateAbsolutePosition() */
	virtual void updateAbsolutePosition();

	virtual EGUISColorSelectEvent getEvent();
	virtual bool OnEvent( const SEvent& event );
private:
	bool onMouseEvent( const SEvent& event );
	void sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement=0 );
	bool onKeyEvent( const SEvent& event );
	inline void setMouseState( c8 pState, bool pResetOthers=false );
	inline void resetMouseState( c8 pState );
	inline bool getMouseState( c8 pState );
	void setColorFieldFromPosition( vector2di pPos );
	void setHueFieldFromPosition( s32 x );
	void setHueFieldFromShift( s32 x );

	void updateHueFieldRectangles();

public:
	virtual void draw();

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() const { return "sColorSelect"; }

	virtual void serializeAttributes(
		irr::io::IAttributes* out,
		irr::io::SAttributeReadWriteOptions* options=0
		);

	virtual void deserializeAttributes(
		irr::io::IAttributes* in,
		irr::io::SAttributeReadWriteOptions* options=0
		);
};

}
}

#endif // #ifndef GUISCOLORSELECT_H
