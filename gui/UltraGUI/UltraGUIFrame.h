/*
(c) Nicolaus Anderson
*/

#include <irrlicht.h>
#include "UltraGUIConstants.h"
//#include "EnumCentralWindowMenuSizing.h"
#include "EnumUltraMenuState.h"
#include "UltraGUIMenu.h"

using irr::core::vector2di;
using irr::core::dimension2du;
using irr::core::recti;


#ifndef _ULTRAGUIFRAME_
#define _ULTRAGUIFRAME_

namespace irr
{
namespace gui
{

class UltraGUIFrame : public IGUIElement
{
	friend UltraGUIMenu;

protected:
	// Handles mouse input
	// NOTE: This may be removed since it seems mostly unnecessary
	ICursorControl* cursor;

	/* Old size - The previous rectangle of the frame before it is resized
	(assuming it is resized) or moved. This is used for resizing and
	repositioning the menus. */
	recti oldRect;

	// Rectangle of the menu bar
	recti menuBarRect;

	// Rows on the menu bar
	irr::u32 menuBarRows;

	// Last open x on bar
	irr::s32 lastOpenBarX;

	// Last occupied location on the bar
	// See requestBarSpace and getBarSlotPosition for details
	irr::s32 occupiedBarX;

	// Last open row
	/* This is the first row with a slot available.
	NOTE: This is a transient number - it is reset and updated every time
	updateAbsolutePosition() is called. */
	irr::u32 lastOpenRow;

	// ---- For the UltraGUIMenu elements ----

	// Left-side menu size
	dimension2du leftSideSize;
	// Right-side menu size
	dimension2du rightSideSize;
	// Central window size
	dimension2du centerSize;
	// Bar icon size
	dimension2du barSlotSize;

	/* Header bar size - The height of the bar that contains the title
	of a menu plus the buttons that control its activity. */
	irr::s32 headerHeight;

	/* Sprite bank containing the sprites for the menus.
	These sprites will be used for when the buttons are hovered over.
	The bank is created and stored here (rather than in the menus)
	to conserve space. */
	IGUISpriteBank* spriteBank;

public:
	UltraGUIFrame(
			IGUIEnvironment* environment,
			ICursorControl* cursorc,
			irr::s32 id=-1,
			IGUIElement* parent=0,
			recti rectangle=recti(0,0,0,0)
		);

	~UltraGUIFrame(void);

	/* Returns a pointer to the environment */
	IGUIEnvironment* getEnvironment() const
	{
		return Environment;
	}

	/*
	Adds a menu to the total.
	\param menuID - The ID of the menu being added.
	*/
	UltraGUIMenu* addMenu(
		irr::s32 menuID,
		irr::video::ITexture* icon=0
		);

	/*
	Removes a menu whose ID is given. If -1 is given, all menus
	are removed.
	\param menuID - The ID of the menu being removed.
	*/
	void removeMenu( irr::s32 menuID );

	/*
	Closes the menu whose ID is given. The reaction depends on what menu
	this is and where it is located.
	>> left side menu - Slides left off the screen.
	>> right side menu - Slides right off the screen.
	>> menu bar menu - Does nothing.
	>> central window menu - Disappears.
	*/
	void closeMenu( irr::s32 menuID );

	/*
	Closes the menu at the given location
	*/
	void closeOpenMenuAtLocation( EUltraMenuLocation location );

	/*
	Opens the menu whose ID is given. The reaction depends on what menu
	this is and where it is located.
	>> left side menu - Slides right if not open. Otherwise, it is highlighted.
	>> right side menu - Slides left if not open. Otherwise, it is highlighted.
	>> menu bar menu - Appears as the central window menu, minimizing
	any menu that was there already.
	>> central window menu - Blinks with a highlighter.
	*/
	void openMenu( irr::s32 menuID );

	/*
	Move the menu to the specific location.
	*/
	void moveMenu( irr::s32 menuID, EUltraMenuLocation location );

