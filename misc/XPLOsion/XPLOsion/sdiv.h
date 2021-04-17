/*
Created by Nicolaus Anderson, 2012

For license terms, see xplo.h
*/

#include <irrlicht.h>
#include "WebTypes.h"

#ifndef _INCLUDE_SDIV_H_
#define _INCLUDE_SDIV_H_

//! Div class
/*
This class is a GUI box that acts like an HTML division.
It contains information for hiding its child GUI elements.
If its overflow is set to auto, scrollbars are generated
for moving the content within the clipping rectangle of this box.
*/
class divBox : public irr::gui::IGUIElement, public HTMLPositioned
{
protected:
	//irr::video::ITexture * background_image;
	irr::video::IImage* background_image;
	irr::core::recti* image_box;
	irr::core::stringc background_image_name;
	HTMLBackgroundImageRepeat background_image_repeat;

	/* Overflow */
	HTMLOverflow overflow;

	/* Background colors */
	irr::video::SColor background_color;
	irr::video::SColor border_color;
	bool ifDrawBorder;

	/* For handling scrolling when overflow is set to auto. */
	bool has_overflow_horiz;
	bool has_overflow_vert;
	irr::core::vector2di scroll_position; // last position of the scroll bar
	irr::gui::IGUIScrollBar* scrollbar_horiz;
	irr::gui::IGUIScrollBar* scrollbar_vert;

	irr::s32 SCROLLBAR_WIDTH;

	/* Region occupied by the child GUI elements. */
	irr::core::recti child_bounds;

public:

		//! Constructor
	/**/
	divBox(
		irr::gui::IGUIEnvironment * env,
		irr::gui::IGUIElement * parent = 0,
		irr::s32 id=-1 )
		: IGUIElement(
			irr::gui::EGUIET_ELEMENT,
			env,
			parent, id,
			irr::core::recti(0,0,0,0)
			)
	{
#ifdef _DEBUG
		setDebugName( "divBox" );
#endif

		background_image = 0;
		background_color = irr::video::SColor( 0,0,0,0 );
		border_color = irr::video::SColor( 255,0,0,0 );
		ifDrawBorder = false;
		image_box = 0;

		position_type = HTM_POS_static;
		overflow = HTM_OVF_show;

		has_overflow_horiz = false;
		has_overflow_vert = false;
		scrollbar_horiz = 0;
		scrollbar_vert = 0;

		SCROLLBAR_WIDTH = 20;
	}

		//! Destructor
	/**/
	~divBox()
	{
		if ( scrollbar_horiz != 0 )
			scrollbar_horiz->drop();

		if ( scrollbar_vert != 0 )
			scrollbar_vert->drop();

		if ( background_image != 0 )
			background_image->drop();
	}

		//! Get GUI element type name
	/* Override of the IGUIElement function getTypeName(). */
	const irr::c8* getTypeName() const
	{
		return "divBox";
	}
	
		//! Hides overflow
	/* Indicates if the overflow of this division is hidden. */
	bool hidesOverflow();

		//! Show the overflow
	/* Allows that which extends outside of this division's bounding box to be shown. */
	void overflowShow();
	
		//! Hide the overflow
	/* Hides that which extends outside of this division's bounding box. */
	void overflowHide();

		//! Automatically handle the overflow
	/* Creates scroll bars to display this division's contents if it extends outside
	of this division's bounding box. */
	void overflowAuto();

		//! Convert overflow parameter to string
	/* Converts this div's current HTMLOverflow value to a string and returns it. */
	irr::core::stringc convertOverFlowToString() const;

		//! Get overflow
	/* Returns the HTMLOverflow value of this div */
	HTMLOverflow getOverflow();

		//! Set overflow
	/* Saves the overflow value to this div */
	void setOverflow( HTMLOverflow new_overflow );

		//! Set overflow
	/* Converts a string to an HTMLOverflow value */
	void setOverflowFromString( const irr::c8* overflow_val );

