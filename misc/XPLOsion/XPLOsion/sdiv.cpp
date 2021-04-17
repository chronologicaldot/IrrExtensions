/*
Created by Nicolaus Anderson, 2012

For license terms, see xplo.h
*/

#include "sdiv.h"

using namespace irr::gui;


bool divBox::hidesOverflow()
{
	return (overflow == HTM_OVF_hide) || (overflow == HTM_OVF_auto);
}

void divBox::overflowShow() { overflow = HTM_OVF_show; }
void divBox::overflowHide() { overflow = HTM_OVF_hide; }
void divBox::overflowAuto() { overflow = HTM_OVF_auto; }


HTMLOverflow divBox::getOverflow() { return overflow; }
void divBox::setOverflow( HTMLOverflow new_overflow ) { overflow = new_overflow; }


irr::core::stringc divBox::convertOverFlowToString() const
{
	if ( overflow == HTM_OVF_show )
		return irr::core::stringc( "visible" );

	if ( overflow == HTM_OVF_hide )
		return irr::core::stringc( "hidden" );

	if ( overflow == HTM_OVF_auto )
		return irr::core::stringc( "auto" );

	return " ";
}

void divBox::setOverflowFromString( const irr::c8* overflow_val )
{
	irr::core::stringc ovrf = overflow_val;

	if ( ovrf == "visible" || ovrf == "show" )
	{
		overflow = HTM_OVF_show;
		return;
	}

	if ( ovrf == "hidden" )
	{
		overflow = HTM_OVF_hide;
		return;
	}

	if ( ovrf == "auto" )
	{
		overflow = HTM_OVF_auto;
		return;
	}
}


void divBox::setImage( irr::io::path filename )
{
	if ( image_box != 0 )
	{
		delete image_box;
		image_box = 0;
	}

	//background_image = Environment->getVideoDriver()->getTexture( filename );
	background_image = Environment->getVideoDriver()->createImageFromFile( filename );

	background_image_name = (irr::core::stringc) filename;
}


void divBox::setDrawBorder( bool draw )
{
	ifDrawBorder = draw;
}


void divBox::setBorderColor( irr::video::SColor color )
{
	border_color = color;
}

void divBox::setScrollBarWidth( irr::s32 width )
{
	SCROLLBAR_WIDTH = (width>0? width:20);
}

irr::core::vector2di divBox::getScrollBorPositions()
{
	return scroll_position;
}

void divBox::addChild(irr::gui::IGUIElement* child)
{
	/* Resets the absolute position of the children to their
	original positions so we can add a new one in the appropriate
	location and update the scrollbars. */
	resetChildPositions();

	IGUIElement::addChild(child);

	child_bounds = getChildBounds();

	updateScrollBars(true);

	// default position for scrollbars is in front
	bringScrollsToFront();
}

void divBox::addChildToEnd(irr::gui::IGUIElement* child)
{
	/* Resets the absolute position of the children to their
	original positions so we can add a new one in the appropriate
	location and update the scrollbars. */
	resetChildPositions();

	IGUIElement::addChildToEnd(child);

	child_bounds = getChildBounds();

	updateScrollBars(true);

	// default position for scrollbars is in front
	bringScrollsToFront();
}


void divBox::removeChild(irr::gui::IGUIElement* child)
{
	/* Resets the absolute position of the children to their
	original positions so we can add a new one in the appropriate
	location and update the scrollbars. */
	resetChildPositions();

	IGUIElement::removeChild(child);

	child_bounds = getChildBounds();

	updateScrollBars(true);
}

void divBox::resetChildBounds()
{
	child_bounds = getChildBounds();
}

void divBox::resetChildPositions()
{
	irr::core::list<irr::gui::IGUIElement*>::Iterator kid;

	kid = Children.begin();
	for ( ; kid != Children.end(); kid++ )
	{
		// skip the scrollbars
		if ( scrollbar_horiz != 0 && *kid == scrollbar_horiz )
			continue;
		if ( scrollbar_vert != 0 && *kid == scrollbar_vert )
			continue;

		(*kid)->move( scroll_position );
	}

	scroll_position.set( 0, 0 );

	if ( scrollbar_horiz != 0 )
		scrollbar_horiz->setPos(0);

	if ( scrollbar_vert != 0 )
		scrollbar_vert->setPos(0);
}

void divBox::bringScrollsToFront()
{
	if ( scrollbar_horiz )
		bringToFront( scrollbar_horiz );

	if ( scrollbar_vert )
		bringToFront( scrollbar_vert );
}


