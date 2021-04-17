// by tom_gamer
// changed to CGUIListBox22 by Nic Anderson
// Modified to store data associated with text.

// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//#ifdef _IRR_COMPILE_WITH_GUI_ // booted from standalone version

// Changed includes to be library <>
#include "CGUIListBox2.h"
#include <IGUISkin.h>
#include <IGUIEnvironment.h>
#include <IVideoDriver.h>
#include <IGUIFont.h>
#include <IGUISpriteBank.h>
#include <irrMath.h>
#include <IGUIScrollBar.h> // changed to <IGUIScrollbar>
//#include <os.h>
#include <ITimer.h>

namespace irr
{
namespace gui
{

//! constructor
// timer param added by Nic Anderson to avoid os.h dependency
CGUIListBox2::CGUIListBox2(IGUIEnvironment* environment, ITimer* timer, IGUIElement* parent,
			s32 id, core::rect<s32> rectangle, bool clip,
			bool drawBack, bool moveOverSelect)
: IGUIListBox2(environment, parent, id, rectangle), Selected(-1),
	ItemHeight(0),ItemHeightOverride(0),
	TotalItemHeight(0), ItemsIconWidth(0), Font(0), IconBank(0),
	ScrollBar(0), selectTime(0), LastKeyTime(0), Selecting(false), DrawBack(drawBack),
	MoveOverSelect(moveOverSelect), AutoScroll(true), HighlightWhenNotFocused(true),
	MultiSelect(false), DidMouseMove(false), LastSelected(-1)
	, Timer(timer)
{
	#ifdef _DEBUG
	setDebugName("CGUIListBox2");
	#endif

	IGUISkin* skin = Environment->getSkin();
	const s32 s = skin->getSize(EGDS_SCROLLBAR_SIZE);

	//ScrollBar = new CGUIScrollBar(false, Environment, this, -1,
	//	core::rect<s32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(), RelativeRect.getHeight()),
	//	!clip);
		// Changed by Nic Anderson
	ScrollBar = Environment->addScrollBar( false,
		core::rect<s32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(), RelativeRect.getHeight()),
		this, -1 );

	ScrollBar->setSubElement(true);
	ScrollBar->setTabStop(false);
	ScrollBar->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
	ScrollBar->setVisible(false);
	ScrollBar->setPos(0);

	setNotClipped(!clip);

	// this element can be tabbed to
	setTabStop(true);
	setTabOrder(-1);

