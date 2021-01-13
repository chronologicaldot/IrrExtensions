// (c) 2014 Nicolaus Anderson

#ifndef GUISCOLORSELECT_CPP
#define GUISCOLORSELECT_CPP

#include "GUISColorSelect.h"
#include <IGUIEnvironment.h>
#include <IVideoDriver.h>

namespace irr {
namespace gui {

using core::clamp;
using core::vector2d;
using core::floor32;
using core::ceil32;
using core::dimension2d;
using video::IVideoDriver;

GUISColorSelect::GUISColorSelect( IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id )
	: IGUISColorSelect( pEnvironment, pParent, pRect, id )
	, color(SOLID_BLACK) // solid black for HSL constructor
	, fieldAlpha(0)
	, HUE_RANGE( 360.f )
	, LUMINANCE_RANGE( 100.f )
	, SATURATION_RANGE( 100.f )
	, lastEvent( EGSCSE_None )
	, mouseStateFlags(0)
	, colorFieldRect(
			vector2di(padding),
			pRect.getSize() - dimension2d<s32>(2*padding, 30 + 2*padding)
			)
	, hueFieldRect(
			vector2di(padding, pRect.getHeight()-30+(2*padding)),
			vector2di(pRect.getWidth()-padding, pRect.getHeight()-padding)
			)
	, firstMousePos(0)
	, startHue(0)
	, lastColorFieldMousePos(0)
{
	colorFieldRect.repair();
	hueFieldRect.repair();

	updateHueFieldRectangles();
}

SColor GUISColorSelect::getColor()
{
	SColorf outf;
	color.toRGB(outf);
	return outf.toSColor();
}

SColor GUISColorSelect::getHueAsColor()
{
	SColorHSL outHSL(color.Hue, SATURATION_RANGE, LUMINANCE_RANGE);
	SColorf outf;
	outHSL.toRGB(outf);
	return outf.toSColor();
}

void GUISColorSelect::setColor( SColor pColor, bool notifyParent )
{
	SColorf cf(pColor);
	color.fromRGB(cf);
	fieldAlpha = u8(pColor.getAlpha());

	// Calculate color coordinates for the last color-field mouse position
	lastColorFieldMousePos.X =
		colorFieldRect.LowerRightCorner.X
		- (s32)( (color.Saturation / SATURATION_RANGE) * (f32)(colorFieldRect.getWidth()-1.f) );

	lastColorFieldMousePos.Y =
		colorFieldRect.LowerRightCorner.Y
		- (s32) ( (color.Luminance / LUMINANCE_RANGE) * (f32)(colorFieldRect.getHeight()-1) );

	// Notify parent
	if ( Parent && notifyParent )
		sendGUIEvent(EGET_EDITBOX_CHANGED);
}

void GUISColorSelect::setHue( SColor pColor, bool notifyParent )
{
	SColorf cf( pColor );
	SColorHSL chsl;
	chsl.fromRGB(cf);
	color.Hue = chsl.Hue;

	if ( Parent && notifyParent )
		sendGUIEvent(EGET_EDITBOX_CHANGED);
}

void GUISColorSelect::setAlpha( u8 pAlpha )
{
	fieldAlpha = pAlpha;
}

void GUISColorSelect::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();

	colorFieldRect.UpperLeftCorner.set(padding,padding);
	colorFieldRect.LowerRightCorner.set(
			vector2di(RelativeRect.getSize()) - vector2di(padding,30+2*padding)
			);

	hueFieldRect.UpperLeftCorner.set( padding, RelativeRect.getHeight()-30+(2*padding) );
	hueFieldRect.LowerRightCorner.set(
			RelativeRect.getWidth()-padding,
			RelativeRect.getHeight()-padding
			);

