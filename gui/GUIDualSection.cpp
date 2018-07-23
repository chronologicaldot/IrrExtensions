// (c) 2015 Nicolaus Anderson

#ifndef GUI_DUAL_SECTION_CPP
#define GUI_DUAL_SECTION_CPP

#include "GUIDualSection.h"
#include <irrMath.h>
#include <IGUISkin.h>
#include <IGUIEnvironment.h>

namespace irr {
namespace gui {

using core::recti;

GUIDualSection::GUIDualSection( bool pVertical, f32 pShift, IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, sectionTopLeft(0)
	, sectionBottomRight(0)
	, vertical( pVertical )
	, dragBarSize(10)
	, currShift(pShift) // Normal is 0.5f
	, dragBarRect()
	, lastMousePos()
	, currMousePos()
	, dragging(false)
{
	sectionTopLeft = new IGUIElement( EGUIET_ELEMENT, Environment, this, -1, pRect );
	sectionBottomRight = new IGUIElement( EGUIET_ELEMENT, Environment, this, -1, pRect );
	sectionTopLeft->setSubElement(true);
	sectionBottomRight->setSubElement(true);

	// set left, right, top, bottom alignments
	setVerticalNoUpdateAbs( vertical );

	cramSections();
}

GUIDualSection::~GUIDualSection()
{
	sectionTopLeft->drop();
	sectionBottomRight->drop();
}

IGUIElement* GUIDualSection::getSectionTopLeft()
{
	return sectionTopLeft;
}

IGUIElement* GUIDualSection::getSectionBottomRight()
{
	return sectionBottomRight;
}

void GUIDualSection::addChild( IGUIElement* child)
{
	if ( ! sectionTopLeft && ! sectionBottomRight )
		IGUIElement::addChild(child);
}

void GUIDualSection::removeChild( IGUIElement* child )
{
	if ( ! sectionTopLeft && ! sectionBottomRight )
		IGUIElement::removeChild(child);
}

void GUIDualSection::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();
	updateDragBar();
}

bool GUIDualSection::OnEvent( const SEvent& event )
{
	if ( !isVisible() || !isEnabled() )
		return false;

	s32 topLeftGap,
		bottomRightGap,
		deltaPos;

	switch( event.EventType )
	{
	case EET_MOUSE_INPUT_EVENT:
		// Don't forget to account for events where the sections reach or are at 0 in width/height
		// I'd also like to show arrows when the mouse is over the drag bar
		currMousePos.set(event.MouseInput.X, event.MouseInput.Y);

		switch( event.MouseInput.Event )
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			if ( dragBarRect.isPointInside( currMousePos ) )
			{
				lastMousePos = currMousePos;
				dragging = true;
				return true;
			}
			break;

		case EMIE_LMOUSE_LEFT_UP:
			if ( dragging )
			{
				recti topLeftRect = sectionTopLeft->getRelativePosition();
				recti bottomRightRect = sectionBottomRight->getRelativePosition();

				if ( vertical )
				{
					topLeftGap = topLeftRect.getHeight();
					bottomRightGap = bottomRightRect.getHeight();
					deltaPos = currMousePos.Y - lastMousePos.Y;
					//deltaPos = core::clamp(deltaPos, -topLeftGap, bottomRightGap);
					topLeftRect.LowerRightCorner.Y += deltaPos;
					bottomRightRect.UpperLeftCorner.Y += deltaPos;
					currShift += (f32)deltaPos / (f32)RelativeRect.getHeight();
				} else {
					topLeftGap = topLeftRect.getWidth();
					bottomRightGap = bottomRightRect.getWidth();
					deltaPos = currMousePos.X - lastMousePos.X;
					//deltaPos = core::clamp(deltaPos, -topLeftGap, bottomRightGap);
					topLeftRect.LowerRightCorner.X += deltaPos;
					bottomRightRect.UpperLeftCorner.X += deltaPos;
					currShift += (f32)deltaPos / (f32)RelativeRect.getWidth();
				}
				if ( deltaPos != 0 )
				{
					sectionTopLeft->setRelativePosition( topLeftRect );
					sectionTopLeft->updateAbsolutePosition();
					sectionBottomRight->setRelativePosition( bottomRightRect );
					sectionBottomRight->updateAbsolutePosition();
					updateDragBar();
				}
				dragging = false;
				return true;
			}
			break;

		default: break;
		}
		break;

	case EET_GUI_EVENT:
		if ( event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST
			&& event.GUIEvent.Caller == this )
		{
			dragging = false;
		}
		break;

	default: break;
	}

