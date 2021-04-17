// (c) 2015 Nicolaus Anderson

#ifndef GUI_DUAL_SECTION_H
#define GUI_DUAL_SECTION_H

#include <IGUIElement.h>

namespace irr {
namespace gui {

//! Class Dual Section
/* Creates and controls two child GUI elements side by side and forced to share the same space.
It also creates a draggable bar that allows the user to resize these elements.
Add children to these child GUI elements via getSectionTopLeft() and getSectionBottomRight().
Other children can be added to this element which are not controlled. Use the normal addChild method. */
class GUIDualSection : public IGUIElement
{
	IGUIElement* sectionTopLeft;
	IGUIElement* sectionBottomRight;
	bool vertical;
	s32 dragBarSize;	// Size of the drag bar (currently, cannot be changed)
	f32 currShift;		// Current % each section shares of the space in the parent GUI element
	core::rect<s32> dragBarRect;
	core::vector2d<s32> lastMousePos;
	core::vector2d<s32> currMousePos;
	bool dragging;

public:
	//! Constructor
	/* \param pVertical - Indicates if the sections should be split vertically, one on top and
				one on the bottom. Otherwise, they will be split horizontally.
	\param pShift - The percentage of the available GUI space occupied by the top/left section.
	*/
	GUIDualSection( bool pVertical, f32 pShift, IGUIEnvironment* pEnvironment, IGUIElement* pParent, core::rect<s32> pRect, s32 id=-1 );

	~GUIDualSection();

	// Grab these to add children to the controlled elements
	IGUIElement* getSectionTopLeft();
	IGUIElement* getSectionBottomRight();

	// Prevent re-adding controlled children
	virtual void addChild( IGUIElement* child);

	// Prevent removal of controlled children
	virtual void removeChild( IGUIElement* child );

	virtual void updateAbsolutePosition();

	virtual bool OnEvent( const SEvent& event );

	virtual void draw();

	void setVertical( bool yes );

	void setShiftPercentage( f32 pShift );

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "dualSection"; }

	virtual void serializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) const;
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

protected:
	// Fits the elements into the window
	// Cannot be in updateAbsolutePosition() because it is needed in the constructor
	void cramSections(); // only used for strict alignment
	void updateDragBar();
	void setVerticalNoUpdateAbs( bool );
};

}}

#endif // #ifndef GUI_DUAL_SECTION_H
