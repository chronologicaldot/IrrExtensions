// (c) 2014 Nicolaus Anderson

#include "GUIColorEditBox.h"
#include <IGUISkin.h>
#include <IGUIFont.h>

namespace irr {
namespace gui {

GUIColorEditBox::GUIColorEditBox(
	IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id
	)
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, color(0xff000000)
	, cursorColor(0x0000ddff)
	, cursorIdx(0)
	, cursorRect(0,0,0,0)
	, focusTime(0)
{
	Text = L"ff000000";
}

GUIColorEditBox::~GUIColorEditBox()
{}

SColor GUIColorEditBox::getColor()
{
	return color;
}

void GUIColorEditBox::setColor( SColor pColor, bool notifyParent )
{
	color = pColor;
	convertColorToText();

	if ( Parent && notifyParent )
	{
		SEvent newEvent;
		newEvent.EventType = EET_GUI_EVENT;
		newEvent.GUIEvent.Caller = this;
		newEvent.GUIEvent.Element = 0;
		newEvent.GUIEvent.EventType = EGET_EDITBOX_CHANGED;
		Parent->OnEvent(newEvent);
	}
}

bool GUIColorEditBox::OnEvent( const SEvent& event )
{
	SEvent newEvent;

	if ( isEnabled() && isVisible() )
	switch( event.EventType )
	{
	case EET_MOUSE_INPUT_EVENT:
		if ( event.MouseInput.isLeftPressed() )
			return setCursorIndexFromPosition(
						vector2di(event.MouseInput.X, event.MouseInput.Y)
						);
		return false;

	case EET_KEY_INPUT_EVENT:
		if ( event.KeyInput.PressedDown )
		if ( insertChar( event.KeyInput.Key ) )
		{
			if ( Parent )
			{
				
				newEvent.EventType = EET_GUI_EVENT;
				newEvent.GUIEvent.Caller = this;
				newEvent.GUIEvent.Element = 0;
				newEvent.GUIEvent.EventType = EGET_EDITBOX_CHANGED;
				Parent->OnEvent(newEvent);
			}
			return true;
		}
		break;

	case EET_GUI_EVENT:
		switch ( event.GUIEvent.EventType )
		{
		case EGET_ELEMENT_FOCUSED:
			updateCursorRect();
			break;

		case EGET_ELEMENT_FOCUS_LOST:
			break;

		default: break;
		}

	default: break;
	}

	return IGUIElement::OnEvent(event);
}

void GUIColorEditBox::draw()
{
	if ( !isVisible() )
		return;

	IGUISkin* skin = Environment->getSkin();

	if ( Environment->getFocus() == this )
	{
		skin->draw3DSunkenPane(
				this,
				skin->getColor(EGDC_EDITABLE),
				true, true,
				AbsoluteRect, &AbsoluteClippingRect );

		skin->getFont(irr::gui::EGDF_DEFAULT)->draw(
							Text,
							AbsoluteRect,
							skin->getColor(EGDC_BUTTON_TEXT),
							true, true, // center
							&AbsoluteClippingRect
							);

		// Draw transitioning-color cursor
		u32 ft = focusTime % 1000;
		if ( ft > 500 )
		{
			ft = 1000 - ft;
		}
		ft /= 4;
		cursorColor.setAlpha( core::clamp( ft, (u32)0, (u32)255 ) );

		recti cR = cursorRect + AbsoluteRect.UpperLeftCorner;
		skin->draw2DRectangle(
							this,
							cursorColor,
							cR,
							&AbsoluteClippingRect
							);
	} else {
		skin->draw3DSunkenPane(
				this,
				skin->getColor(EGDC_GRAY_EDITABLE),
				true, true,
				AbsoluteRect, &AbsoluteClippingRect );

		// Draw grayed text
		skin->getFont()->draw(
						Text,
						AbsoluteRect,
						skin->getColor(EGDC_GRAY_TEXT),
						true, true, // center
						&AbsoluteClippingRect
						);
	}
}

void GUIColorEditBox::OnPostRender(u32 timeMs)
{
	focusTime = timeMs;
}

bool GUIColorEditBox::setCursorIndexFromPosition( vector2di pPos )
{
	// Actual location
	vector2di cursorPos = pPos - AbsoluteRect.UpperLeftCorner;
	if ( ! AbsoluteClippingRect.isPointInside( pPos ) )
		return false;

	// Calculate by starting with the start position
	dimension2du textSize = Environment->getSkin()->getFont()->getDimension(Text.c_str());
	cursorPos.X -= (AbsoluteRect.getWidth() - textSize.Width)/2;
	cursorPos.Y -= (AbsoluteRect.getHeight() - textSize.Height)/2;

	// Cursor must be in text area
	if ( cursorPos.X < 0 || (u32)cursorPos.X >= textSize.Width
		|| cursorPos.Y < 0 || (u32)cursorPos.Y >= textSize.Height)
	{
		return false;
	}

	// KEEP for FAST version
	//cursorIdx = u32( (cursorPos.X * 8) / textSize.Width );

	cursorIdx = Environment->getSkin()->getFont()->getCharacterFromPos(Text.c_str(), cursorPos.X);

	updateCursorRect();

	return true;
}

void GUIColorEditBox::updateCursorRect()
{
	dimension2du textSize = Environment->getSkin()->getFont()->getDimension(Text.c_str());

	vector2di center( RelativeRect.getWidth(), RelativeRect.getHeight() );
	center /= 2;
	vector2di start = center - vector2di( textSize.Width/2, textSize.Height/2 );

	// KEEP THIS FOR FASTER CODE
	//cursorRect.UpperLeftCorner.X = start.X + (textSize.Width * cursorIdx)/8 - 1;
	//cursorRect.UpperLeftCorner.Y = start.Y;
	//cursorRect.LowerRightCorner.X = start.X + (textSize.Width * (cursorIdx+1))/8;
	//cursorRect.LowerRightCorner.Y = start.Y + textSize.Height;

	s32 indexCoord = 0;
	s32 lastIndexCoord = 0;
	core::stringw shortText;
	if ( cursorIdx > 0 )
	{
		shortText = Text.subString(0,cursorIdx);
		indexCoord = Environment->getSkin()->getFont()->getDimension( shortText.c_str() ).Width;
	}
	shortText = Text.subString(cursorIdx,1);
	lastIndexCoord = Environment->getSkin()->getFont()->getDimension( shortText.c_str() ).Width;

	cursorRect.UpperLeftCorner.X = start.X + indexCoord;
	cursorRect.UpperLeftCorner.Y = start.Y;
	cursorRect.LowerRightCorner.X = start.X + indexCoord + lastIndexCoord;
	cursorRect.LowerRightCorner.Y = start.Y + textSize.Height;

	cursorRect.repair();
}

bool GUIColorEditBox::insertChar( EKEY_CODE pKeyCode )
{
	u32 cursorByte = (7-cursorIdx)*4;
	//u32 savedColor = color.color & ( 0xffffffff ^ (0xf0000000 >> cursorByte) );
	u32 savedColor = color.color & ( ~ ( 0x0000000f << cursorByte ) );

	switch ( pKeyCode )
	{
		// Cursor movement
	case KEY_HOME:
		cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_END:
		cursorIdx = 7;
		updateCursorRect();
		return true;

	case KEY_DELETE:
		clear();
		cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_LEFT:
		if ( cursorIdx > 0 )
			cursorIdx--;
		updateCursorRect();
		return true;

	case KEY_RIGHT:
		if ( cursorIdx < 7 )
			cursorIdx++;
		updateCursorRect();
		return true;

		// Inserting characters
	case KEY_KEY_0:
	case KEY_NUMPAD0:
		color.color = savedColor;
		Text[cursorIdx] = '0';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_1:
	case KEY_NUMPAD1:
		color.color = (1 << cursorByte) | savedColor;
		Text[cursorIdx] = '1';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_2:
	case KEY_NUMPAD2:
		color.color = (2 << cursorByte) | savedColor;
		Text[cursorIdx] = '2';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_3:
	case KEY_NUMPAD3:
		color.color = (3 << cursorByte) | savedColor;
		Text[cursorIdx] = '3';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_4:
	case KEY_NUMPAD4:
		color.color = (4 << cursorByte) | savedColor;
		Text[cursorIdx] = '4';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_5:
	case KEY_NUMPAD5:
		color.color = (5 << cursorByte) | savedColor;
		Text[cursorIdx] = '5';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_6:
	case KEY_NUMPAD6:
		color.color = (6 << cursorByte) | savedColor;
		Text[cursorIdx] = '6';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_7:
	case KEY_NUMPAD7:
		color.color = (7 << cursorByte) | savedColor;
		Text[cursorIdx] = '7';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_8:
	case KEY_NUMPAD8:
		color.color = (8 << cursorByte) | savedColor;
		Text[cursorIdx] = '8';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_9:
	case KEY_NUMPAD9:
		color.color = (9 << cursorByte) | savedColor;
		Text[cursorIdx] = '9';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_A:
		// same as: color.color = (0xa0 << cursorByte) | savedColor;
		color.color = (10 << cursorByte) | savedColor;
		Text[cursorIdx] = 'a';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_B:
		color.color = (11 << cursorByte) | savedColor;
		Text[cursorIdx] = 'b';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_C:
		color.color = (12 << cursorByte) | savedColor;
		Text[cursorIdx] = 'c';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_D:
		color.color = (13 << cursorByte) | savedColor;
		Text[cursorIdx] = 'd';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_E:
		color.color = (14 << cursorByte) | savedColor;
		Text[cursorIdx] = 'e';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	case KEY_KEY_F:
		color.color = (15 << cursorByte) | savedColor;
		Text[cursorIdx] = 'f';
		cursorIdx++;
		if ( cursorIdx > 7 ) cursorIdx = 0;
		updateCursorRect();
		return true;

	default: break;
	}

	return false;
}

void GUIColorEditBox::convertTextToColor()
{
	color.color = 0;
	u32 i;
	for ( i=0; i < 8; i++ )
	{
		if ( Text[i] <= 'f' && Text[i] >= 'a' )
			color.color |= (Text[i] - 'a' + 10) >> (i*4);

		else if ( Text[i] <= '9' && Text[i] >= '0' )
			color.color |= (Text[i] - '0') >> (i*4);

		// else
		// User must have used setText()
	}
}

void GUIColorEditBox::convertColorToText()
{
	u32 i;
	u8 chr;
	for ( i=0; i < 8; i++ )
	{
		/* On this computer, when going FROM u32 TO u8, you have to shift the
		bits to the right. I will probably need to have this check for endianess
		if this is going to be portable code. */
		chr = u8( ( color.color & (0xf0000000 >> (i*4)) ) >> ((7-i)*4) );
		if ( chr > 9 )
			Text[i] = c8( (chr-10) + 'a' );
		else
			Text[i] = c8( chr + '0' );
	}
}

void GUIColorEditBox::clear()
{
	color.color = 0;
	Text = L"00000000";
}

}
}
