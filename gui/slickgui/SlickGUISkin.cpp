// (c) 2020 Nicolaus Anderson

#include "SlickGUISkin.h"
#include <IGUIFont.h>
#include <IVideoDriver.h>
#include <IGUISpriteBank.h>
#include <IGUIEnvironment.h>
#include <IGUIElement.h>

namespace irr {
namespace gui {

SlickGUISkin::SlickGUISkin( IGUIEnvironment* environment, bool startIconsFromZero, bool drawMoreRound )
	: DrawMoreRound(drawMoreRound)
	, Environment(environment)
	, VideoDriver(environment->getVideoDriver())
{
	Colors[EGDC_3D_DARK_SHADOW]		= 0xff000000;
	Colors[EGDC_3D_SHADOW]			= 0xff020202;
	Colors[EGDC_3D_FACE]			= 0xff363636;
	Colors[EGDC_3D_HIGH_LIGHT]		= 0xffb1b1b1;
	Colors[EGDC_3D_LIGHT]			= 0xff6e6e6e;
	Colors[EGDC_ACTIVE_BORDER]		= 0xffb1b1b1;
	Colors[EGDC_ACTIVE_CAPTION]		= 0xff006078;
	Colors[EGDC_APP_WORKSPACE]		= 0xff111315;
	Colors[EGDC_BUTTON_TEXT]		= 0xffa6a6a6;
	Colors[EGDC_GRAY_TEXT]			= 0xff6e6e6e;
	Colors[EGDC_HIGH_LIGHT]			= 0xff000000;
	Colors[EGDC_HIGH_LIGHT_TEXT]	= 0xffc5c5c5;
	Colors[EGDC_INACTIVE_BORDER]	= 0xff6e6e6e;
	Colors[EGDC_INACTIVE_CAPTION]	= 0xff475760;
	Colors[EGDC_TOOLTIP]			= 0xffffffff;
	Colors[EGDC_TOOLTIP_BACKGROUND]	= 0xff15587b;
	Colors[EGDC_SCROLLBAR]			= 0xff1b1f24;
	Colors[EGDC_WINDOW]				= 0xff1f1f1f;
	Colors[EGDC_WINDOW_SYMBOL]		= 0xfffcfcfc;
	Colors[EGDC_ICON]				= 0xffb2bac3;
	Colors[EGDC_ICON_HIGH_LIGHT]	= 0xfffcfcfc;
	Colors[EGDC_GRAY_WINDOW_SYMBOL]	= 0xff767b82;
	Colors[EGDC_EDITABLE]			= 0xff051720;
	Colors[EGDC_GRAY_EDITABLE]		= 0xff15252c;
	Colors[EGDC_FOCUSED_EDITABLE]	= 0xff051720;

	Sizes[EGDS_SCROLLBAR_SIZE] = 20;
	Sizes[EGDS_MENU_HEIGHT] = 30;
	Sizes[EGDS_WINDOW_BUTTON_WIDTH] = 20;
	Sizes[EGDS_CHECK_BOX_WIDTH] = 10;
	Sizes[EGDS_BUTTON_WIDTH] = 100;
	Sizes[EGDS_BUTTON_HEIGHT] = 40;
	Sizes[EGDS_TEXT_DISTANCE_X] = 5;
	Sizes[EGDS_TEXT_DISTANCE_Y] = 4;
	Sizes[EGDS_TITLEBARTEXT_DISTANCE_X] = 5;
	Sizes[EGDS_TITLEBARTEXT_DISTANCE_Y] = 4;
	Sizes[EGDS_MESSAGE_BOX_GAP_SPACE] = 17;
	Sizes[EGDS_MESSAGE_BOX_MIN_TEXT_WIDTH] = 0;
	Sizes[EGDS_MESSAGE_BOX_MAX_TEXT_WIDTH] = 500;
	Sizes[EGDS_MESSAGE_BOX_MIN_TEXT_HEIGHT] = 0;
	Sizes[EGDS_MESSAGE_BOX_MAX_TEXT_HEIGHT] = 99999;
	Sizes[EGDS_BUTTON_PRESSED_IMAGE_OFFSET_X] = 2;
	Sizes[EGDS_BUTTON_PRESSED_IMAGE_OFFSET_Y] = 2;
	Sizes[EGDS_BUTTON_PRESSED_TEXT_OFFSET_X] = 2;
	Sizes[EGDS_BUTTON_PRESSED_TEXT_OFFSET_Y] = 2;
	Sizes[EGDS_BUTTON_PRESSED_SPRITE_OFFSET_X] = 2;
	Sizes[EGDS_BUTTON_PRESSED_SPRITE_OFFSET_Y] = 2;

	if ( startIconsFromZero ) {
		Icons[EGDI_WINDOW_MAXIMIZE] = 0;
		Icons[EGDI_WINDOW_RESTORE] = 1;
		Icons[EGDI_WINDOW_CLOSE] = 2;
		Icons[EGDI_WINDOW_MINIMIZE] = 3;
		Icons[EGDI_WINDOW_RESIZE] = 4;
		Icons[EGDI_CURSOR_UP] = 5;
		Icons[EGDI_CURSOR_DOWN] = 6;
		Icons[EGDI_CURSOR_LEFT] = 7;
		Icons[EGDI_CURSOR_RIGHT] = 8;
		Icons[EGDI_MENU_MORE] = 9;
		Icons[EGDI_CHECK_BOX_CHECKED] = 10;
		Icons[EGDI_DROP_DOWN] = 11;
		Icons[EGDI_SMALL_CURSOR_UP] = 12;
		Icons[EGDI_SMALL_CURSOR_DOWN] = 13;
		Icons[EGDI_RADIO_BUTTON_CHECKED] = 14;
		Icons[EGDI_MORE_LEFT] = 15;
		Icons[EGDI_MORE_RIGHT] = 16;
		Icons[EGDI_MORE_UP] = 17;
		Icons[EGDI_MORE_DOWN] = 18;
		Icons[EGDI_EXPAND] = 19;
		Icons[EGDI_COLLAPSE] = 20;
		Icons[EGDI_FILE] = 21;
		Icons[EGDI_DIRECTORY] = 22;
	} else {
		Icons[EGDI_WINDOW_MAXIMIZE] = 225;
		Icons[EGDI_WINDOW_RESTORE] = 226;
		Icons[EGDI_WINDOW_CLOSE] = 227;
		Icons[EGDI_WINDOW_MINIMIZE] = 228;
		Icons[EGDI_CURSOR_UP] = 229;
		Icons[EGDI_CURSOR_DOWN] = 230;
		Icons[EGDI_CURSOR_LEFT] = 231;
		Icons[EGDI_CURSOR_RIGHT] = 232;
		Icons[EGDI_MENU_MORE] = 232;
		Icons[EGDI_CHECK_BOX_CHECKED] = 233;
		Icons[EGDI_DROP_DOWN] = 234;
		Icons[EGDI_SMALL_CURSOR_UP] = 235;
		Icons[EGDI_SMALL_CURSOR_DOWN] = 236;
		Icons[EGDI_RADIO_BUTTON_CHECKED] = 237;
		Icons[EGDI_MORE_LEFT] = 238;
		Icons[EGDI_MORE_RIGHT] = 239;
		Icons[EGDI_MORE_UP] = 240;
		Icons[EGDI_MORE_DOWN] = 241;
		Icons[EGDI_WINDOW_RESIZE] = 242;
		Icons[EGDI_EXPAND] = 243;
		Icons[EGDI_COLLAPSE] = 244;
		Icons[EGDI_FILE] = 245;
		Icons[EGDI_DIRECTORY] = 246;
	}

	Sprites = 0;

	for(u32 i=0; i<EGDF_COUNT; ++i) {
		Fonts[i] = 0;
	}

	Texts[ EGDT_MSG_BOX_OK ] = L"Ok";
	Texts[ EGDT_MSG_BOX_CANCEL ] = L"Cancel";
	Texts[ EGDT_MSG_BOX_YES ] = L"Yes";
	Texts[ EGDT_MSG_BOX_NO ] = L"No";
	Texts[ EGDT_WINDOW_CLOSE ] = L"Close";
	Texts[ EGDT_WINDOW_MAXIMIZE ] = L"Maximize";
	Texts[ EGDT_WINDOW_MINIMIZE ] = L"Minimize";
	Texts[ EGDT_WINDOW_RESTORE ] = L"Restore";
}

SlickGUISkin::~SlickGUISkin()
{
	for (u32 i=0; i<EGDF_COUNT; ++i)
	{
		if (Fonts[i])
			Fonts[i]->drop();
	}

	if (Sprites)
		Sprites->drop();
}

video::SColor SlickGUISkin::getColor(EGUI_DEFAULT_COLOR color) const
{
	if ( color < EGDC_COUNT )
		return Colors[color];

	return video::SColor();
}

void SlickGUISkin::setColor(EGUI_DEFAULT_COLOR which, video::SColor newColor)
{
	Colors[which] = newColor;
}

s32 SlickGUISkin::getSize(EGUI_DEFAULT_SIZE size) const
{
	if (size < EGDS_COUNT)
		return Sizes[size];

	return 0;
}

void SlickGUISkin::setSize(EGUI_DEFAULT_SIZE which, s32 size)
{
	if (which < EGDS_COUNT)
		Sizes[which] = size;
}

const wchar_t* SlickGUISkin::getDefaultText(EGUI_DEFAULT_TEXT which) const
{
	if (which < EGDT_COUNT)
		return Texts[which].c_str();
	return 0;
}

void SlickGUISkin::setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText)
{
	if ((u32)which < EGDT_COUNT)
		Texts[which] = newText;
}

IGUIFont* SlickGUISkin::getFont(EGUI_DEFAULT_FONT which) const
{
	IGUIFont* font = Fonts[which];
	if ( !font )
		return Fonts[EGDF_DEFAULT];
	return font;
}

void SlickGUISkin::setFont(IGUIFont* font, EGUI_DEFAULT_FONT which)
{
	if ((u32)which >= EGDF_COUNT)
		return;

	if (font)
	{
		font->grab();
		if (Fonts[which])
			Fonts[which]->drop();

		Fonts[which] = font;
	}
}

IGUISpriteBank* SlickGUISkin::getSpriteBank() const
{
	return Sprites;
}

void SlickGUISkin::setSpriteBank(IGUISpriteBank* bank)
{
	if (Sprites)
		Sprites->drop();

	Sprites = bank;

	if (Sprites)
		Sprites->grab();
}

u32 SlickGUISkin::getIcon(EGUI_DEFAULT_ICON icon) const
{
	if ((u32)icon < EGDI_COUNT)
		return Icons[icon];

	return 0;
}

void SlickGUISkin::setIcon(EGUI_DEFAULT_ICON icon, u32 index)
{
	if ((u32)icon < EGDI_COUNT)
		Icons[icon] = index;
}

void SlickGUISkin::draw3DButtonPaneStandard(
	IGUIElement* element,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip)
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );

	draw2DRectangle( element, getColor(EGDC_3D_FACE), rect, clip );

	core::vector2di shadowRight( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y-1 );
	core::vector2di shadowBottom( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y );
	draw2DVerticalLine( shadowRight, rect.getHeight()-2, getColor(EGDC_3D_SHADOW), clip );
	draw2DHorizontalLine( shadowBottom, rect.getWidth()-2, getColor(EGDC_3D_SHADOW), clip );

	const video::SColor regularColor = getColor(EGDC_3D_LIGHT);
	const video::SColor leftTopColor = Environment->hasFocus(element) ?
			getColor(EGDC_3D_HIGH_LIGHT) : getColor(EGDC_3D_LIGHT);

	drawRoundOutline( rect, clip, leftTopColor, regularColor, leftTopColor, regularColor );
}

