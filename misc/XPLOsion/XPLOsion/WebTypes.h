/*
Created by Nicolaus Anderson, 2012

For license terms, see xplo.h
*/

#include <irrlicht.h>

#pragma once

//! HTML Position type
/* Determines where an element will be drawn on screen. */
enum HTMLPos
{
	HTM_POS_absolute=0,

	HTM_POS_relative,

	HTM_POS_static,

	HTM_POS_fixed
};

//! HTML Position type
/* Determines where an element will be drawn on screen. */
const irr::c8* const HTMLPosTypes[] =
{
	"absolute",
	"relative",
	"static",
	"fixed",
	0
};

//! HTML Overflow
/* Determines if children should be cropped to stay in the element. */
enum HTMLOverflow
{
	HTM_OVF_show = 0, // don't crop children
	HTM_OVF_hide, // crop children
	HTM_OVF_auto // create a scroll bar
};

//! HTML Overflow
/* Determines if children should be cropped to stay in the element. */
const irr::c8* const WebOverflowTypes[] =
{
	"visible",
	"hidden",
	"auto",
	0
};

//! HTML Positioned-element
/* All such elements that are meant to be positioned in an HTML-style manner
need to implement this. */
class HTMLPositioned
{
protected:
	/* Contains the type of position that determines where this
	element will be drawn. */
	HTMLPos position_type;

public:
	void setPosition( HTMLPos pos_type ) { position_type = pos_type; }

	HTMLPos getPosition() { return position_type; }
};


//! HTML Background image repeat
/* Determines how the background image should be repeated over the
entire object. */
enum HTMLBackgroundImageRepeat
{
	HTM_BIR_no_repeat=0, // no repeat
	HTM_BIR_repeat_x, // only repeat along the x-axis
	HTM_BIR_repeat_y, // only repeat along the y-axis
	HTM_BIR_repeat, // repeat along both the x-axis and the y-axis
};

//! HTML Background image repeat
/* Determines how the background image should be repeated over the
entire object. */
const irr::c8* const HTMLBackgroundImageRepeatTypes[] =
{
	"no-repeat",
	"repeat-x",
	"repeat-y",
	"repeat",
	0
};