void divBox::draw()
{
	if ( !IsVisible ) return;

	irr::video::IVideoDriver * vid = Environment->getVideoDriver();

	irr::core::vector2di image_shift; // location for drawing the background image

	// Draw the background color (default is transparent)
	vid->draw2DRectangle( background_color, AbsoluteRect, &AbsoluteClippingRect);

	// If there is an image, draw it
	if ( background_image != 0 )
	{
		// Box in which to draw the image
		/* For textures
		if ( image_box == 0 )
			image_box = new irr::core::recti(
					irr::core::vector2di(0,0),
					background_image->getOriginalSize()
					);

		vid->draw2DImage(
				background_image,
				AbsoluteRect.UpperLeftCorner,
				*image_box,
				&AbsoluteClippingRect
				);
		*/

		if ( image_box == 0 )
			image_box = new irr::core::recti(
					AbsoluteClippingRect.UpperLeftCorner, // upper left corner
					
					AbsoluteClippingRect.UpperLeftCorner // lower right corner
					+
						// similar to casting the image dimension
					irr::core::vector2di(
						background_image->getDimension().Width,
						background_image->getDimension().Height
						)
					);

		/* Prepare to draw the image across both the x and y axis,
		only stopping if not supposed to continue. */

			// set the starting location for drawing the image
		image_shift = AbsoluteClippingRect.UpperLeftCorner;

		// across the width
		while ( image_shift.X < AbsoluteClippingRect.LowerRightCorner.X )
		{
			while ( image_shift.Y < AbsoluteClippingRect.LowerRightCorner.Y )
			{
				for (
					irr::u32 plx = 0;
					plx < (irr::u32)image_box->getWidth()
					&&
					(irr::s32)plx + image_shift.X < AbsoluteClippingRect.LowerRightCorner.X;
					plx++
					)
				{
					for (
						irr::u32 ply = 0;
						ply < (irr::u32)image_box->getHeight()
						&&
						(irr::s32)ply + image_shift.Y < AbsoluteClippingRect.LowerRightCorner.Y;
						ply++ )
					{
						vid->drawPixel(
							plx + (irr::u32)image_shift.X,
							ply + (irr::u32)image_shift.Y,
							background_image->getPixel( plx, ply )
							);
					}
				}

				/* stop drawing if this isn't supposed to be repeated
				along the y-axis or over all */
				if ( background_image_repeat != HTM_BIR_repeat
					&& background_image_repeat != HTM_BIR_repeat_y )
					break;

				// perform shift to start drawing further along
				image_shift.Y += background_image->getDimension().Height;


			} // end going across the height

			/* stop drawing if this isn't supposed to be repeated
			along the x-axis or over all */
			if ( background_image_repeat != HTM_BIR_repeat
				&& background_image_repeat != HTM_BIR_repeat_x )
				break;

			// perform shift to start drawing further along
			image_shift.X += background_image->getDimension().Width;

			// reset
			image_shift.Y = AbsoluteClippingRect.UpperLeftCorner.Y;

		} // end going across the width
	}
	
	// If it has a border, draw that too
	if ( ifDrawBorder )
		vid->draw2DRectangleOutline( AbsoluteClippingRect, border_color );

	
	// Draw the children and possibly scrollbars.
	drawChildren();
}


void divBox::drawChildren()
{
	// Convenient iterator
	irr::core::list<irr::gui::IGUIElement*>::Iterator kid;

	// Normal drawing
	if ( overflow != HTM_OVF_auto )
	{
		kid = Children.begin();
		for ( ; kid != Children.end(); kid++ )
		{
			// skip the scrollbars
			if ( scrollbar_horiz != 0 && *kid == scrollbar_horiz )
				continue;
			if ( scrollbar_vert != 0 && *kid == scrollbar_vert )
				continue;

			(*kid)->draw();
		}
	}

	// Automatic drawing
	else {

		kid = Children.begin();
		for ( ; kid != Children.end(); kid++ )
		{
			// skip the scrollbars
			if ( scrollbar_horiz != 0 && *kid == scrollbar_horiz )
				continue;
			if ( scrollbar_vert != 0 && *kid == scrollbar_vert )
				continue;

			(*kid)->draw();
		}

		updateScrollBars(false);

		if ( scrollbar_horiz != 0 )
		{
			// Draw the scrollbar
			scrollbar_horiz->draw();
		}

		if ( scrollbar_vert != 0 )
		{
			// Draw the scrollbar
			scrollbar_vert->draw();
		}

	}

}


