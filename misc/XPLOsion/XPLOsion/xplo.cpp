/*
Created by Nicolaus Anderson, 2012

For license terms, see xplo.h
*/

#include "xplo.h"

#pragma once


void XPLOsion::TranslateNode(
		irrXMLTreeNode* node, irr::gui::IGUIElement* parent, bool clipped
		)
{
	irrXMLTreeElement* elem = (irrXMLTreeElement*) node->element;
	irr::gui::IGUIElement* gui_element;

	// empty default bounding box
	irr::core::recti box;

	
	// text division ==========================
	if ( elem->getNodeName() == "text" )
	{
		// GUI element creation
		gui_element =
			irrdev->getGUIEnvironment()->addStaticText(
					// default values are irrelevant
				L" ", box, false, true,
				parent, // parent GUI element - NOT irrelevant
				0, true
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Append the real text if it exists
		elem->getNodeText().trim();
		if ( elem->getNodeText().size() != 0 )
		{
			gui_element->setText(
				irr::core::stringw( elem->getNodeText() ).c_str()
				);
		}

		/* For now, text boxes don't have children, so these operations are done.
		This may change if I decide I want fancy trim around my text boxes and
		I can get that to even work. */

		return;
	}

	// regular division =========================
	else if ( elem->getNodeName() == "div" )
	{
		gui_element = new divBox( irrdev->getGUIEnvironment(), parent, 0 );

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element, true );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Translate the children of this node since divs can contain stuff
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode *)node->get(c),
				gui_element,
				((divBox*)gui_element)->hidesOverflow()
				);
		}

		((divBox*)gui_element)->resetChildBounds();

		// Ensure that the scroll bars start out on top
		((divBox*)gui_element)->bringScrollsToFront();

		return;
	}

	// empty element ==========================
	else if ( elem->getNodeName() == "basic" ) {
		gui_element = new IGUIElement( irrdev->getGUIEnvironment(), irrdev->getGUIEnvironment()->getRootGUIElement() );

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the child nodes. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// button ==================================
	else if ( elem->getNodeName() == "button" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addButton(
					box, // bounding box - irrelevant
					parent, // parent GUI element
					0
					);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element, true );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the children of this node since buttons can contain stuff.
		Like what? - Perhaps we want multiple images on a button or some kind
		of animated trim. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode *)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// text edit box ==========================
	else if ( elem->getNodeName() == "editbox" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addEditBox(
					L" ", box, true, // irrelevant
					parent, // parent GUI element - NOT irrelevant
					0
					);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the children of this node since editboxes can contain stuff.
		Like what? - Perhaps we want a marker to follow the mouse around
		whenever it moves within the text box. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode *)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// window within the main window ==========================
	else if ( elem->getNodeName() == "window" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addWindow(
				box, false, 0,
				parent, // parent GUI element - NOT irrelevant
				0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Translate the children of this node since windows can contain stuff
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode *)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// context menu ==========================
	else if ( elem->getNodeName() == "menu" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addContextMenu(
				box, // bounding box - irrelevant
				parent, // parent GUI element - NOT irrelevant
				0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Translate the children of this node since menus can contain stuff
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// menu bar ==========================
	else if ( elem->getNodeName() == "menubar" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addMenu(
				parent, // parent GUI element - NOT irrelevant
				0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Translate the children of this node since menubars can contain stuff
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// message box ==========================
	else if ( elem->getNodeName() == "messagebox" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addMessageBox(
				L" ", L" ", false, 0, // irrelevant
				parent, // parent GUI element - NOT irrelevant
				0, 0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Translate the children of this node since menubars can contain stuff
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// modal screen ==========================
	else if ( elem->getNodeName() == "modalscreen" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addModalScreen( parent );

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Translate the children of this node since modalscreens can contain stuff
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// mesh viewer ==========================
	else if ( elem->getNodeName() == "meshviewer" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addMeshViewer(
				box, // irrelevant
				parent, // parent GUI element - NOT irrelevant
				0, 0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the children of this node since mesh viewers can contain stuff.
		Contain what? - Toolbars, perhaps for modelling and animation.
		*/
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// checkbox ==========================
	else if ( elem->getNodeName() == "checkbox" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addCheckBox(
				false, box, // irrelevant
				parent, // parent GUI element - NOT irrelevant
				0, 0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// Check boxes shouldn't have children, so these operations are done
		// Why would you give a checkbox child GUI elements?

		return;
	}

	// combobox ==========================
	else if ( elem->getNodeName() == "combobox" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addComboBox(
				box, // irrelevant
				parent, // parent GUI element - NOT irrelevant
				0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the children of this node since mesh viewers can contain stuff.
		Contain what? - Toolbars, perhaps for modelling and animation.
		*/
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// file open dialog box ==========================
	else if ( elem->getNodeName() == "fileopendialog" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addFileOpenDialog(
				L"Open file", // title
				true,
				parent, // parent GUI element - NOT irrelevant
				0
				);

		/* File open dialog boxes establish their own boundaries,
		hence it isn't worth loading a bounding box for them.
		
		Furthermore, file open dialog boxes do not draw children,
		so it isn't necessary to try to add any. */

		return;
	}

	// list box ==========================
	else if ( elem->getNodeName() == "listbox" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addListBox(
				box,
				parent, // parent GUI element - NOT irrelevant
				0, false
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the children of this node since listboxs can contain stuff.
		Like what? - Perhaps something to follow the mouse around while it is
		in the box. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// color select dialog ==========================
	else if ( elem->getNodeName() == "colorselectdialog" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addColorSelectDialog(
				L"Select color", // title
				true,
				parent, // parent GUI element - NOT irrelevant
				0
				);

		/* Color select dialog boxes establish their own boundaries
		by trying to fit within their parent GUI element,
		hence it isn't worth loading a bounding box for them.
		
		Furthermore, file open dialog boxes do not draw children,
		so it isn't necessary to try to add any. */

		return;
	}

	// in out fader ==========================
	else if ( elem->getNodeName() == "inoutfader" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addInOutFader(
				&box,
				parent, // parent GUI element - NOT irrelevant
				0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the children of this node since in out faders can contain
		stuff.
		Like what? - Perhaps something to follow the mouse around while it is
		in the box. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// scroll bars ==========================
	else if ( elem->getNodeName() == "scrollbar" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addScrollBar(
				true, box,
				parent, // parent GUI element - NOT irrelevant
				0
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the child nodes. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// sliders ==========================
	else if ( elem->getNodeName() == "slider" )
	{
		gui_element =
			new irr::gui::CSlider(
				false,
				irrdev->getGUIEnvironment(),
				parent, // parent GUI element - NOT irrelevant
				0, box, false
				);

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the child nodes. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// tab control ==========================
	else if ( elem->getNodeName() == "tabcontrol" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addTabControl(
				box,
				parent, // parent GUI element - NOT irrelevant
				false, true, 0 );

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the child nodes. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// tab ==========================
	else if ( elem->getNodeName() == "tab" )
	{
		if ( parent->getType() == irr::gui::EGUIET_TAB_CONTROL )
		{
			//gui_element =
				//irrdev->getGUIEnvironment()->addTab( box, 0, 0 );
			gui_element = ((irr::gui::IGUITabControl*)parent)->addTab( L" " );
		} else {
			gui_element =
				irrdev->getGUIEnvironment()->addTab(
					box,
					parent,  // parent GUI element - NOT irrelevant
					0 );
		}

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the child nodes. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	// spinbox ==========================
	else if ( elem->getNodeName() == "spinbox" )
	{
		gui_element =
			irrdev->getGUIEnvironment()->addSpinBox(
				L"", box, true,
				parent,  // parent GUI element - NOT irrelevant
				0 );

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		/* Translate the child nodes. */
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
				(irrXMLTreeNode*)node->get(c),
				gui_element,
				true // hides overflow
				);
		}

		return;
	}

	/*************** OTHER TYPES ***************
	Define other GUI elements here, perhaps your own custom GUI element.
	Be sure to add the necessary attribute loaders to the CSSLibraryImpl.cpp
	function CSSLibrary::interpretStyling().
	*/

	// if ( elem->getNodeName() == my_element_name )
	//{

	//	return;
	//}

	// Registered GUI Factories ==========================
	/* Use the registered GUI Factories to create the element. */
	//bool inFactory = false;

	u32 factory_count =
		irrdev->getGUIEnvironment()->getRegisteredGUIElementFactoryCount();

	gui::IGUIElementFactory* factory;

		/* Brute-force trying to create the GUI element by testing out
		every GUI element factory. */
	u32 fac = 0;
	for (; fac < factory_count; fac++ )
	{
		factory = irrdev->getGUIEnvironment()->getGUIElementFactory(fac);

		gui_element =
			factory->addGUIElement( elem->getNodeName().c_str(), parent );

		if ( gui_element == 0 )
			continue;

		// Interpret the parameters of this xml node and apply them
		applyAttributesToElement( node, gui_element );

		// Clipping necessary?
		gui_element->setNotClipped( !clipped );

		// This factory was able to create the GUI element
		if ( gui_element )
		{
			// Translate the children
			for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
			{
				TranslateNode(
						(irrXMLTreeNode*)node->get(c),
						parent,
						false
						);
			}

			return;
		}
	}

	// unknown type ===========================
	//if ( !inFactory )
	//{
		// Translate the children
		for ( irr::s32 c = 0; c < (s32)(node->children.size()); c++ )
		{
			TranslateNode(
					(irrXMLTreeNode*)node->get(c),
					parent,
					false
					);
		}
	//}
}


void XPLOsion::applyAttributesToElement(
				irrXMLTreeNode* node,
				irr::gui::IGUIElement* gui_element,
				bool checkIfPressable
				)
{
	Linus::LListix<CSSTagData*> applied_styles;
	//irr::core::array<CSSTagData*> applied_styles;

	/* A hint for the GUIElemAcceptor to identify if they should save
	a pointer to the specific GUI element. */
	irr::core::stringc BindHint;


	obtainAttributesForElement( node, &applied_styles, checkIfPressable );

	/* Remove duplicate styles */
	for ( irr::s32 i1 = 0; i1 < applied_styles.Size() - 1; i1++ )
	{
		for ( irr::s32 i2 = i1 + 1; i2 < applied_styles.Size(); i2++ )
		{
			if ( applied_styles[i1] == applied_styles[i2] )
				//applied_styles.erase( i2 );
				applied_styles.deleteAt( i2 );
		}
	}
	
	// Stacking CSS styles **********************

	if ( applied_styles.Size() > 0 )
	{
		applied_styles[0]->setParentTag( 0 );

		for ( irr::s32 index = 1; index < applied_styles.Size(); index++ )
		{
			applied_styles[index]->setParentTag( applied_styles[index-1] );
		}

		csslib->applyStyleToGUIElement(
					gui_element,
					applied_styles[applied_styles.Size()-1]
					);

		/* Obtain binding hints
		- Allows us to link GUI elements with classes that use their
		values but aren't dependent on their styling. */
		for ( irr::s32 st = applied_styles.Size()-1; st >= 0; st-- )
		{
			BindHint = applied_styles[st]->getAttributeAsString( "BindHint" );

			if ( BindHint.size() > 0 )
			{
				for ( irr::u32 gea = 0; gea < acceptors.size(); gea++ )
				{
					/* Offer the GUI element to the class to bind it.
					The element is offered to all classes, regardless if one
					accepts it, since multiple classes may use the same
					GUI element. */
					acceptors[gea]->bind( gui_element, BindHint.c_str() );
				}

				break;
			}
		}
	}

	/* Override blank ID if one was not acquired from the styling. */
	if( gui_element->getID() == 0 )
	{
		gui_element->setID( autoID++ );
	}
}


void XPLOsion::obtainAttributesForElement(
				irrXMLTreeNode* node,
				Linus::LListix<CSSTagData*>* applied_styles,
				bool checkIfPressable
				)
{
	irrXMLTreeNode *node_b;
	irrXMLTreeElement * elem;
	irr::s32 csstag_index = 0; // used for finding if the tag exists

	irr::s32 num_attrs = 0;


	/* We're going to use node_b for checking parent nodes for CSS data,
	and that way we don't lose track of what's in "node". */
	node_b = node;


	/* styleInheritance will determine whether or not to inherit parent
	GUI element's CSS data, and thus determine if this loop of grabbing
	XML data will continue. */
	do {
		elem = (irrXMLTreeElement *) node_b->element;

		/* Error: incorrect type */
		if ( elem->getNodeType() != irr::io::EXN_ELEMENT )
			break;

		/* Determine the attributes of the GUI element based on what's in the node's
		attributes list. */
		num_attrs = elem->numAttributes() - 1;
		for( irr::s32 a = num_attrs; a >= 0; a-- )
		{
			if ( elem->getAttrName(a) == "class" )
			{
				// Set the shape of this element based on its class style
				csstag_index = csslib->findCSSClass( elem->getAttrValue(a) );

				if ( csstag_index != -1 )
				{
					applied_styles->push_front(
						csslib->getCSSStyle(csstag_index)
						);
				}
			}
			else if ( elem->getAttrName(a) == "id" )
			{
				// Set the shape of this element based on its ID style
				csstag_index = csslib->findCSSID( elem->getAttrValue(a) );

				if ( csstag_index != -1 )
				{
					applied_styles->push_front(
						csslib->getCSSStyle(csstag_index)
						);
				}
			}
			else if ( elem->getAttrName(a) == "style" )
			{
				// Request that the style be interpreted in-place
				applied_styles->push_front(
					csslib->interpretStyling( elem->getAttrValue(a) )
					);
			}
			else if (
				elem->getAttrName(a) == "width"
				|| elem->getAttrName(a) == "height"
				)
			{
				applied_styles->push_front(
					csslib->interpretStyling(
								elem->getAttrName(a) + ":" + elem->getAttrValue(a) + ";"
								)
					);
			} else // get a random CSS node
			{
				// Set the shape of this element based on a random style whose name matches
				csstag_index = csslib->findCSSRandom( elem->getAttrValue(a) );

				if ( csstag_index != -1 )
				{
					applied_styles->push_front(
						csslib->getCSSStyle(csstag_index)
						);
				}
			}
		}

		/* if styleInheritance is true, this will be relevant */
		if ( node_b->getParent() != 0 )
		{
			node_b = (irrXMLTreeNode*)node_b->getParent();
		} else {
			break; // Normal loop termination when inheritance desired - no parent
		}

	// Normal loop termination when inheritance not desired
	} while ( styleInheritance );
}
