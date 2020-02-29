/*
Project name:
CSS Library

Created by Nicolaus Anderson, 2012
see CSSLibrary.h for terms and conditions.

Implementation of the functions.
*/

#include "CSSLibrary.h"


void CSSLibrary::applyStyleToGUIElement(
		irr::gui::IGUIElement * element,
		CSSTagData * css
		)
{
	// Checking for non-absolute positioning
	HTMLPos pos = (HTMLPos) css->getAttributeAsEnumeration( "position", HTMLPosTypes );

	// Obtain the immediate parent based on type
	irr::gui::IGUIElement * immediate_parent;

	if ( pos == HTM_POS_fixed || element->getParent() == 0 )
	{
		immediate_parent = irrdev->getGUIEnvironment()->getRootGUIElement();
	} else {
		immediate_parent = element->getParent();
	}

	/* Form the CSS data for it to apply to this GUI element based on its positioning*/
	css->finalizeParameters( immediate_parent );


	css->applyToGUIElement( element );
}

void CSSLibrary::applyStyleToGUIElement(
		irr::gui::IGUIElement * element,
		irr::core::stringc stylename,
		irr::core::stringc styletype
		)
{
	// cannot apply a style with no name
	if ( stylename == "" )
		return;

	irr::s32 index; // index of the style in the list of styles

	/* The type name string is made lower so that capitalization is
	irrelevant in checking the type. */
	styletype.make_lower();

	// decide how te get the index based on the type of style wanted
	if ( styletype == "class" )
		index = findCSSClass( stylename );
	else if ( styletype == "id" )
		index = findCSSID( stylename );
	else
		index = findCSSRandom( stylename );

	// break if the style is unavailable
	if ( index == -1 )
		return;

	// apply the style
	applyStyleToGUIElement( element, getCSSStyle( index ) );
}


CSSTagData* CSSLibrary::buildStyleFromGUIElement(
	irr::gui::IGUIElement * element,
	irr::core::stringc stylename
	)
{
	CSSTagData * style = new CSSTagData( irrdev->getVideoDriver() );

	style->setTagName( stylename );

	style->buildStyleFromGUIElement( element );

	return style;
}



