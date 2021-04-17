/*
(c) 2013 Nicolaus Anderson
Licence: zlib
*/

#include "InputStream.h"

#ifndef __INPUT_STREAM_FACTORY__
#define __INPUT_STREAM_FACTORY__

//! class InputPort Factory
/* This is an abstract class designed for generating and destroying
objects of the type InputStream. */
class InputStreamFactory
{
public:
	//! Creator
	/* Generates InputPort instances */
	InputStream* create()=0;

	//! Destroyer
	/* Handles the object when it is no longer needed, possibly
	destroying it. */
	void destroy( InputStream* port )=0;
};

#endif // #ifndef __INPUT_STREAM_FACTORY__