		//! Set background color
	/* Sets the default color that will be drawn for this GUI element when it
	is displayed. */
	void setBackgroundColor( irr::video::SColor color ) { background_color = color; }

		//! Set background image
	/* Loads the image from the given path and assigns it to this GUI element
	as its background image. */
	void setImage( irr::io::path filename );

		//! Set draw outline
	/* Determines if an outline should be draw around the division.
	Note: Current implementation calls for outlines to be drawn under
	anything attached to the edge rather than pushing it inward. */
	void setDrawBorder( bool draw=false );

		//! Set border color
	/* Assigns the color of the border to draw. */
	void setBorderColor( irr::video::SColor color );

		//! Set scrollbar width
	/* Sets the width of the scroll bars, vertical or horizontal. */
	void setScrollBarWidth( irr::s32 width );

		//! Get scroll position
	/* Useful for outside this class if the programmer wants to draw
	other things based on this element's scrollbars' positions. */
	irr::core::vector2di getScrollBorPositions();

		//! Draw
	/* For drawing the division. This is called by the engine. */
	virtual void draw();

		//! Set relative position
	/* Simple override of IGUIElement::setRelativePosition but also calls for
	the updating of the scroll bars. */
	void setRelativePosition(const irr::core::recti &r);
	void setRelativePosition(const irr::core::position2di &position);

		//! Set relative position
	/* Simple override of IGUIElement::setRelativePosition but also calls for
	the updating of the scroll bars. */
	void setRelativePositionProportional(const irr::core::rectf &r);

		//! On Event
	/* Response to user input events. */
	virtual bool OnEvent( const irr::SEvent& event );

		//! Serialize
	/* For obtaining the key attributes of this GUI element. */
	virtual void serializeAttributes(
		irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options=0
		) const;

		//! Deserialize
	/* For defining this GUI element from key attributes. */
	virtual void deserializeAttributes(
		irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options=0
		);


	/* Override methods from IGUIElement and methods for keeping the scroll bars
	properly adjusted. */

		//! Add Child
	/* Handles adding a child to this GUI element. */
	void addChild(irr::gui::IGUIElement* child);

		//! Add Child
	/* Handles adding a child to this GUI element. */
	void addChildToEnd(IGUIElement* child);

		//! Remove Child
	/* Handles removing a child from this GUI element. */
	void removeChild(irr::gui::IGUIElement* child);

		//! Reset child bounds
	/* Resets the child bounds in case changes were made (e.g. GUI elements
	were made the children of this without it knowing). */
	void resetChildBounds();

		//! Reset child positions
	/* Resets the position of the children and the scrollbars. */
	void resetChildPositions();

		//! Bring scrollbars to front
	/* Make the scroll bars the first thing that will respond on user
	interaction. */
	void bringScrollsToFront();

protected:

		//! Draw children
	/* Handles drawing he children and generating scrollbars if necessary. */
	virtual void drawChildren();

		//! Get child GUI element boundaries
	/* Returns a rectangle that encompasses all of the child GUI elements. */
	// Recommend this be added to the irrlicht engine.
	irr::core::recti getChildBounds();

		//! Update scroll bars
	/* Updates the scroll bar positions. */
	void updateScrollBars( bool rescale );

		//! Update horizontal scroll bar
	/* Creates the scroll bar if necessary. Otherwise it simply udates its
	size and location information. This is necessary if the division is
	going to be resized. */
	void updateScrollBarHoriz( bool rescale );

		//! Update vertical scroll bar
	/* Creates the scroll bar if necessary. Otherwise it simply udates its
	size and location information. This is necessary if the division is
	going to be resized. */
	void updateScrollBarVert( bool rescale );
};


//! Div Generator
/* Creates an instance of divBox, assigning it to the root GUI
element if no parent was given. */
irr::gui::IGUIElement* addDivBox(
		irr::gui::IGUIEnvironment*,
		irr::gui::IGUIElement*,
		irr::s32
		);

#endif // ifndef _INCLUDE_SDIV_H_