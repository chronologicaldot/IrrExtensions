/*
Project name:
CSS Library

Created by Nicolaus Anderson, 2012
see CSSLibrary.h for terms and conditions.
*/

#include <irrlicht.h>

using namespace irr::io;

class GUIElemAttributes
{
public:

	irr::core::stringc name;
	irr::core::stringc value;

	E_ATTRIBUTE_TYPE type;

	irr::core::stringw type_string;

	GUIElemAttributes( irr::core::stringc new_name )
	{
		name = new_name;
		type = EAT_UNKNOWN;

		type_string = L"unknown";
	}
};