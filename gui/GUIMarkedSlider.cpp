// Copyright 2018 Nicolaus Anderson

#include "GUIMarkedSlider.h"
#include <irrMath.h>
#include <SColor.h>
#include <IVideoDriver.h>
#include <IGUIFont.h>
#include <IGUISkin.h>
#include <IGUIEnvironment.h>

namespace irr {
namespace gui {

GUIMarkedSlider::GUIMarkedSlider(
	bool  vertical,
	f32  sliderSize,
	core::rect<s32>  rectangle,
	IGUIEnvironment*  environment,
	IGUIElement*  parent,
	s32  id
)
	: IGUIElement( /*EGUIET_SCROLLBAR*/ EGUIET_ELEMENT , environment, parent, id, rectangle )
	, MinValue(0)
	, MaxValue(100.f)
	, CurrentValue(0)
	, DrawFrame(0)
	, DrawTicks(false)
	, DrawNumbers(false)
	, NumberSpacing(10.f)
	, IsVertical( vertical )
	, SliderRadius( sliderSize / 2 )
	, SliderSelected(false)
	, StartMousePos(0)
	, SliderTexture(0)
{
	updateImageCache();
	Environment->getVideoDriver();
}

bool GUIMarkedSlider::OnEvent( const SEvent&  event ) {
	if ( event.EventType == EET_MOUSE_INPUT_EVENT ) {
		switch ( event.MouseInput.Event ) {
		case EMIE_LMOUSE_PRESSED_DOWN:
			if ( isInSliderArea( event.MouseInput.X, event.MouseInput.Y ) ) {
				SliderSelected = true;
				StartMousePos.set( event.MouseInput.X, event.MouseInput.Y );
			} else {
				if ( IsVertical ) {
					setValue( event.MouseInput.Y - AbsoluteRect.UpperLeftCorner.Y - SliderRadius*2 );
				} else {
					setValue( event.MouseInput.X - AbsoluteRect.UpperLeftCorner.X - SliderRadius*2 );
				}
			}
			return true;

		case EMIE_LMOUSE_LEFT_UP:
			if ( Environment->getFocus() == this ) {
				SliderSelected = false;
				return true;
			}
			break;

		case EMIE_MOUSE_MOVED:
			if ( SliderSelected ) {
				if ( IsVertical ) {
					setValue( event.MouseInput.Y - StartMousePos.Y );
				} else {
					setValue( event.MouseInput.X - StartMousePos.X );
				}
				return true;
			}
			break;

		default: break;
		}
	}
	return false;
}

void GUIMarkedSlider::setMinValue( f32 value ) {
	MinValue = value;
	if ( MinValue > MaxValue )
		MaxValue = MinValue;
}

f32 GUIMarkedSlider::getMinValue() {
	return MinValue;
}

void GUIMarkedSlider::setMaxValue( f32 value ) {
	MaxValue = value;
	if ( MaxValue < MinValue )
		MinValue = MaxValue;
}

f32 GUIMarkedSlider::getMaxValue() {
	return MaxValue;
}

void GUIMarkedSlider::setValue( f32 value ) {
	// Odd as it is, to save a headache in drawing, we simply treat the CurrentValue
	// as the negative of whatever the user wants
	CurrentValue = core::clamp( MaxValue - value, MinValue, MaxValue );
}

f32 GUIMarkedSlider::getValue() {
	// Odd as it is, to save a headache in drawing, we simply treat the CurrentValue
	// as the negative of whatever the user wants
	return -CurrentValue;
}

void GUIMarkedSlider::setDrawFrame( bool yes ) {
	DrawFrame = yes;
}

void GUIMarkedSlider::setDrawTicks( bool yes ) {
	DrawTicks = yes;
}

void GUIMarkedSlider::setDrawNumbers( bool yes ) {
	DrawNumbers = yes;
}

void GUIMarkedSlider::setNumberSpacing( f32 distance ) {
	NumberSpacing = distance;
}

void GUIMarkedSlider::setSliderSize( f32 sliderSize ) {
	SliderRadius = sliderSize / 2;
	updateImageCache();
}

bool GUIMarkedSlider::isInSliderArea( s32 x, s32 y ) {
	// Create a rectangle formed by the upper left corner of the absolute position of this element
	// and having a size that is the same as the slider radius times 2
	core::recti r(
		AbsoluteRect.UpperLeftCorner,
		core::dimension2du(SliderRadius*2,SliderRadius*2)
	);
	return r.isPointInside( core::vector2di(x,y) );
}

void GUIMarkedSlider::updateImageCache() {

	video::IVideoDriver*  vid = Environment->getVideoDriver();
	video::SColor  lightColor = Environment->getSkin()->getColor( EGDC_3D_LIGHT );
	video::SColor  shadowColor = Environment->getSkin()->getColor( EGDC_3D_SHADOW );
	video::SColor  textColor = Environment->getSkin()->getColor( EGDC_BUTTON_TEXT );
	core::dimension2du  imageSize( (u32)SliderRadius * 2, (u32)SliderRadius * 2 );
	core::vector2di  sliderSpot( (s32)SliderRadius );

	if ( ! SliderTexture )
		SliderTexture = vid->addRenderTargetTexture( imageSize, "GUI_MARKED_SLIDER_TEXTURE" );

	if ( SliderTexture ) {
		vid->setRenderTarget( SliderTexture );
		vid->draw2DPolygon( sliderSpot, (s32)SliderRadius, shadowColor, 40 );
		vid->draw2DPolygon( core::vector2di( (s32)SliderRadius - 1 ), (s32)SliderRadius, lightColor, 40 );
		vid->draw2DPolygon( core::vector2di( (s32)SliderRadius - 2 ), (s32)SliderRadius, shadowColor, 40 );
		sliderSpot.Y = 3; // Avoid drawing over the other bands
		vid->draw2DLine( sliderSpot, core::vector2di(sliderSpot.X, (s32)imageSize.Height - 3), textColor );
	}
	vid->setRenderTarget(0);
}

void GUIMarkedSlider::draw() {
	if ( ! SliderTexture ) {
		// ERROR, so just draw children
		IGUIElement::draw();
		return;
	}

	// Note to self:
	// It might be faster to precalculate components to be drawn to the screen and then
	// go through a list of them and use a switch to decide how to draw them.

	video::IVideoDriver*  vid = Environment->getVideoDriver();
	IGUISkin*  skin = Environment->getSkin();
	video::SColor  lightColor = skin->getColor( EGDC_3D_LIGHT );
	video::SColor  shadowColor = skin->getColor( EGDC_3D_SHADOW );
	video::SColor  textColor = skin->getColor( EGDC_BUTTON_TEXT );
	video::SColor  faceColor = skin->getColor( EGDC_3D_FACE );
	s32  halfTextHeight = skin->getFont()->getKerningHeight() / 2;

	core::vector2di  drawStart, drawEnd, drawSave, drawSave2; // drawSaves are temporary storage variables

	s32  halfElemWidth = AbsoluteRect.getWidth() / 2;
	s32  halfElemHeight = AbsoluteRect.getHeight() / 2;
	core::recti  SliderTextureRect( SliderTexture->getSize() );
	s32  sSliderRadius = (s32)SliderRadius;
	f32  drawValue;
	f32  drawSpacing;
	f32  lineIndex;
	core::recti  numberTextRect;

	if ( DrawFrame ) {
		skin->draw3DSunkenPane(this, faceColor, false, true, AbsoluteRect, &AbsoluteClippingRect);
	}

	if ( IsVertical ) {
		drawSpacing = NumberSpacing * ((halfElemWidth - SliderRadius) * 2) / (MaxValue - MinValue + 0.00001);

		drawStart.X = AbsoluteRect.UpperLeftCorner.X + halfElemWidth;
		drawStart.Y = AbsoluteRect.UpperLeftCorner.Y + sSliderRadius;
		drawEnd.X = AbsoluteRect.UpperLeftCorner.X + halfElemWidth;
		drawEnd.Y = AbsoluteRect.LowerRightCorner.Y - sSliderRadius;

		vid->draw2DLine( drawStart, drawEnd, lightColor );
		drawStart.X += 1;
		drawEnd.X += 1;
		vid->draw2DLine( drawStart, drawEnd, shadowColor );

		if ( DrawTicks ) {
			// Ticks on the ends
			drawStart.X -= sSliderRadius + 1;
			drawEnd.X += sSliderRadius + 1;
			drawSave.Y = drawEnd.Y;
			drawEnd.Y = drawStart.Y;
			vid->draw2DLine( drawStart, drawEnd, lightColor );

			drawStart.Y = drawSave.Y;
			drawEnd.Y = drawSave.Y;
			vid->draw2DLine( drawStart, drawEnd, lightColor );

			// Draw in reverse-numeric order (since we're always working upside-down)
			drawSave.X = drawStart.X;
			drawSave2.X = drawEnd.X;
			drawSave.Y = drawSave2.Y = drawEnd.Y - (s32)drawSpacing;
			for ( lineIndex = 0; drawSave.Y > drawStart.Y; ++lineIndex ) {
				vid->draw2DLine( drawSave, drawSave2, textColor );
				drawSave.Y = drawSave2.Y = drawEnd.Y - (s32)(drawSpacing * lineIndex);
			}
		}

		if ( DrawNumbers ) {
			// Draw in bottom->up (since we're always working upside-down)
			drawValue = MinValue;
			drawStart.Y = AbsoluteRect.UpperLeftCorner.Y + sSliderRadius;
			drawEnd.Y = AbsoluteRect.LowerRightCorner.Y - sSliderRadius;
			drawSave.X = AbsoluteRect.LowerRightCorner.X - halfElemWidth + 1; // Numbers go to the right of the line
			drawSave.Y = drawEnd.Y - halfTextHeight;
			drawSave2.X = AbsoluteRect.LowerRightCorner.X;
			drawSave2.Y = drawEnd.Y + halfTextHeight;

			numberTextRect = core::recti( drawSave, drawSave2 );

			for ( lineIndex = 0; drawSave.Y > drawStart.Y; ++lineIndex ) {
				const core::stringw numberText( drawValue );
				skin->getFont()->draw( numberText.c_str(), numberTextRect, textColor, false, false, &AbsoluteClippingRect );
				drawValue - NumberSpacing;
				numberTextRect = core::recti( drawSave, drawSave2 );
				numberTextRect += core::vector2di(0, - (s32)( drawSpacing * lineIndex ) );
			}

			// Draw the last number separately so that it's correctly lined up
			drawSave.Y = drawStart.Y - halfTextHeight;
			drawSave2.Y = drawStart.Y + halfTextHeight;
			numberTextRect = core::recti(drawSave, drawSave2);
			const core::stringw numberTextFinal( MinValue );
			skin->getFont()->draw( numberTextFinal.c_str(), numberTextRect, textColor, false, false, &AbsoluteClippingRect );
		}

		// Draw the slider marker last
		drawStart.set( halfElemWidth - sSliderRadius, CurrentValue - sSliderRadius );
		vid->draw2DImage( SliderTexture, AbsoluteRect.UpperLeftCorner + drawStart, SliderTextureRect, &AbsoluteClippingRect, video::SColor(-1), true );
	} else {
		// Horizontal Slider

		drawSpacing = NumberSpacing * ((halfElemHeight - SliderRadius) * 2) / (MaxValue - MinValue + 0.00001);

		drawStart.X = AbsoluteRect.UpperLeftCorner.X + sSliderRadius;
		drawStart.Y = AbsoluteRect.UpperLeftCorner.Y + halfElemHeight;
		drawEnd.X = AbsoluteRect.LowerRightCorner.X - sSliderRadius;
		drawEnd.Y = AbsoluteRect.UpperLeftCorner.Y + halfElemHeight;

		vid->draw2DLine( drawStart, drawEnd, lightColor );
		drawStart.Y += 1;
		drawEnd.Y += 1;
		vid->draw2DLine( drawStart, drawEnd, shadowColor );

		if ( DrawTicks ) {
			// Ticks on the ends
			drawStart.Y -= sSliderRadius + 1;
			drawEnd.Y += sSliderRadius + 1;
			drawSave.X = drawEnd.X;
			drawEnd.X = drawStart.X;
			vid->draw2DLine( drawStart, drawEnd, lightColor );

			drawStart.X = drawSave.X;
			drawEnd.X = drawSave.X;
			vid->draw2DLine( drawStart, drawEnd, lightColor );

			// Draw in reverse-numeric order (since we're always working upside-down)
			drawSave.Y = drawStart.Y;
			drawSave2.Y = drawEnd.Y;
			drawSave.X = drawSave2.X = drawEnd.X - drawSpacing;
			for ( lineIndex = 0; drawSave.X > drawStart.X; ++lineIndex ) {
				vid->draw2DLine( drawSave, drawSave2, lightColor );
				drawSave.X = drawSave2.X = drawEnd.X - drawSpacing * lineIndex;
			}
		}

		if ( DrawNumbers ) {
			// Draw in left->right
			drawValue = MinValue; 
			drawEnd.X = AbsoluteRect.LowerRightCorner.X - sSliderRadius;
			drawSave.X = AbsoluteRect.UpperLeftCorner.Y + sSliderRadius;
			drawSave.Y = AbsoluteRect.LowerRightCorner.Y + halfElemHeight + sSliderRadius * 2 + 1;
			drawSave2.X = drawStart.X + drawSpacing;
			drawSave2.Y = drawSave.Y + halfTextHeight * 2; // We'll let the compiler optimize *2 to <<2

			numberTextRect = core::recti( drawSave, drawSave2 );

			for ( lineIndex = 0; drawSave.X < drawEnd.X; ++lineIndex ) {
				const core::stringw numberText( drawValue );
				skin->getFont()->draw( numberText.c_str(), numberTextRect, textColor, false, false, &AbsoluteClippingRect );
				drawValue + NumberSpacing;
				numberTextRect = core::recti( drawSave, drawSave2 );
				numberTextRect += core::vector2di( (s32)( drawSpacing * lineIndex ), 0 );
			}

			// Draw the last number separately so that it's correctly lined up
			drawSave.X = drawEnd.X;
			drawSave2.X = AbsoluteRect.LowerRightCorner.X;
			numberTextRect = core::recti(drawSave, drawSave2);
			const core::stringw numberTextFinal( MinValue );
			skin->getFont()->draw( numberTextFinal.c_str(), numberTextRect, textColor, false, false, &AbsoluteClippingRect );
		}

		// Draw the slider marker last
		drawStart.set( CurrentValue - sSliderRadius, halfElemHeight );
		vid->draw2DImage( SliderTexture, AbsoluteRect.UpperLeftCorner + drawStart, SliderTextureRect, &AbsoluteClippingRect, video::SColor(-1), true );
	}

	IGUIElement::draw(); // Children
}

void GUIMarkedSlider::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	IGUIElement::serializeAttributes(out,options);

