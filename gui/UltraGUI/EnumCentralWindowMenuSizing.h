/*
(c) Nicolaus Anderson
*/

#pragma once

/*
Enum for changing the size settings of the central window menu.
*/
enum ECWMSize
{
	/* Default settings (450px wide, 400px tall). */
	ECWMSize_default =0,

	/* Attempts to size the window to neatly fit the available
	program window size.
	Applies to both the width and height. */
	ECWMSize_optimize,

	/* Makes the size constant.
	Applies to both the width and height. */
	ECWMSize_constant,

	// specifically the width
	ECWMSize_width_default,
	ECWMSize_width_optimize,
	ECWMSize_width_constant,

	// specifically the height
	ECWMSize_height_default,
	ECWMSize_height_optimize,
	ECWMSize_height_constant,

	// count
	ECWMSize_COUNT
};