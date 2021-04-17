/*
(c) Nicolaus Anderson
Created: Jan 10, 2013

License: zlib
*/

#include "directions.cpp"

#pragma once

#ifdef __ESTDDIRECTION__

EStdDirection std_directions::findDirectionEnumValue( stdc8* str )
{
	if ( str == "left" )
		return ESDIR_LEFT;

	if ( str == "right" )
		return ESDIR_RIGHT;

	if ( str == "up" )
		return ESDIR_UP;

	if ( str == "down" )
		return ESDIR_DOWN;

	if ( str == "forward" )
		return ESDIR_FORWARD;

	if ( str == "backward" )
		return ESDIR_BACKWARD;

	// default return
	return ESDIR_LEFT;
}

#endif


#ifndef __ESTDDIMENSION__

EStdDimension std_dimensions::findDimensionEnumValue( stdc8* str )
{
	if ( str == "x-axis" )
		return ESDIM_X;

	if ( str == "y-axis" )
		return ESDIM_Y;

	if ( str == "z-axis" )
		return ESDIM_Z;

	if ( str == "time" )
		return ESDIM_T;

	// default return
	return ESDIM_X;
}

#endif