	f32  MinValue;
	f32  MaxValue;
	f32  CurrentValue;
	bool  DrawFrame;
	bool  DrawTicks;
	bool  DrawNumbers;
	f32  NumberSpacing;
	bool  IsVertical;
	f32  SliderRadius;

	out->addFloat("MinValue", MinValue);
	out->addFloat("MaxValue", MaxValue);
	out->addFloat("CurrentValue", CurrentValue);
	out->addBool("DrawFrame", DrawFrame);
	out->addBool("DrawTicks", DrawTicks);
	out->addBool("DrawNumbers", DrawNumbers);
	out->addFloat("NumberSpacing", NumberSpacing);
	out->addBool("IsVertical", IsVertical);
	out->addFloat("SliderRadius", SliderRadius);
}

void GUIMarkedSlider::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	IGUIElement::deserializeAttributes(in, options);

	MinValue = in->getAttributeAsFloat("MinValue", MinValue);
	MaxValue = in->getAttributeAsFloat("MaxValue", MaxValue);
	CurrentValue = in->getAttributeAsFloat("CurrentValue", CurrentValue);
	DrawFrame = in->getAttributeAsBool("DrawFrame", DrawFrame);
	DrawTicks = in->getAttributeAsBool("DrawTicks", DrawTicks);
	DrawNumbers = in->getAttributeAsBool("DrawNumbers", DrawNumbers);
	NumberSpacing = in->getAttributeAsFloat("NumberSpacing", NumberSpacing);
	IsVertical = in->getAttributeAsBool("IsVertical", IsVertical);

	f32 sliderRadius = in->getAttributeAsFloat("SliderRadius", SliderRadius);
	if ( sliderRadius != SliderRadius )
		setSliderSize(SliderRadius*2); // Updates image cache
}

}}
