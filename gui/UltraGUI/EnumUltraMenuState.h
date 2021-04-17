/*
(c) Nicolaus Anderson
*/

#pragma once

/*
Enumeration for giving the location of a menu
(for the Ultra GUI Menu).
*/
enum EUltraMenuLocation
{
	// Left side menu
	EUGUIMLoc_Left=0,

	// Central window menu
	EUGUIMLoc_Center,

	// Right side menu
	EUGUIMLoc_Right,

	// Menu bar
	EUGUIMLoc_Bar,

	// Total available locations
	EUGUIMLoc_COUNT
};

enum EUltraMenuState
{
	// hidden completely
	EUGUIMState_Closed=0,

	// closing
	EUGUIMState_Closing,

	// open
	EUGUIMState_Open,

	// opening
	EUGUIMState_Opening,

	// becoming left-side menu
	EUGUIMState_ToLeft_Open,
	EUGUIMState_ToLeft_Closed,

	// becoming right-side menu
	EUGUIMState_ToRight_Open,
	EUGUIMState_ToRight_Closed,

	// becoming central window menu
	EUGUIMState_ToCenter_Open,
	EUGUIMState_ToCenter_Closed,

	// becoming bar menu icon
	EUGUIMState_ToBar,

	// Total number of states
	EUGUIMState_COUNT
};

// What to do when told to close
enum EUltraMenuClose
{
	// Default - hide to the side
	EUGUIMClose_Hide=0,

	// Minimize
	EUGUIMClose_Minimize,

	// Number of options
	EUGUIMClose_COUNT
};