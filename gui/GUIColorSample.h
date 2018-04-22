// (c) 2014 Nicolaus Anderson

#ifndef GUI_COLOR_SAMPLE_H
#define GUI_COLOR_SAMPLE_H

#include <IGUIElement.h>
#include <IGUIEnvironment.h>

namespace irr {
namespace gui {

using core::recti;
using video::SColor;

/* Just a simple square element that shows one or more
colors, depending on its setting. */
class GUIColorSample : public IGUIElement
{
	SColor upperLeft;
	SColor upperRight;
	SColor lowerLeft;
	SColor lowerRight;

	bool drawBorder;
	s32 borderWidth;
	recti colorArea;
	
	/* Note: set enabled to "true" if you want to be able to set this element
	to the focus. */

public:
	GUIColorSample( IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id=-1 );

	void setDrawBorder(bool yes);
	virtual void updateAbsolutePosition();

	void showOneColor( SColor pColor );
	void showTwoColors( SColor pTopLeft, SColor pBottomRight, bool horizontal );
	void showFourColors( SColor pUpperLeft, SColor pUpperRight, SColor pLowerLeft, SColor pLowerRight );

	virtual bool OnEvent( const SEvent& event );
	virtual void draw();
};

}}

#endif // #ifndef GUI_COLOR_SAMPLE_H
