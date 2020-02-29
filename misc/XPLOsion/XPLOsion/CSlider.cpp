/*
Created by Nicolaus Anderson, 2012

For license terms, see CSlider.h
*/

#include "CSlider.h"

namespace irr
{
namespace gui
{

void CSlider::draw()
{
	if ( !IsVisible ) return;

	bool DrawWithDriver = false;
	video::SColor barColor = video::SColor(255,125,125,125);
	core::recti tickMark(
		AbsoluteRect.UpperLeftCorner.X,
		AbsoluteRect.UpperLeftCorner.Y,
		AbsoluteRect.UpperLeftCorner.X + (Horizontal?0:AbsoluteRect.getWidth()),
		AbsoluteRect.UpperLeftCorner.Y + (Horizontal?AbsoluteRect.getHeight():0)
		);
	core::vector2di direction( (Horizontal?1:0), (Horizontal?0:1) );


	IGUISkin* skin = Environment->getSkin();
	if ( !skin ) DrawWithDriver = true;

	video::IVideoDriver* vid = Environment->getVideoDriver();

	// Shape the slider rectangle based on its orientation
	if ( Horizontal )
	{
		SliderRect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X + ActualPos;
		SliderRect.LowerRightCorner.X = SliderRect.UpperLeftCorner.X + ConstantSliderWidth;

		SliderRect.UpperLeftCorner.Y = AbsoluteRect.UpperLeftCorner.Y;
		SliderRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	} else {
		SliderRect.UpperLeftCorner.Y = AbsoluteRect.UpperLeftCorner.Y + ActualPos;
		SliderRect.LowerRightCorner.Y = SliderRect.UpperLeftCorner.Y + ConstantSliderWidth;

		SliderRect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X;
		SliderRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	}


	// Draw the background
	vid->draw2DRectangle( BackgroundColor, AbsoluteRect, &AbsoluteClippingRect);


	// Draw the tick marks ********************

	s32 tkp = 0; // tick point

		// Large ticks first
	if ( ShowLargeTicks )
	for ( tkp = 0 ; tkp <= (Max-Min)/LargeStep; tkp++ )
	{
		vid->draw2DLine( tickMark.UpperLeftCorner, tickMark.LowerRightCorner, TickColor);

		if ( Horizontal )
			tickMark += direction*LargeStep*(RelativeRect.getWidth() - ConstantSliderWidth)/(Max-Min);
		else
			tickMark += direction*LargeStep*(RelativeRect.getHeight() - ConstantSliderWidth)/(Max-Min);
	}

		// Reshape the tick mark - squeeze it together
	if ( Horizontal )
	{
		// squeeze
		tickMark.UpperLeftCorner.Y += AbsoluteRect.getHeight()/4;
		tickMark.LowerRightCorner.Y -= AbsoluteRect.getHeight()/4;

		// reset position
		tickMark.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X;
		tickMark.LowerRightCorner.X = AbsoluteRect.UpperLeftCorner.X;
	} else {
		// squeeze
		tickMark.UpperLeftCorner.X += AbsoluteRect.getWidth()/4;
		tickMark.LowerRightCorner.X -= AbsoluteRect.getWidth()/4;

		// reset position
		tickMark.UpperLeftCorner.Y = AbsoluteRect.UpperLeftCorner.Y;
		tickMark.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y;
	}

		// Small tick marks next
	if ( ShowSmallTicks )
	for ( tkp = 0 ; tkp <= (Max-Min)/SmallStep; tkp++ )
	{
		vid->draw2DLine( tickMark.UpperLeftCorner, tickMark.LowerRightCorner, TickColor);

		if ( Horizontal )
			tickMark += direction*SmallStep*(RelativeRect.getWidth() - ConstantSliderWidth)/(Max-Min);
		else
			tickMark += direction*SmallStep*(RelativeRect.getHeight() - ConstantSliderWidth)/(Max-Min);
	}


	// Draw the slider bar ***********************

	// Use the driver directly if there is no skin
	if ( DrawWithDriver )
	{
		vid->draw2DRectangle( barColor, SliderRect, &AbsoluteClippingRect);
	} else {
		skin->draw2DRectangle(this, skin->getColor(EGDC_SCROLLBAR), SliderRect, &AbsoluteClippingRect);
	}
}


// Function contents (c) Nikolaus Gebhardt
// Altered here and there by Nicolaus Anderson
//! called if an event happened.
bool CSlider::OnEvent(const SEvent& event)
{
	if (IsEnabled)
	{

		switch(event.EventType)
		{
		case EET_KEY_INPUT_EVENT:
			if (event.KeyInput.PressedDown)
			{
				const s32 oldPos = Pos;
				bool absorb = true;
				switch (event.KeyInput.Key)
				{
				case KEY_LEFT:
				case KEY_UP:
					setPos(Pos-SmallStep);
					break;
				case KEY_RIGHT:
				case KEY_DOWN:
					setPos(Pos+SmallStep);
					break;
				case KEY_HOME:
					setPos(Min);
					break;
				case KEY_PRIOR:
					setPos(Pos-LargeStep);
					break;
				case KEY_END:
					setPos(Max);
					break;
				case KEY_NEXT:
					setPos(Pos+LargeStep);
					break;
				default:
					absorb = false;
				}

				if (Pos != oldPos)
				{
					SEvent newEvent;
					newEvent.EventType = EET_GUI_EVENT;
					newEvent.GUIEvent.Caller = this;
					newEvent.GUIEvent.Element = 0;
					newEvent.GUIEvent.EventType = EGET_SCROLL_BAR_CHANGED;
					Parent->OnEvent(newEvent);
				}
				if (absorb)
					return true;
			}
			break;
		case EET_GUI_EVENT:
			// Note: button section erased
			if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
			{
				if (event.GUIEvent.Caller == this)
					Dragging = false;
			}
			break;
		case EET_MOUSE_INPUT_EVENT:
		{
			const core::position2di p(event.MouseInput.X, event.MouseInput.Y);
			bool isInside = isPointInside ( p );
			switch(event.MouseInput.Event)
			{
			case EMIE_MOUSE_WHEEL:
				if (Environment->hasFocus(this))
				{
					// thanks to a bug report by REAPER
					// thanks to tommi by tommi for another bugfix
					// everybody needs a little thanking. hallo niko!;-)
					setPos(	getPos() +
							( (event.MouseInput.Wheel < 0 ? -1 : 1) * SmallStep * (Horizontal ? 1 : -1 ) )
							);

					SEvent newEvent;
					newEvent.EventType = EET_GUI_EVENT;
					newEvent.GUIEvent.Caller = this;
					newEvent.GUIEvent.Element = 0;
					newEvent.GUIEvent.EventType = EGET_SCROLL_BAR_CHANGED;
					Parent->OnEvent(newEvent);
					return true;
				}
				break;
			case EMIE_LMOUSE_PRESSED_DOWN:
			{
				if (isInside)
				{
					Dragging = true;
					DraggedBySlider = SliderRect.isPointInside(p);
					/* This is the offset from the top of the slider itself
					that I (Anderson) use for identifying change in position. */
					OffsetPos = getPosFromMousePos(p) - ActualPos;
					Environment->setFocus ( this );
					return true;
				}
				break;
			}
			case EMIE_LMOUSE_LEFT_UP:
			case EMIE_MOUSE_MOVED:
			{
				if ( !event.MouseInput.isLeftPressed () )
					Dragging = false;

				if ( !Dragging )
				{
					if ( event.MouseInput.Event == EMIE_MOUSE_MOVED )
						break;
					return isInside;
				}

				if ( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
					Dragging = false;

				// Exact positions, not a percentage as for scrollbars
				const s32 newPos = getPosFromMousePos(p);
				const s32 oldPos = ActualPos; // Changed for using exact position

				if (!DraggedBySlider)
				{
					if ( isInside )
					{
						DraggedBySlider = SliderRect.isPointInside(p);
					}

					if (DraggedBySlider)
					{
						// Altered for my purposes - I (Anderson) only want the shift, not a snap
						setActualPos( newPos - OffsetPos );
					}
					else
					{
						if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
							return isInside;
					}
				}

				if (DraggedBySlider)
				{
					// Altered for my purposes - I (Anderson) only want the shift, not a snap
					setActualPos( newPos - OffsetPos );
				}

				if (ActualPos != oldPos && Parent)
				{
					SEvent newEvent;
					newEvent.EventType = EET_GUI_EVENT;
					newEvent.GUIEvent.Caller = this;
					newEvent.GUIEvent.Element = 0;
					newEvent.GUIEvent.EventType = EGET_SCROLL_BAR_CHANGED;
					Parent->OnEvent(newEvent);
				}
				return isInside;
			} break;

			default:
				break;
			}
		} break;
		default:
			break;
		}
	}

	return IGUIElement::OnEvent(event);
}


s32 CSlider::getPosFromMousePos(const core::position2di &pos) const
{
	if ( Horizontal )
		return pos.X - AbsoluteRect.UpperLeftCorner.X;
	else
		return pos.Y - AbsoluteRect.UpperLeftCorner.Y;
}


void CSlider::serializeAttributes(
		irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options
		) const
{
	IGUIElement::serializeAttributes( out, options );

	// Usual attributes
	out->addBool("Horizontal",	Horizontal);
	out->addInt ("Value",		Pos);
	out->addInt ("Min",			Min);
	out->addInt ("Max",			Max);
	out->addInt ("SmallStep",	SmallStep);
	out->addInt ("LargeStep",	LargeStep);

	// Unique attributes
	out->addInt(	"SliderWidth",		ConstantSliderWidth	);
	out->addColor(	"BGColor",			BackgroundColor		);
	out->addColor(	"TickColor",		TickColor			);
	out->addBool(	"ShowSmallTicks",	ShowSmallTicks		);
	out->addBool(	"ShowLargeTicks",	ShowLargeTicks		);
}


void CSlider::deserializeAttributes(
				irr::io::IAttributes *in,
				irr::io::SAttributeReadWriteOptions *options
				)
{
	IGUIElement::deserializeAttributes( in, options );

		// Unique attributes
	ConstantSliderWidth = in->getAttributeAsInt( "SliderWidth" ); // MUST PRECEDE "Value"

	if ( ConstantSliderWidth <= 0 ) // It needs to exist
		ConstantSliderWidth = 10;

	BackgroundColor = in->getAttributeAsColor(	"BGColor"			);
	TickColor		= in->getAttributeAsColor(	"TickColor"			);
	ShowSmallTicks	= in->getAttributeAsBool(	"ShowSmallTicks"	);
	ShowLargeTicks	= in->getAttributeAsBool(	"ShowLargeTicks"	);

	// Usual attributes
	Horizontal = in->getAttributeAsBool("Horizontal");
	setMin(in->getAttributeAsInt("Min"));
	setMax(in->getAttributeAsInt("Max"));
	setPos(in->getAttributeAsInt("Value"));
	setSmallStep(in->getAttributeAsInt("SmallStep"));
	setLargeStep(in->getAttributeAsInt("LargeStep"));
}

} // end namespace gui
} // end namespace irr
