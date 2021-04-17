/*
(c) 2013 Nicolaus Anderson
Licence: zlib
*/

#ifdef _USE_IRRLICHT_
#	include <irrlicht.h>
#else
#	include <irrTypes.h>
#	include <irrString.h>
#endif

//#include <ListInterface.h>

#ifndef __INPUT_STREAM__
#define __INPUT_STREAM__

using irr::s32;
using irr::core::stringc;
using irr::core::stringw;

enum EReadPosition
{
	// Indicates offset from the current token
	EPOS_CURRENT=0,

	// Indicates offset from the beginning
	EPOS_BEGIN,

	// Indicates offset from the end
	EPOS_END,

	// Count
	EPOS_COUNT
};

class InputStream
{
public:
		//! End of stream?
	/*
	\return - "true" if the end of the input stream (such as the end of a file)
	has been reached. */
	virtual bool EOS();

		//! Peek at the next token
	/* Returns the next token from the stream but does NOT increment the
	reading position.
	NOTE: On reaching the end of the stream, it returns an empty string. */
	virtual stringc peek()=0;

		//! Get the next token
	/* Saves the next token from the stream and increments the reading
	position. Note that numbers with decimal places are considered as single
	tokens.
	\return - "true" if it is possible to continue reading the stream.
	"false" if the end of the stream has been reached. */
	virtual bool next( stringc& token )=0;
	virtual bool next( stringw& token )=0;

		//! Set read position
	/* Sets the position within the port's stream that will be next read
	from. This could be a node in an xml file or a simple string from a
	text file.
	\param offset - The number of tokens from the current token
	or the beginning or end of the stream to begin reading from.
	\param epos - Indicates how to position the offset.
	Returns true if the offset was made. */
	virtual bool seek( s32 offset, EReadPosition epos=EPOS_CURRENT )=0;

		//! Get reading position
	/* Returns the current position in the stream. This is required for
	function-style objects that need to return reading to the original
	location (prior to when reading was shifted to the function body). */
	virtual s32 streamPos()=0;

		//! Get token at
	/* Saves the token from a particular position in the stream.
	\return - "true" if it is possible to continue reading the stream.
	"false" if the end of the stream has been reached. */
	virtual bool at( s32 position, stringc& token )=0;
	virtual bool at( s32 position, stringw& token )=0;

		//! Find token
	/* Searches the stream for the given token and returns its location
	in the stream if it was found.
	\param token - The token whose position to find.
	\param cycle - Searching starts from the current reading position and
	goes to the end of the file. If "cycle" is set to true, the reading
	will wrap back around to the beginning and the search will continue until
	the current reading position in the stream has been reached again.
	\return - The position of the token in the stream or -1 if the function
	failed to find it.
	*/
	virtual s32 find( stringc token, bool cycle=true );
	virtual s32 find( stringw token, bool cycle=true );

		//! Find token sequence
	/* Searches the stream for the given token sequence and returns the
	location of the first token in that stream if the sequence was found. */
	//virtual s32 findTokenSequence( ListInterface<string<c8>>* sequence );

		//! Move past token
	/* Slides the current reading position to the position AFTER the given
	token. Returns true if successful. */
	virtual bool movePast( stringc token );
	virtual bool movePast( stringw token );
};

#endif // #ifndef __INPUT_STREAM__