irr::core::recti divBox::getChildBounds()
{
	// Convenient iterator
	irr::core::list<irr::gui::IGUIElement*>::Iterator kid;

	kid = Children.begin();

	// skip the scrollbars
	if ( scrollbar_horiz != 0 && *kid == scrollbar_horiz )
		kid++;
	if ( scrollbar_vert != 0 && *kid == scrollbar_vert )
		kid++;

	/* Reset the region occupied by the child GUI elements. */
	if ( kid != Children.end() )
	{
		child_bounds.constrainTo(
			(*kid)->getRelativePosition()
			);
	}

	// Find out the bounding area of the children
	for ( ; kid != Children.end(); kid++ )
	{
		// skip the scrollbars
		if ( scrollbar_horiz != 0 && *kid == scrollbar_horiz )
			continue;
		if ( scrollbar_vert != 0 && *kid == scrollbar_vert )
			continue;

		child_bounds.addInternalPoint(
			(*kid)->getRelativePosition().UpperLeftCorner
			);

		child_bounds.addInternalPoint(
			(*kid)->getRelativePosition().LowerRightCorner
			);
	}

	return child_bounds;
}


void divBox::updateScrollBars( bool rescale )
{
	/* Pre-update check for vertical overflow - allows us to account for it
	when making the horizontal scroll bar. */
	if ( child_bounds.getHeight() > AbsoluteRect.getHeight() )
		has_overflow_vert = true;

	updateScrollBarHoriz( rescale );
	updateScrollBarVert( rescale );
}


void divBox::updateScrollBarHoriz( bool rescale )
{
	// Is a scroll bar needed?
	if (
		child_bounds.UpperLeftCorner.X < 0
		||
		child_bounds.LowerRightCorner.X	+ ( has_overflow_vert?SCROLLBAR_WIDTH:0 )
		>
		AbsoluteRect.getWidth()
		)
	{
		// Does a scroll bar need to be made?
		if ( scrollbar_horiz == 0 )
		{
			scrollbar_horiz =
				Environment->addScrollBar(
						true, // = horizontal bar
						irr::core::recti(
							0,
							AbsoluteRect.getHeight() - SCROLLBAR_WIDTH,
							AbsoluteRect.getWidth(),
							AbsoluteRect.getHeight()
							),
						this
						/* parent GUI element
						  - Set to a sub-element because we're going to be drawing
						  this from an absolute frame of reference
						  (and we don't want it moved with the other children).
						  */
						);

			scrollbar_horiz->setSmallStep( 5 );
			scrollbar_horiz->setLargeStep( 20 );

			scrollbar_horiz->setSubElement( false );

		} else {
			scrollbar_horiz->setVisible( true );

			// Re-scale if desired
			if ( rescale )
				scrollbar_horiz->setRelativePosition(
						irr::core::recti(
							0,
							AbsoluteRect.getHeight() - SCROLLBAR_WIDTH,
							AbsoluteRect.getWidth(),
							AbsoluteRect.getHeight()
							)
						);
		}

		has_overflow_horiz = true;

		irr::s32 gap_x;

		/* Checking to see if the scroll bar's maximum will allow us to
		show all of the child GUI elements.
		Note: we leave room for the vertical bar if it exists. */
		gap_x = child_bounds.LowerRightCorner.X
				- AbsoluteRect.getWidth();

		if ( gap_x > 0 )
		{
			/* Setting the maximum value for the scroll bar to be
			positioned to. */
			scrollbar_horiz->setMax(
				gap_x + ( has_overflow_vert?SCROLLBAR_WIDTH:0 )
				);
		} else {
			scrollbar_horiz->setMax( ( has_overflow_vert?SCROLLBAR_WIDTH:0 ) );
		}

		/* Checking to see if the scroll bar's minimum will allow us to
		show all of the child GUI elements. */
		gap_x = child_bounds.UpperLeftCorner.X;

		if ( gap_x < 0 )
		{
			/* Setting the minimum value for the scroll bar to be
			positioned to. */
			scrollbar_horiz->setMin( gap_x );
		} else {
			scrollbar_horiz->setMin( 0 );
		}
	}
	else if (
		child_bounds.UpperLeftCorner.X > 0
		&&
		child_bounds.LowerRightCorner.X	+ ( has_overflow_vert?SCROLLBAR_WIDTH:0 )
		<
		AbsoluteRect.getWidth()
		)
	{
		// NO HORIZONTAL SCROLL BAR - elements fit
		has_overflow_horiz = false;

		if ( scrollbar_horiz != 0 )
			scrollbar_horiz->setVisible( false );
	}
}


