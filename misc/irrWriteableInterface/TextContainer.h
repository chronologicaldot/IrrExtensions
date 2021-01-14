/*
(c) 2012 Nicolaus Anderson

See irrWriteableInterface.h for copyright details.
*/

#ifndef IRR_TEXT_CONTAINER_H
#define IRR_TEXT_CONTAINER_H

#include <irrlicht.h>

using namespace irr;

/* Text container interface for reliable return. */

class TextContainer
{
public:
	virtual u32 size()=0;
	virtual void append( core::stringc text )=0;
	virtual void edit( s32 index, bool overwrite, core::stringc text )=0;
	virtual void deleteChars( s32 index, s32 quantity )=0;
	virtual void clear()=0;
	virtual void set( core::stringc newtext )=0;
	virtual void set( core::stringw newtext )=0;
	virtual c8 get( s32 index )=0;
	//virtual core::stringc getText()=0;
	virtual core::stringc toStringc()=0;
	virtual core::stringw toStringw()=0;

	virtual c8& operator[] ( s32 index )=0;
};

#endif