void SlickGUISkin::draw3DButtonPanePressed(
	IGUIElement* element,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip)
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );

	draw2DRectangle( element, getColor(EGDC_WINDOW), rect, clip );

	core::vector2di shadowLeft( rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y+1 );
	core::vector2di shadowTop( rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y );
	draw2DVerticalLine( shadowLeft, rect.getHeight()-2, getColor(EGDC_3D_SHADOW), clip );
	draw2DHorizontalLine( shadowTop, rect.getWidth()-2, getColor(EGDC_3D_SHADOW), clip );

	const video::SColor regularColor = getColor(EGDC_3D_LIGHT);
	drawRoundOutline( rect, clip, regularColor, regularColor, regularColor, regularColor );
}

void SlickGUISkin::draw3DSunkenPane(
	IGUIElement* element,
	video::SColor bgcolor,
	bool flat,
	bool fillBackGround,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip)
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );

	// Ignore "flat" setting

	if ( fillBackGround ) {
		draw2DRectangle( element, bgcolor, rect, clip );
	}

	const video::SColor  outerBorderColor = getColor(EGDC_3D_LIGHT);
	drawSquareOutline(backgroundArea, clip, getColor(EGDC_3D_SHADOW));
	drawRoundOutline(rect, clip, outerBorderColor, outerBorderColor, outerBorderColor, outerBorderColor);
}