	updateHueFieldRectangles();
}

EGUISColorSelectEvent GUISColorSelect::getEvent()
{
	return lastEvent;
}

bool GUISColorSelect::OnEvent( const SEvent& event )
{
	if ( isEnabled() && isVisible() )
	switch ( event.EventType )
	{
	case EET_MOUSE_INPUT_EVENT:
		return onMouseEvent(event);

	case EET_KEY_INPUT_EVENT:
		return onKeyEvent(event);

	default: break;
	}
	return IGUIElement::OnEvent(event);
}

bool GUISColorSelect::onMouseEvent( const SEvent& event )
{
	vector2di lastMousePos( event.MouseInput.X, event.MouseInput.Y );

	/* Note: this GUI Element may respond even when not in focus.
	However, forcing it to respond when in focus forces the user to
	click on it, which can cause the color values to change, messing
	up the value. */
	switch ( event.MouseInput.Event )
	{
	case EMIE_LMOUSE_PRESSED_DOWN:
		setMouseState( MOUSE_LEFT_PRESSED, true );
		/* Check to see if the mouse is in the correct areas
			for modifying the color. */
		if ( colorFieldRect.isPointInside( lastMousePos - AbsoluteRect.UpperLeftCorner ) )
		{
			setMouseState( MOUSE_IN_COLOR_FIELD );
		}
		else if ( hueFieldRect.isPointInside( lastMousePos - AbsoluteRect.UpperLeftCorner ) )
		{
			setMouseState( MOUSE_IN_HUE_FIELD );
		}
		return true;

	case EMIE_LMOUSE_LEFT_UP:
		if ( getMouseState( MOUSE_LEFT_PRESSED ) )
		{
			resetMouseState( MOUSE_LEFT_PRESSED );
			resetMouseState( MOUSE_IN_COLOR_FIELD );
			resetMouseState( MOUSE_IN_HUE_FIELD );
			sendGUIEvent(EGET_EDITBOX_CHANGED);
		}
		return true;

	case EMIE_MMOUSE_PRESSED_DOWN:
		setMouseState( MOUSE_MIDDLE_PRESSED, true );
		firstMousePos = lastMousePos;
		startHue = color.Hue;
		return true;

	case EMIE_MMOUSE_LEFT_UP:
		resetMouseState( MOUSE_MIDDLE_PRESSED );
		sendGUIEvent(EGET_EDITBOX_CHANGED);
		return true;

	case EMIE_MOUSE_MOVED:
		if ( getMouseState( MOUSE_LEFT_PRESSED ) )
		{
			if ( getMouseState( MOUSE_IN_COLOR_FIELD ) )
			{
				setColorFieldFromPosition( lastMousePos - AbsoluteRect.UpperLeftCorner );
				sendGUIEvent(EGET_EDITBOX_CHANGED);
			}
			else if ( getMouseState( MOUSE_IN_HUE_FIELD ) )
			{
				setHueFieldFromPosition(
					(lastMousePos - AbsoluteRect.UpperLeftCorner - padding).X
					);
				sendGUIEvent(EGET_EDITBOX_CHANGED);
			}
			return true; // absorb anyways
		}
		else if ( getMouseState( MOUSE_MIDDLE_PRESSED ) )
		{
			// Change the hue and update the hue bar
			setHueFieldFromShift( lastMousePos.X - firstMousePos.X );

			sendGUIEvent(EGET_EDITBOX_CHANGED);
			return true;
		}
		return false;

	default: break;
	}

	return false;
}

void GUISColorSelect::sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement )
{
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = pElement;
	event.GUIEvent.EventType = pEventType;

	if ( Parent )
		Parent->OnEvent(event);
}

bool GUISColorSelect::onKeyEvent( const SEvent& event )
{
	switch ( event.KeyInput.Key )
	{
	case KEY_KEY_R:
		setColor( SOLID_RED );
		return true;

	case KEY_KEY_Y:
		setColor( SOLID_YELLOW );
		return true;

	case KEY_KEY_G:
		setColor( SOLID_GREEN );
		return true;

	case KEY_KEY_T:
		setColor( SOLID_TEAL );
		return true;

	case KEY_KEY_B:
		setColor( SOLID_BLUE );
		return true;

	case KEY_KEY_P:
		setColor( SOLID_PURPLE );
		return true;

	default: break;
	}
	return false;
}

void GUISColorSelect::setMouseState( c8 pState, bool pResetOthers )
{
	if ( !pResetOthers )
	{
		mouseStateFlags |= pState;
		return;
	}
	mouseStateFlags = pState;
}

