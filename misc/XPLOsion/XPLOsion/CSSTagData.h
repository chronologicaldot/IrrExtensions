/*
Project name:
CSS Library

Created by Nicolaus Anderson, 2012
see CSSLibrary.h for terms and conditions.
*/

#include <irrlicht.h>
#include "WebTypes.h"
#include <CAttributes.h>
//#include <CAttributeImpl.h>
#include "sdiv.h"

#pragma once


//! CSS tag type
/* Whether the is CSS data belongs to a class, an id, or neither */
enum CSSTagType
{
	CSSTYPE_NEITHER=0,
	CSSTYPE_CLASS,
	CSSTYPE_ID,
	CSSTYPE_COUNT // indicates how many types there are
};


//! Class CSS Tag Data
/* Contains data concerning a CSS tag.

The intention here is to take advantage of the engine's built-in
serialization and deserialization aspects that happen to expose
and change the internals of GUI elements.

We can then modify these elements in a CSS manner that maintains simplicity
in assigning parameters while allowing us to organize our programs using
an XML format similar to HTML.
*/
class CSSTagData : public irr::io::CAttributes
{
	// Typical CSS data --------------------------------
	CSSTagType css_type;
	irr::core::stringc name;

	CSSTagData * parent_tag; /* Only global for simplification of code.
							 Applicable only in relative cases: when the
							 element has been nested.
							 e.g. If only "left" is given in this tag
							 and "right", "width", and "height" are given
							 in the tag of the parent GUI element. */

	// end typical CSS data ----------------------------

public:

	// exposed CSS data

	irr::gui::IGUIFont * font;


		//! Constructor
	/* Note that this assigns default parameters to the CSS style. */
	CSSTagData( irr::video::IVideoDriver * driver=0 ) : CAttributes( driver )
	{
		css_type = CSSTYPE_NEITHER;
		name = "";

		parent_tag = 0;
	}

		//! Set tag name
	/* The name of this tag, NOT the name of an attribute. */
	void setTagName( irr::core::stringc tag_name )
	{ name = tag_name; }

		//! Get tag name
	/* The name of this tag, NOT the name of an attribute. */
	irr::core::stringc getTagName()
	{ return name; }

	void setType( CSSTagType type )
	{ css_type = type; }

	CSSTagType getType()
	{ return css_type; }

	bool isNothing()
	{ return ( CSSTYPE_NEITHER == css_type ); }

	bool isClass()
	{ return ( CSSTYPE_CLASS == css_type ); }

	bool isID()
	{ return ( CSSTYPE_ID == css_type ); }

		//! Set the parent tag
	/* Sets the tag from which data will be inherited if it cannot
	be provided by this tag. */
	void setParentTag( CSSTagData * tag )
	{
		parent_tag = 0;
		parent_tag = tag;
	}

		//! Get parent tag
	/* Gets a pointer to the parent CSS tag. */
	CSSTagData* getParentTag()
	{
		return parent_tag;
	}

		//! Create bounded rectangle
	/* Creates a rectangle that obeys this style and is placed with
	respect to the given parent.
	\param parent - The IGUIElement parent of this element.
	*/
	irr::core::recti createBoundedRect( irr::gui::IGUIElement * parent );

		//! Finalize creation
	/* Uses the given information to generate the necessary different types
	of parameters. USE AFTER ASSIGNING DATA TO A CLASS INSTANCE OF THIS TYPE!
	\param parent - The parent GUI element of the element the CSS data is going
	to be applied to. This is necessary in oredr to fit the element within its
	parent's borders using "left", "right", "top", and "bottom".
	\param position_shift - Offset placement of the GUI element despite its
	designed location based on "left", "right", "top", and "bottom". */
	void finalizeParameters(
		irr::gui::IGUIElement * parent
		);


	/* *******************************************************
	The following functions are meant to be called for applying
	the	style to the GUI element or creating a style from it.
	*/

		//! Apply to GUI element
	/* Accepts an element and attempts to apply the style to it.
	USE AFTER finalizeParameters() */
	void applyToGUIElement( irr::gui::IGUIElement * element )
	{
		element->deserializeAttributes(this);
	}

		//! Create a style from the GUI element
	/* Creates a style from the parameters of the given GUI element,
	trying to create a style that mimics its behavior. */
	void buildStyleFromGUIElement( irr::gui::IGUIElement * element );


	/* *******************************************************
	OVERRIDE FUNCTIONS
	These allow us to use the parent's attributes if this CSS tag cannot
	provide them.
	*/
	virtual bool existsAttribute( const irr::c8* attributeName ) const
	{
		return getAttributeP( attributeName ) != 0;
	}
	virtual irr::io::IAttribute* getAttributeP(const irr::c8* attributeName) const;

	virtual irr::core::stringc getAttributeAsString(const irr::c8* attributeName) const;
	virtual void getAttributeAsString(const irr::c8* attributeName, char* target) const;
	virtual void getAttributeAsStringW(const irr::c8* attributeName, wchar_t* target) const;
	virtual irr::core::array<irr::core::stringw> getAttributeAsArray(const irr::c8* attributeName) const;
	virtual bool getAttributeAsBool(const irr::c8* attributeName) const;
	virtual irr::s32 getAttributeAsInt(const irr::c8* attributeName) const;
	virtual irr::f32 getAttributeAsFloat(const irr::c8* attributeName) const;
	virtual irr::video::SColor getAttributeAsColor(const irr::c8* attributeName) const;
	virtual irr::video::SColorf getAttributeAsColorf(const irr::c8* attributeName) const;

	virtual const char* getAttributeAsEnumeration(const irr::c8* attributeName) const;

	virtual irr::s32 getAttributeAsEnumeration(
		const irr::c8* attributeName, const char* const* enumerationLiteralsToUse
		) const;

	virtual void getAttributeEnumerationLiteralsOfEnumeration(
		const irr::c8* attributeName, irr::core::array<irr::core::stringc>& outLiterals
		) const;

	virtual irr::core::rect<irr::s32> getAttributeAsRect(const irr::c8* attributeName) const;
	virtual irr::core::position2di getAttributeAsPosition2d(const irr::c8* attributeName) const;

	virtual irr::video::ITexture* getAttributeAsTexture(const irr::c8* attributeName) const;
};
