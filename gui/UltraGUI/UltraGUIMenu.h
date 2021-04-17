/*
(c) Nicolaus Anderson
*/

#include <irrlicht.h>
#include <IAnimGUIElement.h>
#include "UltraGUIConstants.h"
//#include "EnumCentralWindowMenuSizing.h"
#include "EnumUltraMenuState.h"

#ifndef _ULTRAGUIMENU_
#define _ULTRAGUIMENU_

namespace irr
{
namespace gui
{

class UltraGUIFrame;

//! Class Ultra GUI Menu
/* Container of user-defined GUI elements. */
class UltraGUIMenu : public IAnimGUIElement // IGUIElement
{
	friend UltraGUIFrame;

protected:
	UltraGUIFrame* ultraframe;
	EUltraMenuState menuState;
	EUltraMenuLocation menuLocation;
	EUltraMenuClose menuCloseSetting;

	// Indicates if this object is in transition between two locations
	bool inTransition;
	// Requested state - what to transition to
	EUltraMenuState queuedState;

	// Indicates if the state/location/animation should be refreshed.
	bool refreshState;

	/* Transition speed - The speed at which the menu moves when changing
	positions/states. */
	irr::f32 transitionSpeed;
	/* Transition color - the color of the box representing the menu when
	the menu is being relocated. */
	irr::video::SColor transitionColor;

	// Animator in control of moving this GUI element
	IAnimGUIAnimator* moveAnimator;

	/* Last time
	- The last time OnPostRender was called. This is necessary for finding
	the change for the rectangle. */
	irr::u32 lastTime;

	// Current rectangle of this menu (what is actually drawn)
	irr::core::recti currRect;
	/* NOTE: The rectangle used by the occupants of this element is
	the standard rectangle: AbsoluteRect. AbsoluteRect is NOT used
	for the drawing, so we override isPointInside to ensure this
	GUI element is brought to the front when clicked.
	
	NOTE: AbsoluteRect MUST be updated after a transition is completed.
	*/

	/* Rectangle that the menu is trying to become.
	This is used for optimization of performance. */
	irr::core::recti destinyRect;

	// Left-side menu size
	irr::core::dimension2du leftSideSize;
	// Right-side menu size
	irr::core::dimension2du rightSideSize;
	// Central window size
	irr::core::dimension2du centerSize;
	// Bar icon size
	irr::core::dimension2du barSlotSize;


	// Title text
	/* This is displayed on the header bar. */
	irr::core::recti titleTextBox;

	// Icon to display when the menu is on the bar
	irr::video::ITexture* barSprite;

	// Should show the bar sprite border
	bool hasBarSpriteBorder;

	// Colors for the header
	irr::video::SColor headerColorTop;
	irr::video::SColor headerColorMid;
	irr::video::SColor headerColorBottom;


	// ---- Buttons ----
	IGUIButton* closeButton;	/* "Closes" the menu. */
	IGUIButton* toLeftButton;	/* Sends the menu to the left side.
								Not shown when menu is on the left side. */
	IGUIButton* toRightButton;	/* Sends the menu to the right side.
								Not shown when menu is on the right side. */
	IGUIButton* toCenterButton;	/* Sends the menu to the center area.
								Not shown when menu is in the center. */
	IGUIButton* minimizeButton;	/* Sends the menu to the bar.
								Not shown when menu is on the bar. */

	irr::core::dimension2du buttonSize;


	// ---- OnEvent aspects
	bool isSelected;
	bool isDragging;

public:
	UltraGUIMenu(
		UltraGUIFrame* frame,
		irr::s32 id,
		irr::core::recti rectangle,
		irr::io::path resource_path = ULTRAGUI_RESOURCE_PATH
		);
	~UltraGUIMenu();

	/* Returns a pointer to the ultra frame of this menu. */
	UltraGUIFrame* getUltraFrame()
	{
		return ultraframe;
	}

	/* Returns the menu state of this menu. */
	EUltraMenuState getMenuState()
	{
		return menuState;
	}

	/* Indicates if the menu is open (not just on the bar). */
	bool isTrulyOpen()
	{
		switch ( menuState )
		{
		case EUGUIMState_Open: // to be removed?
		case EUGUIMState_ToLeft_Open:
		case EUGUIMState_ToRight_Open:
		case EUGUIMState_ToCenter_Open:
			return true;
		default:
			return false;
		}
	}

	/* Returns the location state of this menu. */
	EUltraMenuLocation getMenuLocation()
	{
		return menuLocation;
	}

	/* Set the activity for what happens when this menu is told
	to close by its button. */
	void setCloseButtonAction( EUltraMenuClose action )
	{
		menuCloseSetting = action;
	}

	/* Sets the size of the menu rectangle at the appropriate position.
	After you set all of the sizes you wish, calld updateAbsolutePosition(). */
	void setLeftSize( irr::core::dimension2du size )
	{
		leftSideSize = size;
	}

	void setRightSize( irr::core::dimension2du size )
	{
		rightSideSize = size;
	}

	void setCenterSize( irr::core::dimension2du size )
	{
		centerSize = size;
	}

	/* PLEASE DO NOT USE THIS.
	All bars should be kept the same size. This function may be removed
	in the near future. */
	void setBarSize( irr::core::dimension2du size )
	{
		barSlotSize = size;
	}

