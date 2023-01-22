// (C) 2023 Nicolaus Anderson, Nikolaus Gebhardt
// See irrlicht.h for license

#ifndef IRR_SIMAGE_PRESENTER_H_INCLUDED
#define IRR_SIMAGE_PRESENTER_H_INCLUDED

#include "IReferenceCounted.h"
#include "dimension2d.h"
#include "rect.h"
#include "IImage.h"

namespace irr
{
namespace video
{
	class SImagePresenter : public IReferenceCounted
	{
	public:
		virtual ~SImagePresenter() {}
		
		//! Render the graphics
		/** \param surface - The image from the backbuffer of the video driver that
		you get to render or copy to wherever you want.
		\param surfaceId - Use this via Custom Presenter to track where you want the
		image to be rendered/copied-to.
		\param src - The rectangular area of the surface that is meant to be drawn/copied. */
		virtual bool present(video::IImage* surface, s32 surfaceId, core::rect<s32>* src=0 ) = 0;
		
		//! Return the surface
		virtual core::dimension2du getSurfaceSize() = 0;
	};
} // end namespace video
} // end namespace irr

#endif // IRR_SIMAGE_PRESENTER_H_INCLUDED