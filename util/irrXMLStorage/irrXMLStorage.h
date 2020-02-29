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

#ifndef IRRXMLSTORAGE_H
#define IRRXMLSTORAGE_H

#include <irrlicht.h>
#include <irrXMLTree.h>
#include <irrWriteableInterface.h>

using namespace irr;

#pragma once

//! irr XML Storage
/* This class extracts data from an XML file and saves it in an
tree composed of irrXMLTreeNodes.
*/
class irrXMLStorage : public IReferenceCounted
{
	// ID #
	s32 ID;

	// Tree containing the data
	irrXMLTreeNode* root;

	// File data collection
	io::IXMLReader* reader; // XML reader for acquiring the data
	bool hasFile; // contains a file for reading

public:

	irrXMLStorage()
	{
		hasFile = false;

		root = new irrXMLTreeNode();
	}

	~irrXMLStorage()
	{
		// Empty the tree nodes and wipe them from memory
		delete root;
	}

	//! Clear tree
	/* Delete the current XML contents stored here. */
	void ClearTree()
	{
		delete root;
	}

	//! Load file
	/* Attempts to load an XML file (with the given name) into memory.
	If it is unable to load a file, this function will return false
	and the Parse function will not work. */
	bool LoadFile( io::IFileSystem* file_sys, io::path filename )
	{
		// Load the file and create a reader
		reader = file_sys->createXMLReader( filename.c_str() );

		// Indicate if a file could not be opened
		return (hasFile = (reader != 0));
	}


	//! Parse file
	/* Parses the XML file, saving its contents in the tree.
	Returns false if unable to parse the tree for some reason. */
	bool ParseFile()
	{
		// Stop if there is no file to parse
		if ( !hasFile ) return false;

		// necessary variables
			// automatic id assigned to the node that indicates order of addition to tree
		s32 auto_id = 0;

			// pointer to node for saving data
		irrXMLTreeNode* save_node = root; // start at the first node
			// element/object to save data to
		irrXMLTreeElement* save_elem = (irrXMLTreeElement*)(save_node->getElem());

			// node data
		io::EXML_NODE node_type;
		core::stringc node_text = "";
		s32 num_attrs = 0; // number of attributes in the node
		core::stringw node_attr_name;

		// Parse while there are nodes
		while ( reader->read() )
		{
			node_type = reader->getNodeType();

			// pseudo-switch for what to do based on node type encountered
			// if ( node_type == io::EXN_NONE )
			// {
				// no node
			// } else
			if ( node_type == io::EXN_ELEMENT ) // standard node
			{
				// Save current node text
				save_elem->appendNodeText( node_text );
				
				// Erase the node text for the next node to use
				node_text = "";

				// Create a new node to store data
				save_node->addNode( ++auto_id );

				// Go to that node
				save_node = (irrXMLTreeNode*) save_node->children.getLast();

				save_elem = (irrXMLTreeElement*) save_node->getElem();

				// Get the node type and name
				save_elem->setNodeType( io::EXN_ELEMENT );
				save_elem->setNodeName( reader->getNodeName() );

				// Get the attribute information
				num_attrs = reader->getAttributeCount();
				for ( s32 attr = 0; attr < num_attrs; attr++ )
				{
					node_attr_name = reader->getAttributeName( attr );

					save_elem->addAttribute(
						node_attr_name,
						reader->getAttributeValueSafe( node_attr_name.c_str() )
						);
				}

			} else
			if ( node_type == io::EXN_ELEMENT_END ) // end of a standard node
			{
				// Save the internal text, if any
				save_elem->appendNodeText( node_text );
				
				// Erase the node text for the parent node to continue adding to its own
				node_text = "";

				// Return to the parent of this node, thus "closing" it
				save_node = (irrXMLTreeNode*)save_node->getParent();

			} else
			if ( node_type == io::EXN_TEXT ) // text within a node
			{
				node_text += reader->getNodeData();
			}
		}

		return true; // successful run
	}


	//! Get node
	/* Direct access to the XML nodes. */
	irrXMLTreeNode& operator[] ( s32 index )
	{
		return (irrXMLTreeNode&)root[ index ];
	}

	//! Get node
	/* Direct access to the XML nodes. */
	irrXMLTreeNode* get( s32 index )
	{
		return (irrXMLTreeNode*)root->getChildByIndex( index );
	}

	//! Get the root node
	/* Direct access to the root XML node. */
	irrXMLTreeNode* getRoot()
	{
		return (irrXMLTreeNode*)root;
	}

	//! Root size
	/* Returns the number of children at the root. */
	s32 rootSize()
	{
		return (s32)(root->children.size());
	}


	//! Output all contents
	/* Writes the contents of the tree to a given WriteableInterface.
	Simply put, it calls WriteContentsOf for the root node. */
	void WriteAll( WriteableInterface* writer )
	{
		WriteContentsOf( root, writer );
		writer->finalize();
	}

	//! Output the contents of a specific node
	/* Writes the contents of the select node to the given WriteableInterface. */
	void WriteContentsOf( irrXMLTreeNode* node, WriteableInterface* writer )
	{
		writer->finalize();

		irrXMLTreeElement* elem = (irrXMLTreeElement*)(node->element);

		s32 sizeattr = elem->numAttributes();

		if ( elem->getNodeType() == io::EXN_ELEMENT )
		{
			// Output info about this specific node
			writer->print( core::stringc("<") + elem->getNodeName() );

			for ( s32 i = 0; i < sizeattr; i++ )
			{
				writer->print( core::stringc(" ") + elem->getAttrName(i) + "=\""
					+ elem->getAttrValue(i) + "\"" );
			}

			writer->print( ">\n" );

			// Output info about all sub nodes
			for ( s32 c = 0; c < (s32)node->children.size(); c++ )
			{
				WriteContentsOf( (irrXMLTreeNode*)node->get(c), writer );
			}

			writer->print( elem->getNodeText() );

			// Appear to terminate this node
			writer->print( core::stringc("\n</") + elem->getNodeName() + ">\n" );
		} else {
			if ( elem->getNodeType() == io::EXN_UNKNOWN )
			{
				for ( s32 c = 0; c < (s32)node->children.size(); c++ )
				{
					WriteContentsOf( (irrXMLTreeNode*)node->get(c), writer );
				}
			}
		}
	}
};

#endif