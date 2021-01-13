// (c) 2015 Nicolaus Anderson

#ifndef GUI_S_PROGRESS_BAR_H
#define GUI_S_PROGRESS_BAR_H

#include <IGUIElement.h>
#include <IGUISkin.h>

namespace irr {
namespace gui {

class GUISProgressBar : public IGUIElement
{
	f32 value;
	bool horiz;
	video::SColor backColor;

public:
	GUISProgressBar( IGUIEnvironment* pEnv, IGUIElement* pParent, core::recti pRect, s32 id=-1,
					f32 pStartValue=0, bool pHorizontal=true )
		: IGUIElement( EGUIET_ELEMENT, pEnv, pParent, id, pRect )
		, value( pStartValue )
		, horiz( pHorizontal )
		, backColor( 0xff222222 )
	{}

	void setProgress( f32 pPercent )
	{
		value = pPercent;
	}

	void setBackgroundColor( video::SColor pBackColor )
	{
		backColor = pBackColor;
	}

	virtual void draw()
	{
		Environment->getSkin()->draw3DSunkenPane(this, backColor, true, true, AbsoluteRect, &AbsoluteClippingRect );
		core::recti bar(AbsoluteRect);
		if ( horiz )
		{
			bar.LowerRightCorner.X = AbsoluteRect.UpperLeftCorner.X + (s32)(value * RelativeRect.getWidth());
		} else {
			bar.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + (s32)(value * RelativeRect.getHeight());
		}
		Environment->getSkin()->draw3DButtonPaneStandard(this, bar, &AbsoluteClippingRect);
	}

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "sProgressBar"; }

	virtual void serializeAttributes( irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options )
	{
		IGUIElement::serializeAttributes(out,options);
		out->addFloat("Progress", value);
		out->addColor("BackgroundColor", backColor);
	}

	virtual void deserializeAttributes( irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options )
	{
		IGUIElement::serializeAttributes(in,options);
		setProgress( in->getAttributeAsFloat("Progress",value) );
		setBackgroundColor( in->getAttributeAsColor("BackgroundColor") );
	}
};

}}


#endif // #ifndef GUI_S_PROGRESS_BAR_H