void GUISColorSelect::resetMouseState( c8 pState )
{
	mouseStateFlags &= ~pState;
}

bool GUISColorSelect::getMouseState( c8 pState )
{
	return (mouseStateFlags & pState) == pState;
}

void GUISColorSelect::setColorFieldFromPosition( vector2di pPos )
{
	lastColorFieldMousePos = pPos;

	/* NOTE: Padding to the left has already been accounted for. */
	/* Move the position until it is in the field. */
	lastColorFieldMousePos.X = clamp( pPos.X,
									colorFieldRect.UpperLeftCorner.X,
									colorFieldRect.LowerRightCorner.X );

	lastColorFieldMousePos.Y = clamp( pPos.Y,
									colorFieldRect.UpperLeftCorner.Y,
									colorFieldRect.LowerRightCorner.Y );

	/* Color field appearance:
	white -------------------- white
	  |                          |
	  |                          |
	full sat color       unsaturated color
	  |                          |
	  |                          |
	black -------------------- black

	Meaning that the x position correlates with saturation
	and the y position correlates with lightness.
	*/

	f32 sat = SATURATION_RANGE * (
						(f32)((colorFieldRect.LowerRightCorner.X) - lastColorFieldMousePos.X)
						/ (f32)(colorFieldRect.getWidth()-1)
						);
	f32 lum = LUMINANCE_RANGE * (
						(f32)((colorFieldRect.LowerRightCorner.Y) - lastColorFieldMousePos.Y)
						/ (f32)(colorFieldRect.getHeight()-1)
						);

	color.Saturation = clamp( sat, 0.f, SATURATION_RANGE );
	color.Luminance = clamp( lum, 0.f, LUMINANCE_RANGE );
}

//=============================
// Wrong: It sets the color along the diagonal
// Preserved for reference

//void GUISColorSelect::setColorFieldFromPosition( vector2di pPos )
//{
//	lastColorFieldMousePos = pPos;
//
//	/* NOTE: Padding to the left has already been accounted for. */
//	/* Move the position until it is in the field. */
//	pPos.X = min_(
//				colorFieldRect.LowerRightCorner.X,
//				max_( colorFieldRect.UpperLeftCorner.X, pPos.X )
//				);
//	pPos.Y = min_(
//				colorFieldRect.LowerRightCorner.Y,
//				max_( colorFieldRect.UpperLeftCorner.Y, pPos.Y )
//				);
//
//	/* Color field appearance:
//	full sat color ----------- white
//	  |                          |
//	  |                          |
//	  |                          |
//	  |                          |
//	  |                          |
//	black -------------- unsaturated color
//
//	Note: (0,0) is the upper left corner, meaning the fully saturated
//	color lies on the rectangle's diagonal [(0,0),(w,h)] and the black
//	and white lies on the diagonal [(0,h),(w,0)]
//	*/
//
//	vector2d<f32> pos(pPos); // useful cast
//
//	/* Lightness depends only on the projection of the vector
//	onto the black-white diagonal. */
//	vector2di bw(0-colorFieldRect.getWidth(), colorFieldRect.getHeight()-0);
//	vector2d<f32> bw_f(bw);
//	f32 bw_length = bw_f.getLength();
//	f32 bw_projection = pos.dotProduct( bw_f.normalize() );
//
//	color.Luminance = (bw_projection / bw_length) * LUMINANCE_RANGE;
//
//	/* Saturation depends on the diagonal [(w,0),(0,h)] */
//	vector2di sat(colorFieldRect.getWidth(), colorFieldRect.getHeight());
//	vector2d<f32> sat_f(sat);
//	f32 sat_length = sat_f.getLength();
//	f32 sat_projection = pos.dotProduct( sat_f.normalize() );
//
//		// 1 - projection because progression along the diagonal is toward unsaturated
//	color.Saturation = (1.0f - sat_projection) * SATURATION_RANGE;
//}

void GUISColorSelect::setHueFieldFromPosition( s32 x )
{
	if ( x < 0 )
		x = 0;
	if ( x >= hueFieldRect.getWidth() )
		x = hueFieldRect.getWidth() - 1;

	/* NOTE: Padding to the left has already been accounted for. */
	f32 i = f32(x) * HUE_RANGE/(f32)(hueFieldRect.getWidth()-1);
	color.Hue = clamp( i, 0.f, HUE_RANGE );
}