CSSTagData* CSSLibrary::interpretStyling(irr::core::stringc styletext )
{
	// return tag
	CSSTagData* tag = new CSSTagData( irrdev->getVideoDriver() );

	// Last position in the string to be read from
	irr::s32 pos = 0;

	// Last character to be grabbed from the string
	irr::c8 chr;

	// Last word seen in the string
	irr::core::stringc word;

	// Last seen attribute name
	irr::core::stringc attr;

	// flag indicating whether an attribute or its value is being searched for
	// "true" == attribute
	bool attrORval = true;


	// While the entirety of the string has not been searched
	while ( pos != (irr::s32)styletext.size() )
	{
		chr = styletext[pos];


		if ( styletext.subString(pos,2) == "/*" ) // comment found
		{
			do {
				pos++;
			} while ( styletext.subString(pos,2) != "*/" );
			pos++;
		}
		else if ( chr == ':' ) // break point, leading to a value
		{
			word.trim();

			attr = word; // saving the word as the name of the attribute

			attrORval = false; // now searching for a value instead of an attribute name
			word= "";
		}
		else if ( chr == ';' )
		{
			word.trim();

			// ADD ATTRIBUTE BEGIN ***************************


			/* Integer attributes followed by "px" */
			if (
				attr == "left"
				|| attr == "right"
				|| attr == "top"
				|| attr == "bottom"
				|| attr == "height"
				|| attr == "width"

				|| attr == "SliderWidth"
				)
			{
				word.remove( "px" );
				tag->setAttribute( attr.c_str(), (irr::s32)atoi(word.c_str()) );
			}


			/* Integer attributes */
			else if (
				attr == "Id"
				|| attr == "ActiveTab"
				|| attr == "CloseHandling"
				|| attr == "DecimalPlaces"
				|| attr == "ItemCount"
				|| attr == "LargeStep"
				|| attr == "Max"
				|| attr == "MaxChars"
				|| attr == "Min"
				|| attr == "ParentItem"
				|| attr == "SmallStep"
				|| attr == "Selected"
				|| attr == "TabHeight"
				|| attr == "TabMaxWidth"
				|| attr == "TabNumber"
				|| attr == "TabOrder"
				|| attr == "Value"

				|| attr.equalsn( irr::core::stringc("CommandID"), 9 )
				)
			{
				tag->setAttribute( attr.c_str(), atoi( word.c_str() ) );
			}


			/* Floating point number attributes */
			else if (
				attr == "Step"
				)
			{
				tag->setAttribute( attr.c_str(), (irr::f32)atof( word.c_str() ) );
			}


			/* Boolean attributes */
			else if (
				attr == "AutoScroll"
				|| attr == "Background"
				|| attr == "Border"
				|| attr == "CancelButton"
				|| attr == "DrawBackground"
				|| attr == "DrawBack"
				|| attr == "DrawTitlebar"
				|| attr == "FillBackground"
				|| attr == "Horizontal"
				|| attr == "IsCloseVisible"
				|| attr == "IsDraggable"
				|| attr == "IsMinVisible"
				|| attr == "IsRestoreVisible"
				|| attr == "MoveOverSelect"
				|| attr == "MultiLine"
				|| attr == "NoButton"
				|| attr == "NoClip"
				|| attr == "OkayButton"
				|| attr == "OverrideBGColorEnabled"
				|| attr == "OverrideColorEnabled"
				|| attr == "OverrideTextColorEnabled"
				|| attr == "PasswordBox"
				|| attr == "Pressed"
				|| attr == "PushButton"
				|| attr == "ScaleImage"
				|| attr == "ShowSmallTicks"
				|| attr == "ShowLargeTicks"
				|| attr == "TabStop"
				|| attr == "TabGroup"
				|| attr == "UseAlphaChannel"
				|| attr == "Visible"
				|| attr == "WordWrap"
				|| attr == "YesButton"

				|| attr.equalsn( irr::core::stringc("AutoChecking"), 11 )
				|| attr.equalsn( irr::core::stringc("Checked"), 7 )
				|| attr.equalsn( irr::core::stringc("Enabled"), 7 )
				|| attr.equalsn( irr::core::stringc("IsSeparator"), 11 )
				|| attr.equalsn( irr::core::stringc("UseColText"), 10 )
				|| attr.equalsn( irr::core::stringc("UseColIcon"), 10 )
				)
			{
				tag->setAttribute( attr.c_str(), (word == "true") );
			}


			/* String attributes */
			else if (
				attr == "Caption"
				|| attr == "PasswordChar"
				|| attr == "MessageText"
				|| attr == "Name" // Added with irrlicht 1.8

				|| attr.equalsn( irr::core::stringc("Text"), 4 )
				|| attr.equalsn( irr::core::stringc("text"), 4 )

				|| ( attr.equalsn( irr::core::stringc("Item"), 4 )
					&& attr.find( "Text", 0 ) != -1 )
				)
			{
				//word.trim("\""); // in case someone thought it was necessary to add quotes
				tag->setAttribute( attr.c_str(), word.c_str() );
			}


			/* Color attributes */
			else if (
				attr == "background-color"
				|| attr == "BackColor"
				|| attr == "BGColor"
				|| attr == "FullColor"
				|| attr == "OverrideColor"
				|| attr == "TransColor"
				|| attr == "TextColor"
				|| attr == "TickColor"

				|| attr.equalsn( irr::core::stringc("ColIcon"), 7 )
				|| attr.equalsn( irr::core::stringc("ColText"), 7 )
				)
			{
				tag->setAttribute( attr.c_str(), interpretColor( word ) );
			}


			/* Texture attributes */
			else if (
				attr == "background-image"
				|| attr == "Image"
				|| attr == "PressedImage"
				|| attr == "Texture"
				)
			{
				/* In case someone thought quotation marks were necessary,
				remove them. */
				word.trim( "\"" );

				tag->setAttribute( attr.c_str(), irrdev->getVideoDriver()->getTexture( word ) );

				// We need this rectangle if it isn't provided
				if (( attr == "Image" || attr == "PressedImage" )
					&& !tag->existsAttribute( (attr+"Rect").c_str() )
					)
				{
					/* We're going to cheat and pass in an invalid rectangle
					so that the GUI element won't use it. */
					tag->addRect( (attr+"Rect").c_str(),
								core::recti(0,0,-1,1)
								);
				}
			}


			/* Ordinary rectangles */
			else if (
				attr == "ImageRect"
				|| attr == "PressedImageRect"
				)
			{
				tag->setAttribute( attr.c_str(), getRectFromString(word) );
			}


			/* Enumerations */
			else if ( attr == "position" )
			{
				tag->setAttribute( attr.c_str(), word.c_str(), HTMLPosTypes );
			}
			else if ( attr == "background-repeat" )
			{
				tag->setAttribute( attr.c_str(), word.c_str(), HTMLBackgroundImageRepeatTypes );
			}
			else if ( attr == "overflow" )
			{
				if ( word == "show" ) // For backwards compatibility
					word = "visible";

				tag->setAttribute( attr.c_str(), word.c_str(), WebOverflowTypes );
			}
			else if (
				attr == "HTextAlign"
				|| attr == "VTextAlign"
				|| attr == "TabVerticalAlignment"
				)
			{
				tag->setAttribute( attr.c_str(), word.c_str(), irr::gui::GUIAlignmentNames );
			}


			/* Miscellaneous */

			else if ( attr == "Rect" )
			{
				/* For this, we split the rectangle into left, right, top, and bottom
				attributes so as not to create overriding conflicts when we call
				CSSTagData::finalizeParameters() (because we won't be checking for
				an existing rectangle which may or may not need to be overridden). */

				irr::core::recti temp = getRectFromString(word);

				tag->setAttribute( "left", temp.UpperLeftCorner.X );
				tag->setAttribute( "right", temp.LowerRightCorner.X );
				tag->setAttribute( "top", temp.UpperLeftCorner.Y );
				tag->setAttribute( "bottom", temp.LowerRightCorner.Y );
			}

			else if ( attr == "Position" )
			{
				tag->setAttribute( attr.c_str(), getPositionFromString(word) );
			}


			/*********** OTHER TYPES **********
			Here you can add attribute loaders for other types of GUI elements.
			How this works: CSS attributes are stored in CSS tags. In this function,
			we are creaing a tag, which is stored in the pointer "tag".
			Typical design for loading an attribute is tag->setAttribute().
			The function has several overloads for handling different types.

			// for integers:
			tag->setAttribute( c8* name, irr::s32 value );

			// for colors:
			tag->setAttribute( c8* name, irr::video::SColor value );
			// Note: To obtain colors from a string, we use the function
			// CSSLibrary::interpretColor( irr::core::stringc );

			// for enumerations:
			tag->setAttribute( c8* name, c8* value, irr::s32 enumeration_literals );
			// See examples above for how to do this.

			// for rectangles:
			tag->setAttribute( c8* name, irr::core::recti value );
			// Note: To obtain a rectangle from a string, we use the function
			// CSSLibrary::getRectFromString( irr::core::stringc );
			*/

			// else if ( attr == my_css_attribute_name )
			//{

			//}


			// ADD ATTRIBUTE END *************************************

			attrORval = true; // now looking for attribute name instead of its value
			word = "";
		}
		else
		{
			word.append( chr );
		}

		pos++;
	}

	return tag;
}


