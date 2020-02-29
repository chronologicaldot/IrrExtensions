/*
Created by Nicolaus Anderson, 2012

For license terms, see xplo.h
*/

#include <irrlicht.h>
#include "WebTypes.h"
//#include "FuncClass.h"

#include <irrWriteableInterface.h>

#pragma once

//! Div class
/*
This class is a box that can contain children and text.
The box is as wide as the objects inside it make it
unless it has been given strict parameters.

In the latter case, the function clipAgainst() is used.
*/
class divBox : public irr::gui::IGUIElement
{
protected:
	/* Reference to the engine. */
	irr::IrrlichtDevice * irrdev;

	/* Contains the type of position that determines where this
	element will be drawn. */
	HTMLPosType position_type;

	irr::video::ITexture * background_image;
	irr::core::recti * image_box;

	/* Overflow */
	HTMLOverflow overflow;
	
	bool leftPressed;

public:
	/* Background colors */
	irr::video::SColor background_color;
	irr::video::SColor outline_color;
	bool ifDrawBorder;

	/* Responsiveness of the box */
	bool isClickable;

	/* For if this division can be pressed like a button. */
	//SimpleFuncClass * onPressFunction;

	//! Constructor
	/**/
	divBox( irr::IrrlichtDevice * device, irr::gui::IGUIElement * parent = 0, irr::s32 id=-1
		, WriteableInterface * writer=0 )
		: IGUIElement( irr::gui::EGUIET_ELEMENT, device->getGUIEnvironment(), parent, id,
			irr::core::recti(0,0,0,0) )
		, leftPressed(false)
	{
		irrdev = device;

		background_image = 0;
		background_color = irr::video::SColor( 0,0,0,0 );
		outline_color = irr::video::SColor( 255,0,0,0 );
		ifDrawBorder = false;
		image_box = new irr::core::recti( 0,0,0,0 );

		position_type = HTM_POS_static;
		overflow = HTM_OVF_show;

		if ( this->getParent() )
		{
			writer->print(
				irr::core::stringc(" pcs =")
				+ irr::core::stringc( (irr::u32) this->getParent()->getChildren().size() )
				+ irr::core::stringc( " " )
				);
		}
	}

	//! Destructor
	/**/
	~divBox()
	{
	}

	//! Set clipped
	/* Determines if this division should be clipped if its bounding box
	extends outside that of its parent's. */
	void setClipped( bool clip );
	
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

	//void setOnPressFunction( bool NewFunction() );

	bool isLeftAlignedLeft();

	bool isTopAlignedTop();

	void setPosition( HTMLPosType pos_type );

	void setBackgroundColor( irr::video::SColor color );

	void setImage( irr::io::path filename );

	//! Draw
	/* For drawing the division. This is called by the engine. */
	virtual void draw();

	//! On Event
	/* Response to user input events. */
	virtual bool OnEvent( const irr::SEvent& event );

	//! Transfer parameters
	/* Applies GUI element size and display information to the given GUI element. */
	void TransferParameters( irr::gui::IGUIElement * elem, bool dumpMe );
};