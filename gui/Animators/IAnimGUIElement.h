/*
(c) 2013 Nicolaus Anderson
*/

#include <IGUIElement.h>
#include "IAnimGUIAnimator.h"

#ifndef _IANIMATED_GUI_ELEMENT_
#define _IANIMATED_GUI_ELEMENT_

namespace irr
{
namespace gui
{

//! Class Animated GUI Element
/* Allows GUI elements to be animated. */
class IAnimGUIElement : public IGUIElement
{
protected:
	irr::core::list<IAnimGUIAnimator*> Animators;

public:
	//! Constructor
	/* Merely a callback for the IGUIElement constructor. */
	IAnimGUIElement(
		EGUI_ELEMENT_TYPE type,
		IGUIEnvironment* environment,
		IGUIElement* parent,
		s32 id,
		const core::recti& rectangle
		)
		: IGUIElement( type, environment, parent, id, rectangle )
	{}
	
	//! Add animator
	void addAnimator( IAnimGUIAnimator* animator )
	{
		if ( animator )
		{
			animator->grab();
			Animators.push_back( animator );
		}
	}

	//! Remove animator
	void removeAnimator( IAnimGUIAnimator* animator )
	{
		irr::core::list<IAnimGUIAnimator*>::Iterator it = Animators.begin();
		for ( ; it != Animators.end(); ++it )
		{
			if ( *it == animator )
			{
				(*it)->drop();
				Animators.erase(it);
				return;
			}
		}
	}

	//! Remove all animators
	void clearAnimators()
	{
		irr::core::list<IAnimGUIAnimator*>::Iterator it = Animators.begin();
		while ( it != Animators.end() )
		{
			(*it)->drop();
			Animators.erase(it);
			it = Animators.begin();
		}
	}

	//! On Post Render
	/* Applies the animators */
	virtual void OnPostRender( irr::u32 timeMs )
	{
		irr::core::list<IAnimGUIAnimator*>::Iterator it;
		for ( ; it != Animators.end(); ++it )
		{
			(*it)->OnPostRender( timeMs, this );
		}

		IGUIElement::OnPostRender(timeMs);
	}
};

}} // end namespaces gui and irr

#endif // #ifndef _IANIMATED_GUI_ELEMENT_
