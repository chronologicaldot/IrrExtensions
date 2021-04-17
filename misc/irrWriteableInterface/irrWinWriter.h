/*
(c) 2012 Nicolaus Anderson

See irrWriteableInterface.h for copyright details.
*/

#ifndef IRR_WIN_WRITER_H
#define IRR_WIN_WRITER_H

#include <iostream>

#include <irrlicht.h>
#include "TextContainer.h"
#include "irrWriteableInterface.h"
#include "irrStdWriteable.h"

using std::cout;
using std::cin;

using namespace irr;


//! Standard Windows Console Writer for irrlicht
class irrWinWriter : public irrStdWriteable
{
private:

public:

	//! Constructor
	/**/
	irrWinWriter( STDOUT_MODE initial_mode = STDOUT_CONSOLE )
		: irrStdWriteable()
	{
		mode = initial_mode;
	}

	//! Flag = Standard out exists
	/* Indicates if there is something to output to. */
	virtual bool hasRealStdOut()
	{
		switch( mode )
		{
		case STDOUT_FILE:
			if ( hasFile )
				return true;
			else
				return false;
		case STDOUT_BOX:
			if ( hasGUIBox )
				return true;
		case STDOUT_CONSOLE:
			return true;
		default: break;
		}

		return false;
	}

	//! Sets writer
	/* Identifies if a writer can be written to and, if so, allows
	for content to be writen there via standard out. */
	bool setWriter( STDOUT_MODE which )
	{
		mode = which;
	}

	//! Override writeToStdOut
	/* Writes to the console based on the current write setting. */
	virtual void writeToStdOut( core::stringc str )
	{
		switch( mode )
		{
		case STDOUT_FILE:
			writeToFile( str ); break;
		case STDOUT_BOX:
			writeToGUIBox( str ); break;
		case STDOUT_CONSOLE:
			cout << str.c_str();
			break;
		default: break;
		}
	}

	//! Override writeToStdOut
	/* Writes to the console based on the current write setting. */
	virtual void writeToStdOut()
	{
		switch( mode )
		{
		case STDOUT_FILE:
			writeToFile(); break;
		case STDOUT_BOX:
			writeToGUIBox(); break;
		case STDOUT_CONSOLE:
			cout << container->toStringc().c_str();
			container->clear();
			break;
		default: break;
		}
	}

	//! Get console input
	/* Gets the input from the console. */
	core::stringc getInputFromConsole()
	{
		char c;
		core::stringc text;
		do {
			cin >> c;
			text.append( c );
		} while ( cin.peek() != '\n' );
		return text;
	}

	//! Print console input
	/* Gets the input from the console and stores it with print(). */
	void printConsoleInput()
	{
		print( getInputFromConsole() );
	}

	//! Write console input to standard out
	/* Gets the console input and writes it to standard out.
	Note this bypasses the saving of this information to the container. */
	void writeConsoleInputToStdOut()
	{
		writeToStdOut( getInputFromConsole() );
	}
};

#endif
