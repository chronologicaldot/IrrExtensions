/*
(c) 2012 Nic Anderson

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
*/

#ifndef IRRXMLTREE_H
#define IRRXMLTREE_H

#include <irrlicht.h>
#include "../irrTree/irrTree.h"
//#include <irrTree.cpp> - include only when debugging irrXMLTree.h standalone

using namespace irr;

class irrXMLTreeElement : public irrTreeElement
{

private:
	// node type
	io::EXML_NODE type;

	// node name
	core::stringc name;

	// text found inside of the node tags (but not within other tags) if any
	core::stringc text;

	// Attributes taken from the XML sheet
	class Attribute
	{
	public:
		core::stringc name;
		core::stringc value;

		//!
		Attribute() 
		{
			name = "";
			value = "";
		}
		//!
		Attribute( core::stringc new_name, core::stringc new_value )
		{
			name = new_name;
			value = new_value;
		}
	};
	core::array<Attribute> attributes;

public:
	//!
	irrXMLTreeElement()
	{
		type = io::EXN_UNKNOWN;
		name = "null";
		text = "";
	}

	//!
	irrXMLTreeElement( io::EXML_NODE new_type, core::stringc new_name )
	{
		type = new_type;
		name = new_name;
		text = "";
	}
	
	//!
	~irrXMLTreeElement()
	{
		attributes.clear();
	}

	// Adding attributes -----------------
	//!
	void addAttribute()
	{
		attributes.push_back( Attribute() );
	}

	//!
	void addAttribute( core::stringc name, core::stringc value )
	{
		attributes.push_back( Attribute( name, value ) );
	}

	// setters ============================
	//!
	void setNodeType( io::EXML_NODE node_type ) { type = node_type; }
	//!
	void setNodeName( core::stringc node_name ) { name = node_name; }
	//!
	void setNodeText( core::stringc node_text ) { text = node_text; }

	//!
	void appendNodeText( core::stringc node_text ) { text += node_text; }
	
		//! Set attribute name
	/**/
	void setAttrName( s32 which, core::stringc name )
	{
		attributes[which].name = name;
	}

		//! Set attribute value
	/**/
	void setAttrValue( s32 which, core::stringc value )
	{
		attributes[which].value = value;
	}

	// getters =========================
	//!
	io::EXML_NODE getNodeType() { return type; }
	//!
	core::stringc getNodeName() { return name; }
	//!
	core::stringc getNodeText() { return text; }

		//! Get attribute name
	/**/
	core::stringc getAttrName( s32 which ) { return attributes[which].name; }

		//! Get attribute value
	/**/
	core::stringc getAttrValue( s32 which ) { return attributes[which].value; }

		//! Get the total number of attributes
	/**/
	s32 numAttributes() { return (s32)attributes.size(); }

		//! Index the given name
	/* Searches the list of attributes for one whose name matches the given
	one. Returns the index of this attribute or -1 if not found. */
	s32 indexOfName( core::stringc name )
	{
		for ( s32 i = 0; i < (s32)attributes.size(); i++ )
		{
			if ( attributes[i].name == name )
				return i;
		}

		return -1;
	}


	//======== OVERRIDE FUNCTIONS ==========
	// (or those with associated functionality)

	//!
	virtual bool matches( irrTreeElement * match )
	{
		// Conversion necessary
		irrXMLTreeElement * XMLmatch = (irrXMLTreeElement *)match;

		// Comparison
		return ( this->name == XMLmatch->name );
	}


	// associated ===================
	//!
	virtual bool matchesType( irrXMLTreeElement * match )
	{
		return ( this->type == match->type );
	}

	//!
	virtual bool matchesName( irrXMLTreeElement * match )
	{
		return ( this->name == match->name );
	}

	//!
	virtual bool matchesText( irrXMLTreeElement * match )
	{
		return ( this->text == match->text );
	}

	//! Match all attributes
	/* Checks to see if both the given and this element
	contain the same attributes and possibly respective values.
	\param include_values - indicates as to whether the values should be compared */
	virtual bool matchesAttr( irrXMLTreeElement * match, bool include_values )
	{
		// stores index of attribute whose name matches what's being viewed from the given
		s32 matching_index = 0;

		for ( s32 i = 0; i < (s32)attributes.size(); i++ )
		{
			matching_index = indexOfName( match->attributes[i].name );

			if ( matching_index == -1 )
			{
				return false;
			} else {
				if ( include_values
					&& attributes[matching_index].value != match->attributes[i].value
					)
				{
					return false;
				}
			}
		}

		// All entries identified as matching
		return true;
	}

	//! Identical
	/* Indentifies if two elements are identical in every respect. */
	virtual bool identical( irrXMLTreeElement * match )
	{
		if ( type == match->type )
		{
			if ( name == match->name )
			{
				return matchesAttr( match, true );
			} else return false;
		} else return false;
	}

	//! implicit cast to irrTreeElement *
	/**/
	operator irrTreeElement*() const
	{
		return (irrTreeElement *)this;
	}
};


//************************** irrXMLTreeNode *******************************

class irrXMLTreeNode : public irrTreeNode
{
public:
	irrXMLTreeNode( s32 id=0, irrXMLTreeNode * parent_node=0 )
		: irrTreeNode(id, parent_node, new irrXMLTreeElement())
	{
		//delete this->element;
		//this->element = new irrXMLTreeElement();
	}

	//!
	virtual void addNode( s32 id=0 )
	{
		this->children.push_back( new irrXMLTreeNode(id,this) );
	}
};

#endif
