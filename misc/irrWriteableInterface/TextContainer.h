/*
(c) 2012 Nicolaus Anderson

See irrWriteableInterface.h for copyright details.
*/

#include <irrlicht.h>

using namespace irr;

#pragma once


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


/* Standard text container */
class irrStdTextContainer : public TextContainer
{
public:
	core::stringc text;

	irrStdTextContainer()
	{
	}

	virtual u32 size()
	{
		return text.size();
	}

		//! Check bounds
	/* Check if the given index is within the bounds of the list.
	\param index - The index to be checked.
	\param alter - If the index should be altered to ensure it is in range.
	Returns false if the index is NOT out of bounds. Otherwise it returns true. */
	bool CheckBound( s32 * index, bool alter=false)
	{
		// This index cannot be used
		if ( size() == 0 ) return true;

		if ( *index >= (s32)size() || *index < 0 )
		{
			// Is this index fixable?
			if ( alter )
			{
				// Don't worry about corrections if there are no items
				// - let the calling function handle this scenario
				if ( size() == 0 ) return false;

				// Correction for low values
				while ( *index < 0 ) *index = *index + size();

				// Correction for high values
				while ( *index >= (s32)size() ) *index = *index - (s32)size();

				// This index can be used
				return false;
			} else {
				// Indicate this index should not be used
				return true;
			}
		} else {
			// This index can be used
			return false;
		}
	}

	virtual void append( core::stringc newtext )
	{
		text += newtext;
	}

	virtual void edit( s32 index, bool overwrite, core::stringc str )
	{
		if ( !CheckBound( &index, true ) )
		{
			if ( overwrite )
			{
				if ( str.size() < text.size() )
				{
					for ( u32 c = text.size()-1; c >= str.size(); text.erase( c ) );
				} else {
					if ( text.size() < str.size() )
					{
						for ( u32 c = text.size(); c < str.size(); text.append(str[c]) );
					}
				}
			} else {
				core::stringc temp = text.subString( (u32)index, text.size()-1-index );

				text.reserve( index ); // reallocate the space in the string
				text.append( str + temp );
			}
		}
	}

	virtual void deleteChars( s32 index, s32 quantity )
	{
		for ( s32 i = index; i < quantity && i < (s32)size(); i++ )
		{
			text.erase( (u32)i );
		}
	}

	virtual void clear()
	{
		for ( u32 i = size()-1; size() != 0; i-- )
		{
			text.erase( i );
		}
	}

	virtual void set( core::stringc newtext )
	{
		this->text = newtext;
	}

	virtual void set( core::stringw newtext )
	{
		this->text = core::stringc( newtext );
	}

	virtual c8 get( s32 index )
	{
		if ( !CheckBound( &index, true ) )
		{
			return text[index];
		} else {
			return ' ';
		}
	}

	virtual c8& operator[] ( s32 index )
	{
		return text[index];
	}

	//virtual core::stringc getText();

	virtual core::stringc toStringc()
	{
		return text;
	}

	virtual core::stringw toStringw()
	{
		return core::stringw( text );
	}
};