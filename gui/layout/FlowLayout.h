// (C) 2018 Nicolaus Anderson

#include <IGUIElement>

namespace irr {
namespace gui {

//! Flow alignment Layout class
/*
	A simple, temporal class for automatically aligning GUI elements. Meant for quick usage and destruction.
	It can be destroyed at any time.
	Example usage:

	IGUIButton	*b = guiEnvironment->createButton(...),
				*b2 = guiEnvironment->createButton(...);
	FlowLayout flow(300, 0,0, 3,3);
	flow.add(b);
	flow.add(b2);
	flow.setSpanByParent(true);
	flow.alignAsRow(true);
*/
class FlowLayout {
	typename list_type core::list<IGUIElement*>;
	typename list_iterator list_type::Iterator;

	u32 span;
	bool setSpanByParentSize;
	s32 startX;
	s32 startY;
	s32 gapX;
	s32 gapY;
	list_type  queue;
	const IGUIElement* firstElement;

public:
	//! cstor
	/*
	@param span - The max width (or height) before wrapping. If zero, items are always placed on a newline.
	@param startingX - The relative X position of the upper left corner of the first element in the layout.
	@param startingY - The relative Y position of the upper left corner of the first element in the layout.
	@param spacingX - The x-distance between elements in the layout.
	@param spacingY - The y-distance between elements in the layout.
	*/
	FlowLayout( u32 span, s32 startingX, s32 startingY, s32 spacingX, s32 spacingY )
		: span(span)
		, setSpanByParentSize(false)
		, startX(startingX)
		, startY(startingY)
		, gapX(spacingX)
		, gapY(spacingY)
		, queue()
		, firstElement(0)
	{
	}

	//! dstor
	~FlowLayout() {
		list_iterator li = queue.begin();
		for (; li != queue.end(); ++li) {
			(*li)->drop();
		}
	}

	//! Add an element
	//! Does not allow adding elements with different parents.
	void add( IGUIElement* e ) {
		if ( e ) {
			if ( firstElement ) {
				if ( e->getParent() != firstElement->getParent() ) {
					return;
				}
			}
			e->grab();
			queue.push_back(e);
		}
	}

	//! Remove an element
	void remove( IGUIElement* e ) {
		list_iterator  li = queue.begin();
		if ( e ) {
			for (; li != queue.end(); ++li) {
				if ( *li == e ) {
					queue.erase(li);
					e->drop();
					return;
				}
			}
		}
	}

	//! Set span by parent size
	/*
		Uses the size of the parent to determine the value for wrapping.
	*/
	void setSpanByParent(bool yes) {
		setSpanByParentSize = yes;
	}

	//! Align elements in a row
	/*	Elements are aligned according to the left side and from left-to-right.
		Wrapping occurs when the next element's starting position would be outside of the row width (given as "span").
	*/
	void alignAsRow(bool wrap=false) {
		core::recti  itemRect;
		s32  currX = 0;
		s32  currY = 0;
		s32  currMaxItemHeight = 0;
		list_iterator  li = queue.begin();
		if ( setSpanByParentSize ) {
			if ( li != queue.end() ) {
				if ( (*li)->getParent() ) {
					span = (*li)->getParent()->getRelativeRect().getWidth();
				}
			}
		}
		for (; li != queue.end(); ++li) {
			(*li)->setAlignment(EGUIA_UPPERLEFT,EGUIA_UPPERLEFT,EGUIA_UPPERLEFT,EGUIA_UPPERLEFT);
			(*li)->setRelativePosition(core::vector2di(startX + currX, startY + currY));
			itemRect = (*li)->getRelativePosition();
			currX += itemRect.getWidth() + gapX;
			currMaxItemHeight = core::max_( currMaxItemHeight, itemRect.getHeight() );
			// Row wrapping
			if ( wrap && currX > (s32)span ) {
				currX = 0;
				currY += currMaxItemHeight + gapY;
				currMaxItemHeight = 0;
			}
		}
	}

	//! Align elements in a column with no wrapping
	// All elements will be the full width of the parent element
	void alignAsOneFullWidthColumn() {
		core::recti  itemRect;
		s32  currY = 0;
		list_iterator  li = queue.begin();
		for (; li != queue.end(); ++li) {
			(*li)->setAlignment(EGUIA_UPPERLEFT,EGUIA_LOWERRIGHT,EGUIA_UPPERLEFT,EGUIA_UPPERLEFT);
			(*li)->setRelativePosition(core::vector2di(startX, startY + currY));
			itemRect = (*li)->getRelativeRect();
			currY += itemRect.getHeight() + gapY;
		}
	}
};

}}
