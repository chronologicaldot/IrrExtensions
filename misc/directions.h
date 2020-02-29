/*
(c) Nicolaus Anderson
Created: Jan 10, 2013

License: zlib
*/

#include "stdtypes.h"

#pragma once

namespace std_directions
{
	// can be overridden if necessary
#ifndef __ESTDDIRECTION__
#define __ESTDDIRECTION__

/* Enumeration: Standard directions */
enum EStdDirection
{
	ESDIR_LEFT = 0,
	ESDIR_RIGHT,
	ESDIR_UP,
	ESDIR_DOWN,
	ESDIR_FORWARD,
	ESDIR_BACKWARD
};

const stdc8* const EStdDirLiterals[] =
{
	"left",
	"right",
	"up",
	"down",
	"forward",
	"backward",
	0
};

// prototype
EStdDirection findDirectionEnumValue( stdc8* str );

#endif

} // end namespace std_directions


namespace std_dimensions
{
	// can be overridden if necessary
#ifndef __ESTDDIMENSION__
#define __ESTDDIMENSION__

/* Enumeration: Standard dimensions */
enum EStdDimension
{
	ESDIM_X = 0,
	ESDIM_Y,
	ESDIM_Z,
	ESDIM_T
};

const stdc8* const EStdDimLiterals[] =
{
	"x-axis",
	"y-axis",
	"z-axis",
	"time",
	0
};

// prototype
EStdDimension findDimensionEnumValue( stdc8* str );

#endif

} // end namespace std_dimensions