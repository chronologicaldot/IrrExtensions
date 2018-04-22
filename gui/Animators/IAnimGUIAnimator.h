/*
(c) 2013 Nicolaus Anderson
*/

#include <IGUIElement.h>

#ifndef _IANIMATED_GUI_ANIMATOR_
#define _IANIMATED_GUI_ANIMATOR_

namespace irr
{
namespace gui
{

//! class Animated GUI Animator
/* Designed to be used with IAnimGUIElement.
This class is meant to be used during OnPostRender() for changing
the GUI element. */
class IAnimGUIAnimator : virtual public irr::IReferenceCounted
{
public:
	//! On Post-Render / Animate
	/* This is effectively the same as OnAnimate for ISceneNode
	but is so named because it is meant to be called on post-rendering. */
	virtual void OnPostRender( irr::u32 timeMs, IGUIElement* element )=0;

	//! Cast operator
	operator IAnimGUIAnimator*()
	{
		return (IAnimGUIAnimator*)this;
	}
};

}} // end namespaces gui and irr

#endif // #ifndef _IANIMATED_GUI_ANIMATOR_