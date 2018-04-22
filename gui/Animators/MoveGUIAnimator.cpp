/*
(c) 2013 Nicolaus Anderson
*/

#include "MoveGUIAnimator.h"

#ifndef _MOVE_GUI_ANIMATOR_CPP_
#define _MOVE_GUI_ANIMATOR_CPP_

namespace irr
{
namespace gui
{

MoveGUIAnimator::MoveGUIAnimator(
				core::recti* rectangle,
				f64 speed,
				u32 startTimeMs
				)
	: ElemRect( rectangle )
	, OrigRect( *rectangle )
	, TargetRect( *rectangle )
	, transitionSpeed( speed )
	, startTime( startTimeMs )
{
}

MoveGUIAnimator::~MoveGUIAnimator()
{}

void MoveGUIAnimator::setStartTime( u32 timeMs )
{
	if ( inMotion )
		return;

	startTime = timeMs;
}

void MoveGUIAnimator::OnPostRender( u32 timeMs, IGUIElement* element )
{
	// This animator must be started first.
	if ( startTime > timeMs )
	{
		inMotion = false;
		return;
	}

	if ( *ElemRect == TargetRect )
	{
		inMotion = false;
		OrigRect = TargetRect;
		return;
	}

	inMotion = true;

	/* Obtain the transition delta for the rectangle based on start time.
	Note that the direction of interpolate() is backwards, so the transition
	distance is subtracted from 1.0 rather than being used on its own. */
	irr::f64 delta =
			1.0 - irr::core::clamp(
				((irr::f64)(timeMs - startTime)*transitionSpeed),
				0.0, 1.0
				);

	ElemRect->UpperLeftCorner.interpolate(
		OrigRect.UpperLeftCorner,
		TargetRect.UpperLeftCorner,
		delta
		);

	ElemRect->LowerRightCorner.interpolate(
		OrigRect.LowerRightCorner,
		TargetRect.LowerRightCorner,
		delta
		);
}

void MoveGUIAnimator::setTargetRect( core::rect<s32> newTarget )
{
	OrigRect = *ElemRect;
	TargetRect = newTarget;
}

void MoveGUIAnimator::setImmediateTarget( core::rect<s32> newTarget, u32 timeMs )
{
	OrigRect = *ElemRect;
	TargetRect = newTarget;
	startTime = timeMs;
}

}} // end namespaces gui and irr

#endif // #ifndef _MOVE_GUI_ANIMATOR_CPP_
