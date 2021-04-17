/*
Project name:
CSS Library

Created by Nicolaus Anderson, 2012
see CSSLibrary.h for terms and conditions.

Implementation of some important functions.
*/

#include "CSSTagData.h"


void CSSTagData::finalizeParameters(
			irr::gui::IGUIElement * parent
			)
{
	// default = showing the element
	if ( !existsAttribute( "Visible" ) )
		addBool( "Visible", true );
		

	// ensure that this parent's parameters are applied appropriately for this element
	if ( parent_tag != 0 )
		parent_tag->finalizeParameters( parent );


	// position *****************************************


	irr::core::recti frame					= createBoundedRect( parent );
	irr::core::vector2di position_shift;
	HTMLPos position						= (HTMLPos) getAttributeAsEnumeration(
															"position", HTMLPosTypes
															);

	/* Check if this has a static or relative position, which require the tedious
	task of finding the positions of the last added child GUI element (to this
	element's parent) that is positioned likewise. */
	if ( position == HTM_POS_relative || position == HTM_POS_static )
	{
		/* Find the last child of the parent element positioned by "relative" or "static".
		This child must be of type HTMLPositioned for its position to be identified. */
		irr::core::list<irr::gui::IGUIElement*>::ConstIterator it = parent->getChildren().getLast();
		--it; // skip this gui element - we only care about the other children

		for ( irr::u32 index = 0; index < parent->getChildren().size() - 1; ++index)
		{
			if (
				((divBox*)*it)->getPosition() == HTM_POS_static
				|| ((divBox*)*it)->getPosition() == HTM_POS_relative
				)
			{
				position_shift =
					(*it)->getAbsolutePosition().LowerRightCorner
					-
					parent->getAbsolutePosition().UpperLeftCorner;

				// now determine the shift type based on the position type of this element
				if ( position == HTM_POS_static )
				{
					frame += irr::core::vector2di( 0, position_shift.Y );
				} else {
					frame += position_shift;
				}

				break;
			}

			--it; // examine the next child
		}
	}

	// finalize the position
	setAttribute( "Rect", frame );

	if ( existsAttribute("left") )
		setAttribute( "LeftAlign", "upperLeft", irr::gui::GUIAlignmentNames );
	else
		setAttribute( "LeftAlign", "lowerRight", irr::gui::GUIAlignmentNames );

	if ( existsAttribute("right") )
		setAttribute( "RightAlign", "lowerRight", irr::gui::GUIAlignmentNames );
	else
		setAttribute( "RightAlign", "upperLeft", irr::gui::GUIAlignmentNames );

	if ( existsAttribute("top") )
		setAttribute( "TopAlign", "upperLeft", irr::gui::GUIAlignmentNames );
	else
		setAttribute( "TopAlign", "lowerRight", irr::gui::GUIAlignmentNames );

	if ( existsAttribute("bottom") )
		setAttribute( "BottomAlign", "lowerRight", irr::gui::GUIAlignmentNames );
	else
		setAttribute( "BottomAlign", "upperLeft", irr::gui::GUIAlignmentNames );
}


void CSSTagData::buildStyleFromGUIElement( irr::gui::IGUIElement * element )
{
	// serialize only attributes common to all GUI elements
	// (automatically done because the element has been casted to the basic type)
	element->serializeAttributes( this );

	/* NOTE: Given that AlignLeft, AlignRight, AlignTop, and AlignBottom may be
	set to upperLEFT, lowerRIGHT, center, or scale, all four of these must be
	taken into account.
	
	Currently, only upperLeft is taken into account. */

	irr::core::recti boundbox = element->getAbsolutePosition();

	setAttribute( "left", boundbox.UpperLeftCorner.X );
	setAttribute( "right", boundbox.LowerRightCorner.X );
	setAttribute( "top", boundbox.UpperLeftCorner.Y );
	setAttribute( "bottom", boundbox.LowerRightCorner.Y );
}