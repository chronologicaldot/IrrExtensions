// (c) 2015 Nicolaus Anderson

#ifndef GUI_GROUPING_PANEL_H
#define GUI_GROUPING_PANEL_H

#include <IGUIElement.h>

namespace irr {

namespace video {
	class ITexture;
}

namespace gui {

using core::dimension2du;
using core::recti;
using video::ITexture;

//! Class GUI Grouping Panel
/* The sole purpose of this class is to allow for easy grouping of elements, both visibly
and programmatically. */
class GUIGroupingPanel : public IGUIElement
{
	dimension2du textSize;
	bool showBorder;
	f32 borderRadius;

	ITexture* ULtexture;
	ITexture* URtexture;
	ITexture* LLtexture;
	ITexture* LRtexture;
public:
	GUIGroupingPanel( const wchar_t* pText, IGUIEnvironment* pEnvironment, IGUIElement* pParent, const recti& pRect, s32 id=-1 );

	//! Set the text
	/* This function both sets the text and peforms some calculations for composing the client area. */
	virtual void setText(const wchar_t* text);

	//! Set border visible
	/* Sets if the border is visible. */
	void setShowBorder( bool pShow );

	//! Set border edge radius
	/* Sets the radius of the edge border. Note, this will effect the client area.
	The larger the radius, the smaller the client area will be. */
	void setBorderEdgeRadius( f32 pRadius );

	/* Returns the area available to the child elements. Note that, if this area is to change,
	child GUI elements should be set to retain their positions relative to the walls, NOT using EGUIA_SCALE. */
	recti getClientArea();

	/* Updates the absolute rectangle and the client area rectangle. */
	//virtual void updateAbsolutePosition();

	/* Draws the border of this element. */
	virtual void draw();

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "groupingPanel"; }

	virtual void serializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) const;
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

protected:
	//! Update image cache
	/* Creates images to be drawn around the border. These are the corners. */
	void updateImageCache();

	//! Checks if a line is in the clipping rectangle and if so, bounds it
	bool ensureVerticalLineInBounds(core::vector2di& start, core::vector2di& end);
	bool ensureHorizontalLineInBounds(core::vector2di& start, core::vector2di& end);
};

}}

#endif // #ifndef GUI_GROUPING_PANEL_H
