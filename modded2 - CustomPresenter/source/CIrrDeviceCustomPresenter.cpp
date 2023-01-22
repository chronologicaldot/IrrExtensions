// (C) 2023 Nicolaus Anderson, Nikolaus Gebhardt
// See irrlicht.h for license

#include "CIrrDeviceCustomPresenter.h"

#ifdef _IRR_COMPILE_WITH_CUSTOM_PRESENTER_DEVICE_

#include <SIrrCreationParameters.h>
#include "os.h"
#include "CSceneManager.h"

namespace irr
{

CIrrDeviceCustomPresenter::CIrrDeviceCustomPresenter(const SIrrlichtCreationParameters& param)
	: CIrrDeviceStub(param)
	, Presenter(0)
	, SurfaceId(0)
	, SurfaceSize(param.WindowSize)
{
	createDriver();
	createScene();
}

CIrrDeviceCustomPresenter::~CIrrDeviceCustomPresenter()
{
	if (Presenter)
	{
		Presenter->drop();
		Presenter = 0;
	}
}

bool CIrrDeviceCustomPresenter::run()
{
	os::Timer::tick();
	updateSurfaceSize();
	return true;
}

bool CIrrDeviceCustomPresenter::isWindowActive() const
{
	return Presenter != 0;
}

void CIrrDeviceCustomPresenter::setWindowSize(const irr::core::dimension2d<u32>& size)
{
	// Cannot change the surface size because that's determined by the presenter,
	// but we can update it.
	updateSurfaceSize();
}

bool CIrrDeviceCustomPresenter::setImagePresenter( video::SImagePresenter* presenter, s32 surfaceId )
{
	if ( Presenter )
	{
		Presenter->drop();
		Presenter = 0;
	}
	if ( presenter )
	{
		Presenter = presenter;
		Presenter->grab();
	}
	SurfaceId = surfaceId;
	return true;
}

bool CIrrDeviceCustomPresenter::present(video::IImage* surface, void* windowId, core::rect<s32>* src )
{
	// TODO: Future consideration is to allow windowId to have some meaning.
	// According to CSoftwareDriver2 line 986, what is stored is:
	// WindowId = videoData.D3D9.HWnd;
	// and it is this value that is passed to the present() method.
	if (Presenter)
	{
		return Presenter->present(surface, SurfaceId, src);
	}
	return false;
}

void CIrrDeviceCustomPresenter::createDriver()
{
	switch(CreationParams.DriverType)
	{
#ifdef _IRR_COMPILE_WITH_X11_
	case video::EDT_SOFTWARE:
#ifdef _IRR_COMPILE_WITH_SOFTWARE_
		VideoDriver = video::createSoftwareDriver(CreationParams.WindowSize, CreationParams.Fullscreen, FileSystem, this);
#else
		os::Printer::log("No Software driver support compiled in.", ELL_ERROR);
#endif
		break;
	case video::EDT_BURNINGSVIDEO:
#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
		VideoDriver = video::createBurningVideoDriver(CreationParams, FileSystem, this);
#else
		os::Printer::log("Burning's video driver was not compiled in.", ELL_ERROR);
#endif
		break;
	case video::EDT_OPENGL:
		os::Printer::log("OpenGL is not supported by this device.", ELL_ERROR);
		break;

	case video::DEPRECATED_EDT_DIRECT3D8_NO_LONGER_EXISTS:
	case video::EDT_DIRECT3D9:
		os::Printer::log("This driver is not supported by this device.",
			ELL_ERROR);
		break;
	case video::EDT_NULL:
		VideoDriver = video::createNullDriver(FileSystem, CreationParams.WindowSize);
		break;
	default:
		os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
		break;
#else
	case video::EDT_NULL:
		VideoDriver = video::createNullDriver(FileSystem, CreationParams.WindowSize);
		break;
	default:
		os::Printer::log("Operating with no video driver.", ELL_INFORMATION);
		break;
#endif
	}
}

void CIrrDeviceCustomPresenter::createScene()
{
	// create Scene manager
	SceneManager = scene::createSceneManager(VideoDriver, FileSystem, CursorControl, GUIEnvironment);

	setEventReceiver(UserReceiver);
}

void CIrrDeviceCustomPresenter::updateSurfaceSize()
{
	if ( !Presenter )
		return;

	core::dimension2du newSurfaceSize = Presenter->getSurfaceSize();
	if ( SurfaceSize != newSurfaceSize )
	{
		SurfaceSize = newSurfaceSize;
		if (VideoDriver)
			VideoDriver->OnResize(SurfaceSize);
	}
}

} // end namespace irr

#endif // _IRR_COMPILE_WITH_CUSTOM_PRESENTER_DEVICE_