	updateAbsolutePosition();
}


//! destructor
CGUIListBox2::~CGUIListBox2()
{
	if (ScrollBar)
		ScrollBar->drop();

	if (Font)
		Font->drop();

	if (IconBank)
		IconBank->drop();
}


//! returns amount of list items
u32 CGUIListBox2::getItemCount() const
{
	return Items.size();
}


//! returns string of a list item. the may be a value from 0 to itemCount-1
const wchar_t* CGUIListBox2::getListItem(u32 id) const
{
	if (id>=Items.size())
		return 0;

	return Items[id].text.c_str();
}

//! returns data associated with the selected list item
IReferenceCounted* CGUIListBox2::getListItemData(u32 id) const // added by Nic Anderson
{
	if (id>=Items.size())
		return 0;

	return Items[id].data;
}


//! Returns the icon of an item
s32 CGUIListBox2::getIcon(u32 id) const
{
	if (id>=Items.size())
		return -1;

	return Items[id].icon;
}


//! adds a list item, returns id of item
u32 CGUIListBox2::addItem(const wchar_t* text, IReferenceCounted* data) // changed by Nic anderson
{
	return addItem(text, -1, data);
}


//! removes a list item whose id is given
void CGUIListBox2::removeItem(u32 id)
{
	if (id >= Items.size())
		return;

	if ((u32)Selected==id)
	{
		Selected = -1;
	}
	else if ((u32)Selected > id)
	{
		Selected -= 1;
		//selectTime = os::Timer::getTime();
		selectTime = Timer->getTime(); // change by Nic Anderson
	}

	Items.erase(id);

	recalculateItemHeight();
}


s32 CGUIListBox2::getItemAt(s32 xpos, s32 ypos) const
{
	if ( 	xpos < AbsoluteRect.UpperLeftCorner.X || xpos >= AbsoluteRect.LowerRightCorner.X
		||	ypos < AbsoluteRect.UpperLeftCorner.Y || ypos >= AbsoluteRect.LowerRightCorner.Y
		)
		return -1;

	if ( ItemHeight == 0 )
		return -1;

	s32 item = ((ypos - AbsoluteRect.UpperLeftCorner.Y - 1) + ScrollBar->getPos()) / ItemHeight;
	if ( item < 0 || item >= (s32)Items.size())
		return -1;

	return item;
}

//! clears the list
void CGUIListBox2::clear()
{
	Items.clear();
	ItemsIconWidth = 0;
	Selected = -1;

	if (ScrollBar)
		ScrollBar->setPos(0);

	recalculateItemHeight();
}


void CGUIListBox2::recalculateItemHeight()
{
	IGUISkin* skin = Environment->getSkin();

	if (Font != skin->getFont())
	{
		if (Font)
			Font->drop();

		Font = skin->getFont();
		if ( 0 == ItemHeightOverride )
			ItemHeight = 0;

		if (Font)
		{
			if ( 0 == ItemHeightOverride )
				ItemHeight = Font->getDimension(L"A").Height + 4;

			Font->grab();
		}
	}

	TotalItemHeight = ItemHeight * Items.size();
	ScrollBar->setMax( core::max_(0, TotalItemHeight - AbsoluteRect.getHeight()) );
	s32 minItemHeight = ItemHeight > 0 ? ItemHeight : 1;
	ScrollBar->setSmallStep ( minItemHeight );
	ScrollBar->setLargeStep ( 2*minItemHeight );

	if ( TotalItemHeight <= AbsoluteRect.getHeight() )
		ScrollBar->setVisible(false);
	else
		ScrollBar->setVisible(true);
}


//! returns id of selected item. returns -1 if no item is selected.
s32 CGUIListBox2::getSelected() const
{
    if (MultiSelect)
    {
        for (u32 i=0; i<Items.size(); i++)
            if (Items[i].selected)
                return((s32)i);
        return(-1);
    }
	return Selected;
}


//! sets the selected item. Set this to -1 if no item should be selected
void CGUIListBox2::setSelected(s32 id)
{
	setMultiSelect(false);
	if ((u32)id>=Items.size())
		Selected = -1;
	else
		Selected = id;
    LastSelected = Selected;

	//selectTime = os::Timer::getTime();
	selectTime = Timer->getTime(); // change by Nic Anderson

	recalculateScrollPos();
}

//! sets the selected item. Set this to -1 if no item should be selected
void CGUIListBox2::setSelected(const wchar_t *item)
{
	s32 index = -1;

	if ( item )
	{
		for ( index = 0; index < (s32) Items.size(); ++index )
		{
			if ( Items[index].text == item )
				break;
		}
	}
	setSelected ( index );
}

//! called if an event happened.
bool CGUIListBox2::OnEvent(const SEvent& event)
{
	if (isEnabled())
	{
		switch(event.EventType)
		{
		case EET_KEY_INPUT_EVENT:
			if (MultiSelect)       // ignore key inputs in multiselect mode
				break;

			if (event.KeyInput.PressedDown &&
				(event.KeyInput.Key == KEY_DOWN ||
				event.KeyInput.Key == KEY_UP   ||
				event.KeyInput.Key == KEY_HOME ||
				event.KeyInput.Key == KEY_END  ||
				event.KeyInput.Key == KEY_NEXT ||
				event.KeyInput.Key == KEY_PRIOR ) )
			{
				s32 oldSelected = Selected;
				switch (event.KeyInput.Key)
				{
					case KEY_DOWN:
						Selected += 1;
						break;
					case KEY_UP:
						Selected -= 1;
						break;
					case KEY_HOME:
						Selected = 0;
						break;
					case KEY_END:
						Selected = (s32)Items.size()-1;
						break;
					case KEY_NEXT:
						Selected += AbsoluteRect.getHeight() / ItemHeight;
						break;
					case KEY_PRIOR:
						Selected -= AbsoluteRect.getHeight() / ItemHeight;
						break;
					default:
						break;
				}
				if (Selected >= (s32)Items.size())
					Selected = Items.size() - 1;
				else
				if (Selected<0)
					Selected = 0;
                LastSelected = Selected;

				recalculateScrollPos();

				// post the news

				if (oldSelected != Selected && Parent && !Selecting && !MoveOverSelect)
				{
					SEvent e;
					e.EventType = EET_GUI_EVENT;
					e.GUIEvent.Caller = this;
					e.GUIEvent.Element = 0;
					e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
					Parent->OnEvent(e);
				}

				return true;
			}
			else
			if (!event.KeyInput.PressedDown && ( event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE ) )
			{
				if (Parent)
				{
					SEvent e;
					e.EventType = EET_GUI_EVENT;
					e.GUIEvent.Caller = this;
					e.GUIEvent.Element = 0;
					e.GUIEvent.EventType = EGET_LISTBOX_SELECTED_AGAIN;
					Parent->OnEvent(e);
				}
				return true;
			}
			else if (event.KeyInput.PressedDown && event.KeyInput.Char)
			{
				// change selection based on text as it is typed.
				//u32 now = os::Timer::getTime();
				u32 now = Timer->getTime(); // change by Nic Anderson

				if (now - LastKeyTime < 500)
				{
					// add to key buffer if it isn't a key repeat
					if (!(KeyBuffer.size() == 1 && KeyBuffer[0] == event.KeyInput.Char))
					{
						KeyBuffer += L" ";
						KeyBuffer[KeyBuffer.size()-1] = event.KeyInput.Char;
					}
				}
				else
				{
					KeyBuffer = L" ";
					KeyBuffer[0] = event.KeyInput.Char;
				}
				LastKeyTime = now;

				// find the selected item, starting at the current selection
				s32 start = Selected;
				// dont change selection if the key buffer matches the current item
				if (Selected > -1 && KeyBuffer.size() > 1)
				{
					if (Items[Selected].text.size() >= KeyBuffer.size() &&
						KeyBuffer.equals_ignore_case(Items[Selected].text.subString(0,KeyBuffer.size())))
						return true;
				}

				s32 current;
				for (current = start+1; current < (s32)Items.size(); ++current)
				{
					if (Items[current].text.size() >= KeyBuffer.size())
					{
						if (KeyBuffer.equals_ignore_case(Items[current].text.subString(0,KeyBuffer.size())))
						{
							if (Parent && Selected != current && !Selecting && !MoveOverSelect)
							{
								SEvent e;
								e.EventType = EET_GUI_EVENT;
								e.GUIEvent.Caller = this;
								e.GUIEvent.Element = 0;
								e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
								Parent->OnEvent(e);
							}
							setSelected(current);
							return true;
						}
					}
				}
				for (current = 0; current <= start; ++current)
				{
					if (Items[current].text.size() >= KeyBuffer.size())
					{
						if (KeyBuffer.equals_ignore_case(Items[current].text.subString(0,KeyBuffer.size())))
						{
							if (Parent && Selected != current && !Selecting && !MoveOverSelect)
							{
								Selected = current;
								SEvent e;
								e.EventType = EET_GUI_EVENT;
								e.GUIEvent.Caller = this;
								e.GUIEvent.Element = 0;
								e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
								Parent->OnEvent(e);
							}
							setSelected(current);
							return true;
						}
					}
				}

				return true;
			}
			break;

		case EET_GUI_EVENT:
			switch(event.GUIEvent.EventType)
			{
			case gui::EGET_SCROLL_BAR_CHANGED:
				if (event.GUIEvent.Caller == ScrollBar)
					return true;
				break;
			case gui::EGET_ELEMENT_FOCUS_LOST:
				{
					if (event.GUIEvent.Caller == this)
						Selecting = false;
				}
			default:
			break;
			}
			break;

		case EET_MOUSE_INPUT_EVENT:
			{
				core::position2d<s32> p(event.MouseInput.X, event.MouseInput.Y);

				switch(event.MouseInput.Event)
				{
				case EMIE_MOUSE_WHEEL:
					ScrollBar->setPos(ScrollBar->getPos() + (event.MouseInput.Wheel < 0 ? -1 : 1) * (-ItemHeight)/2); // Nic added parentheses around -ItemHeight
					return true;

				case EMIE_LMOUSE_PRESSED_DOWN:
				{
					Selecting = true;
					DidMouseMove = false;
					if (MultiSelect && !event.MouseInput.Control && !event.MouseInput.Shift)
                        for (u32 i=0; i<Items.size(); i++)
                            Items[i].selected = false;
					return true;
				}

				case EMIE_LMOUSE_LEFT_UP:
				{
					Selecting = false;

					if ((!DidMouseMove || !MultiSelect) && isPointInside(p))
						selectNew(event.MouseInput.Y, false, event.MouseInput.Control, event.MouseInput.Shift);

					return true;
				}

				case EMIE_MOUSE_MOVED:
					if (Selecting || MoveOverSelect)
					{
						if (!event.MouseInput.Control && !event.MouseInput.Shift && isPointInside(p))
						{
							DidMouseMove = true;
							selectNew(event.MouseInput.Y, true);
							return true;
						}
					}
				default:
				break;
				}
			}
			break;
		default: break; // Changed to "default" by Nic Anderson
		}
	}

	return IGUIElement::OnEvent(event);
}


void CGUIListBox2::selectNew(s32 ypos, bool onlyHover, bool ctrlState, bool shftState)
{
	//u32 now = os::Timer::getTime();
	u32 now = Timer->getTime(); // change by Nic Anderson
	s32 oldSelected = Selected;
    s32 newSelected;


	newSelected = getItemAt(AbsoluteRect.UpperLeftCorner.X, ypos);

	if (newSelected<0)
		return;

    if (MultiSelect)
    {
        if (onlyHover)
        {
            if (!Selecting)
            {
                for (u32 i=0; i<Items.size(); i++)
                    Items[i].selected = false;
            }
            Items[newSelected].selected = true;
        }
        else
        {
            /* single mouse click -> check ctrl and shift state */
            if (ctrlState)
                Items[newSelected].selected = !Items[newSelected].selected;
            else
            if (shftState)
            {
                if (LastSelected < 0)
                    Items[newSelected].selected = true;
                else
                {
                    u32 mi, ma;
                    mi = core::min_(newSelected, LastSelected);
                    ma = core::max_(newSelected, LastSelected);
                    for (u32 i=mi; i<=ma; i++)
                        Items[i].selected = true;
                }
            }
            else
            {
                for (u32 i=0; i<Items.size(); i++)
                    Items[i].selected = false;
                Items[newSelected].selected = true;
            }
        }
        LastSelected = newSelected;
    }
    else
    {
        Selected = newSelected;
        LastSelected = Selected;
    }

	recalculateScrollPos();

	gui::EGUI_EVENT_TYPE eventType = (!MultiSelect && Selected == oldSelected && now < selectTime + 500) ? EGET_LISTBOX_SELECTED_AGAIN : EGET_LISTBOX_CHANGED;
	selectTime = now;
	// post the news
	if (Parent && (MultiSelect || !onlyHover))
	{
		SEvent event;
		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.Element = 0;
		event.GUIEvent.EventType = eventType;
		Parent->OnEvent(event);
	}
}


//! Update the position and size of the listbox, and update the scrollbar
void CGUIListBox2::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();

