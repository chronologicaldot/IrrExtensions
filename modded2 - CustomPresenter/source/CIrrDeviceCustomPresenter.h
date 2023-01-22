// (C) 2023 Nicolaus Anderson, Nikolaus Gebhardt
// See irrlicht.h for license

#ifndef IRR_CUSTOM_PRESENTER_H_INCLUDED
#define IRR_CUSTOM_PRESENTER_H_INCLUDED

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_CUSTOM_PRESENTER_DEVICE_

#include <position2d.h>
#include <dimension2d.h>
#include <SImagePresenter.h>
#include "IImagePresenter.h"
#include "CIrrDeviceStub.h"

namespace irr
{
	class CIrrDeviceCustomPresenter : public CIrrDeviceStub, public video::IImagePresenter
	{
	public:
		//! Constructor
		CIrrDeviceCustomPresenter(const SIrrlichtCreationParameters& param);

		//! Destructor
		virtual ~CIrrDeviceCustomPresenter();
		
		//! Run the device
		/** This both increments the clock and allows for resizing the window. */
		virtual bool run() IRR_OVERRIDE;

		//! Does nothing for this device.
		/** These methods are only here to fulfill the interface requirements. */		
		virtual void yield() IRR_OVERRIDE {}
		virtual void sleep(u32 timeMs, bool pauseTimer) IRR_OVERRIDE {}
		virtual void setWindowCaption(const wchar_t* text) IRR_OVERRIDE {}
		virtual void closeDevice() IRR_OVERRIDE {}

		//! Checks if the window is active
		/** \return Actually returns whether there is an SImagePresenter for the
		surface/backbuffer to be presented to. */
		virtual bool isWindowActive() const IRR_OVERRIDE;

		//! Checks if the Irrlicht window has the input focus
		/** \return Always false. This device does not support input. */
		virtual bool isWindowFocused() const IRR_OVERRIDE { return false; }

		//! Checks if the Irrlicht window is minimized
		/** \return Always false. This device has no window. */
		virtual bool isWindowMinimized() const IRR_OVERRIDE { return false; }

		//! Checks if the Irrlicht window is running in fullscreen mode
		/** \return Always false. This device has no window. */
		virtual bool isFullscreen() const IRR_OVERRIDE { return false; }

		//! Meant to set if the window should be resizable in windowed mode.
		/** This does nothing as the render surface size is set by the SImagePresenter. */
		virtual void setResizable(bool resize) IRR_OVERRIDE {}

		//! Set the surface size for rendering		
		virtual void setWindowSize(const irr::core::dimension2d<u32>& size) IRR_OVERRIDE;
		
		//! Does nothing for this device.
		/** These methods are only here to fulfill the interface requirements. */
		virtual void minimizeWindow() IRR_OVERRIDE {}
		virtual void maximizeWindow() IRR_OVERRIDE {}
		virtual void restoreWindow() IRR_OVERRIDE {}
		virtual core::position2di getWindowPosition() IRR_OVERRIDE { return core::position2di(0); }
		
		//! Sets the class that receives the backbuffer image created by the video driver
		virtual bool setImagePresenter( video::SImagePresenter* presenter, s32 surfaceId ) IRR_OVERRIDE;
		
		//! Used internally
		virtual bool present(video::IImage* surface, void* windowId=0, core::rect<s32>* src=0 ) IRR_OVERRIDE;

		//! Indicates the type of this device
		virtual E_DEVICE_TYPE getType() const IRR_OVERRIDE
		{
			return EIDT_CUSTOM_PRESENTER;
		}
		
	protected:
		void createDriver();
		void createScene();
		void updateSurfaceSize();
		
	private:
		video::SImagePresenter* Presenter;
		s32 SurfaceId;
		core::dimension2du SurfaceSize;
	};
} // end namespace irr

#endif // _IRR_COMPILE_WITH_CUSTOM_PRESENTER_DEVICE_
#endif // IRR_CUSTOM_PRESENTER_H_INCLUDED