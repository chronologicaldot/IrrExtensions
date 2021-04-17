/*
Project name: CSlider

Created by Nicolaus Anderson, 2012

Code copied from Nikolaus Gebhardt is copyrighted by him and subject to
the license of the irrlicht engine.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "irrMath.h"
#include "IGUIScrollBar.h"
#include "IGUISkin.h"
#include "IVideoDriver.h"
#include "IGUIEnvironment.h"

#pragma once

namespace irr
{
namespace gui
{

//! Class CSlider
/* This is merely an implementation of IGUIScrollBar with a different
face for interaction. We reuse much of Gebhardt's code to save time. */
class CSlider : public IGUIScrollBar
{
private:
	core::rect<s32> SliderRect;
	s32 ConstantSliderWidth;

	bool Dragging;
	bool Horizontal;
	bool DraggedBySlider;

	s32 Pos;
	s32 ActualPos; // Left side / Top of the slider
	s32 OffsetPos;
	s32 SmallStep;
	s32 LargeStep;
	s32 Min;
	s32 Max;
	u32 LastChange;

	bool ShowSmallTicks;
	bool ShowLargeTicks;
	f32 SmallTickSpacing;
	f32 LargeTickSpacing;

	video::SColor TickColor;
	video::SColor BackgroundColor;

public:
	//! constructor
	CSlider(
		bool horiz, // if horizontal
		IGUIEnvironment* environment,
		IGUIElement* parent,
		s32 id,
		irr::core::rect<s32> rectangle, // shape
		bool noclip
		)
		: IGUIScrollBar( environment, parent, id, rectangle ),
		Pos(0), ActualPos(0), OffsetPos(0),
		SmallStep(-1), LargeStep(-1), Min(0), Max(10),
		LastChange(0), Dragging(false), Horizontal(horiz), DraggedBySlider(false),
		ShowSmallTicks(false), ShowLargeTicks(false),
		SmallTickSpacing(0), LargeTickSpacing(0), ConstantSliderWidth(10),
		TickColor( video::SColor(255,0,0,0) )
	{
#ifdef _DEBUG
		setDebugName("CSlider");
#endif
		setNotClipped(noclip);
	}

	//! destructor
	~CSlider()
	{
	}

	//! sets the maximum value of the slider.
	void setMax(s32 max)
	{
		Max = max;
	}
	//! gets the maximum value of the slider.
	s32 getMax() const
	{
		return Max;
	}

	//! sets the minimum value of the slider.
	void setMin(s32 min)
	{
		Min = min;
	}
	//! gets the minimum value of the slider.
	s32 getMin() const
	{
		return Min;
	}

	//! gets the small step value
	s32 getSmallStep() const
	{
		return SmallStep;
	}

	//! Sets the small step
	/** That is the amount that the value changes by when clicking
	on the bar near the current slider position.
	Default is -1, meaning exact snap.
	Set the step if you want the slider to move in integer values only. */
	void setSmallStep(s32 step)
	{
		SmallStep = step;
	}

	//! gets the large step value
	s32 getLargeStep() const
	{
		return LargeStep;
	}

	//! Sets the large step
	/** That is the amount that the value changes by when clicking
	on the bar far away from the current slider position.
	Default is -1, meaning exact snap.
	Set the step if you want the slider to move in integer values only. */
	void setLargeStep(s32 step)
	{
		LargeStep = step;
	}

	//! gets the current position of the slider
	s32 getPos() const
	{
		return Pos;
	}

	//! sets the current position of the slider
	void setPos(s32 pos)
	{
		Pos = core::s32_clamp(pos,Min,Max);

		if ( Horizontal )
			ActualPos = (Max>Min?(Pos-Min)*(RelativeRect.getWidth()-ConstantSliderWidth)/(Max-Min):0);
		else
			ActualPos = (Max>Min?(Pos-Min)*(RelativeRect.getHeight()-ConstantSliderWidth)/(Max-Min):0);
	}

	void setActualPos(s32 pos)
	{
		if ( Horizontal )
		{
			ActualPos = core::s32_clamp(pos,0,RelativeRect.getWidth()-ConstantSliderWidth);
			Pos = (Max-Min)*ActualPos/(RelativeRect.getWidth()-ConstantSliderWidth) + Min;
		} else {
			ActualPos = core::s32_clamp(pos,0,RelativeRect.getHeight()-ConstantSliderWidth);
			Pos = (Max-Min)*ActualPos/(RelativeRect.getHeight()-ConstantSliderWidth) + Min;
		}
	}

		//! Get the actual position
	s32 getActualPos()
	{
		return ActualPos;
	}

		//! Get percentage/100
	/* Returns the fraction of the distance the slider has traveled along
	the available pathway.
	This can be more useful to have than the position %, which is always between
	0 and 100. */
	f32 getPercentTraveled()
	{
		if ( Horizontal )
			return (f32)ActualPos/(f32)(RelativeRect.getWidth()-ConstantSliderWidth);

		return (f32)ActualPos/(f32)(RelativeRect.getHeight()-ConstantSliderWidth);
	}

		//! Draw
	/* For drawing the division. This is called by the engine. */
	void draw();

		//! On Event
	/* Response to user input events. */
	bool OnEvent( const irr::SEvent& event );

		//! Get position from mouse position
	/* Returns the position of the slider region that the mouse is
	currently hovering over. */
	s32 getPosFromMousePos(const core::position2di &pos) const;

		//! Serialize
	/* For obtaining the key attributes of this GUI element. */
	void serializeAttributes(
		irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options=0
		) const;

		//! Deserialize
	/* For defining this GUI element from key attributes. */
	void deserializeAttributes(
		irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options=0
		);
};

} // end namespace gui
} // end namespace irr