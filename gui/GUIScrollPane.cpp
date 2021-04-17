// Copyright 2016 Nic Anderson
#include "GUIScrollPane.h"
#include <IGUIScrollBar.h>
#include <IGUIEnvironment.h>

namespace irr {
namespace gui {

GUIScrollPane::GUIScrollPane( IGUIEnvironment* pGUIEnvironment, IGUIElement* pParent, recti pRect, irr::s32 id )
	: IGUIElement( EGUIET_ELEMENT, pGUIEnvironment, pParent, id, pRect )
	, horizontalScrollBar(0)
	, verticalScrollBar(0)
	, barWidth(20)
	, childWrapper(0)
{
	constructChildWrapper(pGUIEnvironment, pRect);

	recti r = recti(0, pRect.getHeight()-barWidth, pRect.getWidth(), pRect.getHeight());
	horizontalScrollBar = pGUIEnvironment->addScrollBar(true, r, this);
	horizontalScrollBar->setAlignment(EGUIA_UPPERLEFT,EGUIA_LOWERRIGHT,EGUIA_LOWERRIGHT,EGUIA_LOWERRIGHT);
	horizontalScrollBar->setSubElement(true);
	horizontalScrollBar->setSmallStep(5);
	horizontalScrollBar->setLargeStep(20);
	horizontalScrollBar->setPos(0);

	r = recti(pRect.getWidth()-barWidth, 0, pRect.getWidth(), pRect.getHeight()-barWidth);
	verticalScrollBar = pGUIEnvironment->addScrollBar(false, r, this);
	verticalScrollBar->setAlignment(EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
	verticalScrollBar->setSubElement(true);
	verticalScrollBar->setSmallStep(5);
	verticalScrollBar->setLargeStep(20);
	verticalScrollBar->setPos(0);
}

GUIScrollPane::~GUIScrollPane()
{
/*	if ( horizontalScrollBar )
		horizontalScrollBar->drop();

	if ( verticalScrollBar )
		verticalScrollBar->drop();*/
}

void GUIScrollPane::constructChildWrapper(IGUIEnvironment* pGUIEnvironment, recti position)
{
	recti r = position;
	r.LowerRightCorner.X -= barWidth;
	r.LowerRightCorner.Y -= barWidth;
	childWrapper = new IGUIElement( EGUIET_ELEMENT, pGUIEnvironment, this, -1, r );
	childWrapper->drop(); // This element will handle the reference counting
	childWrapper->setTabGroup(true);
}

IGUIElement* GUIScrollPane::getChildWrapper() const
{
	return childWrapper;
}

void GUIScrollPane::showHorizontalScrollBar(bool yes)
{
	recti r;
	if ( yes && horizontalScrollBar->isVisible() == false )
	{
		r = childWrapper->getRelativePosition();
		r.LowerRightCorner.X -= barWidth;
		childWrapper->setRelativePosition(r);

		r = verticalScrollBar->getRelativePosition();
		r.LowerRightCorner.Y -= barWidth;
		verticalScrollBar->setRelativePosition(r);
	} else if ( !yes && horizontalScrollBar->isVisible() ) {
		r = childWrapper->getRelativePosition();
		r.LowerRightCorner.X += barWidth;
		childWrapper->setRelativePosition(r);

		r = verticalScrollBar->getRelativePosition();
		r.LowerRightCorner.Y += barWidth;
		verticalScrollBar->setRelativePosition(r);
	}
	horizontalScrollBar->setVisible(yes);
	horizontalScrollBar->setEnabled(yes);
	recalculateChildBounds();
}

void GUIScrollPane::showVerticalScrollBar(bool yes)
{
	recti r;
	if ( yes && verticalScrollBar->isVisible() == false )
	{
		r = childWrapper->getRelativePosition();
		r.LowerRightCorner.Y -= barWidth;
		childWrapper->setRelativePosition(r);
	} else if ( !yes && verticalScrollBar->isVisible() ) {
		r = childWrapper->getRelativePosition();
		r.LowerRightCorner.Y += barWidth;
		childWrapper->setRelativePosition(r);
	}
	verticalScrollBar->setVisible(yes);
	verticalScrollBar->setEnabled(yes);
	recalculateChildBounds();
}

IGUIScrollBar* GUIScrollPane::getHorizontalBar() const
{
	return horizontalScrollBar;
}

IGUIScrollBar* GUIScrollPane::getVerticalBar() const
{
	return verticalScrollBar;
}

void GUIScrollPane::setRelativePosition( recti pPosition )
{
	IGUIElement::setRelativePosition(pPosition);
	updateAbsolutePosition();
}

void GUIScrollPane::addChild( IGUIElement* pElement )
{
	//	IGUIElement::addChild(pElement);
	childWrapper->addChild(pElement);
	childWrapper->updateAbsolutePosition();
	childStartPositions.push_back(
		GUIElementPosition( pElement, pElement->getRelativePosition().UpperLeftCorner )
		);
	recalculateChildBounds();
	// Should I have added an offset? Chat clients would not benefit from it but other programs would.
}

void GUIScrollPane::removeChild( IGUIElement* pElement )
{
	u32 index = 0;
	for ( ; index < childStartPositions.size(); index++ )
	{
		if ( childStartPositions[index].element == pElement )
		{
			childStartPositions.erase(index);
			break;
		}
	}
	childWrapper->removeChild(pElement);
	recalculateChildBounds();
}
void GUIScrollPane::clearChildren()
{
	core::list<IGUIElement*>::ConstIterator kid = childWrapper->getChildren().begin();
	while ( kid != childWrapper->getChildren().end() ) {
		childWrapper->removeChild(*kid);
		kid = childWrapper->getChildren().begin();
	}
}

void GUIScrollPane::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();
	recalculateChildBounds();
}

bool GUIScrollPane::OnEvent(const SEvent& event)
{
	if ( !IsEnabled )
		return false;

	switch ( event.EventType )
	{
	case EET_MOUSE_INPUT_EVENT:
		break;

	case EET_KEY_INPUT_EVENT:
		// Scroll on arrow keys?
		// Remember to set the scroll bars and get the position from them
		break;

	case EET_GUI_EVENT:
		if ( event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED )
		{
			shiftChildrenToPosition(
				horizontalScrollBar->getPos(),
				verticalScrollBar->getPos()
				);
		}
		break;

	default: break;
	}

	return IGUIElement::OnEvent( event );
}

void
GUIScrollPane::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const {
	IGUIElement::serializeAttributes(out, options);

	out->addBool("ShowHorizontalBar", horizontalScrollBar->isVisible() );
	out->addBool("ShowVerticalBar", verticalScrollBar->isVisible() );
}

void
GUIScrollPane::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
	IGUIElement::deserializeAttributes(in, options);

