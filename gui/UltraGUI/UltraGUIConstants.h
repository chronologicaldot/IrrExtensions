/*
(c) Nicolaus Anderson
*/

/* A list of constants used for UltraGUI.
Feel free to modify these constants to suit your purposes. */

/* Resouce path function.
This is the default resource path option. It can be changed here
to change the default, or you can pass to the constructor a resource
path. */
#ifndef ULTRAGUI_RESOURCE_PATH
#define ULTRAGUI_RESOURCE_PATH irr::io::path("../resources/art/")
#endif

/* Button imagery. Appended to the resource path in order to obtain
the imagery for display on the buttons. */
#define ULTRAGUI_RESOURCE_CLOSEBUTTON "closeButton.png"
#define ULTRAGUI_RESOURCE_TOLEFTBUTTON "toLeftButton.png"
#define ULTRAGUI_RESOURCE_TORIGHTBUTTON "toRightButton.png"
#define ULTRAGUI_RESOURCE_TOCENTERBUTTON "toCenterButton.png"
#define ULTRAGUI_RESOURCE_MINIMIZEBUTTON "minimizeButton.png"

#define ULTRAGUI_RESOURCE_NOHOVERTEXTURE ""
#define ULTRAGUI_RESOURCE_HOVERTEXTURE_0 "hoverTex0.png"
#define ULTRAGUI_RESOURCE_HOVERTEXTURE_1 "hoverTex1.png"
#define ULTRAGUI_RESOURCE_HOVERTEXTURE_2 "hoverTex2.png"
#define ULTRAGUI_RESOURCE_HOVERTEXTURE_3 "hoverTex3.png"

/* Size of an UltraGUIMenu as it appears on the menu bar
of the UltraGUIFrame: */
#define ULTRAGUI_DEFAULT_MENUBAR_BUTTON_WIDTH 80
#define ULTRAGUI_DEFAULT_MENUBAR_BUTTON_HEIGHT 30

/* Spacing between a UltraGUIMenu bar and the edge of the menu bar
of UltraGUIFrame: */
#define ULTRAGUI_MENUBAR_PADDING 4

#define ULTRAGUI_DEFAULT_CENTRAL_MENU_WIDTH 500
#define ULTRAGUI_DEFAULT_CENTRAL_MENU_HEIGHT 400

#define ULTRAGUI_DEFAULT_SIDE_MENU_WIDTH 250
#define ULTRAGUI_DEFAULT_SIDE_MENU_HEIGHT 500

#define ULTRAGUI_DEFAULT_MOVE_SPEED 0.005f

/* Default header information: */
#define ULTRAGUI_DEFAULT_MENU_HEADER_HEIGHT 30

// Plain header colors (easier to read text on)
//#define ULTRAGUI_DEFAULT_HEADER_COLOR_TOP irr::video::SColor(255,0,100,155)
//#define ULTRAGUI_DEFAULT_HEADER_COLOR_MID irr::video::SColor(255,0,50,95)
//#define ULTRAGUI_DEFAULT_HEADER_COLOR_BOTTOM irr::video::SColor(255,0,70,135)

// Slick header colors
#define ULTRAGUI_DEFAULT_HEADER_COLOR_TOP irr::video::SColor(255,140,180,205)
#define ULTRAGUI_DEFAULT_HEADER_COLOR_MID irr::video::SColor(255,0,50,95)
#define ULTRAGUI_DEFAULT_HEADER_COLOR_BOTTOM irr::video::SColor(255,20,110,165)

/* Size of the width or height of a button on an UltraGUIMenu.
For now, set to match small texture size (16x16) to load without image
manipulation or engine modification. */
#define ULTRAGUI_DEFAULT_MENU_BUTTON_SIZE 16

// Button hover color
#define ULTRAGUI_DEFAULT_MENU_BUTTON_HOVERCOLOR irr::video::SColor( 255,255,255,255 )
//irr::video::SColor( 155,40,200,255 )

/* The spacing between the first button of an UltraGUIMenu
and the right side of the menu. */
#define ULTRAGUI_BUTTON_RIGHT_PADDING 5

/* The time (in milliseconds) that the menus are highlighted. */
#define ULTRAGUI_HIGHLIGHT_TIME 1000