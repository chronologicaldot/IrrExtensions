/*
(c) 2012 Nicolaus Anderson

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

#ifndef IRR_WRITEABLE_INTERFACE_H
#define IRR_WRITEABLE_INTERFACE_H

#include <irrlicht.h>
#include "TextContainer.h"

using namespace irr;

// Mode for writing
enum STDOUT_MODE
{
	// nothing to output to
	STDOUT_NONE = 0,

	// output to the given file
	STDOUT_FILE,

	// output to the given text box
	STDOUT_BOX,

	// output to the standard Windows console
	STDOUT_CONSOLE
};

//! Class Writeable Interface
/* This is a class used for allowing things to write to an imaginary console or file.
It can contain any number of characters that can be edited before the writing is
finalized (output to file or console). */
class WriteableInterface
{
protected:
	//! Container
	/* Container used for holding all of the characters before they
	are output to file or console. */
	TextContainer* container;

	//! Output mode
	/* Indicates to what the information should be written.
	Need not be implemented. */
	STDOUT_MODE mode;

public:

	//! Constructor
	/* Template that takes the type TextContainer. */
	WriteableInterface()
	{
		container = (TextContainer *) new irrStdTextContainer();
		mode = STDOUT_NONE;
	}

	//! Deconstructor
	/**/
	~WriteableInterface()
	{
		container->clear();
	}

	//! Flag = Standard out exists
	/* Indicates if there is something to output to. */
	virtual bool hasRealStdOut() {
		return false;
	}

	//! Set new container
	/* Establishes a new type of container to hold the text. */
	void setNewContainer( TextContainer * newcontainer )
	{
		container = newcontainer;
	}

	//! Print
	/* Saves text to the container to be written. */
	void print( core::stringc text )
	{
		container->append( text );
	}

	//! Edit
	/* Allows for changes to be made at a specific point in the text
	before it is output.
	\param index - The starting position for changes to be made.
	Negative values will be assumed to begin at the end of the text.
	\param overwrite - Indicates whether existing text should be overwritten.
	\param text - The text to save to the container. */
	void edit( s32 index, bool overwrite, core::stringc text )
	{
		container->edit( index, overwrite, text );
	}

	//! Erase
	/* Deletes the text at a specific point.
	\param index - The starting position for changes to be made.
	Negative values will be assumed to begin at the end of the text.
	\param quantity - The number of characters to be deleted. */
	void erase( s32 index, s32 quantity )
	{
		container->deleteChars( index, quantity );
	}

	//! Return container text
	/* Returns all of the text currently being held in the container
	as a stringc. */
	core::stringc getText()
	{
		return container->toStringc();
	}

	//! Finalize
	/* Outputs everything to the console or file and clears the container
	holding all of the letters to be output.
	Returns whether or not this could be done. (There may be no real file or console.)
	\param permanent - Empties the container regardless of whether its contents were
	output to the console or file. */
	bool finalize( bool permanent=true )
	{
		if ( !hasRealStdOut() )
		{
			// Should the contents of the container be erased anyways?
			if ( permanent )
				container->clear();

			return false;
		}

		// Assume an existing Standard out
		writeToStdOut();

		return true;
	}

	//! Write to standard out
	/* Direct write to the output console.
	Function must be determined by programmer of WriteableInterface. */
	virtual void writeToStdOut( core::stringc str ) {}

	//! Write to standard out
	/* Direct write to the output console.
	Function must be determined by programmer of WriteableInterface. */
	virtual void writeToStdOut() { container->clear(); }
};