core::rect<s32> SlickGUISkin::draw3DWindowBackground(
	IGUIElement* element,
	bool drawTitleBar,
	video::SColor titleBarColor,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip,
	core::rect<s32>* checkClientArea)
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );

	core::rect<s32> titlebarArea( rect.UpperLeftCorner + core::vector2di(2),
							core::vector2di(
								rect.LowerRightCorner.X - 2,
								rect.UpperLeftCorner.Y + getSize(EGDS_WINDOW_BUTTON_WIDTH)
							));

	if ( checkClientArea ) {
		if ( drawTitleBar ) {
			checkClientArea->UpperLeftCorner =
				rect.UpperLeftCorner - core::vector2di(2, 2+getSize(EGDS_WINDOW_BUTTON_WIDTH));
		} else {
			checkClientArea->UpperLeftCorner = rect.UpperLeftCorner - core::vector2di(2);
		}
		checkClientArea->LowerRightCorner = rect.LowerRightCorner - core::vector2di(2);
		return titlebarArea + core::vector2di(5,5); // Offset titlebar text
	}

	// Draw background
	draw2DRectangle( element, getColor(EGDC_WINDOW), backgroundArea, clip );

	// Draw titlebar
	draw2DRectangle( element, Environment->hasFocus(element)?
					getColor(EGDC_ACTIVE_CAPTION) : getColor(EGDC_INACTIVE_CAPTION),
					titlebarArea, clip );

	const video::SColor  focusColor = Environment->hasFocus(element)?
							getColor(EGDC_3D_HIGH_LIGHT) : getColor(EGDC_3D_LIGHT);
	drawSquareOutline(backgroundArea, clip, getColor(EGDC_3D_SHADOW));
	drawRoundOutline(rect, clip, focusColor, getColor(EGDC_3D_LIGHT), focusColor, getColor(EGDC_3D_LIGHT));

	return titlebarArea + core::vector2di(5,5); // Offset titlebar text
}

