// (c) 2015 Nicolaus Anderson

#ifndef GUI_TEXTURE_H
#define GUI_TEXTURE_H

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

// GUI element for displaying a texture, one that can be rendered to as a render target.
class GUITextureView : public IGUIElement
{
	ITexture* texture;
	ITexture* depthStencil;
	IRenderTarget* renderTarget;
	io::path internalName;
	bool useAlphaChannel;
	bool removeTexturesOnDestroy;

public:
	// Texture-display version (no IImage version at the moment)
	GUITextureView( ITexture* pTexture, IGUIEnvironment* pEnv, IGUIElement* pParent, recti pRect, s32 id=-1, bool pRemoveTexturesOnDestroy=true )
		: IGUIElement( EGUIET_ELEMENT, pEnv, pParent, id, pRect )
		, texture( pTexture )
		, depthStencil(0)
		, renderTarget(0)
		, useAlphaChannel(false)
		, removeTexturesOnDestroy(pRemoveTexturesOnDestroy)
	{
		internalName = io::path(stringc("GUI_TEXTURE_") + stringc(id));
	}

	~GUITextureView()
	{
		IVideoDriver* videoDriver = Environment->getVideoDriver();

		if ( renderTarget )
			videoDriver->removeRenderTarget( renderTarget );
		if ( depthStencil )
			videoDriver->removeRenderTarget( depthStencil );

		if ( removeTexturesOnDestroy )
		{
			videoDriver->removeTexture( texture );
			videoDriver->removeTexture( depthStencil );
		}
	}

	//! Set the texture
	/* Sets the texture displayed by this GUI element.
	Note: It is the responsibility of the programmer to ensure this texture is removed when
	no longer needed. */
	void setTexture( ITexture* pTexture )
	{
		texture = pTexture;
	}

	ITexture* getTexture()
	{
		return texture;
	}

/*
	// Commented out until I know how the depth stencil works
	void setDepthStencil( ITexture* pDepthStencil )
	{
		depthStencil = pDepthStencil;
	}
*/

	void setUseAlphaChannelOnDraw( bool yes )
	{
		useAlphaChannel = yes;
	}

	//! Get the render target
	/* Creates and returns a render target.
	Note: This will erase the pointer to any existing texture, so it is important
	that you remove the texture yourself prior to calling this function. */
	IRenderTarget* getRenderTarget( bool pUseCurrentTargetSize=true ) // core::dimension2du pOverrideSize = core::dimension2du() )
	{
		IVideoDriver* videoDriver;
		dimension2du targetSize;
		if ( !renderTarget )
		{
			AbsoluteRect.repair(); // Ensure it is positive
			videoDriver = Environment->getVideoDriver();
			// Using the AbsoluteRect size could be problematic since the texture size must be both:
			// 1) Equal to or smaller than the screen size (since it shares the z-buffer) AND
			// 2) A power of 2 in both width and height (which the screen size might not be)

			if ( pUseCurrentTargetSize )
				targetSize = videoDriver->getCurrentRenderTargetSize();
			else
				targetSize = dimension2du( AbsoluteRect.getSize() );

			texture = videoDriver->addRenderTargetTexture( targetSize, internalName, video::ECF_A8R8G8B8 );
			depthStencil = videoDriver->addRenderTargetTexture( targetSize, internalName + "Depth", video::ECF_G16R16F );
			// ^ What color format should the stencil buffer be for OpenGL? A8R8G8B8?
			// Entity uses ECF_G16R16F, though I suppose it doesn't matter as long as its 32-bits.

			renderTarget = videoDriver->addRenderTarget();
			renderTarget->setTexture(texture, depthStencil);
		}
		return renderTarget;
	}

	void removeRenderTarget()
	{
		if ( renderTarget )
		{
			Environment->getVideoDriver()->removeRenderTarget(renderTarget);
			renderTarget = 0;
		}
		if ( depthStencil )
		{
			Environment->getVideoDriver()->removeRenderTarget(depthStencil);
			depthStencil = 0;
		}
	}

	bool setUseAlphaChannel( bool yes )
	{
		useAlphaChannel = yes;
	}

	virtual void draw()
	{
		if ( !texture ) return;
		recti sourceRect( texture->getOriginalSize() );
		if ( isVisible() && isEnabled() )
		{
			//Environment->getVideoDriver()->draw2DRectangle( video::SColor(0xffff0000), AbsoluteRect, &AbsoluteClippingRect );

			// Yes, it is drawing, but the background is black, even though the lighting is working.
			Environment->getVideoDriver()->draw2DImage( texture, AbsoluteRect.UpperLeftCorner,
												sourceRect, &AbsoluteClippingRect,
												video::SColor(0xffffffff), useAlphaChannel );
		}
	}

	// TODO: Serialization
};

}}

#endif // #ifndef GUI_TEXTURE_H
