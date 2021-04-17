/*
(c) 2013 Nicolaus Anderson
*/

#include <irrlicht.h>
#include "IAnimGUIAnimator.h"

#ifndef _MOVE_GUI_ANIMATOR_H_
#define _MOVE_GUI_ANIMATOR_H_

namespace irr
{
namespace gui
{

class MoveGUIAnimator : public IAnimGUIAnimator
{
protected:
	//! The element rectangle to be changed.
	/* Notice that this is a pointer, allowing us to change any rectangle
	that is passed to this animator by the GUI element / creator of this
	animator. */
	core::rect<s32>* ElemRect;

	//! Original rectangle.
	/* This is the initial rectangle that is to be changed.
	Once the animation is complete or its course has been altered,
	this is set to the element's current rectangle. */
	core::rect<s32> OrigRect;

	//! Target rectangle
	/* This is the final destination rectangle. */
	core::rect<s32> TargetRect;

	/* The starting time of this element in milliseconds. */
	u32 startTime;

	/* Speed of the transition. */
	f64 transitionSpeed;

	/* In motion.
	When this is set to true, the start time cannot be changed
	without starting a new animation. */
	bool inMotion;

public:
	//! Constructor and destructor
	MoveGUIAnimator(
		core::recti* rectangle,
		f64 speed,
		u32 startTimeMs
		);

	~MoveGUIAnimator();

	//! Set start time.
	/* Sets the animator's start time. Note that this does nothing if
	there is an animation that has not been completed yet. */
	void setStartTime( u32 timeMs );

	//! On Post Render / Animate
	virtual void OnPostRender( u32 timeMs, IGUIElement* element );

	//! Set target rectangle
	void setTargetRect( core::rect<s32> newTarget );

	//! Set immediate target (for interrupting a current transition)
	void setImmediateTarget( core::rect<s32> newTarget, u32 timeMs );
};

}} // end namespaces gui and irr

#endif // #ifndef _MOVE_GUI_ANIMATOR_H_