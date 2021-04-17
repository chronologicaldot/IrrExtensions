/*
Project name:
XML Program Layout arranger ("XPLOsion")

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
similar to html.
The layout of the program's interface is to be done in an XML format.
The unique attributes of the GUI elements are to be done in a CSS-style format.

List of some recognized tags:
<text>
<div>
<button>
<editbox>
*/

#include <irrlicht.h>
#include <LinusListix.h>
#include <irrXMLStorage.h>
#include "GUIElemAcceptor.h"
#include "CSSLibrary.h"
#include "sdiv.h"
#include "CSlider.h"

#ifndef XPLOSION_H
#define XPLOSION_H

#define XPLO_Version 1.411


//! Class XPLOsion
/*
Default class. It handles ALL of the GUI layout operations
and the event reciever information.
Use it to load a GUI that is described by an XML file.
*/
class XPLOsion : public irr::IReferenceCounted
{
protected:
	//! Reference to the engine
	/* This is used for things like adding buttons. */
	IrrlichtDevice* irrdev;

	//! XML storage
	/* Stores the XML tree to be interpreted. */
	irrXMLStorage* xmldata;

	//! CSS library
	/* Contains a list of nodes of CSS information loaded from file
	and functions for styling divisions. */
	CSSLibrary* csslib;

	//! Allow style inheritance flag
	/* Indicates if CSS information should be loaded from parent
	XML elements. */
	bool styleInheritance;

	//! Class associates list
	/* A list of several classes inheriting from GUIElemAcceptor. */
	irr::core::array<GUIElemAcceptor*> acceptors;

	// ID tags for sub-elements (elements created by reading the file)
	irr::s32 autoID;

	// ID tag for this element
	irr::s32 ID;

public:

		//! constructor
	/* NOTE: You must assign a CSSLibrary to csslib if you wish to use
	styling, which is really the point of this in the first place.
	\param device - Pointer to the irrlicht engine.
	\param inherit_css - Flag indicating whether CSS information should be
	inherited from parent XML nodes.
	*/
	XPLOsion( IrrlichtDevice* device, bool inherit_css = false )
	{
		irrdev = device;
		xmldata = 0;
		autoID = 1;
		styleInheritance = inherit_css; // by default, do not inherit XML parents' CSS

			// ensure constant value when checking for these
		csslib = 0;
	}

		//! destructor
	/**/
	~XPLOsion()
	{
		if ( xmldata != 0 )
			xmldata->drop();

		if ( csslib != 0 )
			csslib->drop();

		acceptors.clear();
	}

		//! Get the ID of this instance
	/**/
	irr::s32 getID() const
	{
		return ID;
	}

		//! Set the ID of this instance
	/**/
	void setID( s32 newID )
	{
		ID = newID;
	}

		//! Set auto ID
	/* Sets the number that will begin how all successive GUI elements
	created by this xplo are given IDs. Actual starting value will be
	this + 1. */
	void setAutoID( irr::s32 new_start )
	{
		autoID = new_start;
	}

		//! Set parent CSS inheritance
	/* Indicate whether CSS information should be inherited from parent
	XML nodes in addition to the styling provided by the GUI element's
	own node. */
	void setParentCSSInheritance( bool inherit_css = false )
	{
		styleInheritance = inherit_css;
	}

		//! Get parent CSS inheritance
	/* Returns whether or not CSS information is inherited from parent
	XML nodes in addition to the styling provided by the GUI element's
	own node. */
	bool getParentCSSInheritance()
	{
		return styleInheritance;
	}

		//! Get xml data
	/* Returns a pointer to the XML data as an irrXMLStorage instance. */
	irrXMLStorage* getXMLData()
	{
		return xmldata;
	}

		//! Drop data
	/* Drops the XML data stored here. */
	void dropXMLData()
	{
		if ( xmldata != 0 )
			xmldata->drop();
		xmldata = 0;
	}

		//! Set XML data
	/* Stores a pointer to irrXMLStorage instance to be read and parsed
	by this class. You should call destroyXMLData() to remove it. */
	void setXMLDataPointer( irrXMLStorage* new_data )
	{
		if ( xmldata != 0 )
			xmldata->drop();

		xmldata = new_data;
		xmldata->grab();
	}

