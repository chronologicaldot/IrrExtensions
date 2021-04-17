/*
Project name:
CSS Library

Functions based on those (C) Nikolaus Gebhardt
Edited by Nicolaus Anderson, 2012
see irrlicht.h for terms and conditions.

Overriding the usual IAttributes functions with duplicates
that contain slight modifications for our purposes.
that call our getAttributeP() function.
*/

#include "CSSTagData.h"
#include "CAttributeImpl.h"


using namespace irr;
using namespace io;


IAttribute* CSSTagData::getAttributeP(const c8* attributeName) const
{
	for (irr::u32 i=0; i<Attributes.size(); ++i)
		if (Attributes[i]->Name == attributeName)
			return Attributes[i];

	// Added for our purposes:
	// Now search the parent's attributes if the parent exists
	if ( parent_tag != 0 )
		return parent_tag->getAttributeP( attributeName );

	return 0;
}

//! Gets a string attribute.
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setStringAttribute()
//! or 0 if attribute is not set.
core::stringc CSSTagData::getAttributeAsString(const c8* attributeName) const
{
	core::stringc str;

	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getString();
	else
		return str;
}

//! Gets a string attribute.
//! \param attributeName: Name of the attribute to get.
//! \param target: Buffer where the string is copied to.
void CSSTagData::getAttributeAsString(const c8* attributeName, char* target) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
	{
		core::stringc str = att->getString();
		strcpy(target,str.c_str());
	}
	else
		target[0] = 0;
}

//! Gets a string attribute.
//! \param attributeName: Name of the attribute to get.
//! \param target: Buffer where the string is copied to.
void CSSTagData::getAttributeAsStringW(const c8* attributeName, wchar_t* target) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
	{
		core::stringw str = att->getStringW();
		wcscpy(target,str.c_str());
	}
	else
		target[0] = 0;
}

//! Gets an attribute as an array of wide strings.
core::array<core::stringw> CSSTagData::getAttributeAsArray(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getArray();
	else
		return core::array<core::stringw>();
}

//! Gets a attribute as boolean value
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute() as bool
//! or 0 if attribute is not set.
bool CSSTagData::getAttributeAsBool(const c8* attributeName) const
{
	bool ret = false;

	IAttribute* att = getAttributeP(attributeName);
	if (att)
		ret = att->getBool();

	return ret;
}

//! Gets a attribute as integer value
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute() as integer
//! or 0 if attribute is not set.
s32 CSSTagData::getAttributeAsInt(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getInt();
	else
		return 0;
}

//! Gets a attribute as integer value
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute() as float value
//! or 0 if attribute is not set.
f32 CSSTagData::getAttributeAsFloat(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getFloat();

	return 0.f;
}

//! Gets an attribute as color
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
video::SColor CSSTagData::getAttributeAsColor(const c8* attributeName) const
{
	video::SColor ret(0);

	IAttribute* att = getAttributeP(attributeName);
	if (att)
		ret = att->getColor();

	return ret;
}

//! Gets an attribute as floating point color
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
video::SColorf CSSTagData::getAttributeAsColorf(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getColorf();
	else
		return video::SColorf();
}

//! Gets an attribute as enumeration
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
const char* CSSTagData::getAttributeAsEnumeration(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getEnum();
	else
		return 0;
}

//! Gets an attribute as enumeration
s32 CSSTagData::getAttributeAsEnumeration(const c8* attributeName, const char* const* enumerationLiteralsToUse) const
{
	IAttribute* att = getAttributeP(attributeName);

	if (enumerationLiteralsToUse && att)
	{
		const char* value = att->getEnum();
		if (value)
		{
			for (s32 i=0; enumerationLiteralsToUse[i]; ++i)
				if (!strcmp(value, enumerationLiteralsToUse[i]))
					return i;
		}
	}

	return -1;
}

//! Gets the list of enumeration literals of an enumeration attribute
//! \param attributeName: Name of the attribute to get.
void CSSTagData::getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, core::array<core::stringc>& outLiterals) const
{
	IAttribute* att = getAttributeP(attributeName);

	if (att && att->getType() == EAT_ENUM)
		outLiterals = ((irr::io::CEnumAttribute*)att)->EnumLiterals;
}

//! Gets an attribute as rectangle
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
core::rect<s32> CSSTagData::getAttributeAsRect(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getRect();
	else
		return core::rect<s32>();
}

//! Gets an attribute as 2d position
//! \param attributeName: Name of the attribute to get.
//! \return Returns value of the attribute previously set by setAttribute()
core::position2di CSSTagData::getAttributeAsPosition2d(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getPosition();
	else
		return core::position2di();
}

//! Gets an attribute as texture reference
//! \param attributeName: Name of the attribute to get.
video::ITexture* CSSTagData::getAttributeAsTexture(const c8* attributeName) const
{
	IAttribute* att = getAttributeP(attributeName);
	if (att)
		return att->getTexture();
	else
		return 0;
}


/* NOTE:

For those wishing to implement this for loading models (etc.) in a CSS manner:
The following functions need to be appended to this list:

(beginning on line 432 of CAttributes.cpp)

core::vector3df CSSTagData::getAttributeAsVector3d(const c8* attributeName)
void CSSTagData::getAttributeAsBinaryData(const c8* attributeName, void* outData, s32 maxSizeInBytes)
E_ATTRIBUTE_TYPE CSSTagData::getAttributeType(const c8* attributeName)
const wchar_t* CSSTagData::getAttributeTypeString(const c8* attributeName)

(beginning on line 946 of CAttributes.cpp)

core::matrix4 CSSTagData::getAttributeAsMatrix(const c8* attributeName)
core::quaternion CSSTagData::getAttributeAsQuaternion(const c8* attributeName)
core::aabbox3df CSSTagData::getAttributeAsBox3d(const c8* attributeName)
core::plane3df CSSTagData::getAttributeAsPlane3d(const c8* attributeName)
core::triangle3df CSSTagData::getAttributeAsTriangle3d(const c8* attributeName)
core::line2df CSSTagData::getAttributeAsLine2d(const c8* attributeName)
core::line3df CSSTagData::getAttributeAsLine3d(const c8* attributeName)
void* CSSTagData::getAttributeAsUserPointer(const c8* attributeName)


All that is required is copying and pasting these functions (and the aforemented,
slightly-modified headers) into this file and the prototypes into CSSTagData.h
*/