	recalculateItemHeight();
}


//! draws the element and its children
void CGUIListBox2::draw()
{
	if (!IsVisible)
		return;

	recalculateItemHeight(); // if the font changed

	IGUISkin* skin = Environment->getSkin();

	core::rect<s32>* clipRect = 0;

	// draw background
	core::rect<s32> frameRect(AbsoluteRect);

	// draw items

	core::rect<s32> clientClip(AbsoluteRect);
	clientClip.UpperLeftCorner.Y += 1;
	clientClip.UpperLeftCorner.X += 1;
	if (ScrollBar->isVisible())
		clientClip.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);
	clientClip.LowerRightCorner.Y -= 1;
	clientClip.clipAgainst(AbsoluteClippingRect);

	skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_HIGH_LIGHT), true,
		DrawBack, frameRect, &clientClip);

	if (clipRect)
		clientClip.clipAgainst(*clipRect);

	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.X += 1;
	if (ScrollBar->isVisible())
		frameRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);

	frameRect.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + ItemHeight;

	frameRect.UpperLeftCorner.Y -= ScrollBar->getPos();
	frameRect.LowerRightCorner.Y -= ScrollBar->getPos();

	bool hl = (HighlightWhenNotFocused || Environment->hasFocus(this) || Environment->hasFocus(ScrollBar));

	for (s32 i=0; i<(s32)Items.size(); ++i)
	{
		if (frameRect.LowerRightCorner.Y >= AbsoluteRect.UpperLeftCorner.Y &&
			frameRect.UpperLeftCorner.Y <= AbsoluteRect.LowerRightCorner.Y)
		{
                    if (MultiSelect)
                    {
                        if (Items[i].selected)
				skin->draw2DRectangle(this, skin->getColor(EGDC_HIGH_LIGHT), frameRect, &clientClip);
                    }
                    else
			if (i == Selected && hl)
				skin->draw2DRectangle(this, skin->getColor(EGDC_HIGH_LIGHT), frameRect, &clientClip);

			core::rect<s32> textRect = frameRect;
			textRect.UpperLeftCorner.X += 3;

			if (Font)
			{
				if (IconBank && (Items[i].icon > -1))
				{
					core::position2di iconPos = textRect.UpperLeftCorner;
					iconPos.Y += textRect.getHeight() / 2;
					iconPos.X += ItemsIconWidth/2;

					if ( i==Selected && hl )
					{
						IconBank->draw2DSprite( (u32)Items[i].icon, iconPos, &clientClip,
							hasItemOverrideColor(i, EGUI_LBC_ICON_HIGHLIGHT) ?
							getItemOverrideColor(i, EGUI_LBC_ICON_HIGHLIGHT) : getItemDefaultColor(EGUI_LBC_ICON_HIGHLIGHT),
							//selectTime, os::Timer::getTime(), false, true);
							selectTime, Timer->getTime(), false, true); // change by Nic Anderson
					}
					else
					{
						IconBank->draw2DSprite( (u32)Items[i].icon, iconPos, &clientClip,
							hasItemOverrideColor(i, EGUI_LBC_ICON) ? getItemOverrideColor(i, EGUI_LBC_ICON) : getItemDefaultColor(EGUI_LBC_ICON),
							0 ,
							//(i==Selected) ? os::Timer::getTime() : 0,
							(i==Selected)? Timer->getTime() : 0, // change by Nic Anderson
							false, true);
					}
				}

				textRect.UpperLeftCorner.X += ItemsIconWidth+3;

				if ( i==Selected && hl )
				{
					Font->draw(Items[i].text.c_str(), textRect,
						hasItemOverrideColor(i, EGUI_LBC_TEXT_HIGHLIGHT) ?
						getItemOverrideColor(i, EGUI_LBC_TEXT_HIGHLIGHT) : getItemDefaultColor(EGUI_LBC_TEXT_HIGHLIGHT),
						false, true, &clientClip);
				}
				else
				{
					Font->draw(Items[i].text.c_str(), textRect,
						hasItemOverrideColor(i, EGUI_LBC_TEXT) ? getItemOverrideColor(i, EGUI_LBC_TEXT) : getItemDefaultColor(EGUI_LBC_TEXT),
						false, true, &clientClip);
				}

				textRect.UpperLeftCorner.X -= ItemsIconWidth+3;
			}
		}

		frameRect.UpperLeftCorner.Y += ItemHeight;
		frameRect.LowerRightCorner.Y += ItemHeight;
	}

	IGUIElement::draw();
}


