// (c) Nicolaus Anderson

#ifndef GUI_NODESCENE_VIEW_H
#define GUI_NODESCENE_VIEW_H

#include <irrString.h>
#include <IGUIElement.h>
//#include <ITexture.h>
#include <IVideoDriver.h>

namespace irr {
namespace gui {

using core::recti;
using core::stringc;
using video::ITexture;
using video::IRenderTarget;
using video::IVideoDriver;
//! Class GUI Node Scene View
/*
This class is intended to simplify the handling of the rendering of the Node Scene.

Note that currently, Burning's Renderer does not support multiple depth buffers.
There is a getDepthBuffer() method on SoftwareDriver2, but it is not a public interface, and it isn't possible to cast to the Burning's Renderer without including that entire implementation.
As a work-around, I could add the getDepthBuffer() method to the interface and have it return 0 from the NullDriver, which would allow me to copy and save the one from Burning's Renderer while at the same time identifying when the other buffer won't work.
The other option is figuring out how to utilize the existing interface to control the Burning's Renderer depth.

Currently, only the Burning's Renderer works correctly for multiple render targets, assuming the depth buffer has not been changed.
OpenGL messes up the depth buffer. I have to restart my computer to get the API fixed, but I don't think that will fix the problem.
^ I may have fixed this now, since I updated to OpenGL 2.
*/
class GUINodeSceneView : public IGUIElement
{
protected:
	NodeScene* nodeScene;
	IVideoDriver* videoDriver;
	ITexture* texture;
	ITexture* depthStencil;
	IRenderTarget* renderTarget;
	io::path internalName;
	bool dirty;
	bool useAlphaChannel;
	bool removeTexturesOnDestroy;

public:
	GUINodeSceneView( NodeScene* pNodeScene, IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id=-1, bool pRemoveTexturesOnDestroy=true );

	~GUINodeSceneView();

	ITexture* getTexture() const;
	bool setUseAlphaChannel( bool yes );

	void update();
	void reset();

	virtual void draw();

protected:
	IRenderTarget* getRenderTarget( bool pUseCurrentTargetSize );
};

}}

#endif // #ifndef GUI_NODESCENE_VIEW_H
