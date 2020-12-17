// (c) 2014 Nicolaus Anderson

#ifndef GUI_COLOR_SAMPLE_CPP
#define GUI_COLOR_SAMPLE_CPP

#include "GUIColorSample.h"
#include <IVideoDriver.h>

namespace irr {
namespace gui {

using irr::core::vector2d;
using irr::video::IVideoDriver;

GUIColorSample::GUIColorSample( IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, upperLeft(0)
	, upperRight(0)
	, lowerLeft(0)
	, lowerRight(0)
	, drawBorder(false)
	, borderWidth(3)
//	, colorArea(pRect)
{
	colorArea = AbsoluteClippingRect;
}

void GUIColorSample::setDrawBorder(bool yes)
{
	drawBorder = yes;

	colorArea = AbsoluteClippingRect;

	if ( drawBorder )
	{
		colorArea.UpperLeftCorner += vector2d<s32>(borderWidth);
		colorArea.LowerRightCorner -= vector2d<s32>(borderWidth);
	}
}

void GUIColorSample::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();

	colorArea = AbsoluteClippingRect;

	if ( drawBorder )
	{
		colorArea.UpperLeftCorner += vector2d<s32>(borderWidth);
		colorArea.LowerRightCorner -= vector2d<s32>(borderWidth);
	}
}

void GUIColorSample::showOneColor( SColor pColor )
{
	upperLeft = pColor;
	upperRight = pColor;
	lowerLeft = pColor;
	lowerRight = pColor;
}

void GUIColorSample::showTwoColors( SColor pTopLeft, SColor pBottomRight, bool horizontal )
{
	upperLeft = pTopLeft;
	lowerRight = pBottomRight;
	if ( horizontal )
	{
		upperRight = pTopLeft;
		lowerRight = pBottomRight;
	} else {
		upperRight = pBottomRight;
		lowerRight = pTopLeft;
	}
}

void GUIColorSample::showFourColors( SColor pUpperLeft, SColor pUpperRight, SColor pLowerLeft, SColor pLowerRight )
{
	upperLeft = pUpperLeft;
	upperRight = pUpperRight;
	lowerLeft = pLowerLeft;
	lowerRight = pLowerRight;
}

bool GUIColorSample::OnEvent( const SEvent& event )
{
	/* This element cannot be set in focus. */
	if ( !isVisible() || !isEnabled() )
	if ( event.EventType == EET_GUI_EVENT
		&& event.GUIEvent.Caller == this
		&& event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
	{
		if ( Parent )
			Environment->setFocus( Parent );
		else
			Environment->setFocus(0);
		return true;
	}

	return IGUIElement::OnEvent(event);
}

void GUIColorSample::draw()
{
	if ( isVisible() )
	{
		if ( drawBorder )
		{
			Environment->getSkin()->draw3DSunkenPane(
										this,
										Environment->getSkin()->getColor(EGDC_3D_FACE),
										true, false,
										AbsoluteRect,
										&AbsoluteClippingRect
										);
		}

		Environment->getVideoDriver()->draw2DRectangle(
					colorArea,
					upperLeft, upperRight, lowerLeft, lowerRight,
					&AbsoluteClippingRect
					);
	}
}

void GUIColorSample::serializeAttributes(
	irr::io::IAttributes* out,
	irr::io::SAttributeReadWriteOptions* options
	)
{
	IGUIElement::serializeAttributes(out,options);

	out->addColor("UpperLeftColor", upperLeft);
	out->addColor("UpperRightColor", upperRight);
	out->addColor("LowerLeftColor", lowerLeft);
	out->addColor("LowerRightColor", lowerRight);
	out->addBool("DrawBorder", drawBorder);
	out->addInt("BorderWidth", borderWidth);
}

void GUIColorSample::deserializeAttributes(
	irr::io::IAttributes* in,
	irr::io::SAttributeReadWriteOptions* options
	)
{
	IGUIElement::deserializeAttributes(in,options);

	upperLeft = in->getColor("UpperLeftColor", upperLeft);
	upperRight = in->getColor("UpperRightColor", upperRight);
	lowerLeft = in->getColor("LowerLeftColor", lowerLeft);
	lowerRight = in->getColor("LowerRightColor", lowerRight);
	borderWidth = in->getInt("BorderWidth", borderWidth);
	setDrawBorder( in->getBool("DrawBorder", drawBorder) );
}

}}

#endif // #ifndef GUI_COLOR_SAMPLE_CPP
