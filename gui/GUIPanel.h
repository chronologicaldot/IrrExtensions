// (C) 2020 Nicolaus Anderson
/*
	A convenient GUI element whose standard appearance can be changed to mimic that of other elements.
*/

#ifndef GUI_PANEL_H
#define GUI_PANEL_H

#include <IGUIElement.h>
#include <IGUIEnvironment.h>
#include <IGUISkin.h>

namespace irr {
namespace gui {

enum EGUIDrawType {
	EGUIDT_BUTTON,
	EGUIDT_BUTTON_PRESSED,
	EGUIDT_SUNKENPANE,
	EGUIDT_WINDOW,
};

const c8* const GUIDrawTypeNames[] = {
	"button",
	"button pressed",
	"sunken pane",
	"window",
	0
};

struct GUIPanel : public IGUIElement
{
	GUIPanel( IGUIEnvironment* environment, IGUIElement* parent, const core::recti& position, s32 id=-1 );

	void setDrawBackground( bool yes );
	void setBackgroundType( EGUIDrawType );
	void draw();

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "panel"; }

	void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;
	void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

protected:
	bool			DrawBackground;
	EGUIDrawType	BackgroundType;
	video::SColor	BackgroundColor;
	bool			DrawTitleBar; // Only applies when drawing as as Window
	video::SColor	TitleBarColor;
};

}
}

#endif