void GUISColorSelect::setHueFieldFromShift( s32 x )
{
	color.Hue = startHue + (f32)x;
	// cancel overshoots
	while ( color.Hue < 0 )
	{
		color.Hue += HUE_RANGE;
	}
	while ( color.Hue >= HUE_RANGE )
	{
		color.Hue -= HUE_RANGE;
	}
}

void GUISColorSelect::updateHueFieldRectangles()
{
	f32 spacing = hueFieldRect.getWidth() / 6.f;
	/* Spacing, in terms of hue, is 60, but there are six color segments
	over a space of the hue field rectangle width.
	red = 0
	yellow = 60
	green = 120
	teal = 180
	blue = 240
	purple = 300
	red top = 360
	*/

	redYellowRect	= recti(0, 0, s32(spacing), hueFieldRect.getHeight()),
	yellowGreenRect	= recti(s32(spacing), 0, s32(spacing*2), hueFieldRect.getHeight()),
	greenTealRect	= recti(s32(spacing*2), 0, s32(spacing*3), hueFieldRect.getHeight()),
	tealBlueRect	= recti(s32(spacing*3), 0, s32(spacing*4), hueFieldRect.getHeight()),
	bluePurpleRect	= recti(s32(spacing*4), 0, s32(spacing*5), hueFieldRect.getHeight()),
	purpleRedRect	= recti(s32(spacing*5), 0, s32(spacing*6), hueFieldRect.getHeight());

	redYellowRect	+= AbsoluteRect.UpperLeftCorner + hueFieldRect.UpperLeftCorner;
	yellowGreenRect	+= AbsoluteRect.UpperLeftCorner + hueFieldRect.UpperLeftCorner;
	greenTealRect	+= AbsoluteRect.UpperLeftCorner + hueFieldRect.UpperLeftCorner;
	tealBlueRect	+= AbsoluteRect.UpperLeftCorner + hueFieldRect.UpperLeftCorner;
	bluePurpleRect	+= AbsoluteRect.UpperLeftCorner + hueFieldRect.UpperLeftCorner;
	purpleRedRect	+= AbsoluteRect.UpperLeftCorner + hueFieldRect.UpperLeftCorner;
}