//! adds an list item with an icon
u32 CGUIListBox2::addItem(const wchar_t* text, s32 icon, IReferenceCounted* data) // changed by Nic anderson
{
	ListItem i;
	i.text = text;
	i.icon = icon;
	i.setData(data);

	Items.push_back(i);
	recalculateItemHeight();
	recalculateItemWidth(icon);

	return Items.size() - 1;
}


void CGUIListBox2::setSpriteBank(IGUISpriteBank* bank)
{
    if ( bank == IconBank )
        return;
	if (IconBank)
		IconBank->drop();

	IconBank = bank;
	if (IconBank)
		IconBank->grab();
}


void CGUIListBox2::recalculateScrollPos()
{
	if (!AutoScroll)
		return;

	const s32 selPos = (LastSelected == -1 ? TotalItemHeight : LastSelected * ItemHeight) - ScrollBar->getPos();

	if (selPos < 0)
	{
		ScrollBar->setPos(ScrollBar->getPos() + selPos);
	}
	else
	if (selPos > AbsoluteRect.getHeight() - ItemHeight)
	{
		ScrollBar->setPos(ScrollBar->getPos() + selPos - AbsoluteRect.getHeight() + ItemHeight);
	}
}


void CGUIListBox2::setAutoScrollEnabled(bool scroll)
{
	AutoScroll = scroll;
}


