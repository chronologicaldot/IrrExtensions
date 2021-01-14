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
#include "../irrXMLTree/irrXMLTree.h"

// TODO: Replace this
#ifdef USE_IRR_WRITEABLE_INTERFACE
#include "../../misc/irrWriteableInterface/irrWriteableInterface.h"
#endif

using namespace irr;

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

	irrXMLStorage();

	~irrXMLStorage();

	//! Clear tree
	/* Delete the current XML contents stored here. */
	void ClearTree();

	//! Load file
	/* Attempts to load an XML file (with the given name) into memory.
	If it is unable to load a file, this function will return false
	and the Parse function will not work. */
	bool LoadFile( io::IFileSystem* file_sys, io::path filename );

	//! Parse file
	/* Parses the XML file, saving its contents in the tree.
	Returns false if unable to parse the tree for some reason. */
	bool ParseFile();

	//! Get node
	/* Direct access to the XML nodes. */
	irrXMLTreeNode& operator[] ( s32 index );

	//! Get node
	/* Direct access to the XML nodes. */
	irrXMLTreeNode* get( s32 index );

	//! Get the root node
	/* Direct access to the root XML node. */
	irrXMLTreeNode* getRoot();

	//! Root size
	/* Returns the number of children at the root. */
	s32 rootSize();

#ifdef USE_IRR_WRITEABLE_INTERFACE
	//! Output all contents
	/* Writes the contents of the tree to a given WriteableInterface.
	Simply put, it calls WriteContentsOf for the root node. */
	void WriteAll( WriteableInterface* writer );

	//! Output the contents of a specific node
	/* Writes the contents of the select node to the given WriteableInterface. */
	void WriteContentsOf( irrXMLTreeNode* node, WriteableInterface* writer );
#endif
};

#endif