		//! Get CSS data
	/* Returns a pointer to the CSS Library containing information on
	how the GUI elements are to be formatted. */
	CSSLibrary* getCSSData()
	{
		return csslib;
	}

		//! Drop CSS Data
	/* Drop the CSS data stored here. */
	void dropCSSData()
	{
		if ( csslib != 0 )
			csslib->drop();
		csslib = 0;
	}

		//! Set CSS Data
	/* Stores a pointer to the CSS library to be used by this class instance for
	formatting the GUI elements. You should call destroyCSSData to remove it. */
	void setCSSDataPointer( CSSLibrary* library )
	{
		if ( csslib != 0 )
			csslib->drop();
		
		csslib = library;
		csslib->grab();
	}

		//! Add GUIElemAcceptor
	/* Registers a GUIElemAcceptor. */
	void registerGUIElemAcceptor( GUIElemAcceptor* acceptor )
	{
		acceptors.push_back( acceptor );
	}

		//! Remove GUIElemAcceptor
	/* Removes from the list the pointer to the given GUIElemAcceptor. */
	void removeGUIElemAcceptor( GUIElemAcceptor* acceptor )
	{
		for ( irr::u32 i = 0; i < acceptors.size(); i++ )
		{
			if ( acceptors[i] == acceptor )
				acceptors.erase(i);
		}
	}

		//! Load file
	/* Attepts to load the file into memory but does not parse it. */
	bool LoadFile( irr::io::path filename )
	{
		if ( xmldata == 0 )
		{
			xmldata = new irrXMLStorage();
		}

		return xmldata->LoadFile(
			irrdev->getFileSystem(), filename
			);
	}

		//! Parse File
	/* If a file has been loaded into memory, it is parsed and the
	resulting tree, if correctly parsed, is used to generate a
	GUI interface that is assigned to the parent. Default parent is the
	root GUI element. */
	bool ParseFile( irr::gui::IGUIElement * parent=0 )
	{
		if ( xmldata != 0 )
		{
			if ( xmldata->ParseFile() )
			{
				if ( parent == 0 )
				{
					TranslateXMLNode(
						xmldata->getRoot(),
						irrdev->getGUIEnvironment()->getRootGUIElement()
						);
				} else {
					TranslateXMLNode(
						xmldata->getRoot(),
						parent
						);
				}

				return true;
			}
		}
		
		return false;
	}

		//! Translate XML node
	/* Attempts to create a GUI element from the given XML node.
	It will then do the same for all subsequent child nodes. */
	void TranslateXMLNode(
		irrXMLTreeNode* node, irr::gui::IGUIElement* parent, bool clipped=false
		)
	{
		// Unable to work without a parent element
		if ( parent == 0 )
			return;

		/* Create a dummy library if necessary */
		if ( csslib == 0 )
			csslib = new CSSLibrary( irrdev );

		TranslateNode( node, parent, clipped );
	}

protected:

		//! Translate XML node
	/* Attempts to create a GUI element from the given XML node.
	It will then do the same for all subsequent child nodes. */
	void TranslateNode(
		irrXMLTreeNode * node, irr::gui::IGUIElement * parent, bool clipped=false
		);

		//! Apply attributes to GUI element
	/* Reads the attributes of an irrXMLTreeElement and applies them to the
	given GUI element.
	\param checkIfPressable - Flag that indicates if there should be a search for
	an "onClick" or "onPress" attribute, indicating this GUI element responds to
	user interaction. */
	void applyAttributesToElement(
				irrXMLTreeNode* node,
				irr::gui::IGUIElement* gui_element,
				bool checkIfPressable=false
				);

		//! Obtain attributes for GUI element
	/* Reads the attributes of an irrXMLTreeElement so they can be applied
	to a GUI element. */
	void obtainAttributesForElement(
				irrXMLTreeNode* node,
				Linus::LListix<CSSTagData*>* applied_styles,
				bool checkIfPressable=false
				);
};

#endif