void divBox::updateScrollBarVert( bool rescale )
{
	// Is a scroll bar needed?
	if (
		child_bounds.UpperLeftCorner.Y < 0
		||
		child_bounds.LowerRightCorner.Y	+ ( has_overflow_horiz?SCROLLBAR_WIDTH:0 )
		>
		AbsoluteRect.getHeight()
		)
	{
		// Create the vertical scrollbar if it doesn't exist
		if ( scrollbar_vert == 0 )
		{
			scrollbar_vert =
				Environment->addScrollBar(
						false, // = vertical bar
						irr::core::recti(
							AbsoluteRect.getWidth() - SCROLLBAR_WIDTH,
							0,
							AbsoluteRect.getWidth(),
							AbsoluteRect.getHeight()
							+ (has_overflow_horiz? -SCROLLBAR_WIDTH:0)
							),
						this
						/* parent GUI element
						  - Set to a sub-element because we're going to be drawing
						  this from an absolute frame of reference
						  (and we don't want it moved with the other children).
						  */
						);

			scrollbar_vert->setSmallStep( 5 );
			scrollbar_vert->setLargeStep( 20 );

			scrollbar_vert->setSubElement( false );

		} else {
			scrollbar_vert->setVisible( true );

			// Re-scale if desired
			if ( rescale )
				scrollbar_vert->setRelativePosition(
						irr::core::recti(
							AbsoluteRect.getWidth() - SCROLLBAR_WIDTH,
							0,
							AbsoluteRect.getWidth(),
							AbsoluteRect.getHeight()
							+ (has_overflow_horiz? -SCROLLBAR_WIDTH:0)
							)
						);
		}

		has_overflow_vert = true;

		irr::s32 gap_y;

		/* Checking to see if the scroll bar's maximum will allow us to
		show all of the child GUI elements.
		Note: we leave room for the horizontal bar if it exists. */
		gap_y = child_bounds.LowerRightCorner.Y
				- AbsoluteRect.getHeight();

		if ( gap_y > 0 )
		{
			/* Setting the maximum value for the scroll bar to be
			positioned to. */
			scrollbar_vert->setMax(
				gap_y + ( has_overflow_horiz?SCROLLBAR_WIDTH:0 )
				);
		} else {
			scrollbar_vert->setMax( ( has_overflow_horiz?SCROLLBAR_WIDTH:0 ) );
		}

		/* Checking to see if the scroll bar's minimum will allow us to
		show all of the child GUI elements. */
		gap_y = child_bounds.UpperLeftCorner.Y;

		if ( gap_y < 0 )
		{
			/* Setting the minimum value for the scroll bar to be
			positioned to. */
			scrollbar_vert->setMin( gap_y );
		} else {
			scrollbar_vert->setMin( 0 );
		}
	}
	else if (
		child_bounds.UpperLeftCorner.Y > 0
		&&
		child_bounds.LowerRightCorner.Y	+ ( has_overflow_horiz?SCROLLBAR_WIDTH:0 )
		<
		AbsoluteRect.getHeight()
		)
	{
		// NO VERTICAL SCROLL BAR - elements fit
		has_overflow_vert = false;

		if ( scrollbar_vert != 0 )
			scrollbar_vert->setVisible( false );
	}
}


/* ****** Overrides of setRelative Position */

void divBox::setRelativePosition(const irr::core::recti &r)
{
	IGUIElement::setRelativePosition(r);

	updateScrollBars(true);
}

void divBox::setRelativePosition(const irr::core::position2di &position)
{
	IGUIElement::setRelativePosition(position);

	updateScrollBars(true);
}

void divBox::setRelativePositionProportional(const irr::core::rectf &r)
{
	IGUIElement::setRelativePositionProportional(r);

	updateScrollBars(true);
}

/* ****** END Overrides of setRelative Position */


