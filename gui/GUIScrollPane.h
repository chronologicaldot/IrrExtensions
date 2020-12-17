// Copyright 2016 Nic Anderson
#ifndef GUIScrollPane_H
#define GUIScrollPane_H

#include <IGUIElement.h>

namespace irr {
namespace gui {

class IGUIScrollBar;

using irr::core::recti;

class GUIScrollPane : public IGUIElement
{
protected:
	struct GUIElementPosition
	{
		IGUIElement* element;
		core::vector2d<s32> position;

		GUIElementPosition( IGUIElement* e, core::vector2d<s32> p )
			: element(e), position(p)
		{}
	};

	IGUIScrollBar* horizontalScrollBar;
	IGUIScrollBar* verticalScrollBar;
	u32 barWidth; // Width of a scroll width across its short side
	IGUIElement* childWrapper;
	core::array<GUIElementPosition> childStartPositions; // (not in same order as Children)
	core::rect<s32> childBounds; // rectangle around all children

public:
	GUIScrollPane( IGUIEnvironment* pGUIEnvironment, IGUIElement* pParent, recti pRect, irr::s32 id=-1 );
	~GUIScrollPane();

	void constructChildWrapper(IGUIEnvironment* pGUIEnvironment, recti position);
	IGUIElement* getChildWrapper() const;

	void showHorizontalScrollBar(bool yes);
	void showVerticalScrollBar(bool yes);
	IGUIScrollBar* getHorizontalBar() const;
	IGUIScrollBar* getVerticalBar() const;

	virtual void setRelativePosition( recti pPosition );
	virtual void addChild( IGUIElement* pElement );
	virtual void removeChild( IGUIElement* pElement );
	void clearChildren();
	virtual void updateAbsolutePosition();
	virtual bool OnEvent(const SEvent& event);

	virtual void serializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) const;
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() const { return "scrollPane"; }

protected:
	void recalculateChildBounds();
	void shiftChildrenToPosition( s32 x, s32 y );
};

}}

#endif