void SlickGUISkin::draw3DMenuPane(
	IGUIElement* element,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip)
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );
	core::rect<s32> middleArea( rect.UpperLeftCorner + core::vector2di(2),
								rect.LowerRightCorner - core::vector2di(2) );

	draw2DRectangle( element, getColor(EGDC_3D_FACE), middleArea, clip );
	const video::SColor  outerBorderColor = getColor(EGDC_3D_LIGHT);
	drawSquareOutline(backgroundArea, clip, getColor(EGDC_3D_SHADOW));
	drawSquareOutline(rect, clip, getColor(EGDC_3D_LIGHT));
}

void SlickGUISkin::draw3DToolBar(
	IGUIElement* element,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip)
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );
	core::rect<s32> middleArea( rect.UpperLeftCorner + core::vector2di(2),
								rect.LowerRightCorner - core::vector2di(2) );

	draw2DRectangle( element, getColor(EGDC_3D_FACE), middleArea, clip );
	const video::SColor  outerBorderColor = getColor(EGDC_3D_LIGHT);
	drawSquareOutline(backgroundArea, clip, getColor(EGDC_3D_SHADOW));
	VideoDriver->draw2DRectangle( getColor(EGDC_3D_LIGHT), rect, clip );
}

void SlickGUISkin::draw3DTabButton(
	IGUIElement* element,
	bool active,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip,
	gui::EGUI_ALIGNMENT alignment)
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );
	VideoDriver->draw2DRectangle( getColor(EGDC_3D_FACE), backgroundArea, clip );

	video::SColor shadowColor = getColor(EGDC_3D_SHADOW);
	draw2DVerticalLine(backgroundArea.UpperLeftCorner, rect.getHeight()-1, shadowColor, clip); 
	core::vector2di upperRightCorner( backgroundArea.LowerRightCorner.X, backgroundArea.UpperLeftCorner.Y );
	draw2DVerticalLine(upperRightCorner, rect.getHeight()-1, shadowColor, clip);

	video::SColor borderColor = active ? getColor(EGDC_3D_HIGH_LIGHT) : getColor(EGDC_3D_LIGHT);
	core::rect<s32> trueRect = rect;
	core::vector2di lowerLeftCorner( backgroundArea.UpperLeftCorner.X, backgroundArea.LowerRightCorner.Y );
	if ( alignment == EGUIA_UPPERLEFT ) {
		trueRect.LowerRightCorner.Y += 1;
		draw2DHorizontalLine(lowerLeftCorner, rect.getWidth() - 2, shadowColor, clip);
		drawRoundOutline(trueRect, clip, borderColor, borderColor, borderColor, getColor(EGDC_3D_FACE));
	} else {
		trueRect.LowerRightCorner.Y -= 1;
		draw2DHorizontalLine(backgroundArea.UpperLeftCorner, rect.getWidth() - 2, shadowColor, clip);
		drawRoundOutline(trueRect, clip, borderColor, borderColor, getColor(EGDC_3D_FACE), borderColor);
	}
}

