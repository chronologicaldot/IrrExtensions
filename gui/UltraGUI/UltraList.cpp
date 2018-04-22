/*
(c) Nicolaus Anderson
*/

#include "UltraList.h"

namespace irr
{
namespace gui
{

UltraList::UltraList(
		IGUIEnvironment* environment,
		IGUIElement* parent,
		irr::s32 id,
		irr::core::recti rectangle
		)
	: IGUIElement( EGUIET_ELEMENT, environment, parent, id, rectangle )
{
}

UltraList::~UltraList()
{
}

UltraIcon* UltraList::addListItem( irr::s32 id )
{
	UltraIcon* icon = new UltraIcon(EGUIET_ELEMENT,Environment,this,id,iconRect);

	Children.push_back((IGUIElement*)icon);

	return (IGUIElement*)icon;
}

void UltraList::removeListItem( irr::s32 id )
{
	irr::core::list<IGUIElement*>::Iterator i = Children.begin();
	for ( ; i != Children.end(); ++i )
	{
		if ( (*i)->getID() == id )
		{
			Children.erase(i);
			break;
		}
	}
}

void UltraList::setHorizontalSpacing(irr::s32 gap)
{
	xSpacing = gap;
}

void UltraList::setVerticalSpacing(irr::s32 gap)
{
	ySpacing = gap;
}

void UltraList::setUniversalSpacing(irr::s32 gap)
{
	xSpacing = gap;
	ySpacing = gap;
}

void UltraList::OnEvent(const irr::SEvent& event)
{
}

void UltraList::draw()
{
	/* Arrange the icons on the basis of what is visible
	and how much space there is. */

	// Now draw the children
	IGUIElement::draw();
}

}} // end namespace gui and irr