bool CGUIListBox2::isAutoScrollEnabled() const
{
	//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX; // removed by Nic. I don't care about .Net.
	return AutoScroll;
}


bool CGUIListBox2::getSerializationLabels(EGUI_LISTBOX_COLOR colorType, core::stringc & useColorLabel, core::stringc & colorLabel) const
{
	switch ( colorType )
	{
	case EGUI_LBC_TEXT:
		useColorLabel = "UseColText";
		colorLabel = "ColText";
		break;
	case EGUI_LBC_TEXT_HIGHLIGHT:
		useColorLabel = "UseColTextHl";
		colorLabel = "ColTextHl";
		break;
	case EGUI_LBC_ICON:
		useColorLabel = "UseColIcon";
		colorLabel = "ColIcon";
		break;
	case EGUI_LBC_ICON_HIGHLIGHT:
		useColorLabel = "UseColIconHl";
		colorLabel = "ColIconHl";
		break;
	default:
		return false;
	}
	return true;
}


//! Writes attributes of the element.
void CGUIListBox2::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
	IGUIListBox2::serializeAttributes(out,options);

	// todo: out->addString	("IconBank",		IconBank->getName?);
	out->addBool    ("MultiSelect",     MultiSelect);
	out->addBool    ("DrawBack",        DrawBack);
	out->addBool    ("MoveOverSelect",  MoveOverSelect);
	out->addBool    ("AutoScroll",      AutoScroll);

	out->addInt("ItemCount", Items.size());
	for (u32 i=0;i<Items.size(); ++i)
	{
		core::stringc label("text");
		label += i;
		out->addString(label.c_str(), Items[i].text.c_str() );

		for ( s32 c=0; c < (s32)EGUI_LBC_COUNT; ++c )
		{
			core::stringc useColorLabel, colorLabel;
			if ( !getSerializationLabels((EGUI_LISTBOX_COLOR)c, useColorLabel, colorLabel) )
				return;
			label = useColorLabel; label += i;
			if ( Items[i].OverrideColors[c].Use )
			{
				out->addBool(label.c_str(), true );
				label = colorLabel; label += i;
				out->addColor(label.c_str(), Items[i].OverrideColors[c].Color);
			}
			else
			{
				out->addBool(label.c_str(), false );
			}
		}
	}
}