void SlickGUISkin::draw3DTabBody(
	IGUIElement* element,
	bool border,
	bool background,
	const core::rect<s32>& rect,
	const core::rect<s32>* clip,
	s32 tabHeight,
	gui::EGUI_ALIGNMENT alignment )
{
	core::rect<s32> backgroundArea( rect.UpperLeftCorner + core::vector2di(1),
								rect.LowerRightCorner - core::vector2di(1) );

	if ( background ) {
		if ( alignment == EGUIA_UPPERLEFT ) {
			backgroundArea.UpperLeftCorner.Y += tabHeight+1;
		} else {
			backgroundArea.LowerRightCorner.Y -= tabHeight+1;
		}
		VideoDriver->draw2DRectangle(getColor(EGDC_3D_FACE), backgroundArea, clip);
	}
	core::rect<s32> outerArea( backgroundArea.UpperLeftCorner - core::vector2di(1),
							backgroundArea.LowerRightCorner + core::vector2di(1) );
	if ( border ) {
		drawSquareOutline(backgroundArea, clip, getColor(EGDC_3D_SHADOW));
		drawSquareOutline(outerArea, clip, getColor(EGDC_3D_LIGHT));
	}
}

void SlickGUISkin::drawIcon(
	IGUIElement* element,
	EGUI_DEFAULT_ICON icon,
	const core::position2di position,
	u32 starttime,
	u32 currenttime,
	bool loop,
	const core::rect<s32>* clip)
{
	if (!Sprites)
		return;

	bool gray = element && !element->isEnabled();
	Sprites->draw2DSprite(Icons[icon], position, clip,
			Colors[gray? EGDC_GRAY_WINDOW_SYMBOL : EGDC_WINDOW_SYMBOL],
			starttime, currenttime, loop, true);
}

void SlickGUISkin::draw2DRectangle(
	IGUIElement* element,
	const video::SColor &color,
	const core::rect<s32>& pos,
	const core::rect<s32>* clip)
{
	VideoDriver->draw2DRectangle(color, pos, clip);
}

void SlickGUISkin::drawPixel( u32 x, u32 y, const video::SColor& color, const core::rect<s32>* clip )
{
	if ( clip ) {
		if ( !clip->isPointInside(core::vector2di(x,y)) )
			return;
	}
	VideoDriver->drawPixel(x,y,color);
}

void SlickGUISkin::draw2DVerticalLine( core::vector2d<s32> start, u32 length,
		const video::SColor& color, const core::rect<s32>* clip )
{
	core::vector2d<s32> finish( start.X, start.Y + length );

	if ( clip ) {
		if ( clip->getWidth() == 0 || clip->getHeight() == 0 )
			return;

		if ( clip->UpperLeftCorner.X > start.X )
			return; // Clipped horizontally

		if ( clip->UpperLeftCorner.Y > start.Y )
			start.Y = clip->UpperLeftCorner.Y;
		if ( clip->LowerRightCorner.Y < finish.Y )
			finish.Y = clip->LowerRightCorner.Y;

		if ( start.Y > finish.Y ) // Maybe should be start.Y >= finish.Y
			return;
	}

	VideoDriver->draw2DLine(start, finish, color);
}