/* In response to user interaction, divisions do nothing more than
focus the activity on them, indicating that something interacted
with them. It is up to the programmer to decide on their response.
Response can be handled using xplo and GUIfunctionFire. */
bool divBox::OnEvent( const irr::SEvent& event )
{
	switch( event.EventType )
	{
	case irr::EET_GUI_EVENT:

		switch ( event.GUIEvent.EventType )
		{
		case irr::gui::EGET_ELEMENT_FOCUS_LOST:

			if ( event.GUIEvent.Caller == this )
			{
				// Bring the scrollbar elements to the front by default
				bringScrollsToFront();

				return false;
			}

			break; // end case irr::gui::EGET_ELEMENT_FOCUS_LOST

		case irr::gui::EGET_SCROLL_BAR_CHANGED:

			/* For shifting the location of children when using automatic overflow setting. */
			irr::core::vector2di child_shift;

			/* Identify the shift in location of the GUI element children based
			on the scrollbar's bar location. */
			child_shift.X =
				(scrollbar_horiz?
				scroll_position.X - scrollbar_horiz->getPos()
				: 0 );

			child_shift.Y =
				(scrollbar_vert?
				scroll_position.Y - scrollbar_vert->getPos()
				: 0 );


			// Update the scroll position

			if ( scrollbar_horiz )
				scroll_position.X = scrollbar_horiz->getPos();

			if ( scrollbar_vert )
				scroll_position.Y = scrollbar_vert->getPos();


			/* Shift all of the child GUI elements according to the difference
			in current scrollbar position from original position. */
			irr::core::list<irr::gui::IGUIElement*>::Iterator kid = Children.begin();
			for ( ; kid != Children.end(); kid++ )
			{
				// skip the scrollbars
				if ( scrollbar_horiz != 0 && *kid == scrollbar_horiz )
					continue;
				if ( scrollbar_vert != 0 && *kid == scrollbar_vert )
					continue;

				(*kid)->move( child_shift );
			}

			break; // end case irr::gui::EGET_SCROLL_BAR_CHANGED

		}

		break;

	case irr::EET_MOUSE_INPUT_EVENT:

		if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
		{
			if (
				!AbsoluteClippingRect.isPointInside(
					irr::core::position2d<irr::s32>(
							event.MouseInput.X, event.MouseInput.Y
							)
					)
				)
			{
				Environment->setFocus(this);
				return true;
			}
		}
		break;

	default: break;
	}

	return IGUIElement::OnEvent( event );
}


void divBox::serializeAttributes(
		irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options
		) const
{
	// ensure the usual details are also accounted for
	IGUIElement::serializeAttributes( out, options );

	out->addEnum(		"position",			position_type, HTMLPosTypes	);

	/*
	if ( background_image != 0 )
		out->addTexture("background-image",	background_image	);
		*/
	if ( background_image != 0 )
		out->addString( "background-image", background_image_name.c_str() );

	out->addEnum(		"background-repeat",background_image_repeat,
						HTMLBackgroundImageRepeatTypes);


	out->addColor(		"background-color", background_color	);
	out->addColor(		"outline-color",	border_color		);
	out->addBool(		"Border",			ifDrawBorder		);

	out->addEnum(		"overflow",			overflow, WebOverflowTypes );

	out->addInt(		"scrollbar-width",	SCROLLBAR_WIDTH		);
}


void divBox::deserializeAttributes(
				irr::io::IAttributes *in,
				irr::io::SAttributeReadWriteOptions *options
				)
{
	// Have the superclass handle the rest of the deserialization
	IGUIElement::deserializeAttributes( in, options );

	// Set the position/layout type
	position_type = (HTMLPos) in->getAttributeAsEnumeration( "position", HTMLPosTypes );

	// Get the background image
	//background_image = in->getAttributeAsTexture( "background-image" );
	setImage( (irr::io::path) in->getAttributeAsString( "background-image" ) );

	// Get the background image repetition style
	background_image_repeat =
		(HTMLBackgroundImageRepeat)
		 in->getAttributeAsEnumeration( "background-repeat", HTMLBackgroundImageRepeatTypes );

	// Get the background color
	irr::s32 color_attr = in->findAttribute( "background-color" );
	if ( color_attr != -1 )
		background_color = in->getAttributeAsColor( color_attr );

	// Identify if there should be an outline	
	if ( in->existsAttribute( "Border" ) && in->getAttributeAsBool( "Border" ) )
	{
		ifDrawBorder = true;
		border_color = in->getAttributeAsColor( "outline-color" );
	}

	// Set the overflow
	overflow = (HTMLOverflow) in->getAttributeAsEnumeration( "overflow", WebOverflowTypes );

	if ( in->existsAttribute( "scrollbar-width" ) )
		SCROLLBAR_WIDTH = in->getAttributeAsInt( "scrollbar-width" );
}



irr::gui::IGUIElement* addDivBox(
		irr::gui::IGUIEnvironment * env,
		irr::gui::IGUIElement * parent = 0,
		irr::s32 id=-1
		)
{
	divBox* box;

	if ( parent == 0 )
		box = new divBox( env, env->getRootGUIElement(), id );
	else
		box = new divBox( env, parent, id );

	box->drop();

	return (irr::gui::IGUIElement*)box;
}
