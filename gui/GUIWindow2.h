/* Taken from the irrlicht engine
and slightly modified by adding
an event parent and changing names.
Modifications by Nic Anderson. */

#ifndef _GUIWINDOW2_H_
#define _GUIWINDOW2_H_

#include <IGUIWindow.h>

namespace irr {
namespace gui {

using core::rect;

class GUIWindow2 : public IGUIWindow
{
public:
	GUIWindow2( IGUIEnvironment* pEnvironment, IGUIElement* pParent, rect<s32> pRect, s32 id=-1 );

	~GUIWindow2();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! set event parent
	/* Nic Anderson mod */
	void setEventParent( IEventReceiver* pEventParent );

	//! update absolute position
	virtual void updateAbsolutePosition();

	//! draws the element and its children
	virtual void draw();

	//! Returns pointer to the close button
	virtual IGUIButton* getCloseButton() const;

	//! Returns pointer to the minimize button
	virtual IGUIButton* getMinimizeButton() const;

	//! Returns pointer to the maximize button
	virtual IGUIButton* getMaximizeButton() const;

	//! Returns true if the window is draggable, false if not
	virtual bool isDraggable() const;

	//! Sets whether the window is draggable
	virtual void setDraggable(bool draggable);

	//! Set if the window background will be drawn
	virtual void setDrawBackground(bool draw);

	//! Get if the window background will be drawn
	virtual bool getDrawBackground() const;

	//! Set if the window titlebar will be drawn
	//! Note: If the background is not drawn, then the titlebar is automatically also not drawn
	virtual void setDrawTitlebar(bool draw);

	//! Get if the window titlebar will be drawn
	virtual bool getDrawTitlebar() const;

	//! Returns the rectangle of the drawable area (without border and without titlebar)
	virtual core::rect<s32> getClientRect() const;

protected:

	void updateClientRect();
	void refreshSprites();

	IGUIButton* CloseButton;
	IGUIButton* MinButton;
	IGUIButton* RestoreButton;
	core::rect<s32> ClientRect;
	video::SColor CurrentIconColor;

	IEventReceiver* EventParent; // Nic Anderson mod

	core::position2d<s32> DragStart;
	bool Dragging, IsDraggable;
   	bool DrawBackground;
   	bool DrawTitlebar;
	bool IsActive;
};

}} // end namespaces

#endif // #ifndef _GUIWINDOW2_H_
