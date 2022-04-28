// (c) 2015 Nicolaus Anderson

#ifndef GUI_GROUPING_PANEL_CPP
#define GUI_GROUPING_PANEL_CPP

#include "GUIGroupingPanel.h"
#include <IGUIFont.h>
#include <IGUISkin.h>
#include <IGUIEnvironment.h>
#include <ITexture.h>
#include <IVideoDriver.h>

namespace irr {
namespace gui {

using video::IVideoDriver;
using video::SColor;
using core::vector2di;

GUIGroupingPanel::GUIGroupingPanel( const wchar_t* pText, IGUIEnvironment* pEnvironment, IGUIElement* pParent, const recti& pRect, s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, showBorder( true )
	, borderRadius( 6 )
{
	textSize = pEnvironment->getSkin()->getFont()->getDimension( pText );
	Text = pText;
	updateImageCache();
	setTabGroup(true);
}

void GUIGroupingPanel::setText(const wchar_t* text)
{
	Text = text;
	textSize = Environment->getSkin()->getFont()->getDimension( text );
}

void GUIGroupingPanel::setShowBorder( bool pShow )
{
	showBorder = pShow;
}

void GUIGroupingPanel::setBorderEdgeRadius( f32 pRadius )
{
	borderRadius = pRadius;
	updateImageCache();
}

recti GUIGroupingPanel::getClientArea()
{
	return recti( borderRadius, borderRadius + textSize.Height, RelativeRect.getWidth() - borderRadius, RelativeRect.getHeight() - borderRadius );
}

//void GUIGroupingPanel::updateAbsolutePosition()
//{
//}

void GUIGroupingPanel::draw()
{
	IVideoDriver* vid = Environment->getVideoDriver();
	IGUISkin* skin = Environment->getSkin();
	SColor color = skin->getColor( EGDC_3D_LIGHT );
	SColor shadowColor = skin->getColor( EGDC_3D_SHADOW );
	SColor textColor = skin->getColor( EGDC_BUTTON_TEXT );
	s32 textPad = ( textSize.Width > 0 ) ? 3 : 0; // Padding is only needed when there is text

	// Draw border corners
	vector2di textPush(0, textSize.Height/2);
	recti cornerSource( ULtexture->getSize() ); // Source rectangle for the images
	vid->draw2DImage( ULtexture, AbsoluteRect.UpperLeftCorner + textPush, cornerSource, &AbsoluteClippingRect, SColor(-1), true );

	vector2di corner2( AbsoluteRect.getWidth() - borderRadius, 0 );
	vid->draw2DImage( URtexture, corner2 + AbsoluteRect.UpperLeftCorner + textPush, cornerSource, &AbsoluteClippingRect, SColor(-1), true );

	vector2di corner( 0, AbsoluteRect.getHeight() - borderRadius );
	vid->draw2DImage( LLtexture, corner + AbsoluteRect.UpperLeftCorner, cornerSource, &AbsoluteClippingRect, SColor(-1), true );

	corner2 += vector2di( 0, AbsoluteRect.getHeight() - borderRadius ); // Use .move() in custom Irrlicht
	vid->draw2DImage( LRtexture, corner2 + AbsoluteRect.UpperLeftCorner, cornerSource, &AbsoluteClippingRect, SColor(-1), true );

	// Draw border walls
		// Left side
	vector2di vline( AbsoluteRect.UpperLeftCorner );
	vline.Y += borderRadius + (textSize.Height / 2);
	vector2di vlineEnd( AbsoluteRect.UpperLeftCorner.X, AbsoluteRect.LowerRightCorner.Y - borderRadius );

	// TODO: Clipping the borders!
	//if ( AbsoluteClippingRect.isPointInside(vline) )
	vid->draw2DLine( vline, vlineEnd, color );
	vline.X		+= 1;
	vlineEnd.X	+= 1;
	vid->draw2DLine( vline, vlineEnd, shadowColor ); // Shadow

		// Top side
	vector2di hline( AbsoluteRect.UpperLeftCorner );
	hline += vector2di( borderRadius + textPad*2 + textSize.Width, textSize.Height / 2 ); // Use .move() in custom Irrlicht
	vector2di hlineEnd( AbsoluteRect.LowerRightCorner.X - borderRadius, AbsoluteRect.UpperLeftCorner.Y + (textSize.Height / 2) );

	vid->draw2DLine( hline, hlineEnd, color );
	hline.Y		+= 1;
	hlineEnd.Y	+= 1;
	vid->draw2DLine( hline, hlineEnd, shadowColor ); // Shadow

		// Right side
	vline.set( AbsoluteRect.LowerRightCorner.X, AbsoluteRect.UpperLeftCorner.Y + borderRadius + (textSize.Height / 2) );
	vlineEnd.set( AbsoluteRect.LowerRightCorner );
	vlineEnd.Y -= borderRadius;

	vid->draw2DLine( vline, vlineEnd, color );
	vline.X		-= 1;
	vlineEnd.X	-= 1;
	vid->draw2DLine( vline, vlineEnd, shadowColor ); // Shadow

		// Bottom side
	hline.set( AbsoluteRect.UpperLeftCorner.X + borderRadius, AbsoluteRect.LowerRightCorner.Y );
	hlineEnd.set( AbsoluteRect.LowerRightCorner );
	hlineEnd.X -= borderRadius;

	vid->draw2DLine( hline, hlineEnd, color );
	hline.Y		-= 1;
	hlineEnd.Y	-= 1;
	vid->draw2DLine( hline, hlineEnd, shadowColor ); // Shadow

	// Draw label text
	recti labelRect( borderRadius + 3, 0, borderRadius + textPad, textSize.Height );
	Environment->getSkin()->getFont()->draw( Text.c_str(), labelRect + AbsoluteRect.UpperLeftCorner, textColor, false, false, &AbsoluteClippingRect );

	// Draw child elements
	IGUIElement::draw();
}

void GUIGroupingPanel::updateImageCache()
{
	IVideoDriver* vid = Environment->getVideoDriver();
	SColor color = Environment->getSkin()->getColor( EGDC_3D_LIGHT );
	SColor shadowColor = Environment->getSkin()->getColor( EGDC_3D_SHADOW );

	dimension2du imageSize( borderRadius, borderRadius );
	u32 borderResolution = (u32)borderRadius;

	// Produces cropped circles used as end-caps / corners

	ULtexture = vid->addRenderTargetTexture( imageSize, "GUI_GROUPING_PANEL_CORNER_UL" );
	if ( ULtexture )
	{
		// Irrlicht 5104
		//vid->setRenderTarget( ULtexture );
		// Irrlicht 5589
		vid->setRenderTarget( ULtexture, u16(irr::video::ECBF_COLOR), SColor(0,0,0,0) );
		vid->draw2DPolygon( vector2di( borderRadius+1 ), borderRadius, shadowColor, borderResolution );
		vid->draw2DPolygon( vector2di( borderRadius, borderRadius ), borderRadius, color, borderResolution );
	}

	URtexture = vid->addRenderTargetTexture( imageSize, "GUI_GROUPING_PANEL_CORNER_UR" );
	if ( URtexture )
	{
		// Irrlicht 5104
		//vid->setRenderTarget( URtexture );
		// Irrlicht 5589
		vid->setRenderTarget( URtexture, u16(irr::video::ECBF_COLOR), SColor(0,0,0,0) );
		vid->draw2DPolygon( vector2di( -1, borderRadius+1 ), borderRadius, shadowColor, borderResolution );
		vid->draw2DPolygon( vector2di( 0, borderRadius ), borderRadius, color, borderResolution );
	}

	LLtexture = vid->addRenderTargetTexture( imageSize, "GUI_GROUPING_PANEL_CORNER_LL" );
	if ( LLtexture )
	{
		// Irrlicht 5104
		//vid->setRenderTarget( LLtexture );
		// Irrlicht 5589
		vid->setRenderTarget( LLtexture, u16(irr::video::ECBF_COLOR), SColor(0,0,0,0) );
		vid->draw2DPolygon( vector2di( borderRadius+1, -1 ), borderRadius, shadowColor, borderResolution );
		vid->draw2DPolygon( vector2di( borderRadius, 0 ), borderRadius, color, borderResolution );
	}

	LRtexture = vid->addRenderTargetTexture( imageSize, "GUI_GROUPING_PANEL_CORNER_LR" );
	if ( LRtexture )
	{
		// Irrlicht 5104
		//vid->setRenderTarget( LRtexture );
		// Irrlicht 5589
		vid->setRenderTarget( LRtexture, u16(irr::video::ECBF_COLOR), SColor(0,0,0,0) );
		vid->draw2DPolygon( vector2di( -1 ), borderRadius, shadowColor, borderResolution );
		vid->draw2DPolygon( vector2di( 0 ), borderRadius, color, borderResolution );
		// ^ Works better with vector2di(-1)
	}

	vid->setRenderTarget(0); // Should restore old target
}

void
GUIGroupingPanel::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const {
	IGUIElement::serializeAttributes(out, options);

	out->addBool("ShowBorder", showBorder );
	out->addFloat("BorderRadius", borderRadius );
}

void
GUIGroupingPanel::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
	IGUIElement::deserializeAttributes(in, options);

	// Required so that the text size can be precalculated - NO LONGER NEEDED
	//setText( in->getAttributeAsString("Caption", L"") );
	showBorder = in->getAttributeAsBool("ShowBorder", showBorder);
	borderRadius = in->getAttributeAsFloat("BorderRadius", borderRadius);

	updateImageCache();
}

}}

#endif // #ifndef GUI_GROUPING_PANEL_CPP