//! Reads attributes of the element
void CGUIListBox2::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	clear();

	MultiSelect     = in->getAttributeAsBool("MultiSelect");
	DrawBack        = in->getAttributeAsBool("DrawBack");
	MoveOverSelect  = in->getAttributeAsBool("MoveOverSelect");
	AutoScroll      = in->getAttributeAsBool("AutoScroll");

	IGUIListBox2::deserializeAttributes(in,options);

	const s32 count = in->getAttributeAsInt("ItemCount");
	for (s32 i=0; i<count; ++i)
	{
		core::stringc label("text");
		ListItem item;

		label += i;
		item.text = in->getAttributeAsStringW(label.c_str());

 		// changed by Nic anderson
		addItem(item.text.c_str(), item.icon, 0); // Should actually use a data-creator class

		for ( u32 c=0; c < EGUI_LBC_COUNT; ++c )
		{
			core::stringc useColorLabel, colorLabel;
			if ( !getSerializationLabels((EGUI_LISTBOX_COLOR)c, useColorLabel, colorLabel) )
				return;
			label = useColorLabel; label += i;
			Items[i].OverrideColors[c].Use = in->getAttributeAsBool(label.c_str());
			if ( Items[i].OverrideColors[c].Use )
			{
				label = colorLabel; label += i;
				Items[i].OverrideColors[c].Color = in->getAttributeAsColor(label.c_str());
			}
		}
	}
}


void CGUIListBox2::recalculateItemWidth(s32 icon)
{
	if (IconBank && icon > -1 &&
		IconBank->getSprites().size() > (u32)icon &&
		IconBank->getSprites()[(u32)icon].Frames.size())
	{
		u32 rno = IconBank->getSprites()[(u32)icon].Frames[0].rectNumber;
		if (IconBank->getPositions().size() > rno)
		{
			const s32 w = IconBank->getPositions()[rno].getWidth();
			if (w > ItemsIconWidth)
				ItemsIconWidth = w;
		}
	}
}


void CGUIListBox2::setItem(u32 index, const wchar_t* text, s32 icon, IReferenceCounted* data) // changed by Nic anderson
{
	if ( index >= Items.size() )
		return;

	Items[index].text = text;
	Items[index].icon = icon;
	Items[index].setData(data);

	recalculateItemHeight();
	recalculateItemWidth(icon);
}


