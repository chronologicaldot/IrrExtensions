// (C) 2020 Nicolaus Anderson

#include "GUIPanel.h"

namespace irr {
namespace gui {

GUIPanel::GUIPanel( IGUIEnvironment* environment, IGUIElement* parent, const core::recti& position, s32 id )
	: IGUIElement(EGUIET_ELEMENT, environment, parent, id, position)
	, DrawBackground(true)
	, BackgroundType(EGUIDT_SUNKENPANE)
	, BackgroundColor(0xff707070)
	, DrawTitleBar(false)
	, TitleBarColor(0xffc0c0c0)
{}

void GUIPanel::setDrawBackground( bool yes )
{
	DrawBackground = yes;
}

void GUIPanel::setBackgroundType( EGUIDrawType type )
{
	BackgroundType = type;
}

void GUIPanel::draw()
{
	switch(BackgroundType) {
	case EGUIDT_BUTTON:
		Environment->getSkin()->draw3DButtonPaneStandard(this, AbsoluteRect, &AbsoluteClippingRect);
		break;

	case EGUIDT_BUTTON_PRESSED:
		Environment->getSkin()->draw3DButtonPanePressed(this, AbsoluteRect, &AbsoluteClippingRect);
		break;

	case EGUIDT_SUNKENPANE:
		Environment->getSkin()->draw3DSunkenPane(this, BackgroundColor, false, true, AbsoluteRect, &AbsoluteClippingRect);
		break;

	case EGUIDT_WINDOW:
		Environment->getSkin()->draw3DWindowBackground(this, DrawTitleBar, TitleBarColor, AbsoluteRect, &AbsoluteClippingRect);
		break;

	default:
		break;
	}
}

void GUIPanel::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	IGUIElement::serializeAttributes(out, options);

	out->addBool( "DrawBackground", DrawBackground );
	out->addEnum( "BackgroundType", BackgroundType, GUIDrawTypeNames );
	out->addColor( "BackgroundColor", BackgroundColor );
	out->addBool( "DrawTitleBar", DrawTitleBar );
	out->addColor( "TitleBarColor", TitleBarColor);
}

void GUIPanel::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	IGUIElement::deserializeAttributes(in, options);

	DrawBackground = in->getAttributeAsBool( "DrawBackground", DrawBackground );
	BackgroundType = (EGUIDrawType)in->getAttributeAsEnumeration( "BackgroundType", GUIDrawTypeNames, BackgroundType );
	BackgroundColor = in->getAttributeAsColor( "BackgroundColor", BackgroundColor );
	DrawTitleBar = in->getAttributeAsBool( "DrawTitleBar", DrawTitleBar );
	TitleBarColor = in->getAttributeAsColor( "TitleBarColor", TitleBarColor );
}

}
}
