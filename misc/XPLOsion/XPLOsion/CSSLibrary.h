/*
Project name:
CSS Library

Created by Nicolaus Anderson, 2012

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Since this uses the irrlicht engine, see irrlicht.h for more details.

Purpose:
To allow you to design your program using a type of xml format
similar to html and style it like CSS.
*/

#include <irrlicht.h>
#include <LinusListix.h>
#include "WebTypes.h"
#include "CSSTagData.h"
#include <irrVFreader.cpp>

#pragma once


//! Class CSS Library
/* Capable of loading and interpreting CSS data in properly formatted files.
Note that not all data will apply to the GUI interface based on certain
restrictions on the GUI elements themselves. */
class CSSLibrary : public IReferenceCounted
{
	//! Irrlicht Device pointer
	/* Necessary for certain things, and also needed if I decide to have this
	also handle 3D elements and such and not just GUI things. */
	irr::IrrlichtDevice * irrdev;

	//! CSS tag list
	/* Contains the tags that hold the CSS information. */
	Linus::LListix<CSSTagData*> tags;

	//! System characters
	/* These are the types of characters that are interpreted as being
	individual tokens. */
	irr::core::stringc syschars;
	irr::c8 * system_chars;
	irr::s32 _maxsystemchars; // total number of system characters

	irr::core::stringc bodysyschars; /* Because the body cares about spaces */

public:

	//! Constructor
	/**/
	CSSLibrary( irr::IrrlichtDevice * device )
	{
		irrdev = device;

		syschars = irr::core::stringc( " \t\n\r.#{}:;/*\"" );
		bodysyschars = irr::core::stringc( "\n\r.#{}:;/*\"" );

		system_chars = (irr::c8 *)(syschars.c_str());
		_maxsystemchars = (irr::s32)(syschars.size());
	}

	//! Destructor
	/**/
	~CSSLibrary()
	{
		tags.clear();
	}

	//! Add style
	/* Adds an empty style to the list of available styles. */
	void addStyle()
	{
		tags.push_back( new CSSTagData( irrdev->getVideoDriver() ) );
		(*(tags.itemAt(-1)))->setTagName( irr::core::stringc(tags.Size()) );
	}

	//! Add style
	/* Adds a style to the list of available styles. */
	//void addStyle( CSSTagData style )
	//{
	//	tags.push_back( style );
	//}

	//! Add style from string
	void addStyle(
		irr::core::stringc styletext,
		irr::core::stringc stylename="",
		CSSTagType styletype=CSSTYPE_NEITHER
		)
	{
		tags.push_back( interpretStyling(styletext) );

		if ( stylename.size() == 0 )
		{
			stylename = irr::core::stringc(tags.Size());
		}
		(*(tags.itemAt(-1)))->setTagName( stylename );

		(*(tags.itemAt(-1)))->setType( styletype );
	}

	//! Add style and return
	/* Adds a style to the list of available styles and returns a
	pointer to it. */
	CSSTagData* addStyleAndGet()
	{
		tags.push_back( new CSSTagData( irrdev->getVideoDriver() ) );
		return (*(tags.itemAt(-1)));
	}

	//! Load and Parse CSS File
	/* Attempts to load a CSS file into the memory bank	and parse it.
	Returns true for a successful loading and parsing. */
	bool ParseCSSFile( irr::io::path filename );
	
	//! Load and Parse CSS File
	/* Attempts to load a CSS file into the memory bank	and parse it.
	Returns true for a successful loading and parsing. */
	bool ParseCSSFile( irr::io::IReadFile * file );

	//! Get CSS Style
	/* Returns a pointer to the CSS style in the list based on the
	given index. NOTE: There is no out-of-bounds checking, but
	negative indices are allowed. */
	CSSTagData* getCSSStyle( irr::s32 index )
	{
		return (CSSTagData*)(*(tags.itemAt( (si32)index )));
	}

	//! Find CSS class
	/* Searches for a CSS class whose name matches the given.
	If a match is found, it returns the index of that CSS class. */
	irr::s32 findCSSClass( irr::core::stringc name );

	//! Find CSS ID
	/* Searches for a CSS ID whose name matches the given.
	If a match is found, it returns the index of that CSS ID. */
	irr::s32 findCSSID( irr::core::stringc name );

	//! Find CSS random
	/* Searches for a CSS tag whose name matches the given. */
	irr::s32 findCSSRandom( irr::core::stringc name );

	//! Apply styling to a GUI element
	/* Attempts to apply a particular CSS style to the GUI element. */
	void applyStyleToGUIElement(
		irr::gui::IGUIElement * element,
		CSSTagData * css
		);

	//! Apply styling to a GUI element
	/* Attempts to apply a style type whose name matches the given and whose
	type is either "class", "id", or "random" (any). Default is random. */
	void applyStyleToGUIElement(
		irr::gui::IGUIElement * element,
		irr::core::stringc stylename,
		irr::core::stringc styletype="random"
		);

	//! Build style from GUI element
	/* Attempts to create a style from the given GUI element.
	NOTE: It is assumed the element has had its absolute position calculated. */
	CSSTagData* buildStyleFromGUIElement(
		irr::gui::IGUIElement * element, irr::core::stringc stylename=""
		);

	//! Interpret styling
	/* Takes a string that contains the interior of a CSS tag body
	and creates a CSS tag out of it. It then returns the tag so it
	can either be stored or used once (for example, applied to a
	single division). */
	CSSTagData* interpretStyling( irr::core::stringc styletext );

	//! Apply inline styling
	/* Takes a string containing styling information, generates the
	style, and applies the style to the passed in GUI element. */
	void applyInlineStyling(
		irr::gui::IGUIElement * element,
		irr::core::stringc styletext
		);

protected:

	//! Translate string into color
	/* Interprets a string as a color. Accepted English words that give
	color values are: "white", "black", "gray", "red", "blue", "green". */
	irr::video::SColor interpretColor( irr::core::stringc word );

	//! Interpret color value
	/* Accepts two characters that indicate a color value (red, green, or blue)
	for an HTML color. Returns the resulting color value. Should be used to set
	red, green, or blue in SColor. */
	irr::u32 getColorValue( irr::c8 val1, irr::c8 val2 );

	//! Get rectangle from string
	/* Separates the given string into four values and attempts to make a
	rectangle out of it. The values must be in the following order and
	separated by spaces: left, top, right, bottom. */
	irr::core::recti getRectFromString( irr::core::stringc word );

	//! Get position from string
	/* Separates a string into two values and attempts to make a position2d
	out of it. The values must be in the following order and separated by
	spaces: left, top. */
	irr::core::position2di getPositionFromString( irr::core::stringc word );
};