void SlickGUISkin::draw2DHorizontalLine( core::vector2d<s32> start, u32 length,
		const video::SColor& color, const core::rect<s32>* clip )
{
	core::vector2d<s32> finish( start.X + length, start.Y );

	if ( clip ) {
		if ( clip->getWidth() == 0 || clip->getHeight() == 0 )
			return;

		if ( clip->UpperLeftCorner.Y > start.Y )
			return; // Clipped vertically

		if ( clip->UpperLeftCorner.X > start.X )
			start.X = clip->UpperLeftCorner.X;
		if ( clip->LowerRightCorner.X < finish.X )
			finish.X = clip->LowerRightCorner.X;

		if ( start.X > finish.X ) // Maybe should be start.X >= finish.X
			return;
	}

	VideoDriver->draw2DLine(start, finish, color);
}

void SlickGUISkin::drawRoundOutline(
	const core::rect<s32>& rect, const core::rect<s32>* clip,
	const video::SColor& colorLeft, const video::SColor& colorRight,
	const video::SColor& colorTop, const video::SColor& colorBottom
	)
{
	const s32 left = rect.UpperLeftCorner.X;
	const s32 right = rect.LowerRightCorner.X;
	const s32 top = rect.UpperLeftCorner.Y;
	const s32 bottom = rect.LowerRightCorner.Y;

	if ( DrawMoreRound ) {
		draw2DVerticalLine( core::vector2di(left, top+2), bottom-top-3, colorLeft );
		draw2DHorizontalLine( core::vector2di(left+2, top), right-left-3, colorTop );
		draw2DVerticalLine( core::vector2di(right, top+2), bottom-top-3, colorRight );
		draw2DHorizontalLine( core::vector2di(left+2, bottom), right-left-3, colorBottom );

		drawPixel( left+1, top+1, colorTop, clip );
		drawPixel( right-1, top+1, colorTop, clip );
		drawPixel( left+1, bottom-1, colorBottom, clip );
		drawPixel( right-1, bottom-1, colorBottom, clip );
	} else {
		draw2DVerticalLine( core::vector2di(left, top+1), bottom-top-1, colorLeft );
		draw2DHorizontalLine( core::vector2di(left+1, top), right-left-1, colorTop );
		draw2DVerticalLine( core::vector2di(right, top+1), bottom-top-1, colorRight );
		draw2DHorizontalLine( core::vector2di(left+1, bottom), right-left-1, colorBottom );
	}
}

/*
void SlickGUISkin::drawSquareOutline(
	const core::rect<s32>& rect, const core::rect<s32>* clip,
	const video::SColor& colorLeft, const video::SColor& colorRight,
	const video::SColor& colorTop, const video::SColor& colorBottom
	)
{
	const s32 left = pos.UpperLeftCorner.X;
	const s32 right = pos.LowerRightCorner.X;
	const s32 top = pos.UpperLeftCorner.Y;
	const s32 bottom = pos.LowerRightCorner.Y;

	draw2DVerticalLine( vector2di(left, top+1), bottom-top, colorLeft, clip );
	draw2DHorizontalLine( vector2di(left, top), right-left, colorTop, clip);
	draw2DVerticalLine( vector2di(right, top+1), bottom-top, colorRight, clip );
	draw2DHorizontalLine( vector2di(left, bottom), right-left, colorBottom, clip);
}
*/

void SlickGUISkin::drawSquareOutline(
	const core::rect<s32>& rect, const core::rect<s32>* clip,
	const video::SColor& color )
{
	const s32 left = rect.UpperLeftCorner.X;
	const s32 right = rect.LowerRightCorner.X;
	const s32 top = rect.UpperLeftCorner.Y;
	const s32 bottom = rect.LowerRightCorner.Y;

	draw2DVerticalLine( core::vector2di(left, top+1), bottom-top, color, clip );
	draw2DHorizontalLine( core::vector2di(left, top), right-left, color, clip);
	draw2DVerticalLine( core::vector2di(right, top+1), bottom-top, color, clip );
	draw2DHorizontalLine( core::vector2di(left, bottom), right-left, color, clip);
}

} // namespace gui
} // namespace irr