void CSSLibrary::applyInlineStyling(
			irr::gui::IGUIElement * box,
			irr::core::stringc styletext
			)
{
	CSSTagData * style;

	style = interpretStyling( styletext );

	applyStyleToGUIElement( box, style );

	delete style;
}


irr::video::SColor CSSLibrary::interpretColor( irr::core::stringc word )
{
	// Check first if this is an HTML color - this should be the fastest color input
	if ( word[0] == '#' )
	{
		irr::video::SColor color( 255, 0, 0, 0);

		// programmer forgot to provide all of the needed color values
		if ( word.size() < 7 )
			return color;

		// Each two letters indicate a value for the color
		color.setRed( getColorValue( word[1], word[2] ) );
		color.setGreen( getColorValue( word[3], word[4] ) );
		color.setBlue( getColorValue( word[5], word[6] ) );

		return color;
	}

	// Check second to see if four numeric values were given for the color
	irr::core::array<irr::core::stringc> components;
	word.split( components, " ", 1 );

	if ( components.size() == 4 && atoi(components[0].c_str()) != 0 )
	{
		return irr::video::SColor(
			atoi( components[0].c_str() ),
			atoi( components[1].c_str() ),
			atoi( components[2].c_str() ),
			atoi( components[3].c_str() )
			);
	}

	// Check for word descriptions of colors
	if ( word == "clear" || word == "transparent" )
	{
		return irr::video::SColor( 0, 0, 0, 0 );
	}
	else if ( word == "white" )
	{
		return irr::video::SColor( 255, 255, 255, 255 );
	}
	else if ( word == "black" )
	{
		return irr::video::SColor( 255, 0, 0, 0 );
	}
	else if ( word == "gray" )
	{
		return irr::video::SColor( 255, 125, 125, 125 );
	}
	else if ( word == "red" )
	{
		return irr::video::SColor( 255, 255, 0, 0 );
	}
	else if ( word == "blue" )
	{
		return irr::video::SColor( 255, 0, 0, 255 );
	}
	else if ( word == "green" )
	{
		return irr::video::SColor( 255, 0, 200, 0 );
	}
	else if ( word == "pink" )
	{
		return irr::video::SColor( 255, 255, 0, 255 );
	}
	else if ( word == "yellow" )
	{
		return irr::video::SColor( 255, 255, 255, 0 );
	}
	else if ( word == "orange" )
	{
		return irr::video::SColor( 255, 255, 142, 0 );
	}
	else if ( word == "purple" )
	{
		return irr::video::SColor( 255, 125, 0, 255 );
	}
	else if ( word == "maroon" )
	{
		return irr::video::SColor( 255, 136, 30, 156 );
	}
	else if ( word == "light pink" )
	{
		return irr::video::SColor( 255, 255, 125, 255 );
	}
	else if ( word == "sky blue" )
	{
		return irr::video::SColor( 255, 0, 220, 255 );
	}
	else if ( word == "light blue" )
	{
		return irr::video::SColor( 255, 255, 125, 125 );
	}
	else if ( word == "light green" || word == "lime" )
	{
		return irr::video::SColor( 255, 0, 255, 0 );
	}
	else if ( word == "medium blue" )
	{
		return irr::video::SColor( 255, 0, 118, 255 );
	}
	//else
	//{
	// ?
	//}

	// default return transparent white
	return irr::video::SColor( 0, 255, 255, 255 );
}