	return IGUIElement::OnEvent(event);
}

void GUIDualSection::draw()
{
	IGUIElement::draw();
	Environment->getSkin()->draw3DButtonPaneStandard( this, dragBarRect, &AbsoluteClippingRect );

	if ( !dragging ) return;

	// Draw drag bar potential position
	recti dbpp;
	s32 w = AbsoluteRect.getWidth();
	s32 h = AbsoluteRect.getHeight();
	s32 left; // top/left - near bound
	s32 right; // bottom/right - far bound
	s32 halfDragBarSize = dragBarSize / 2;
	core::vector2di ulc = AbsoluteRect.UpperLeftCorner;

	if ( vertical )
	{
		left = core::clamp( currMousePos.Y - halfDragBarSize, 0, h );
		right = core::clamp( currMousePos.Y + halfDragBarSize, 0, h );
		dbpp = recti( ulc.X, left, ulc.X + w, right );
	} else {
		left = core::clamp( currMousePos.X - halfDragBarSize, 0, w );
		right = core::clamp( currMousePos.X + halfDragBarSize, 0, w );
		dbpp = recti( left, ulc.Y, right, ulc.Y + h );
	}

	Environment->getSkin()->draw2DRectangle( this, video::SColor(0x88888888), dbpp, &AbsoluteClippingRect );
}

void GUIDualSection::setVertical( bool yes ) {
	setVerticalNoUpdateAbs(yes);
	updateAbsolutePosition();
}

void GUIDualSection::setShiftPercentage( f32 pShift )
{
	currShift = pShift;
	cramSections();
}

void GUIDualSection::cramSections()
{
	s32 open = 0; // open/available area
	s32 w = RelativeRect.getWidth();
	s32 h = RelativeRect.getHeight();

	// Calculate sizes of the sections
	if ( vertical )
	{
		open = h - dragBarSize;
	} else {
		open = w - dragBarSize;
	}
	f32 topLeftPerc = currShift * (f32)open;
	f32 bottomRightPerc = (f32)open - topLeftPerc;

	recti topLeftRect;		// GUI area devoted to the top/left section
	recti bottomRightRect;	// GUI area devoted to the bottom/right section

	if ( vertical )
	{
		topLeftRect = recti(0, 0, w, s32(h*topLeftPerc) );
		bottomRightRect = recti(0, s32(h*bottomRightPerc + dragBarSize), w, h );

		dragBarRect = recti( 0, topLeftRect.LowerRightCorner.Y, w, bottomRightRect.UpperLeftCorner.Y );
	} else {
		topLeftRect = recti(0, 0, s32(w*topLeftPerc), h );
		bottomRightRect = recti(s32(w*bottomRightPerc + dragBarSize), w, 0, h );

		dragBarRect = recti( topLeftRect.LowerRightCorner.X, 0, bottomRightRect.UpperLeftCorner.X, h );
	}
	sectionTopLeft->setRelativePosition( topLeftRect );
	sectionBottomRight->setRelativePosition( bottomRightRect );
	sectionTopLeft->updateAbsolutePosition();
	sectionBottomRight->updateAbsolutePosition();

	// Put drag rectangle in absolute coordinate space
	dragBarRect += AbsoluteRect.UpperLeftCorner;
}

void GUIDualSection::updateDragBar()
{
	s32 halfDragBarSize = dragBarSize / 2;
	s32 pos, w, h;
	w = RelativeRect.getWidth();
	h = RelativeRect.getHeight();

	if ( vertical )
	{
		pos = (s32)((f32)h * currShift);
		dragBarRect = recti( 0, pos - halfDragBarSize, w, pos + halfDragBarSize );
	} else {
		pos = (s32)((f32)w * currShift);
		dragBarRect = recti( pos - halfDragBarSize, 0, pos + halfDragBarSize, h );
	}
	dragBarRect += AbsoluteRect.UpperLeftCorner;
}

void
GUIDualSection::setVerticalNoUpdateAbs( bool yes )
{
	vertical = yes;
	// set left, right, top, bottom alignments
	if ( vertical ) // one section above the other
	{
		sectionTopLeft->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT );
		sectionBottomRight->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT );
	} else { // sections side by side
		sectionTopLeft->setAlignment( EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT );
		sectionBottomRight->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT );
	}
}

void
GUIDualSection::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	IGUIElement::serializeAttributes(out, options);

	out->addBool( "IsVertical", vertical );
	out->addInt( "DragBarSize", dragBarSize );
	out->addFloat( "Shift", currShift );
}

void
GUIDualSection::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	IGUIElement::deserializeAttributes(in, options);

	setVertical( in->getAttributeAsBool( "IsVertical", false ) );
	dragBarSize = in->getAttributeAsInt( "DragBarSize", 10 );
	setShiftPercentage( in->getAttributeAsFloat( "Shift", 50 ) );
}

}}

#endif // #ifndef GUI_DUAL_SECTION_CPP
