/*
Project name:
CSS Library

Created by Nicolaus Anderson, 2012
see CSSLibrary.h for terms and conditions.

Necessities
*/

#include "CSSTagData.h"


irr::core::recti CSSTagData::createBoundedRect(
			irr::gui::IGUIElement * parent
			)
{
	irr::s32 left =0;
	irr::s32 right =0;
	irr::s32 top = 0;
	irr::s32 bottom = 0;
	irr::s32 height = 0;
	irr::s32 width = 0;
	bool hasLeft, hasRight, hasTop, hasBottom, hasHeight, hasWidth;

	// obtain information about the walls
	hasLeft = existsAttribute( "left" );
	hasRight = existsAttribute( "right" );
	hasTop = existsAttribute( "top" );
	hasBottom = existsAttribute( "bottom" );
	hasHeight = existsAttribute( "height" );
	hasWidth = existsAttribute( "width" );

	if ( hasLeft )
		left = getAttributeAsInt( "left" );

	if ( hasRight )
		right = getAttributeAsInt( "right" );

	if ( hasTop )
		top = getAttributeAsInt( "top" );

	if ( hasBottom )
		bottom = getAttributeAsInt( "bottom" );

	if ( hasHeight )
		height = getAttributeAsInt( "height" );

	if ( hasWidth )
		width = getAttributeAsInt( "width" );


	// Save the current parent parameters for use in the alignment process
	irr::core::recti temp_parent_pos = parent->getAbsolutePosition();

	// Create the rectangle to be changed
	irr::core::recti r = irr::core::recti(
		left,
		top,
		temp_parent_pos.getWidth() - right,
		temp_parent_pos.getHeight() - bottom
		);


	/* Alter rectangle if one of the needed boundary parameters is missing
	AND there is a parameter to replace it. */
	if ( (!hasLeft || !hasRight) && hasWidth )
	{
		if ( hasRight )
		{
			r.UpperLeftCorner.X = temp_parent_pos.getWidth() - right - width;
		} else {
			r.LowerRightCorner.X = left + width;
		}
	}
	if ( (!hasTop || !hasBottom) && hasHeight )
	{
		if ( hasBottom )
		{
			r.UpperLeftCorner.Y = temp_parent_pos.getHeight() - bottom - height;
		} else {
			r.LowerRightCorner.Y = top + height;			
		}
	}

	return r;
}