//! Insert the item at the given index
//! Return the index on success or -1 on failure.
s32 CGUIListBox2::insertItem(u32 index, const wchar_t* text, s32 icon, IReferenceCounted* data) // changed by Nic anderson
{
	ListItem i;
	i.text = text;
	i.icon = icon;
	i.setData(data);

	Items.insert(i, index);
	recalculateItemHeight();
	recalculateItemWidth(icon);

	return index;
}


void CGUIListBox2::swapItems(u32 index1, u32 index2)
{
	if ( index1 >= Items.size() || index2 >= Items.size() )
		return;

	ListItem dummmy = Items[index1];
	Items[index1] = Items[index2];
	Items[index2] = dummmy;
}


void CGUIListBox2::setItemOverrideColor(u32 index, video::SColor color)
{
	for ( u32 c=0; c < EGUI_LBC_COUNT; ++c )
	{
		Items[index].OverrideColors[c].Use = true;
		Items[index].OverrideColors[c].Color = color;
	}
}


void CGUIListBox2::setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, video::SColor color)
{
	if ( index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return;

	Items[index].OverrideColors[colorType].Use = true;
	Items[index].OverrideColors[colorType].Color = color;
}


void CGUIListBox2::clearItemOverrideColor(u32 index)
{
	for (u32 c=0; c < (u32)EGUI_LBC_COUNT; ++c )
	{
		Items[index].OverrideColors[c].Use = false;
	}
}


void CGUIListBox2::clearItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType)
{
	if ( index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return;

	Items[index].OverrideColors[colorType].Use = false;
}


bool CGUIListBox2::hasItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const
{
	if ( index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return false;

	return Items[index].OverrideColors[colorType].Use;
}


video::SColor CGUIListBox2::getItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const
{
	if ( (u32)index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return video::SColor();

	return Items[index].OverrideColors[colorType].Color;
}


video::SColor CGUIListBox2::getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const
{
	IGUISkin* skin = Environment->getSkin();
	if ( !skin )
		return video::SColor();

	switch ( colorType )
	{
		case EGUI_LBC_TEXT:
			return skin->getColor(EGDC_BUTTON_TEXT);
		case EGUI_LBC_TEXT_HIGHLIGHT:
			return skin->getColor(EGDC_HIGH_LIGHT_TEXT);
		case EGUI_LBC_ICON:
			return skin->getColor(EGDC_ICON);
		case EGUI_LBC_ICON_HIGHLIGHT:
			return skin->getColor(EGDC_ICON_HIGH_LIGHT);
		default:
			return video::SColor();
	}
}

//! set global itemHeight
void CGUIListBox2::setItemHeight( s32 height )
{
	ItemHeight = height;
	ItemHeightOverride = 1;
}


//! Sets whether to draw the background
void CGUIListBox2::setDrawBackground(bool draw)
{
    DrawBack = draw;
}

//! Sets whether this is a multi-select list box
void CGUIListBox2::setMultiSelect(bool multiSelect)
{
    MultiSelect = multiSelect;
    if (multiSelect)      /* switch to multiselect */
        Selected = -1;
    else                  /* switch to singleselect */
    {
        for (u32 i=0; i<Items.size(); i++)
            Items[i].selected = false;
    }
}

//! returns ids of selected items.
void CGUIListBox2::getSelected(core::array<s32> &sel)
{
    sel.clear();
    if (!MultiSelect)
    {
        if (Selected >= 0)
            sel.push_back(Selected);
        return;
    }
    for (u32 i=0; i<Items.size(); i++)
        if (Items[i].selected)
            sel.push_back((s32)i);
}

//! sets the selected items.
void CGUIListBox2::setSelected(core::array<u32> sel)
{
    setMultiSelect(true);
    for (u32 i=0; i<Items.size(); i++)
        Items[i].selected = false;
    for (u32 i=0; i<sel.size(); i++)
        setItemSelection(sel[i], true);

    //selectTime = os::Timer::getTime();
	selectTime = Timer->getTime(); // change by Nic Anderson
    if (sel.size() == 0)
        LastSelected = -1;
    else
        LastSelected = sel[0];

    recalculateScrollPos();
}

//! sets the select-state of an item.
void CGUIListBox2::setItemSelection(u32 id, bool selected)
{
    if (!MultiSelect)
        setSelected(id);
    else
    {
        if (id < Items.size())
            Items[id].selected = true;
    }
}


} // end namespace gui
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_GUI_
