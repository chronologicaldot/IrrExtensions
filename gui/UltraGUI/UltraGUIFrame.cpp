#include "UltraGUIFrame.h"

namespace irr
{
namespace gui
{

UltraGUIFrame::UltraGUIFrame(
	IGUIEnvironment* environment,
	ICursorControl* cursorc, /* Not implemented yet: Change cursor to arrow when menu is movable */
	irr::s32 id,
	IGUIElement* parent,
	recti rectangle
	)
	: IGUIElement(
		EGUIET_ELEMENT,
		environment,
		(parent? parent : environment->getRootGUIElement()),
		id,
		(rectangle!=recti()? rectangle : (parent? parent->getAbsolutePosition() : environment->getRootGUIElement()->getAbsolutePosition()))
		)
	, cursor( cursorc )
	, menuBarRows( 1 )
	, lastOpenBarX( 0 )
	, lastOpenRow( 0 )
	, leftSideSize(
		dimension2du(
			ULTRAGUI_DEFAULT_SIDE_MENU_WIDTH,
			ULTRAGUI_DEFAULT_SIDE_MENU_HEIGHT
			)
		)
	, rightSideSize(
		dimension2du(
			ULTRAGUI_DEFAULT_SIDE_MENU_WIDTH,
			ULTRAGUI_DEFAULT_SIDE_MENU_HEIGHT
			)
		)
	, centerSize(
		dimension2du(
			ULTRAGUI_DEFAULT_CENTRAL_MENU_WIDTH,
			ULTRAGUI_DEFAULT_CENTRAL_MENU_HEIGHT
			)
		)
	, barSlotSize(
		dimension2du(
			ULTRAGUI_DEFAULT_MENUBAR_BUTTON_WIDTH,
			ULTRAGUI_DEFAULT_MENUBAR_BUTTON_HEIGHT
			)
		)
	, headerHeight( ULTRAGUI_DEFAULT_MENU_HEADER_HEIGHT )
	, spriteBank( 0 )
{
	/* Save the size for use in scaling the menus if this frame
	is ever resized. */
	oldRect = (rectangle!=recti()? rectangle : (parent? parent->getAbsolutePosition() : environment->getRootGUIElement()->getAbsolutePosition()));

	/* Set the size of the bar on which everything is drawn. */
	menuBarRect = recti(
					oldRect.UpperLeftCorner.X,
					oldRect.LowerRightCorner.Y - ULTRAGUI_DEFAULT_MENUBAR_BUTTON_HEIGHT,
					oldRect.LowerRightCorner.X,
					oldRect.LowerRightCorner.Y
					);
}

UltraGUIFrame::~UltraGUIFrame()
{
	if ( spriteBank )
		spriteBank->drop();

	// The IGUIElement destructor will release the children
}

UltraGUIMenu* UltraGUIFrame::addMenu( irr::s32 menuID, irr::video::ITexture* icon )
{
	// Rectangle of the menu (left-side shape)
	//recti shape(
	//	AbsoluteRect.UpperLeftCorner.X - (irr::s32)leftSideSize.Width, // Set off screen
	//	menuBarRect.UpperLeftCorner.Y - (irr::s32)leftSideSize.Height, // Set above the menu bar
	//	AbsoluteRect.UpperLeftCorner.X, // Set off screen
	//	menuBarRect.UpperLeftCorner.Y // Set above the menu bar
	//	);

	recti shape(
			AbsoluteRect.UpperLeftCorner.X,
			menuBarRect.UpperLeftCorner.Y - (irr::s32)leftSideSize.Height,
			AbsoluteRect.UpperLeftCorner.X + (irr::s32)leftSideSize.Width,
			menuBarRect.UpperLeftCorner.Y
			);

	// Create the menu
	// This step also makes the menu a child of this element
	UltraGUIMenu* menu = new UltraGUIMenu( this, menuID, shape );

	menu->setLeftSize( dimension2du(shape.getSize()) );
	menu->setRightSize( dimension2du(shape.getSize()) );
	menu->setCenterSize(
			dimension2du(
				ULTRAGUI_DEFAULT_CENTRAL_MENU_WIDTH,
				ULTRAGUI_DEFAULT_CENTRAL_MENU_HEIGHT
				)
			);
	menu->setBarSize(
			dimension2du(
				ULTRAGUI_DEFAULT_MENUBAR_BUTTON_WIDTH,
				ULTRAGUI_DEFAULT_MENUBAR_BUTTON_HEIGHT
				)
			);

	if ( icon )
		menu->setMenuBarSprite( icon );

	// Now have the menu locate it in the correct place
	menu->setMyState( EUGUIMState_ToLeft_Open, true );
	menu->snapState();

	// Make sure the buttons appear
	menu->refreshButtons();
	// unnecessary when there is an updateAbsolutePosition() call

	return menu;
}

void UltraGUIFrame::removeMenu( irr::s32 menuID )
{
	// Search the children for the menu
	irr::core::list<IGUIElement*>::Iterator kid;
	for ( ; kid != Children.end(); ++kid )
	{
		if ( (*kid)->getID() == menuID )
		{
			/* Menu bar menus need to be removed to deallocate space on the bar,
			allowing other menus to occupy the slot once occupied by this menu. */
			if ( ((UltraGUIMenu*)(*kid))->getMenuLocation() == EUGUIMLoc_Bar )
			{
				leavingBar( (UltraGUIMenu*)(*kid) );
			}

			removeChild(*kid);
		}
	}
}

void UltraGUIFrame::closeMenu( irr::s32 menuID )
{
	UltraGUIMenu* result=0;

	if ( getMenu( menuID, result ) )
		result->setMyState( EUGUIMState_Closed );
}

void UltraGUIFrame::closeOpenMenuAtLocation( EUltraMenuLocation location )
{
	irr::core::list<IGUIElement*>::Iterator kid = Children.begin();
	for ( ; kid != Children.end(); ++kid )
	{
		if ( ((UltraGUIMenu*)(*kid))->getMenuLocation() == location )
		{
			((UltraGUIMenu*)(*kid))->setMyState( EUGUIMState_Closed );
		}
	}
}

void UltraGUIFrame::openMenu( irr::s32 menuID )
{
	UltraGUIMenu* result=0;
	irr::core::list<IGUIElement*>::Iterator kid = Children.begin();

	if ( !getMenu( menuID, result ) )
		return;

	for ( ; kid != Children.end(); ++kid )
	{
		if ( ((UltraGUIMenu*)(*kid))->getMenuLocation() == result->getMenuLocation() )
		{
			if ( (*kid) == result )
			{
				((UltraGUIMenu*)(*kid))->setMyState( EUGUIMState_Open);
			} else {
				((UltraGUIMenu*)(*kid))->setMyState( EUGUIMState_Closed );
			}
		}
	}
}

void UltraGUIFrame::moveMenu( irr::s32 menuID, EUltraMenuLocation location )
{
	UltraGUIMenu* result=0;

	if ( !getMenu( menuID, result ) )
		return;

	switch ( result->getMenuState() )
	{
	case EUGUIMState_Open:
		switch ( location )
		{
		case EUGUIMLoc_Right:
			result->setMyState( EUGUIMState_ToRight_Open );
			break;
		case EUGUIMLoc_Center:
			result->setMyState( EUGUIMState_ToCenter_Open);
			break;
		case EUGUIMLoc_Bar:
			result->setMyState( EUGUIMState_ToBar );
			break;
		default: // Move to left
			result->setMyState( EUGUIMState_ToLeft_Open );
			break;
		}
		break;

	default:
		// Assume closed state
		switch ( location )
		{
		case EUGUIMLoc_Right:
			result->setMyState( EUGUIMState_ToRight_Closed );
			break;
		case EUGUIMLoc_Center:
			result->setMyState( EUGUIMState_ToCenter_Closed );
			break;
		case EUGUIMLoc_Bar:
			result->setMyState( EUGUIMState_ToBar );
			break;
		default: // Move to left
			result->setMyState( EUGUIMState_ToLeft_Closed );
			break;
		}
		break;
	}

	//updateAbsolutePosition();
}

void UltraGUIFrame::setMenuState( irr::s32 menuID, EUltraMenuState state )
{
	/* Note that the opening of any menu requires the closing of
	another unless that menu is a bar menu (in which case it can't close). */

	// Find the menu
	UltraGUIMenu* menu =0;	

	// Menu not found, so terminate processing
	if ( !getMenu( menuID, menu ) )
		return;

	/* Both the state and location of the menu determine the next
	course of action. */
	switch ( state )
	{
	case EUGUIMState_Opening:
	case EUGUIMState_Open:
		closeOpenMenuAtLocation( menu->menuLocation );
		menu->setMyState( EUGUIMState_Open );
		bringToFront( menu );
		break;

	case EUGUIMState_Closing:
	case EUGUIMState_Closed:
		menu->setMyState( EUGUIMState_Closed );
		break;

	default:
		menu->setMyState( state );

		if ( state == EUGUIMState_ToLeft_Open
			|| state == EUGUIMState_ToRight_Open
			|| state == EUGUIMState_ToCenter_Open
			)
		{
			bringToFront(menu);
		}

		break;
	}
}

bool UltraGUIFrame::getMenu( irr::s32 menuID, UltraGUIMenu*& result )
{
	irr::core::list<IGUIElement*>::Iterator it = Children.begin();
	for ( ; it != Children.end(); ++it )
	{
		if ( (*it)->getID() == menuID )
		{
			result = (UltraGUIMenu*)(*it);
			return true;
		}
	}

	// Not found
	return false;
}

void UltraGUIFrame::requestBarSpace( UltraGUIMenu* menu )
{
	/* Calculate if a new row needs to be added for the menu. */

	if ( lastOpenBarX + (irr::s32)menu->getBarSize().Width
		> menuBarRect.getWidth() ) // + ULTRAGUI_MENUBAR_PADDING )
	{
		// Add new row
		menuBarRows++;
		menuBarRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - (menuBarRows * ULTRAGUI_DEFAULT_MENUBAR_BUTTON_HEIGHT);

		// Set the new last opening
		lastOpenBarX = 0; // ULTRAGUI_MENUBAR_PADDING;
		occupiedBarX = menu->getBarSize().Width; // ULTRAGUI_MENUBAR_PADDING;
		lastOpenRow++;
	} else {
		occupiedBarX = lastOpenBarX + menu->getBarSize().Width;
	}

	/* Since minimized bars should not have the focus, search
	for the first open menu that is not on the bar. */
	irr::core::list<IGUIElement*>::Iterator iter = Children.begin();
	for ( ; iter != Children.end(); ++iter )
	{
		if ( *iter != menu
			&& ((UltraGUIMenu*)*iter)->isTrulyOpen() )
		{
			bringToFront( *iter );
			break;
		}
	}
}

void UltraGUIFrame::leavingBar( UltraGUIMenu* menu )
{
	// Reset and reprepare
	lastOpenBarX = 0; // ULTRAGUI_MENUBAR_PADDING;
	occupiedBarX = 0; // ULTRAGUI_MENUBAR_PADDING;
	menuBarRows = 1;
	menuBarRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - (menuBarRows * ULTRAGUI_DEFAULT_MENUBAR_BUTTON_HEIGHT);

	irr::core::list<IGUIElement*>::Iterator iter = Children.begin();
	for ( ; iter != Children.end(); ++iter )
	{
		if ( *iter != menu
			&& ((UltraGUIMenu*)*iter)->getMenuLocation() == EUGUIMLoc_Bar )
		{
			((UltraGUIMenu*)*iter)->refresh();
		}
	}
}

IGUISpriteBank* UltraGUIFrame::getMenuSpriteBank()
{
	// Has the sprite bank already been created?
	if ( spriteBank )
	{
		return spriteBank;
	}

	/* Create the sprite bank for the button highlighters.
	One to two sprites will be created:
	1) An on-mouse-hover sprite, which is blue.
	2) An on-click sprite, which is white. */
	spriteBank = getEnvironment()->addEmptySpriteBank( ULTRAGUI_RESOURCE_PATH + "ultraMenuSpriteBank" );

	/* Create the frames for the hover sprite. Note that all of them have
	the same size. */
	SGUISpriteFrame noHoverFrame;
	noHoverFrame.rectNumber = 0;
	noHoverFrame.textureNumber = 0;

	SGUISpriteFrame hoverFrame0;
	hoverFrame0.rectNumber = 0;
	hoverFrame0.textureNumber = 1;

	SGUISpriteFrame hoverFrame1;
	hoverFrame1.rectNumber = 0;
	hoverFrame1.textureNumber = 2;

	SGUISpriteFrame hoverFrame2;
	hoverFrame2.rectNumber = 0;
	hoverFrame2.textureNumber = 3;

	SGUISpriteFrame hoverFrame3;
	hoverFrame3.rectNumber = 0;
	hoverFrame3.textureNumber = 4;

	// sprite for when the mouse hovers over the button
	SGUISprite hoverSprite;
	hoverSprite.frameTime = 100; // 100 ms
	hoverSprite.Frames.push_back( noHoverFrame );
	hoverSprite.Frames.push_back( hoverFrame0 );
	hoverSprite.Frames.push_back( hoverFrame1 );
	hoverSprite.Frames.push_back( hoverFrame2 );
	hoverSprite.Frames.push_back( hoverFrame3 );

	// sprite for when the mouse moves off the button
	// just a simple reverse of the first sprite
	SGUISprite hoverOffSprite;
	hoverOffSprite.frameTime = 100; // 100 ms
	hoverOffSprite.Frames.push_back( hoverFrame2 );
	hoverOffSprite.Frames.push_back( hoverFrame1 );
	hoverOffSprite.Frames.push_back( hoverFrame0 );
	hoverOffSprite.Frames.push_back( noHoverFrame );

	// add the sprites to the bank
	spriteBank->getSprites().push_back( hoverSprite );
	spriteBank->getSprites().push_back( hoverOffSprite );

	// add the button area rectangle to the bank
	spriteBank->getPositions().push_back(
		irr::core::recti(
			0,0,
			ULTRAGUI_DEFAULT_MENU_BUTTON_SIZE,
			ULTRAGUI_DEFAULT_MENU_BUTTON_SIZE
			)
		);

	// add the on-hover textures to the bank
	spriteBank->addTexture(
		getEnvironment()->getVideoDriver()->getTexture(
			ULTRAGUI_RESOURCE_PATH + ULTRAGUI_RESOURCE_NOHOVERTEXTURE
			) );

	spriteBank->addTexture(
		getEnvironment()->getVideoDriver()->getTexture(
			ULTRAGUI_RESOURCE_PATH + ULTRAGUI_RESOURCE_HOVERTEXTURE_0
			) );

	spriteBank->addTexture(
		getEnvironment()->getVideoDriver()->getTexture(
			ULTRAGUI_RESOURCE_PATH + ULTRAGUI_RESOURCE_HOVERTEXTURE_1
			) );

	spriteBank->addTexture(
		getEnvironment()->getVideoDriver()->getTexture(
			ULTRAGUI_RESOURCE_PATH + ULTRAGUI_RESOURCE_HOVERTEXTURE_2
			) );

	spriteBank->addTexture(
		getEnvironment()->getVideoDriver()->getTexture(
			ULTRAGUI_RESOURCE_PATH + ULTRAGUI_RESOURCE_HOVERTEXTURE_3
			) );


	return spriteBank;
}

bool UltraGUIFrame::OnEvent( const irr::SEvent& event )
{
	// For now, unless features are added
	return IGUIElement::OnEvent(event);
}

void UltraGUIFrame::updateAbsolutePosition()
{
	// Reset the menu bar count and rows
	lastOpenBarX = ULTRAGUI_MENUBAR_PADDING;
	lastOpenRow = 0;

	// Save the old rectangle
	// It will be needed by the menus for repositioning and resizing
	oldRect = AbsoluteRect;

	// Do NOT do: IGUIElement::updateAbsolutePosition();
	// since the children should be updated last.
	recalculateAbsolutePosition(false);
	/* Recalculating here updates the AbsoluteRect, making its size different
	from the previous size (in oldSize) if in fact the size has been changed. */

	// Update the child sizes stored here (in ultra frame)
	// By this, I mean update the sizes in case the frame itself has changed.
	// This is probably unnecessary.
	// To do??

	// Update the menu bar rectangle
	/* This is necessary in the event that another slot on the
	menu bar is requested. */
	menuBarRect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X;
	menuBarRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	menuBarRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	menuBarRect.UpperLeftCorner.Y =
					AbsoluteRect.LowerRightCorner.Y
					- menuBarRows * ULTRAGUI_DEFAULT_MENUBAR_BUTTON_HEIGHT;
	/* Eventually, I may add a spacer between the top of the bar and the
	button. However, it would require considering such spacing in the
	function getBarSlotPosition(). */

	// Now update the child elements
	irr::core::list<IGUIElement*>::Iterator it = Children.begin();
	for (; it != Children.end(); ++it)
	{
		(*it)->updateAbsolutePosition();
	}
}

void UltraGUIFrame::draw()
{
	if ( !isVisible() )
		return;

	// Draw the menu bar
	Environment->getSkin()->draw3DMenuPane(this, menuBarRect);

	// Draw Everything else
	IGUIElement::draw();
}

void UltraGUIFrame::OnPostRender(irr::u32 timeMs)
{
	IGUIElement::OnPostRender(timeMs);
}

bool UltraGUIFrame::isPointInside( const irr::core::vector2di& point ) const
{
	/* Normally, the clipping rectangle is used.
	Whether it should be used here is unknown, but AbsoluteRect
	should work. */
	return AbsoluteRect.isPointInside(point);
}

vector2di UltraGUIFrame::getLeftSidePosition()
{
	// Place on the left side just above the menu bar
	return vector2di(
			AbsoluteRect.UpperLeftCorner.X,
			AbsoluteRect.LowerRightCorner.Y - menuBarRect.getHeight()
			);
}

vector2di UltraGUIFrame::getRightSidePosition()
{
	// Place on the right size just above the menu bar
	return vector2di(
			AbsoluteRect.LowerRightCorner.X,
			AbsoluteRect.LowerRightCorner.Y - menuBarRect.getHeight()
			);
}

vector2di UltraGUIFrame::getCenterCenterPosition()
{
	// Place directly in the center of the screen (for now)
	return (AbsoluteRect.UpperLeftCorner + AbsoluteRect.LowerRightCorner)/2;
	// same as AR.ULC + (AR.LRC - AR.ULC)/2
}

vector2di UltraGUIFrame::getBarSlotPosition()
{
	/* This requires that the first position be reset by
	updateAbsolutePosition of this class. */
	
	vector2di slot( lastOpenBarX, menuBarRect.LowerRightCorner.Y - menuBarRect.getHeight() );

	/* Indicate the slot has now been occupied by setting
	the next open slot to the location after this one. */
	lastOpenBarX = occupiedBarX;

	return slot;
}

dimension2du UltraGUIFrame::getCenterWindowSize()
{
	/* Calculate the size based on the defaults and the
	available screen size. */

	// Initialize the window size to the set size.
	dimension2du windowSize = centerSize;

	/* In the future, the window may be resized according to a
	user-selected option. */

	/* The window is supposed to be 90% of the width and 80% of
	the height of the available space AT MAXIMUM.
	If it is smaller than that, no change is made to its size. */
	irr::u32 widthMax = (irr::u32) ( 0.9f * (irr::f32)AbsoluteRect.getWidth() );
	irr::u32 heightMax = (irr::u32) ( 0.8f * (irr::f32)AbsoluteRect.getHeight() );

	if ( windowSize.Width > widthMax )
		windowSize.Width = widthMax;

	if ( windowSize.Height > heightMax )
		windowSize.Height = heightMax;

	return windowSize;
}

irr::s32 UltraGUIFrame::getBarHeight()
{
	return menuBarRect.getHeight();
}

bool UltraGUIFrame::isInLeftDragRegion( irr::s32 x, irr::s32 y )
{
	// Left side of the region
	if ( x < AbsoluteRect.UpperLeftCorner.X )
		return false;

	// Bottom of the region
	if ( y > AbsoluteRect.LowerRightCorner.Y - menuBarRect.getHeight() )
		return false;

	// Right side of the region = 15% frame width (as opposed to leftSideSize.Width)
	if ( x > AbsoluteRect.UpperLeftCorner.X + (AbsoluteRect.getWidth()*0.15) )
		return false;

	// Top of the region = 15% frame height (as opposed to leftSideSize.Height)
	if ( y < AbsoluteRect.LowerRightCorner.Y - (AbsoluteRect.getHeight()*0.85) - menuBarRect.getHeight() )
		return false;

	// Must be inside
	return true;
}

bool UltraGUIFrame::isInRightDragRegion( irr::s32 x, irr::s32 y )
{
	// Right side of the region
	if ( x > AbsoluteRect.LowerRightCorner.X )
		return false;

	// Bottom of the region
	if ( y > AbsoluteRect.LowerRightCorner.Y - menuBarRect.getHeight() )
		return false;

	// Left side of the region = 15% frame width (as opposed to rightSideSize.Width)
	if ( x < AbsoluteRect.LowerRightCorner.X - (AbsoluteRect.getWidth()*0.15) )
		return false;

	// Top of the region = 15% frame height (as opposed to rightSideSize.Height)
	if ( y < AbsoluteRect.LowerRightCorner.Y - (AbsoluteRect.getHeight()*0.85) - menuBarRect.getHeight() )
		return false;

	// Must be inside
	return true;
}

bool UltraGUIFrame::isInCenterDragRegion( irr::s32 x, irr::s32 y )
{
	// Left side of the region = 35% frame width
	if ( x < AbsoluteRect.UpperLeftCorner.X + (AbsoluteRect.getWidth()*0.35) )
		return false;

	// Right side of the region = (100-35)% frame width
	if ( x > AbsoluteRect.LowerRightCorner.X - (AbsoluteRect.getWidth()*0.35) )
		return false;

	// Top of the region = 10% frame height
	if ( y < AbsoluteRect.UpperLeftCorner.X + (AbsoluteRect.getHeight()*0.1) )
		return false;

	// Bottom of the region = (100-35)% frame height
	// Note, this may cause collisions with the menu bar for very tiny screens, haha
	if ( y > AbsoluteRect.LowerRightCorner.Y - (AbsoluteRect.getHeight()*0.35) )
		return false;

	// Must be inside
	return true;
}

bool UltraGUIFrame::isInBarDragRegion( irr::s32 x, irr::s32 y )
{
	irr::core::vector2di pt(x,y);

	if ( menuBarRect.isPointInside( pt ) )
		return true;

	return false;
}

recti UltraGUIFrame::getFrameRectChange()
{
	vector2di ulc( AbsoluteRect.UpperLeftCorner - oldRect.UpperLeftCorner );
	vector2di lrc( AbsoluteRect.LowerRightCorner - oldRect.LowerRightCorner );

	return recti( ulc, lrc );
}

}} // end namespace gui and irr