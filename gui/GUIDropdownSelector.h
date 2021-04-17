// (c) 2015 Nicolaus Anderson
// zlib license

#ifndef GUI_DROPDOWN_SELECTOR_H
#define GUI_DROPDOWN_SELECTOR_H

#include <IGUIElement.h>

namespace irr {
namespace gui {

class IGUIButton;
class IGUIContextMenu;

using core::rect;

//! GUI Drop-down selector
/*
This is merely a controller for a button and a menu.
The menu is used to select what is displayed on the button.
*/
class GUIDropdownSelector : public IGUIElement
{
	IGUIButton* button;
	IGUIContextMenu* menu;
	bool dirty;
	bool wasMenuFocus;
	rect<s32> iconRect;
	static const s32 arrowHeight = 8;
public:
	GUIDropdownSelector( IGUIEnvironment* pEnvironment, IGUIElement* pParent, rect<s32> pRect, s32 id=-1 );

	~GUIDropdownSelector();

	/* Sets the current text (which may not be in the menu).
	Note that this invalidates the current selection. */
	virtual void setText( const wchar_t* pText );
	virtual void setToolTipText(const wchar_t* text);

	/* Returns the index of the selected item. */
	s32 getSelected() const;

	/* Returns the text of the selected item. */
	const wchar_t* getSelectedText() const;

	const wchar_t* getItemText( u32 pIndex ) const;
	void setItemText( u32 pIndex, const wchar_t* pText );
	void setItemEnabled( u32 pIndex, bool pEnable );
	bool isItemEnabled( u32 pIndex ) const;
	u32 getItemCount() const;

	void removeItem( u32 pIndex );
	void removeAllItems();

	void addItem( const wchar_t* pText, bool pEnabled=true );

	virtual void updateAbsolutePosition()  _IRR_OVERRIDE_;
	virtual bool OnEvent( const SEvent& event )  _IRR_OVERRIDE_;
	virtual void draw()  _IRR_OVERRIDE_;
	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "dropdownSelector"; }
	virtual void serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options ) const  _IRR_OVERRIDE_;
	virtual void deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options )  _IRR_OVERRIDE_;

protected:
	void sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement=0 );
};

}}

#endif // #ifndef GUI_DROPDOWN_SELECTOR_H
