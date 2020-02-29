/*
(c) 2013 Nicolaus Anderson
Licence: zlib
*/

#include "InputStream.h"
#include "OutputStream.h"
#include "irrTypes.h"

#ifndef __STREAM_MANAGER__
#define __STREAM_MANAGER__

//! class Stream Manager
/* This is an abstract class designed for managing the accessing of objects
of the types InputStream and OutputStream. */
class StreamManager
{
public:
	//! Accesser for input streams
	/* Attempts to load the input stream whose name is given. */
	InputStream* getInputStream( irr::c8* name )=0;

	//! Accessor for output streams
	/* Attempts to load the output stream whose name is given. */
	OutputStream* getOutputStream( irr::c8* name )=0;
};

#endif // #ifndef __STREAM_MANAGER__