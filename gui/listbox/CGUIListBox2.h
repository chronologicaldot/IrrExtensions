// by tom_gamer
// changed to CGUIListBox2 by Nic Anderson
// Modified to store data associated with text.

// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_GUI_LIST_BOX2_H_INCLUDED__
#define __C_GUI_LIST_BOX2_H_INCLUDED__

// booted from standalone version
//#include "IrrCompileConfig.h"
//#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIListBox2.h"
#include <irrArray.h>

namespace irr
{
	class ITimer; // added by Nic Anderson to avoid os.h dependency

namespace gui
{

	class IGUIFont;
	class IGUIScrollBar;

	class CGUIListBox2 : public IGUIListBox2
	{
	public:
		//! constructor
		// timer param added by Nic Anderson to avoid os.h dependency
		CGUIListBox2(IGUIEnvironment* environment, ITimer* timer, IGUIElement* parent,
			s32 id, core::rect<s32> rectangle, bool clip=true,
			bool drawBack=false, bool moveOverSelect=false);

		//! destructor
		virtual ~CGUIListBox2();

		//! returns amount of list items
		virtual u32 getItemCount() const;

		//! returns string of a list item. the id may be a value from 0 to itemCount-1
		virtual const wchar_t* getListItem(u32 id) const;

		//! returns data associated with the selected list item
		virtual IReferenceCounted* getListItemData(u32 id) const; // added by Nic Anderson

		//! adds an list item, returns id of item
		virtual u32 addItem(const wchar_t* text, IReferenceCounted* data); // changed by Nic Anderson

		//! clears the list
		virtual void clear();

		//! returns id of first selected item. returns -1 if no item is selected.
		virtual s32 getSelected() const;

		//! returns ids of selected items.
		virtual void getSelected(core::array<s32> &sel);

		//! sets the selected item. Set this to -1 if no item should be selected
		virtual void setSelected(s32 id);

		//! sets the selected items.
		virtual void setSelected(core::array<u32> sel);

		//! sets the selected item. Set this to -1 if no item should be selected
		virtual void setSelected(const wchar_t *item);

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! adds an list item with an icon
		//! \param text Text of list entry
		//! \param icon Sprite index of the Icon within the current sprite bank. Set it to -1 if you want no icon
		//! \return
		//! returns the id of the new created item
		virtual u32 addItem(const wchar_t* text, s32 icon, IReferenceCounted* data); // changed by Nic anderson

		//! Returns the icon of an item
		virtual s32 getIcon(u32 id) const;

		//! removes an item from the list
		virtual void removeItem(u32 id);

		//! get the the id of the item at the given absolute coordinates
		virtual s32 getItemAt(s32 xpos, s32 ypos) const;

		//! Sets the sprite bank which should be used to draw list icons. This font is set to the sprite bank of
		//! the built-in-font by default. A sprite can be displayed in front of every list item.
		//! An icon is an index within the icon sprite bank. Several default icons are available in the
		//! skin through getIcon
		virtual void setSpriteBank(IGUISpriteBank* bank);

		//! set whether the listbox should scroll to newly selected items
		virtual void setAutoScrollEnabled(bool scroll);

		//! returns true if automatic scrolling is enabled, false if not.
		virtual bool isAutoScrollEnabled() const;

		//! Update the position and size of the listbox, and update the scrollbar
		virtual void updateAbsolutePosition();

		//! Writes attributes of the element.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

		//! set all item colors at given index to color
		virtual void setItemOverrideColor(u32 index, video::SColor color);

		//! set all item colors of specified type at given index to color
		virtual void setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, video::SColor color);

		//! clear all item colors at index
		virtual void clearItemOverrideColor(u32 index);

		//! clear item color at index for given colortype
		virtual void clearItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType);

		//! has the item at index its color overwritten?
		virtual bool hasItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const;

		//! return the overwrite color at given item index.
		virtual video::SColor getItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const;

		//! return the default color which is used for the given colorType
		virtual video::SColor getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const;

		//! set the item at the given index
		virtual void setItem(u32 index, const wchar_t* text, s32 icon, IReferenceCounted* data); // changed by Nic anderson

		//! Insert the item at the given index
		//! Return the index on success or -1 on failure.
		virtual s32 insertItem(u32 index, const wchar_t* text, s32 icon, IReferenceCounted* data); // changed by Nic anderson

		//! Swap the items at the given indices
		virtual void swapItems(u32 index1, u32 index2);

		//! set global itemHeight
		virtual void setItemHeight( s32 height );

        //! Sets whether to draw the background
        virtual void setDrawBackground(bool draw);

		//! Sets whether this is a multi-select list box
		virtual void setMultiSelect(bool multiSelect);

	private:

		struct ListItem
		{
			core::stringw text;
			s32 icon;
			IReferenceCounted* data; // added by Nic Anderson
			bool selected;

			// changed by Nic Anderson
			ListItem() : icon(-1), data(0), selected(false)
			{}

			// added by Nic Anderson
			void setData( IReferenceCounted* pData )
			{
				if ( data ) data->drop();
				data = pData;
				if ( data ) data->grab();
			}

			// added by Nic Anderson
			~ListItem()
			{
				if ( data ) data->drop();
			}

			// A multicolor extension
			struct ListItemOverrideColor
			{
				ListItemOverrideColor() : Use(false) {}
				bool Use;
				video::SColor Color;
			};
			ListItemOverrideColor OverrideColors[EGUI_LBC_COUNT];
		};

		void recalculateItemHeight();
		void selectNew(s32 ypos, bool onlyHover, bool ctrlState=false, bool shftState=false);
		void recalculateScrollPos();

		// extracted that function to avoid copy&paste code
		void recalculateItemWidth(s32 icon);

		// get labels used for serialization
		bool getSerializationLabels(EGUI_LISTBOX_COLOR colorType, core::stringc & useColorLabel, core::stringc & colorLabel) const;

		//! sets the select-state of an item.
		void setItemSelection(u32 id, bool selected);


		core::array< ListItem > Items;
		s32 Selected;
		s32 ItemHeight;
		s32 ItemHeightOverride;
		s32 TotalItemHeight;
		s32 ItemsIconWidth;
		gui::IGUIFont* Font;
		gui::IGUISpriteBank* IconBank;
		gui::IGUIScrollBar* ScrollBar;
		u32 selectTime;
		u32 LastKeyTime;
		core::stringw KeyBuffer;
		bool Selecting;
		bool DrawBack;
		bool MoveOverSelect;
		bool AutoScroll;
		bool HighlightWhenNotFocused;
                bool MultiSelect;
                bool DidMouseMove;
                s32 LastSelected;

		// timer added by Nic Anderson to avoid os.h dependency
		ITimer* Timer;
	};


} // end namespace gui
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_GUI_

#endif