	/* Gets the size of the menu rectangle at the appropriate position. */
	irr::core::dimension2du getLeftSize()
	{
		return leftSideSize;
	}

	irr::core::dimension2du getRightSize()
	{
		return rightSideSize;
	}

	irr::core::dimension2du getCenterSize()
	{
		return centerSize;
	}

	irr::core::dimension2du getBarSize()
	{
		return barSlotSize;
	}

	/*
	Set the state of the menu.
	This determines its open or close state as well as its final location.
	It should not matter if you call this function while a menu is in
	transition from one location to another.
	NOTE: The states of EUGUIMState_Opening and EUGUIMState_Closing are
	ignored.
	*/
	void setMyState( EUltraMenuState state, bool noHighlight=false );

	/* Snap-to-state
	Immediately brings the menu to the queued state. */
	void snapState();

	/* Location-from-state
	A useful function that converts the state to a location.
	*/
	EUltraMenuLocation locationFromState( EUltraMenuState state );

	/*
	Sets the menu bar button sprite.
	*/
	void setMenuBarSprite( irr::video::ITexture* sprite );

	/*
	Sets whether there should be a border around the menu bar sprite.
	*/
	void setBorderOnBarSprite( bool setting )
	{
		hasBarSpriteBorder = setting;
	}

	/*
	Set menu bar header bar colors for gradient
	*/
	void setHeaderGradient( irr::video::SColor top, irr::video::SColor mid, irr::video::SColor bottom )
	{
		headerColorTop = top;
		headerColorMid = mid;
		headerColorBottom = bottom;
	}


	// ---------- Engine stuff ------------

	virtual bool OnEvent( const irr::SEvent& event );

	virtual void draw();

	/* For animation of the menu */
	virtual void OnPostRender( irr::u32 timeMs );

	virtual bool isPointInside( const irr::core::position2di& point ) const;

	virtual void updateAbsolutePosition();

	/* Overridden to avoid error in the rectangles.
	Note that the locations of the rectangles will be determined
	by the UltraGUIFrame. */
	virtual void setRelativePosition( const irr::core::recti& r ) {}
	virtual void setRelativePosition( const irr::core::position2di& position ) {}
	
	// Alias for setting the left and right sizes
	virtual void setRelativePositionProportional(const irr::core::rect<f32>& r)
	{
		//leftSideSize = r.getSize() * ultraframe->getAbsolutePosition().getSize();
		//rightSideSize = r.getSize() * ultraframe->getAbsolutePosition().getSize();
	}

	/* Overridden to avoid error in the rectangles.
	Note that the locations of the rectangles will be determined
	by the UltraGUIFrame. */
	virtual void setAlignment(
		EGUI_ALIGNMENT left,
		EGUI_ALIGNMENT right,
		EGUI_ALIGNMENT top,
		EGUI_ALIGNMENT bottom
		)
	{}


	// ----- Internal assistance functions -----
protected:

	/* Function that create rectangles based on the proper locations
	(as given by UltraGUIFrame) and the designated dimensions. */
	irr::core::recti getLeftRectangle();
	irr::core::recti getRightRectangle();
	irr::core::recti getCenterRectangle();
	irr::core::recti getBarRectangle(); /* Should only be called during an update of position. */

	irr::core::recti getClosedLeftRectangle();
	irr::core::recti getClosedRightRectangle();
	irr::core::recti getClosedCenterRectangle();
	irr::core::recti getClosedBarRectangle();

	/* Function that moves the current rectangle towards a particular
	location. */
	void moveToQueuedLocation( irr::u32 timeMs );

	/* Refreshes the state/location/animation of the menu. */
	void refresh();

	/* Sets the destination rectangle based on what is queued. */
	void setDestinyByQueue();

public:
	/* Sets the position and visibility of buttons based on the location
	of the menu. */
	void refreshButtons();

protected:
	/* Sets the relative position of the requested button to the given
	rectangle.
	\param whichButton - A value between 1 and 4 (inclusive) that gives
	the button slot. */
	void assignRectToButton( irr::core::recti rectangle, irr::u32 whichButton );

	/* Sets the children to visible/enabled */
	inline void setChildrenOn( bool setting );


	/* For drawing a highlight around the menu.
	The menu should glow for a moment and then return to normal.
	This requires keeping track of time. */
	
	bool IsHighlighted;
	irr::u32 highlightTime;
	irr::u32 resetTime; // Time to reset the timer to if the highlighting is reset
	irr::video::SColor highlightColor;

	// Indicates if the highlighter should be loaded
	bool loadHighlight;

	// Queues a highlight
	inline void queueDrawHighlight( irr::u32 timeMs )
	{
		resetTime = timeMs;
		IsHighlighted = true;
	}

	inline void resetDrawHighlight()
	{
		IsHighlighted = false;
		highlightTime = 0;
		highlightColor.setAlpha(0);
	}

public:
	//! Is highlighted
	/* Indicates if this GUI element will be highlighted if it is
	open and the user wants it open. */
	bool isHighlighted()
	{
		return IsHighlighted;
	}
};

}} // end namespace gui and irr

#endif // #ifndef _ULTRAGUIMENU_