	showHorizontalScrollBar( in->getAttributeAsBool("ShowHorizontalBar", horizontalScrollBar->isVisible()) );
	showVerticalScrollBar( in->getAttributeAsBool("ShowVerticalBar", verticalScrollBar->isVisible()) );
}

void GUIScrollPane::recalculateChildBounds()
{
	if ( childWrapper->getChildren().size() == 0 ) {
		childBounds = core::rect<s32>();
		return;
	}
	core::list<IGUIElement*>::ConstIterator iter = childWrapper->getChildren().begin();
	childBounds = (*iter)->getRelativePosition();
	core::rect<s32> bound;
	for ( ; iter != childWrapper->getChildren().end(); ++iter )
	{
		bound = (*iter)->getRelativePosition();
		childBounds.addInternalPoint( bound.UpperLeftCorner );
		childBounds.addInternalPoint( bound.LowerRightCorner );
	}

	// Update the scroll bars
	horizontalScrollBar->setMax( childBounds.getWidth() );
	verticalScrollBar->setMax( childBounds.getHeight() );
}

void GUIScrollPane::shiftChildrenToPosition( s32 x, s32 y )
{
	if ( childWrapper->getChildren().size() == 0 )
		return;
	// Since all children shift the same, only one start position is needed to find the shift
	core::list<IGUIElement*>::ConstIterator iter = childWrapper->getChildren().begin();
	core::rect<s32> bound = (*iter)->getRelativePosition();
	u32 i=0;
	core::vector2d<s32> startPos,
						currShift,
						newShift;

	for ( ; i < childStartPositions.size(); ++i )
	{
		if ( childStartPositions[i].element == *iter )
		{
			startPos = childStartPositions[i].position;
			break;
		}
	}
	currShift = startPos - bound.UpperLeftCorner;
	//newShift.set( currShift.X - x, currShift.Y - y ); // TODO: Uncomment and comment out the next line?? What was wrong?
	newShift.set( 0, currShift.Y - y );

	for ( ; iter != childWrapper->getChildren().end(); ++iter )
	{
		(*iter)->move(newShift);
	}
}

}}
