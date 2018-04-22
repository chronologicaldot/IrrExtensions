/*
(c) 2013 Nicolaus Anderson
*/

#include "UltraGUIMenu.h"
#include "UltraGUIFrame.h"
#include <MoveGUIAnimator.h>

namespace irr
{
namespace gui
{

UltraGUIMenu::UltraGUIMenu(
	UltraGUIFrame* frame,
	irr::s32 id,
	irr::core::recti rectangle,
	irr::io::path resource_path
	)
	: IAnimGUIElement(
		EGUIET_ELEMENT,
		frame->getEnvironment(),
		frame,// was frame->getParent()
		id,
		irr::core::recti()//rectangle
		)
	, ultraframe( frame )
	, menuState( EUGUIMState_ToLeft_Open )
	, menuLocation( EUGUIMLoc_Left )
	, menuCloseSetting( EUGUIMClose_Minimize )
	, queuedState( EUGUIMState_ToLeft_Open )
	, inTransition( false )
	, refreshState( false )
	, transitionSpeed( ULTRAGUI_DEFAULT_MOVE_SPEED )
	, transitionColor( frame->getEnvironment()->getSkin()->getColor(EGDC_WINDOW) ) //( 50, 150, 150, 150 ) // transparent gray
	, lastTime( 0 )
	, currRect( rectangle )
	, destinyRect( rectangle )
	, leftSideSize(		irr::core::dimension2du(250,400) )
	, rightSideSize(	irr::core::dimension2du(250,400) )
	, centerSize(		irr::core::dimension2du(600,400) )
	, barSlotSize(		irr::core::dimension2du(100,40) )
	, barSprite( 0 )
	, headerColorTop( ULTRAGUI_DEFAULT_HEADER_COLOR_TOP )
	, headerColorMid( ULTRAGUI_DEFAULT_HEADER_COLOR_MID )
	, headerColorBottom( ULTRAGUI_DEFAULT_HEADER_COLOR_BOTTOM )
	, hasBarSpriteBorder( false )
	, buttonSize(
		ULTRAGUI_DEFAULT_MENU_BUTTON_SIZE,
		ULTRAGUI_DEFAULT_MENU_BUTTON_SIZE
		)
	, isSelected( false )
	, isDragging( false )
	, resetTime( 0 )
	, IsHighlighted( false )
	, highlightTime( 0 )
	, highlightColor( irr::video::SColor(0,125,125,255) )
	, loadHighlight( false )
{
	ultraframe->grab();

	/* Animator to be used in moving this element. */
	moveAnimator = new MoveGUIAnimator( &currRect, transitionSpeed, 0 );
	addAnimator( moveAnimator );

	/* Create the title text.
	This is updated with the buttons.
	Note that in bar mode, this is invisible. */
	titleTextBox = rectangle;
	titleTextBox.LowerRightCorner.Y = titleTextBox.UpperLeftCorner.Y + ultraframe->headerHeight;


	/* Prepare the buttons.
	Notice, the buttons are NOT children of this element.
	This is to keep them separate from the other children of this
	element because the buttons must be handled differently. */
	irr::core::recti btn_rect(0,0, (irr::s32)buttonSize.Width, (irr::s32)buttonSize.Height);

	closeButton = frame->getEnvironment()->addButton( btn_rect, this ); //, -1, 0, L"Close" );
	toLeftButton = frame->getEnvironment()->addButton( btn_rect, this ); //, -1, 0, L"Move to Left" );
	toRightButton =  frame->getEnvironment()->addButton( btn_rect, this ); //, -1, 0, L"Move to Right" );
	toCenterButton = frame->getEnvironment()->addButton( btn_rect, this ); //, -1, 0, L"Move to Center" );
	minimizeButton = frame->getEnvironment()->addButton( btn_rect, this ); //, -1, 0, L"Minimize" );

	/* Make the buttons sub-elements so that their events can be handled
	by this GUI element. Note that this GUI element must be the direct
	parent. */
	closeButton->setSubElement(true);
	toLeftButton->setSubElement(true);
	toRightButton->setSubElement(true);
	toCenterButton->setSubElement(true);
	minimizeButton->setSubElement(true);

	/* Remove the default border around the buttons (since the button
	textures drawn by this element take up the full button. */
	closeButton->setDrawBorder(false);
	toLeftButton->setDrawBorder(false);
	toRightButton->setDrawBorder(false);
	toCenterButton->setDrawBorder(false);
	minimizeButton->setDrawBorder(false);

	// Close button image
	closeButton->setImage(
		frame->getEnvironment()->getVideoDriver()->getTexture(
			resource_path + ULTRAGUI_RESOURCE_CLOSEBUTTON
			)
		);
	closeButton->setUseAlphaChannel(true);

	// To-Left button image
	toLeftButton->setImage(
		frame->getEnvironment()->getVideoDriver()->getTexture(
			resource_path + ULTRAGUI_RESOURCE_TOLEFTBUTTON
			)
		);
	toLeftButton->setUseAlphaChannel(true);

	// To-Right button image
	toRightButton->setImage(
		frame->getEnvironment()->getVideoDriver()->getTexture(
			resource_path + ULTRAGUI_RESOURCE_TORIGHTBUTTON
			)
		);
	toRightButton->setUseAlphaChannel(true);

	// To-center button image
	toCenterButton->setImage(
		frame->getEnvironment()->getVideoDriver()->getTexture(
			resource_path + ULTRAGUI_RESOURCE_TOCENTERBUTTON
			)
		);
	toCenterButton->setUseAlphaChannel(true);

	// Minimize button image
	minimizeButton->setImage(
		frame->getEnvironment()->getVideoDriver()->getTexture(
			resource_path + ULTRAGUI_RESOURCE_MINIMIZEBUTTON
			)
		);
	minimizeButton->setUseAlphaChannel(true);

	// Get the sprite bank
	IGUISpriteBank* spritebank = ultraframe->getMenuSpriteBank();

	if ( !spritebank )
		return;

	/* Assign the sprite bank to all of the buttons.
	All we want is for them to have the same effect on hover
	(and maybe mouse-pressed-down). */
	closeButton->setSpriteBank( spritebank );
	toLeftButton->setSpriteBank( spritebank );
	toRightButton->setSpriteBank( spritebank );
	toCenterButton->setSpriteBank( spritebank );
	minimizeButton->setSpriteBank( spritebank );

	/* Set the button to use the first sprite (hoverSprite) as the
	sprite for the hovering state and the second sprite (hoverOffSprite)
	as the sprite for the state when the mouse moves away. */
	closeButton->setSprite(
					EGBS_BUTTON_MOUSE_OVER,
					0, // hover sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					//irr::video::SColor(200,0,80,250), // blue used for coloring
					false // loop frames
					);
	closeButton->setSprite(
					EGBS_BUTTON_MOUSE_OFF,
					1, // hover off sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);

	toLeftButton->setSprite(
					EGBS_BUTTON_MOUSE_OVER,
					0, // hover sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);
	toLeftButton->setSprite(
					EGBS_BUTTON_MOUSE_OFF,
					1, // hover off sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);

	toRightButton->setSprite(
					EGBS_BUTTON_MOUSE_OVER,
					0, // hover sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);
	toRightButton->setSprite(
					EGBS_BUTTON_MOUSE_OFF,
					1, // hover off sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);

	toCenterButton->setSprite(
					EGBS_BUTTON_MOUSE_OVER,
					0, // hover sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);
	toCenterButton->setSprite(
					EGBS_BUTTON_MOUSE_OFF,
					1, // hover off sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);

	minimizeButton->setSprite(
					EGBS_BUTTON_MOUSE_OVER,
					0, // hover sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);
	minimizeButton->setSprite(
					EGBS_BUTTON_MOUSE_OFF,
					1, // hover off sprite index in the sprite bank
					ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR,
					false // loop frames
					);

	/* It is expected that UltraGUIFrame will call refreshButtons()
	since 1) it can't be done here (it isn't a constant function)
	and 2) other things will have to be done to this menu anyways
	(such as setting the dimensions). */

} // END FREAKING HUGE CONSTRUCTOR

UltraGUIMenu::~UltraGUIMenu()
{
	/* Because it was grabbed in the constructor, the frame
	is dropped here. */
	ultraframe->drop();

	if ( barSprite )
		barSprite->drop();

	closeButton->drop();
	toLeftButton->drop();
	toRightButton->drop();
	toCenterButton->drop();
	minimizeButton->drop();

	// Remove all animators
	clearAnimators();
}

void UltraGUIMenu::setMyState( EUltraMenuState state, bool noHighlight )
{
	// Logic: Request the menu state be changed by queuing a new state.

	// Please remove the following if these states are removed!!!
	/* Ignore states that are only partials (ex: opening and closing). */
	if ( state == EUGUIMState_Opening || state == EUGUIMState_Closing )
	{
		return; // Cannot set the state
	}

	// Please remove the following if these states are removed!!!
	if ( state == EUGUIMState_Open )
	{
		switch ( menuLocation )
		{
		case EUGUIMLoc_Right:
			queuedState = EUGUIMState_ToRight_Open;
			break;

		case EUGUIMLoc_Center:
			queuedState = EUGUIMState_ToCenter_Open;
			break;

		case EUGUIMLoc_Bar:
			queuedState = EUGUIMState_ToBar;
			break;

		default:
			queuedState = EUGUIMState_ToLeft_Open;
			break;
		}

		// Please remove the following if these states are removed!!!
	} else if ( state == EUGUIMState_Closed )
	{
		switch ( menuLocation )
		{
		case EUGUIMLoc_Right:
			queuedState = EUGUIMState_ToRight_Closed;
			break;

		case EUGUIMLoc_Center:
			queuedState = EUGUIMState_ToCenter_Closed;
			break;

		case EUGUIMLoc_Bar:
			queuedState = EUGUIMState_ToBar;
			break;

		default:
			queuedState = EUGUIMState_ToLeft_Closed;
			break;
		}
	} else {
		queuedState = state;
	}

	/* Account for moving to and from the bar. */

	if ( menuLocation == EUGUIMLoc_Bar
		&& queuedState != EUGUIMState_ToBar )
	{
		ultraframe->leavingBar( this );
		setChildrenOn(true);
	}

	if ( menuLocation != EUGUIMLoc_Bar
		&& queuedState == EUGUIMState_ToBar )
	{
		ultraframe->requestBarSpace(this);
		setChildrenOn(false);
	}

	// Already open?
	if ( !noHighlight )
	if (
			(	menuState == EUGUIMState_ToLeft_Open
				|| menuState == EUGUIMState_ToRight_Open
				|| menuState == EUGUIMState_ToCenter_Open
			) && (
				menuState == queuedState
			)
		)
	{
		loadHighlight = true;
	}

	// Set the destination/target rectangle
	setDestinyByQueue();
}

void UltraGUIMenu::snapState()
{
	inTransition = false;

	// Assume the menu is leaving the bar
	if ( menuLocation == EUGUIMLoc_Bar )
	{
		ultraframe->leavingBar( this );
	}

	switch( queuedState )
	{
	case EUGUIMState_ToLeft_Open:
		menuLocation = EUGUIMLoc_Left;
		currRect = getLeftRectangle();
		break;

	case EUGUIMState_ToLeft_Closed:
		menuLocation = EUGUIMLoc_Left;
		currRect = getClosedLeftRectangle();
		break;

	case EUGUIMState_ToRight_Open:
		menuLocation = EUGUIMLoc_Right;
		currRect = getRightRectangle();
		break;

	case EUGUIMState_ToRight_Closed:
		menuLocation = EUGUIMLoc_Right;
		currRect = getClosedRightRectangle();
		break;

	case EUGUIMState_ToCenter_Open:
		menuLocation = EUGUIMLoc_Center;
		currRect = getCenterRectangle();
		break;

	case EUGUIMState_ToCenter_Closed:
		menuLocation = EUGUIMLoc_Center;
		currRect = getClosedCenterRectangle();
		break;

	case EUGUIMState_ToBar:
		// Add the menu back onto the bar
		ultraframe->requestBarSpace(this);
		menuLocation = EUGUIMLoc_Bar;
		currRect = getBarRectangle();
		break;

	default:
		break;
	}

	menuState = queuedState;

	destinyRect = currRect;
}

EUltraMenuLocation UltraGUIMenu::locationFromState( EUltraMenuState state )
{
	switch( state )
	{
	case EUGUIMState_ToRight_Open:
	case EUGUIMState_ToRight_Closed:
		return EUGUIMLoc_Right;

	case EUGUIMState_ToCenter_Open:
	case EUGUIMState_ToCenter_Closed:
		return EUGUIMLoc_Center;

	case EUGUIMState_ToBar:
		return EUGUIMLoc_Bar;

	default:
		return EUGUIMLoc_Left;
	}
}

void UltraGUIMenu::setMenuBarSprite( irr::video::ITexture* sprite )
{
	if ( barSprite )
		barSprite->drop();

	barSprite = 0; /* So it isn't referenced again. */

	if ( sprite )
	{
		barSprite = sprite;
		barSprite->grab();
	}
}

bool UltraGUIMenu::OnEvent( const irr::SEvent& event )
{
	if ( !isEnabled() || !isVisible() || inTransition )
		return IGUIElement::OnEvent(event);

	/* irrlicht 1.7.2 does not have the subElement feature,
	so you will have to separate buttons and check them yourself
	if you wish to use the earlier version. */

	// This is an open menu, so it can receive event activity
	// e.g. click and drag to new designated region
	switch ( event.EventType )
	{
	case EET_MOUSE_INPUT_EVENT:

		switch( event.MouseInput.Event )
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			if ( currRect.isPointInside(
				irr::core::vector2di( event.MouseInput.X, event.MouseInput.Y )
				) )
			{
				ultraframe->bringToFront(this);

				// Set to selected
				isSelected = true;

				// Prevent other elements from processing
				return true;
			}
			break;

		case EMIE_MOUSE_MOVED:
			if ( isSelected )
			{
				isDragging = true;
				/* Does NOT return true - allows other things to process
				with mouse movement. */
				return false;
			}
			break;

		case EMIE_LMOUSE_LEFT_UP:

			// Environment Focus is irrelevant at this point

			if ( isSelected )
			{
				isSelected = false;

				if ( isDragging )
				{
					isDragging = false;

					// Drag to left
					if (
						ultraframe->isInLeftDragRegion(
							event.MouseInput.X,
							event.MouseInput.Y )
						)
					{
						setMyState( EUGUIMState_ToLeft_Open, true );
						return true;
					}

					// Drag to right
					else if (
						ultraframe->isInRightDragRegion(
							event.MouseInput.X,
							event.MouseInput.Y )
						)
					{
						setMyState( EUGUIMState_ToRight_Open, true );
						return true;
					}

					// Drag to bar
					else if (
						ultraframe->isInBarDragRegion(
							event.MouseInput.X,
							event.MouseInput.Y )
						)
					{
						setMyState( EUGUIMState_ToBar, true );
						return true;
					}

					// Drag to center
					else if (
						ultraframe->isInCenterDragRegion(
							event.MouseInput.X,
							event.MouseInput.Y )
						)
					{
						setMyState( EUGUIMState_ToCenter_Open, true );
						return true;
					}
				}

				/* The bar state acts different from other states.
				When you click on the bar without hitting a button,
				it causes the menu to move to the center.
				NOTE: This is AFTER dragging because dragging must
				take precedence.
				If not dependent on isSelected, the environment focus may
				be required because if this menu is NOT the focus, it
				could mean the user is dragging a menu to the bar. */
				if ( isPointInside(
						irr::core::position2di( event.MouseInput.X, event.MouseInput.Y )
						)
					&& menuLocation == EUGUIMLoc_Bar
					//&& Environment->getFocus() == this
					// ^ removed because the menu is selected (in isSelected condition)
					)
				{
					// Move to center
					setMyState( EUGUIMState_ToCenter_Open );

					// Make the active GUI element
					ultraframe->bringToFront(this);

					// Prevent further processing
					return true;
				}

			} // end isSelected condition

			/* Does NOT return true by default because nothing
			has happened to this element. In fact, it may not have
			even been selected. */
			break;

		default:
			break;
		}

		break; // mouse input event

		// ========== GUI Event ===========
	case EET_GUI_EVENT:

		switch ( event.GUIEvent.EventType )
		{
		case EGET_ELEMENT_FOCUS_LOST:
			isSelected = false;
			isDragging = false;
			break;

		case EGET_ELEMENT_HOVERED:
			break;

		/* If this is just a GUI focus event, then the buttons will
		call the parent (this element) for click events.
		If a button has been clicked, then find which button has
		been clicked and act accordingly. */

		case EGET_ELEMENT_FOCUSED:
			break;

		case EGET_BUTTON_CLICKED:
			ultraframe->bringToFront(this);

			if ( event.GUIEvent.Caller == closeButton )
			{
				// If only told to minimize, go to the bar
				if ( menuCloseSetting == EUGUIMClose_Minimize )
				{	setMyState( EUGUIMState_ToBar );
				} else {
					switch ( menuLocation )
					{
					case EUGUIMLoc_Left:
						setMyState( EUGUIMState_ToLeft_Closed );
						break;
					case EUGUIMLoc_Right:
						setMyState( EUGUIMState_ToRight_Closed );
						break;
					case EUGUIMLoc_Center:
						setMyState( EUGUIMState_ToCenter_Closed );
						break;
					default: // Do nothing with bar
						break;
					}
					//setMyState( EUGUIMState_Closed );
				}
				return true;
			}
			else if ( event.GUIEvent.Caller == toLeftButton )
			{
				setMyState( EUGUIMState_ToLeft_Open );
				ultraframe->bringToFront(this);
				return true;
			}
			else if ( event.GUIEvent.Caller == toRightButton )
			{
				setMyState( EUGUIMState_ToRight_Open );
				ultraframe->bringToFront(this);
				return true;
			}
			else if ( event.GUIEvent.Caller == toCenterButton )
			{
				setMyState( EUGUIMState_ToCenter_Open );
				ultraframe->bringToFront(this);
				return true;
			}
			else if ( event.GUIEvent.Caller == minimizeButton )
			{
				setMyState( EUGUIMState_ToBar );
				return true;
			}

			break;
		}
		break; // GUI event

	default:
		break;
	}

	return IGUIElement::OnEvent(event);
}

void UltraGUIMenu::draw()
{
	// Don't bother drawing if not visible or off-screen
	if ( !isVisible() || menuState == EUGUIMState_Closed )
		return;

	irr::core::recti drawRect = currRect;
	drawRect.clipAgainst( ultraframe->getAbsoluteClippingRect() );

	if ( inTransition )
	{
		Environment->getSkin()->draw2DRectangle(this, transitionColor, drawRect);
		return;
	}

	// For drawing the header
		// Rectangle used for determining the drawing area
	irr::core::recti headerDrawRect(
					currRect.UpperLeftCorner.X + 1,
					currRect.UpperLeftCorner.Y + 1,
					currRect.LowerRightCorner.X - 1,
					currRect.UpperLeftCorner.Y + (ultraframe->headerHeight / 3)
					);
		// Colors of the header
	irr::video::SColor headerColor1 = headerColorTop;
	irr::video::SColor headerColor2 = headerColorMid;
	irr::video::SColor headerShadow = irr::video::SColor(155,0,0,0);

		// Sprite border box (may be removed or made optional)
	irr::core::recti spriteBorder;


	// Only draw the children and other aspects if not in transition

	if ( menuLocation == EUGUIMLoc_Bar ) // on the menu bar
	{
		// Draw as a button
		Environment->getSkin()->draw3DButtonPaneStandard(this,drawRect);

		// Draw the icon (just after the toLeftButton)
		if ( barSprite )
		{
			// Location of the bar sprite = center of the bar
			spriteBorder.UpperLeftCorner.X = currRect.getCenter().X - barSprite->getSize().Width/2;
			spriteBorder.UpperLeftCorner.Y = currRect.getCenter().Y - barSprite->getSize().Height/2;
				// lower right corner is the image dimension away from upper left
			spriteBorder.LowerRightCorner.X = spriteBorder.UpperLeftCorner.X + (s32)barSprite->getSize().Width;
			spriteBorder.LowerRightCorner.Y = spriteBorder.UpperLeftCorner.Y + (s32)barSprite->getSize().Height;

			// Draw the sprite
			Environment->getVideoDriver()->draw2DImage(
				barSprite,
				irr::core::vector2di(
					currRect.getCenter().X - barSprite->getSize().Width/2,
					currRect.getCenter().Y - barSprite->getSize().Height/2
					),
				irr::core::recti(0,0, (s32)barSprite->getSize().Width, (s32)barSprite->getSize().Height),
				&spriteBorder,
				irr::video::SColor(255,255,255,255),
				true
				);

			// ---- Draw the border of the icon ----
			if ( hasBarSpriteBorder )
			{
				spriteBorder.UpperLeftCorner.X = currRect.getCenter().X + barSprite->getSize().Width/2 + 1;
				spriteBorder.UpperLeftCorner.Y = currRect.getCenter().Y - barSprite->getSize().Height/2 - 1;
				spriteBorder.LowerRightCorner.X = spriteBorder.UpperLeftCorner.X + 1;
				spriteBorder.LowerRightCorner.Y = spriteBorder.UpperLeftCorner.Y + barSprite->getSize().Height + 2;

				Environment->getSkin()->draw2DRectangle( this,
					Environment->getSkin()->getColor( EGDC_3D_DARK_SHADOW ),
					spriteBorder );
			}

		}

		// Draw the buttons
		toLeftButton->draw();
		toRightButton->draw();
			/* Not needed since the center of the bar brings the menu
			to the center. */

		/* Notice that the other two need not be drawn because the
		bar won't fit them. */

	} else {
		// Don't draw if closed
		if ( menuState == EUGUIMState_ToLeft_Closed
			|| menuState == EUGUIMState_ToRight_Closed
			|| menuState == EUGUIMState_ToCenter_Closed )
		{ return; }

		// Standard drawing
		Environment->getSkin()->draw3DMenuPane(this, drawRect);

		// Draw the header box for pizaz
		if ( Environment->getFocus() == this )
		{
			// Draw top part
			Environment->getVideoDriver()->draw2DRectangle(
				headerDrawRect,
				headerColor1, headerColor1,
				headerColor2, headerColor2,
				&drawRect );

			// Draw the mid-section
			headerDrawRect.UpperLeftCorner.Y = currRect.UpperLeftCorner.Y + (ultraframe->headerHeight / 3);
			headerDrawRect.LowerRightCorner.Y = currRect.UpperLeftCorner.Y + (ultraframe->headerHeight / 3 + 2);

			Environment->getVideoDriver()->draw2DRectangle(
				headerDrawRect,
				headerColor2, headerColor2,
				headerColor2, headerColor2,
				&drawRect );

			// Draw the bottom section
			headerDrawRect.UpperLeftCorner.Y = currRect.UpperLeftCorner.Y + (ultraframe->headerHeight / 3 + 2);
			headerDrawRect.LowerRightCorner.Y = currRect.UpperLeftCorner.Y + ultraframe->headerHeight - 1;

			headerColor1 = headerColorBottom;

			Environment->getVideoDriver()->draw2DRectangle(
				headerDrawRect,
				headerColor2, headerColor2,
				headerColor1, headerColor1,
				&drawRect );

			// Draw the bottom line
			headerDrawRect.UpperLeftCorner.Y = currRect.UpperLeftCorner.Y + ultraframe->headerHeight - 1;
			headerDrawRect.LowerRightCorner.Y = currRect.UpperLeftCorner.Y + ultraframe->headerHeight;

			Environment->getVideoDriver()->draw2DRectangle(
				headerDrawRect,
				headerShadow, headerShadow,
				headerShadow, headerShadow,
				&drawRect );

		} else {
			// Not in focus

			Environment->getVideoDriver()->draw2DRectangle(
				irr::core::recti(
					currRect.UpperLeftCorner.X + 1,
					currRect.UpperLeftCorner.Y + 1,
					currRect.LowerRightCorner.X - 1,
					currRect.UpperLeftCorner.Y + ultraframe->headerHeight - 1
				),
				irr::video::SColor(255,0,0,0), irr::video::SColor(255,0,0,0),
				irr::video::SColor(255,100,100,100), irr::video::SColor(255,100,100,100),
				&drawRect );

			// Draw the underline bar (for definition of the header box)
			Environment->getVideoDriver()->draw2DRectangle(
				irr::core::recti(
					currRect.UpperLeftCorner.X + 1,
					currRect.UpperLeftCorner.Y + ultraframe->headerHeight - 1,
					currRect.LowerRightCorner.X - 1,
					currRect.UpperLeftCorner.Y + ultraframe->headerHeight
				),
				headerShadow, headerShadow,
				headerShadow, headerShadow,
				&drawRect );
		}

		// Draw the icon of this element
		if ( barSprite )
		{
			spriteBorder.UpperLeftCorner = currRect.UpperLeftCorner + 5;
			spriteBorder.LowerRightCorner.X = currRect.UpperLeftCorner.X + (s32)barSprite->getSize().Width + 5;
			spriteBorder.LowerRightCorner.Y = currRect.UpperLeftCorner.Y + (s32)barSprite->getSize().Height + 5;

			Environment->getVideoDriver()->draw2DImage(
				barSprite,
				currRect.UpperLeftCorner + 5, // ULC.x+5, ULC.y+5
				irr::core::recti(0,0, (s32)barSprite->getSize().Width, (s32)barSprite->getSize().Height),
				&spriteBorder,
				irr::video::SColor(255,255,255,255),
				true
				);

			// ---- Draw the border of the icon ----
			if ( hasBarSpriteBorder )
			{

				// ulc to urc
				spriteBorder.UpperLeftCorner = currRect.UpperLeftCorner + 4;
				spriteBorder.LowerRightCorner.Y = spriteBorder.UpperLeftCorner.Y + 1;
				spriteBorder.LowerRightCorner.X = spriteBorder.UpperLeftCorner.X + barSprite->getSize().Width + 2;

				Environment->getSkin()->draw2DRectangle( this,
					Environment->getSkin()->getColor( EGDC_3D_LIGHT ),
					spriteBorder );

				// urc to lrc
				spriteBorder.UpperLeftCorner = spriteBorder.LowerRightCorner - 1;
				spriteBorder.LowerRightCorner.Y += barSprite->getSize().Height + 1;

				Environment->getSkin()->draw2DRectangle( this,
					Environment->getSkin()->getColor( EGDC_3D_SHADOW ),
					spriteBorder );

				// lrc to llc
				spriteBorder.UpperLeftCorner.X = spriteBorder.LowerRightCorner.X - (barSprite->getSize().Width + 2);
				spriteBorder.UpperLeftCorner.Y = spriteBorder.LowerRightCorner.Y - 1;

				Environment->getSkin()->draw2DRectangle( this,
					Environment->getSkin()->getColor( EGDC_3D_DARK_SHADOW ),
					spriteBorder );

				// llc to ulc
				spriteBorder.LowerRightCorner = spriteBorder.UpperLeftCorner + 1;
				spriteBorder.UpperLeftCorner.Y -= barSprite->getSize().Height;

				Environment->getSkin()->draw2DRectangle( this,
					Environment->getSkin()->getColor( EGDC_3D_LIGHT ),
					spriteBorder );
			
			}
			// End draw border for icon
		}

		// Draw the child elements if this is not a bar
		if ( menuLocation != EUGUIMLoc_Bar )
			IGUIElement::draw();

		// Header text shadow
		titleTextBox += irr::core::vector2di(1,1);
		Environment->getSkin()->getFont()->draw(
			Text, titleTextBox, // irr::video::SColor(255,255,255,255),
			Environment->getSkin()->getColor( EGDC_3D_DARK_SHADOW ),
			false, false,
			&drawRect );

		// Header text
		titleTextBox -= irr::core::vector2di(1,1);
		Environment->getSkin()->getFont()->draw(
			Text, titleTextBox, // irr::video::SColor(255,255,255,255),
			Environment->getSkin()->getColor( EGDC_HIGH_LIGHT_TEXT ),
			false, false,
			&drawRect );

		// Drawing the highlight
		if ( isHighlighted() )
		{
			Environment->getSkin()->draw2DRectangle(this, highlightColor, drawRect);
		}
	} // else not on the menu bar
}

void UltraGUIMenu::OnPostRender( irr::u32 timeMs )
{
	if ( isHighlighted() )
	{
		highlightTime = timeMs - resetTime;

		if ( highlightTime < ULTRAGUI_HIGHLIGHT_TIME )
		{
			highlightColor.setAlpha(
				irr::core::clamp(
					255*highlightTime/(ULTRAGUI_HIGHLIGHT_TIME*2),
					(irr::u32)0,
					(irr::u32)255 )
				);
		} else {
			highlightColor.setAlpha(
				irr::core::clamp(
					255 - 255*highlightTime/(ULTRAGUI_HIGHLIGHT_TIME*2),
					(irr::u32)0,
					(irr::u32)255 )
				);
		}

		if ( highlightTime >= ULTRAGUI_HIGHLIGHT_TIME*2 )
		{
			resetDrawHighlight();
		}
	}

	if ( menuState == queuedState )
	{
		if ( loadHighlight )
		{
			loadHighlight = false;
			resetDrawHighlight();
			queueDrawHighlight(timeMs); // must be called second
		}
	}

	moveToQueuedLocation(timeMs);

	// Save the last time run
	lastTime = timeMs;

	IGUIElement::OnPostRender(timeMs);
}

bool UltraGUIMenu::isPointInside(const irr::core::position2di& point) const
{
	return currRect.isPointInside(point);
}

void UltraGUIMenu::updateAbsolutePosition()
{
	/* If ultraframe has been resized, resize this for the new frame.
	Note that this also accounts for relocation of the frame with or
	without size changes. */
	irr::core::recti frameSizeDif = ultraframe->getFrameRectChange();


	// ********** To uncomment and finish implementing later ***********

	//if ( menuResize == EUGUIMResize_ToScale )
	//{
	//	currRect.UpperLeftCorner += frameSizeDif.UpperLeftCorner;
	//	currRect.LowerRightCorner += frameSizeDif.LowerRightCorner;
	//} else {
		// No scaling, just location shift
		currRect += frameSizeDif.UpperLeftCorner;
	//}

	// Update Absolute Rectangle
	/* Note that the rectangle is only for the elements within the menu. */

	AbsoluteRect = currRect;
	AbsoluteClippingRect = AbsoluteRect;

	// Update buttons before changing the Absolute Rectangle
	refreshButtons();

	// Change the Absolute Rectangle for the other elements
	AbsoluteRect.UpperLeftCorner.Y += ultraframe->headerHeight;
	AbsoluteClippingRect = AbsoluteRect;

	// update all children
	irr::core::list<IGUIElement*>::Iterator it = Children.begin();
	for (; it != Children.end(); ++it)
	{
		(*it)->updateAbsolutePosition();
	}
}

irr::core::recti UltraGUIMenu::getLeftRectangle()
{
	// lower left corner
	irr::core::vector2di leftSpot = ultraframe->getLeftSidePosition();
	leftSpot.Y -= leftSideSize.Height;

	// lower right corner
	irr::core::vector2di lowerSpot = leftSpot + irr::core::dimension2di(leftSideSize);

	return irr::core::recti( leftSpot, lowerSpot );
}

irr::core::recti UltraGUIMenu::getRightRectangle()
{
	// lower right corner
	irr::core::vector2di rightSpot = ultraframe->getRightSidePosition();
	
	// upper left corner
	irr::core::vector2di upperSpot = rightSpot - irr::core::dimension2di(rightSideSize);

	return irr::core::recti( upperSpot, rightSpot );
}

irr::core::recti UltraGUIMenu::getCenterRectangle()
{
	irr::core::vector2di centerSpot = ultraframe->getCenterCenterPosition();
	
	irr::core::dimension2di actualSize( (irr::s32)centerSize.Width, (irr::s32)centerSize.Height );
	// Calculate actual center size based on center window settings
	irr::core::dimension2di available = irr::core::dimension2di(ultraframe->getCenterWindowSize());
	
	if ( available.Width < actualSize.Width )
		actualSize.Width = available.Width;

	if ( available.Height < actualSize.Height )
		actualSize.Height = available.Height;

	// Construct the window bounds and return them
	return irr::core::recti(
				centerSpot - actualSize / 2,
				centerSpot + actualSize / 2
				);
}

irr::core::recti UltraGUIMenu::getBarRectangle()
{
	// upper left corner
	irr::core::vector2di upperSpot = ultraframe->getBarSlotPosition();

	return irr::core::recti(
				upperSpot,
				upperSpot + irr::core::dimension2di(barSlotSize)
				);
}

irr::core::recti UltraGUIMenu::getClosedLeftRectangle()
{
	// get lower left corner
	irr::core::vector2di leftSpot = ultraframe->getLeftSidePosition();

	// move to upper left corner and off screen
	leftSpot -= irr::core::dimension2di( leftSideSize );

	// fully hidden but full size
	return irr::core::recti( leftSpot, leftSideSize );
}

irr::core::recti UltraGUIMenu::getClosedRightRectangle()
{
	// get lower left corner
	irr::core::vector2di rightSpot = ultraframe->getRightSidePosition();

	// move to upper left corner
	rightSpot.Y -= rightSideSize.Height;

	// now move off screen to the right
	rightSpot.X += rightSideSize.Width;

	// fully hidden but full size
	return irr::core::recti( rightSpot, rightSideSize );
}

irr::core::recti UltraGUIMenu::getClosedCenterRectangle()
{
	// This is convenient - no fancy calculations necessary :D
	return irr::core::recti();
}

irr::core::recti UltraGUIMenu::getClosedBarRectangle()
{
	return getBarRectangle();
}

void UltraGUIMenu::moveToQueuedLocation( irr::u32 timeMs )
{
	// Move the rectangle in the direction of the destiny rectangle
	//interpolateCurrToRect( destinyRect, timeMs );

	if (
		( !inTransition && menuState != queuedState )
		|| refreshState
		)
	{
		// Target state has been refreshed
		refreshState = false;

		((MoveGUIAnimator*)moveAnimator)->setImmediateTarget( destinyRect, timeMs );
		inTransition = true;
	}
	moveAnimator->OnPostRender( timeMs, this );

	/* Check for a matching destiny rectangle and current rectangle.
	This means that the GUI menu has reached its destination and the
	state can change. */
	if ( currRect == destinyRect )
	{
		menuLocation = locationFromState( queuedState );
		menuState = queuedState;
		inTransition = false;

		// Update Absolute Rectangle
		AbsoluteRect = currRect;
		AbsoluteClippingRect = AbsoluteRect;

		// Update buttons before changing the Absolute Rectangle
		refreshButtons();

		// Change the Absolute Rectangle for the other elements
		AbsoluteRect.UpperLeftCorner.Y += ultraframe->headerHeight;
		AbsoluteClippingRect = AbsoluteRect;

		// update all children
		irr::core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
		{
			if ( *it != closeButton && *it != toLeftButton && *it != toRightButton
				&& *it != toCenterButton && *it != minimizeButton )
			{
				(*it)->updateAbsolutePosition();
			}
		}
	}
}

void UltraGUIMenu::refresh()
{
	ultraframe->requestBarSpace(this);
	setDestinyByQueue();
	refreshState = true;
}

void UltraGUIMenu::setDestinyByQueue()
{
	// Obtain the destiny rectangle
	switch ( queuedState )
	{
		// left side
	case EUGUIMState_ToLeft_Open:
		destinyRect = getLeftRectangle();
		break;

	case EUGUIMState_ToLeft_Closed:
		destinyRect = getClosedLeftRectangle();
		break;

		// right side
	case EUGUIMState_ToRight_Open:
		destinyRect = getRightRectangle();
		break;

	case EUGUIMState_ToRight_Closed:
		destinyRect = getClosedRightRectangle();
		break;

		// to center
	case EUGUIMState_ToCenter_Open:
		destinyRect = getCenterRectangle();
		break;

	case EUGUIMState_ToCenter_Closed:
		destinyRect = getClosedCenterRectangle();
		break;

		// to bar
	case EUGUIMState_ToBar:
		destinyRect = getBarRectangle();
		break;

	default:
		break;
	}

	destinyRect.repair();
}

void UltraGUIMenu::refreshButtons()
{
	irr::core::recti buttonLoc;
	irr::core::dimension2di tTB_size;

	/* Update the buttons based on the current state and location of
	the menus */

	switch( menuState )
	{
		/* Only time the buttons are visible is when the menu is
		open. */
	case EUGUIMState_ToLeft_Open:
	case EUGUIMState_ToRight_Open:
	case EUGUIMState_ToCenter_Open:
	case EUGUIMState_ToBar:
		// Calculate first position ( lower right corner )
		buttonLoc.LowerRightCorner =
			//currRect.UpperLeftCorner
			//+
			irr::core::vector2di(
				currRect.getWidth() - ULTRAGUI_BUTTON_RIGHT_PADDING,
				(( menuLocation == EUGUIMLoc_Bar )? barSlotSize.Height : ultraframe->headerHeight - 3)
				);

		buttonLoc.UpperLeftCorner = buttonLoc.LowerRightCorner - irr::core::dimension2di( buttonSize );

		/* For anywhere EXCEPT the bar menu, apply the following */
		if ( menuLocation != EUGUIMLoc_Bar )
		{

			// first button
			assignRectToButton( buttonLoc, 1 );
			// second button
			buttonLoc -= irr::core::vector2di( (irr::s32)buttonSize.Width, 0 );
			assignRectToButton( buttonLoc, 2 );
			// third button
			buttonLoc -= irr::core::vector2di( (irr::s32)buttonSize.Width, 0 );
			assignRectToButton( buttonLoc, 3 );
			// fourth button
			buttonLoc -= irr::core::vector2di( (irr::s32)buttonSize.Width, 0 );
			assignRectToButton( buttonLoc, 4 );
		}
		else { // bar

			// Move to center of the bar
			buttonLoc += irr::core::position2di(
				0, // no change in x
				(0 - currRect.getHeight()/2) + buttonSize.Height/2
				);

			// first button
			assignRectToButton( buttonLoc, 1 );
			/* Move the button to the other side of the bar */
			buttonLoc -= irr::core::vector2di(
							(irr::s32) (barSlotSize.Width - buttonSize.Width - 2*ULTRAGUI_BUTTON_RIGHT_PADDING),
							0
							);
			// second button
			assignRectToButton( buttonLoc, 2 );
		}

		// Show and enable all buttons for the initial seeting
		closeButton->setVisible(true);
		toLeftButton->setVisible(true);
		toRightButton->setVisible(true);
		toCenterButton->setVisible(true);
		minimizeButton->setVisible(true);

		closeButton->setEnabled(true);
		toLeftButton->setEnabled(true);
		toRightButton->setEnabled(true);
		toCenterButton->setEnabled(true);
		minimizeButton->setEnabled(true);

		// Based on location, turn buttons off
		if ( menuLocation == EUGUIMLoc_Left )
		{
			toLeftButton->setVisible(false);
			toLeftButton->setEnabled(false);
		}

		if ( menuLocation == EUGUIMLoc_Right )
		{
			toRightButton->setVisible(false);
			toRightButton->setEnabled(false);
		}

		if ( menuLocation == EUGUIMLoc_Center )
		{
			toCenterButton->setVisible(false);
			toCenterButton->setEnabled(false);
		}

		if ( menuLocation == EUGUIMLoc_Bar )
		{
			minimizeButton->setVisible(false);
			minimizeButton->setEnabled(false);
			toCenterButton->setVisible(false);
			toCenterButton->setEnabled(false);
			// recall the bar icon is a button for sending the menu to center
		}

		// Finally, update the title text
		tTB_size = titleTextBox.getSize();

		titleTextBox.UpperLeftCorner = irr::core::position2di(
				( barSprite? barSprite->getSize().Width + 12 : 12 ),
				5
				);
		titleTextBox.LowerRightCorner = titleTextBox.UpperLeftCorner + tTB_size;

		titleTextBox += currRect.UpperLeftCorner;
		titleTextBox.clipAgainst( currRect );

		break;

		/* Hide and disable buttons for every other state of this menu
		since the menu is in transition or closed. */
	default:
		closeButton->setVisible(false);
		toLeftButton->setVisible(false);
		toRightButton->setVisible(false);
		toCenterButton->setVisible(false);
		minimizeButton->setVisible(false);

		closeButton->setEnabled(false);
		toLeftButton->setEnabled(false);
		toRightButton->setEnabled(false);
		toCenterButton->setEnabled(false);
		minimizeButton->setEnabled(false);
		break;
	}
}

void UltraGUIMenu::assignRectToButton(
	irr::core::recti rectangle,
	irr::u32 whichButton
	)
{
	// First slot
	if ( whichButton == 1 )
	{
		// On the bar, the first slot belongs to the toRight button
		if ( menuLocation == EUGUIMLoc_Bar )
		{
			toRightButton->setRelativePosition( rectangle );
			return;
		}
		
		// Otherwise, it belongs to the close button
		closeButton->setRelativePosition( rectangle );
		return;
	}

	// Second slot
	if ( whichButton == 2 )
	{
		// On the bar, the second slot belongs to the to-left button
		if ( menuLocation == EUGUIMLoc_Bar )
		{
			toLeftButton->setRelativePosition( rectangle );
			return;
		}

		// Check if this is not the center
		// If not, this slot belongs to the to-center button
		if ( menuLocation != EUGUIMLoc_Center )
		{
			toCenterButton->setRelativePosition( rectangle );
			return;
		}

		// Otherwise, this slot belongs to the minimize button
		minimizeButton->setRelativePosition( rectangle );
		return;
	}

	// Third slot
	/* NOTE: This is never requested when the menu is on the bar,
	so that location is not considered. */
	if ( whichButton == 3 )
	{
		if ( menuLocation != EUGUIMLoc_Center )
		{
			minimizeButton->setRelativePosition( rectangle );
			return;
		}

		toRightButton->setRelativePosition( rectangle );
		return;
	}

	// Fourth slot
	if ( whichButton == 4 )
	{
		if ( menuLocation == EUGUIMLoc_Left )
		{
			toRightButton->setRelativePosition( rectangle );
			return;
		}

		toLeftButton->setRelativePosition( rectangle );
		return;
	}
}

void UltraGUIMenu::setChildrenOn( bool setting )
{
	irr::core::list<IGUIElement*>::Iterator iter = Children.begin();
	for ( ; iter != Children.end(); ++iter )
	{
		if ( *iter != closeButton && *iter != toLeftButton && *iter != toRightButton
			&& *iter != toCenterButton && *iter != minimizeButton )
		{
			(*iter)->setVisible(setting);
			(*iter)->setEnabled(setting);
		}
	}
}

}} // end namespace gui and irr