irr::u32 CSSLibrary::getColorValue(irr::c8 val1, irr::c8 val2)
{
	irr::s32 n1 = (irr::s32) (val1 - 'a');
	irr::s32 n2 = (irr::s32) (val2 - 'a');

	if ( n1 >= 0 )
		n1 += 10;
	else
		if ( (irr::s32) val1 == '0' )
			n1 = 0;
		else
			n1 = (irr::s32)(val1 - '1') + 1;

	if ( n2 >= 0 )
		n2 += 10;
	else
		if ( (irr::s32) val2 == '0' )
			n2 = 0;
		else
			n2 = (irr::s32)(val2 - '1') + 1;

	n1 *= 16;

	return (irr::u32)(n1 + n2);
}


irr::core::recti CSSLibrary::getRectFromString(irr::core::stringc word)
{
	irr::core::array<irr::core::stringc> components(4);
	word.split( components, " ", 1 );

	if ( components.size() == 4 )
	{
		return irr::core::recti(
			atoi( components[0].c_str() ), // left
			atoi( components[1].c_str() ), // top
			atoi( components[2].c_str() ), // right (or left + width)
			atoi( components[3].c_str() ) // bottom (or top + height)
			);
	}

	return irr::core::recti( 0, 0, 100, 100 );
}


irr::core::position2di CSSLibrary::getPositionFromString( irr::core::stringc word )
{
	irr::core::array<irr::core::stringc> components(2);
	word.split( components, " ", 1 );

	if ( components.size() == 2 )
	{
		return
			irr::core::position2di(
				atoi( components[0].c_str() ), // left
				atoi( components[1].c_str() ) // top
				);
	}
	else if ( components.size() == 1 )
	{
		return
			irr::core::position2di(
				atoi( components[0].c_str() ) // left and top
				);
	}

	return irr::core::position2di( 0 );
}
