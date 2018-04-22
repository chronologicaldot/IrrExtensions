// (c) Nicolaus Anderson

#ifndef GUI_NODESCENE_VIEW_CPP
#define GUI_NODESCENE_VIEW_CPP

namespace irr {
namespace gui {

GUINodeSceneView::GUINodeSceneView( NodeScene* pNodeScene, IGUIEnvironment* pEnvironment, IGUIElement* pParent, recti pRect, s32 id, bool pRemoveTexturesOnDestroy )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, nodeScene( pNodeScene )
	, videoDriver( pEnvironment->getVideoDriver() )
	, texture( pTexture )
	, depthStencil(0)
	, renderTarget(0)
	, dirty(false)
	, useAlphaChannel(false)
	, removeTexturesOnDestroy(pRemoveTexturesOnDestroy)
{
	internalName = io::path(stringc("GUI_NODESCENE_VIEW_") + stringc(id));
}

GUINodeSceneView::~GUINodeSceneView()
{
	if ( renderTarget )
		videoDriver->removeRenderTarget( renderTarget );

	if ( removeTexturesOnDestroy )
	{
		videoDriver->removeTexture( texture );
		videoDriver->removeTexture( depthStencil );
	}
}

ITexture* GUINodeSceneView::getTexture() const
{
	return texture;
}

bool GUINodeSceneView::setUseAlphaChannel( bool yes )
{
	useAlphaChannel = yes;
}

void GUINodeSceneView::update()
{
	if ( ! dirty ) return;

	nodeScene->setRenderTarget( getRenderTarget(true) );

	// Draw until there is no more drawing to do
	if ( ! nodeScene->drawAll() )
	{
		dirty = false;
	}
}

void GUINodeSceneView::reset()
{
	dirty = true;
	nodeScene->getState().reset();
}

void GUINodeSceneView::draw()
{
	if ( !texture || dirty ) return;
	recti sourceRect( texture->getOriginalSize() );
	if ( isVisible() && isEnabled() )
	{
		//videoDriver->draw2DRectangle( video::SColor(0xffff0000), AbsoluteRect, &AbsoluteClippingRect );

		// Yes, it is drawing, but the background is black, even though the lighting is working
		videoDriver->draw2DImage( texture, AbsoluteRect.UpperLeftCorner,
											sourceRect, &AbsoluteClippingRect,
											video::SColor(0xffffffff), useAlphaChannel );
	}
}

IRenderTarget* GUINodeSceneView::getRenderTarget( bool pUseCurrentTargetSize )
{
	IVideoDriver* videoDriver;
	dimension2du targetSize;
	if ( !renderTarget )
	{
		AbsoluteRect.repair(); // Ensure it is positive
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

}}

#endif // #ifndef GUI_NODESCENE_VIEW_CPP
