// Copyright 2018 Nicolaus Anderson

#include "IGUIElement.h"

#ifndef __GUI_MARKED_SLIDER_H__
#define __GUI_MARKED_SLIDER_H__

namespace irr {
namespace gui {

//! Marked Slider class
/*
	Draws a long line (the number line) with marked values (below it if the line is vertical or to the right of it if the line is horizontal).
	It draws value marker that marks the current value of the slider. It can have different appearances.
		- A circle
		- (Not yet) Two arrows pointing at a short line in between them which is perpendicular to the number line.
	Greyed out when disabled.
*/
class GUIMarkedSlider : public IGUIElement {

public:
	GUIMarkedSlider(bool  vertical, f32  sliderSize, core::rect<s32>  rectangle, IGUIEnvironment*  environment, IGUIElement*  parent=0, s32  id=-1 );
	~GUIMarkedSlider();

	virtual bool OnEvent( const SEvent&  event );
	virtual void draw();

	void setMinValue( f32 );
	f32 getMinValue();
	void setMaxValue( f32 );
	f32 getMaxValue();
	void setValue( f32 );
	f32 getValue();

	void setDrawFrame( bool );
	void setDrawTicks( bool );
	void setDrawNumbers( bool );
	void setNumberSpacing( f32 ); // Distance between drawn numbers
	void setSliderSize( f32 );

	bool isInSliderArea( s32 x, s32 y );

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "markedSlider"; }

	virtual void serializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) const;
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

private:
	void updateImageCache();

	f32  MinValue;
	f32  MaxValue;
	f32  CurrentValue;
	bool  DrawFrame;
	bool  DrawTicks;
	bool  DrawNumbers;
	f32  NumberSpacing;
	bool  IsVertical;
	f32  SliderRadius;
	bool SliderSelected;
	core::vector2di  StartMousePos;
	video::ITexture*  SliderTexture;
};

}}

#endif