void GUISColorSelect::draw()
{
	if ( !isVisible() )
		return;

	IGUISkin* skin = Environment->getSkin();

	// Draw color field border ----- (5-pixel padding is for this border)
	recti colorFieldBorder(
			vector2di(0),
			colorFieldRect.getSize() + dimension2d<s32>(2*padding, 2*padding)
			);
		// move to absolute location
	colorFieldBorder += AbsoluteRect.UpperLeftCorner;
		// draw
	skin->draw3DButtonPaneStandard(this, colorFieldBorder, &AbsoluteClippingRect);


	// Draw color field ---------
	SColorf fieldColorf;
	SColorHSL hsl = color;
	hsl.Luminance = 50;
	hsl.Saturation = 100;
	hsl.toRGB( fieldColorf );

	recti colorFieldRectAbs = colorFieldRect + AbsoluteRect.UpperLeftCorner;
	recti colorFieldRectAbs2 = colorFieldRectAbs;
	colorFieldRectAbs2.UpperLeftCorner.Y += colorFieldRect.getHeight()/2;
	colorFieldRectAbs.LowerRightCorner.Y = colorFieldRectAbs2.UpperLeftCorner.Y;

	/* Problem: Burning's isn't coloring the rectangles correctly, so
	to fix this, I need to change either
	a) Burnings (a hassle) or
	b) Create images that I draw myself (slow for drawing and chews resources) */

	// top rectangle
	Environment->getVideoDriver()->draw2DRectangle(
		colorFieldRectAbs,
		SOLID_WHITE, SOLID_WHITE, fieldColorf.toSColor(), SOLID_GREY,
		&AbsoluteClippingRect
		);

	// bottom rectangle
	Environment->getVideoDriver()->draw2DRectangle(
		colorFieldRectAbs2,
		fieldColorf.toSColor(), SOLID_GREY, SOLID_BLACK, SOLID_BLACK,
		&AbsoluteClippingRect
		);

	// should be ((SOLID_BLACK & 0x00ffffff) | (fieldAlpha<<24 & 0xff000000))

						// TO DO!!

	/* ^ Note above, the current problem is that there is no checkered pattern
	in the background in the case of transparent values. Furthermore, there is
	no handling of transparent values. That's on the to-do list.
	*/


	// Draw the color tracker -------
	recti colorTrackerRect(
				lastColorFieldMousePos - vector2di(2),
				lastColorFieldMousePos + vector2di(2)
				);
		// move to absolute location
	colorTrackerRect += AbsoluteRect.UpperLeftCorner;
		// draw
	Environment->getVideoDriver()->draw2DRectangleOutline( colorTrackerRect, SOLID_GREY );


	// Draw hue field border ----- (5-pixel padding is for this border also)
	recti hueFieldBorder(
			hueFieldRect.UpperLeftCorner - vector2di(padding),
			hueFieldRect.LowerRightCorner + vector2di(padding)
			);
		// move to absolute location
	hueFieldBorder += AbsoluteRect.UpperLeftCorner;
		// draw
	skin->draw3DButtonPaneStandard(this, hueFieldBorder, &AbsoluteClippingRect);


	// Draw hue field ----------
	/* Note: While it would be nice to use createGradientTexture(),
	the engine resizes textures to be squares, needlessly chewing up
	memory. However, it might be faster on the processor and would
	be more convenient for programming. */

	Environment->getVideoDriver()->draw2DRectangle(
					redYellowRect,
					SOLID_RED, SOLID_YELLOW,
					SOLID_RED, SOLID_YELLOW,
					&AbsoluteClippingRect
					);

	Environment->getVideoDriver()->draw2DRectangle(
					yellowGreenRect,
					SOLID_YELLOW, SOLID_GREEN,
					SOLID_YELLOW, SOLID_GREEN,
					&AbsoluteClippingRect
					);

	Environment->getVideoDriver()->draw2DRectangle(
					greenTealRect,
					SOLID_GREEN, SOLID_TEAL,
					SOLID_GREEN, SOLID_TEAL,
					&AbsoluteClippingRect
					);

	Environment->getVideoDriver()->draw2DRectangle(
					tealBlueRect,
					SOLID_TEAL, SOLID_BLUE,
					SOLID_TEAL, SOLID_BLUE,
					&AbsoluteClippingRect
					);

	Environment->getVideoDriver()->draw2DRectangle(
					bluePurpleRect,
					SOLID_BLUE, SOLID_PURPLE,
					SOLID_BLUE, SOLID_PURPLE,
					&AbsoluteClippingRect
					);

	Environment->getVideoDriver()->draw2DRectangle(
					purpleRedRect,
					SOLID_PURPLE, SOLID_RED,
					SOLID_PURPLE, SOLID_RED,
					&AbsoluteClippingRect
					);


	// Draw the hue field bar ------
	s32 hueBarX = (s32)( (f32)(hueFieldRect.getWidth()) * color.Hue / HUE_RANGE );
	recti hueFieldBar( hueBarX - 2, -2, hueBarX + 2, hueFieldRect.getHeight() + 2 );
		// Move to draw location
	hueFieldBar += hueFieldRect.UpperLeftCorner + AbsoluteRect.UpperLeftCorner;
		// draw
	Environment->getVideoDriver()->draw2DRectangleOutline(
					hueFieldBar, SOLID_GREY
					);
}

void GUISColorSelect::serializeAttributes(
	irr::io::IAttributes* out,
	irr::io::SAttributeReadWriteOptions* options
	)
{
	IGUIElement::serializeAttributes(out,options);

	out->addColor("Color", getColor());
}

void GUISColorSelect::deserializeAttributes(
	irr::io::IAttributes* in,
	irr::io::SAttributeReadWriteOptions* options
	)
{
	IGUIElement::deserializeAttributes(in,options);

	setColor(in->getAttributeAsColor("Color", getColor()));
}

}
}

#endif // #ifndef GUISCOLORSELECT_CPP
