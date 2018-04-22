// (c) 2015 Nicolaus Anderson
// zlib license

#ifndef GUI_DROPDOWN_SELECTOR_CPP
#define GUI_DROPDOWN_SELECTOR_CPP

#include "GUIDropdownSelector.h"
#include <IGUIButton.h>
#include <IGUIContextMenu.h>
#include <IGUIEnvironment.h>
#include <IGUISpriteBank.h>
#include <IGUISkin.h>

namespace irr {
namespace gui {

using video::SColor;

GUIDropdownSelector::GUIDropdownSelector( IGUIEnvironment* pEnvironment, IGUIElement* pParent, rect<s32> pRect, s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, dirty( true )
	, wasMenuFocus( false )
	, skipClick( false )
	, iconRect()
{
	button = pEnvironment->addButton( rect<s32>(pRect.getSize()), this, -1, L"", L"Click to select" );
	button->setDrawBorder(true);
	recalculateAbsolutePosition(false);
	menu = pEnvironment->addContextMenu(
				AbsoluteRect + core::vector2d<s32>(0, pRect.getHeight() ),
				pEnvironment->getRootGUIElement()
				);
	menu->grab();
	menu->setVisible(false);
	menu->setCloseHandling( ECMC_HIDE );
	menu->setEventParent(this);

	s32 h = arrowHeight;
	s32 hspace = (AbsoluteRect.getHeight() - h) / 2;
	iconRect = rect<s32>(
				AbsoluteRect.LowerRightCorner.X - (h + hspace),			// upper left x
				AbsoluteRect.UpperLeftCorner.Y + hspace,			// upper left y
				AbsoluteRect.LowerRightCorner.X - hspace,			// lower right x
				AbsoluteRect.LowerRightCorner.Y - hspace			// lower right y
			);
}

GUIDropdownSelector::~GUIDropdownSelector()
{
	menu->remove(); // Removes from environment
	menu->drop();
}

void GUIDropdownSelector::setText( const wchar_t* pText )
{
	dirty = true;
	button->setText( pText );
}

void GUIDropdownSelector::setToolTipText(const wchar_t* text)
{
	button->setToolTipText(text);
}

s32 GUIDropdownSelector::getSelected()
{
	return menu->getSelectedItem();
}

const wchar_t* GUIDropdownSelector::getSelectedText()
{
	s32 i = menu->getSelectedItem();
	if ( i != -1 && !dirty )
	{
		return menu->getItemText( i );
	}
	return L"";
}

const wchar_t* GUIDropdownSelector::getItemText( u32 pIndex )
{
	return menu->getItemText( pIndex );
}

void GUIDropdownSelector::setItemText( u32 pIndex, const wchar_t* pText )
{
	menu->setItemText( pIndex, pText );
}

void GUIDropdownSelector::setItemEnabled( u32 pIndex, bool pEnable )
{
	menu->setItemEnabled( pIndex, pEnable );
}

bool GUIDropdownSelector::isItemEnabled( u32 pIndex )
{
	return menu->isItemEnabled( pIndex );
}

void GUIDropdownSelector::removeItem( u32 pIndex )
{
	menu->removeItem( pIndex );
}

void GUIDropdownSelector::removeAllItems()
{
	menu->removeAllItems();
	button->setText(L"");
}

void GUIDropdownSelector::addItem( const wchar_t* pText, bool pEnabled )
{
	menu->addItem( pText, -1, pEnabled, false, false, false );

	if ( menu->getItemCount() == 1 ) // first item
	{
		button->setText( pText );
	}
}

void GUIDropdownSelector::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();

	rect<s32> menuRect = menu->getRelativePosition();
	s32 mW = menuRect.getWidth();
	s32 mH = menuRect.getHeight();

	/* Recall, the menu is added to the root GUI element, so its relative position is relative
	to the root, and consequently at the NEW absolute position of this element. */
	menu->setRelativePosition(
				rect<s32>(0,0,mW,mH) + AbsoluteRect.UpperLeftCorner + core::vector2d<s32>( 0, AbsoluteRect.getHeight() )
				);

	s32 h = arrowHeight;
	s32 hspace = (AbsoluteRect.getHeight() - h) / 2;
	iconRect = rect<s32>(
				AbsoluteRect.LowerRightCorner.X - (h + hspace),			// upper left x
				AbsoluteRect.UpperLeftCorner.Y + hspace,			// upper left y
				AbsoluteRect.LowerRightCorner.X - hspace,			// lower right x
				AbsoluteRect.LowerRightCorner.Y - hspace			// lower right y
			);
}

bool GUIDropdownSelector::OnEvent( const SEvent& event )
{
	if ( ! isVisible() || ! isEnabled() )
		return false;

	if ( event.EventType != EET_GUI_EVENT )
		return false;

	switch ( event.GUIEvent.EventType )
	{
	case EGET_ELEMENT_FOCUSED:
		if ( event.GUIEvent.Caller == button )
		{
			/* Irrlicht's CGUIEnvironment maintains the last GUI focus until the end of its setFocus() method. */
			wasMenuFocus = ( Environment->getFocus() == menu );
		}
		break;

	case EGET_BUTTON_CLICKED:
		if ( event.GUIEvent.Caller == button )
		{
			/* Avoid opening the menu if it was already open. */
			if ( ! wasMenuFocus )
			{
				//Parent->bringToFront(this);
				menu->setVisible( true );
				Environment->getRootGUIElement()->bringToFront(menu);
				Environment->setFocus(menu);
				return true;
			}
			wasMenuFocus = false;
			return true;
		}
		break;

	case EGET_MENU_ITEM_SELECTED:
		if ( event.GUIEvent.Caller == menu )
		{
			dirty = false;
			button->setText( getSelectedText() );
			sendGUIEvent( EGET_LISTBOX_CHANGED );
			return true;
		}

	default: break;
	}

	return false;
}

void GUIDropdownSelector::draw()
{
	if ( ! isVisible() )
	{
		menu->setVisible(false); // Ensure an accidentally-opened menu is hidden
		return;
	}

	//IGUIElement::draw();
	button->draw();
	menu->draw();

	// Drawing a down-arrow
	u32 dropDownIcon = Environment->getSkin()->getIcon( EGDI_CURSOR_DOWN /*EGDI_DROP_DOWN*/ );
	//video::SColor iconColor = Environment->getSkin()->getColor( isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL );

		// if too small to draw
	if ( iconRect.getWidth() < 0 || iconRect.getHeight() < 0 )
		return;
	Environment->getSkin()->getSpriteBank()->draw2DSprite( dropDownIcon, iconRect, &AbsoluteClippingRect );
}

void GUIDropdownSelector::sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement )
{
	if ( ! Parent ) return;

	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = pElement;
	event.GUIEvent.EventType = pEventType;

	Parent->OnEvent( event );
}

}}

#endif // #ifndef GUI_DROPDOWN_SELECTOR_CPP