	/*
	Set the menu state. This does everything that the functions
	openMenu(), closeMenu(), and moveMenu() do and more.
	*/
	void setMenuState( irr::s32 menuID, EUltraMenuState state );

	/*
	Saves a pointer to the requested menu.
	Returns true if the menu could be found.
	*/
	bool getMenu( irr::s32 menuID, UltraGUIMenu*& result );

	/*
	\param size - The universal size of the left and right side menus.
	*/
	void setSideMenuSize( dimension2du size );

	/*
	Sets the bounds for the central window menu.
	These are treated as maximum/normal dimensions.
	If you wish to treat them as constant, call setCentralWindowMenuSizing()
	*/
	void setCentralWindowMenuBounds( dimension2du size );

	/*
	\param size_setting - The new size setting.
	Setting one of these may override other, previously-set settings.
	*/
	//void setCentralWindowMenuSizing( ECWMSize size_setting );

	/*
	Requests a slot on the menu bar.
	This is to ensure that there is available space on the menu bar.
	It may require adding another row.
	*/
	void requestBarSpace( UltraGUIMenu* menu );
	
	/*
	Accounts for the given menu leaving the bar.
	This allows for other menus to occupy its former location.
	*/
	void leavingBar( UltraGUIMenu* menu );

	/*
	Get sprite bank for menus.
	This sprite bank contains textures for the menus that will be
	used when the menu is hovered over.
	*/
	IGUISpriteBank* getMenuSpriteBank();


	// ---------- Engine stuff ------------

	virtual bool OnEvent( const irr::SEvent& event );

	virtual void updateAbsolutePosition();

	virtual void draw();

	/* For any animation of the frame */
	virtual void OnPostRender( irr::u32 timeMs );

	/* This GUI element is actually determined by what is visible to
	the user. Hence, at any given time, whether or not a point is actually
	hovering over this frame is determined by the position of the mouse.
	For the left, right, and center, whether or not there is a menu being
	displayed determines if the mouse is in the frame. On the bottom, there
	is a menu bar that always exists. It is this that is checked first. */
	virtual bool isPointInside( const vector2di& point ) const;


	// ---------- Calculating destinations

	/* Returns the position where the lower left corner of the
	menu should reside when it is the left-side menu. */
	vector2di getLeftSidePosition();

	/* Returns the position where the lower right corner of the
	menu should reside when it is the right-side menu. */
	vector2di getRightSidePosition();

	/* Returns the position where the exact center of the menu
	should reside when it is the center menu. */
	vector2di getCenterCenterPosition();

	/* Returns the next open location on the bar where the upper left
	corner of the menu should reside when it is a bar icon.
	IMPORTANT NOT: This increments the position, and thus it should
	not be called by anything but menus that are children of this frame. */
	vector2di getBarSlotPosition();


	/* Returns the actual available size for the central window menu,
	working in cooperation with getCenterCenterPosition() */
	dimension2du getCenterWindowSize();


	/* Returns the height of the menu bar.
	Useful if the implementation of the menu bar ever changes. */
	irr::s32 getBarHeight();


	// ---------- Drag-to regions

	/* Functions that indicate if the mouse is in a region
	where a menu can be dragged. The menus themselves will
	determine their own sizing, but this ensures that every menu
	is just as easy to drag around the screen as every other.
	Furthermore, it ensures that screen re-sizing and small screens
	can be accounted for. */
	bool isInLeftDragRegion( irr::s32 x, irr::s32 y );
	bool isInRightDragRegion( irr::s32 x, irr::s32 y );
	bool isInCenterDragRegion( irr::s32 x, irr::s32 y );
	bool isInBarDragRegion( irr::s32 x, irr::s32 y );


	// ---------- Internal stuff -----------

	recti getFrameRectChange();
};

}} // end namespace gui and irr

#endif // #ifndef _ULTRAGUIFRAME_