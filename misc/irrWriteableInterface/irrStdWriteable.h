/*
(c) 2012 Nicolaus Anderson

See irrWriteableInterface.h for copyright details.
*/

#include <irrlicht.h>
#include "TextContainer.h"
#include "irrWriteableInterface.h"

using namespace irr;

#pragma once


//! Class irrStdWriteable
/* This class is a cross-platform writer for the irrlicht engine.
It can write to files or to irrlicht GUI text boxes. */
class irrStdWriteable : public WriteableInterface
{
protected:
	// GUI box
	gui::IGUIEditBox * box;
	bool hasGUIBox;
	core::stringw textbox_text;

	// file
	io::IWriteFile * output_file;
	bool hasFile;

public:

	//! Constructor
	/**/
	irrStdWriteable() : WriteableInterface()
	{
		hasGUIBox = false;
		hasFile = false;
	}

	//! Deconstructor
	/* Drops the output file. */
	~irrStdWriteable()
	{
		if ( hasFile )
			output_file->drop();
	}

	//! Flag = Standard out exists
	/* Indicates if there is something to output to. */
	virtual bool hasRealStdOut()
	{
		switch( mode )
		{
		case STDOUT_FILE:
			if ( hasFile ) {
				return true;
			} else {
				return false;
			}
		case STDOUT_BOX:
			if ( hasGUIBox ) {
				return true;
			} else {
				return false;
			}
		default: break;
		}

		return false;
	}

	//! Create file
	/* Creates a new file to write to.
	\param path - String or path containing where to put this new file.
	Path includes file name.
	\param append - Whether to append to a file with this name that has
	already been opened by the program.
	*/
	void createFile( IrrlichtDevice * device, io::path path, bool append )
	{
		output_file =
			device->getFileSystem()->createAndWriteFile( path, append );

		if ( output_file != 0 ) // if the file exists
		{
			hasFile = true;
		}
	}

	//! Set file
	/* Sets the file to be written to. */
	void setFile( io::IWriteFile * file )
	{
		output_file = file;
		output_file->grab();
	}

	//! Write to file
	/* Writes the text to the file. */
	void writeToFile( core::stringc text )
	{
		if ( hasFile )
			output_file->write( (void*)text.c_str(), text.size()*sizeof(c8) );
	}

	//! Write to file
	/* Writes the text to the file. */
	void writeToFile()
	{
		if ( hasFile )
		{
			output_file->write(
				(void*)(container->toStringc().c_str()),
				(container->size())*sizeof(c8)
				);

			container->clear();
		}
	}

	//! Create GUI text box
	/* Creates a GUI text box to be used. */
	gui::IGUIEditBox& createGUIBox(
		gui::IGUIEnvironment * envir,
		gui::IGUIElement * parent,
		s32 id=-1,
		core::rect<s32> shape = core::rect<s32>(0,0,400,400)
		)
	{
		box = envir->addEditBox( L"", shape, true, parent, id );
		hasGUIBox = true;
	}

	//! Set GUI box
	/* Takes a pointer to the IGUIEditBox that this writer
	will now write to. */
	void setGUIBox( gui::IGUIEditBox * new_box )
	{
		if ( new_box != 0 )
		{
			box = new_box;
			hasGUIBox = true;
		}
	}

	//! Write to irrlight GUI edit box
	/* Writes the given text to the GUI box */
	bool writeToGUIBox( core::stringc text )
	{
		// Convert
		textbox_text = text;

		// Write to the box
		if ( hasGUIBox )
		{
			box->setText( textbox_text.c_str() );
			return true;
		} else {
			return false;
		}
	}

	//! Write to irrlight GUI edit box
	/* Writes the given text to the GUI box */
	bool writeToGUIBox()
	{
		// Write to the box
		if ( hasGUIBox )
		{
			box->setText( container->toStringw().c_str() );
			container->clear();
			return true;
		} else {
			return false;
		}
	}

	//! Write to file from GUI box
	/* Writes the contents of the GUI box to the given file if possible.
	Returns false if unable to do so. */
	bool writeFromGUIBoxToFile()
	{
		// Check if there is both a GUI box and a file
		if ( !hasGUIBox || !hasFile )
			return false;

		// Convert the text in the GUI box to standard string text
		container->set( textbox_text );

		// Write this text to file
		writeToFile( container->toStringc() );
	}

	//! Write to standard out
	/* Writes the contents to the standard output. */
	virtual void writeToStdOut( core::stringc str )
	{
		switch( mode )
		{
		case STDOUT_FILE:
			writeToFile( str ); container->clear(); break;
		case STDOUT_BOX:
			writeToGUIBox( str ); container->clear(); break;
		default: break;
		}
	}

	//! Write to standard out
	/* Writes the contents to the standard output. */
	virtual void writeToStdOut()
	{
		switch( mode )
		{
		case STDOUT_FILE:
			writeToFile(); break;
		case STDOUT_BOX:
			writeToGUIBox(); break;
		default: break;
		}
	}
};