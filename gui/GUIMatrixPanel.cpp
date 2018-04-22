// (c) 2015 Nicolaus Anderson

#ifndef GUI_MATRIX_PANEL_CPP
#define GUI_MATRIX_PANEL_CPP

#include "GUIMatrixPanel.h"
#include "GUIVectorPanel.h"
#include <IGUIEnvironment.h>
#include <IGUIFont.h>
#include <IGUISkin.h>
#include <IGUIStaticText.h>

namespace irr {
namespace gui {

using core::dimension2d;
using core::vector2d;

GUIMatrixPanel::GUIMatrixPanel( bool pRotateInDegrees, IGUIEnvironment* pEnvironment, IGUIElement* pParent, rect<s32> pRect, s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, translationVectorPanel(0)
	, rotationVectorPanel(0)
	, scaleVectorPanel(0)
	, rotInDeg( pRotateInDegrees )
{
	// labelSpan = for "Translation", "Rotation", and "Scale" text
	// xyzSpan = for "X", "Y", and "Z" text

	/* Intended appearance:

        labelSpan -pad- xyzSpan  xyzSpan  xyzSpan
        labelSpan -pad- vectorPanel
        labelSpan -pad- vectorPanel
        labelSpan -pad- vectorPanel
	*/

	s32 pad = 3; // padding between elements
	dimension2d<u32> labelSpan = pEnvironment->getSkin()->getFont()->getDimension(L"Translate");
	s32 boxH = (pRect.getHeight() - (3*pad)) / 4; // used regardless of the text height
	s32 xyzSpan = ( pRect.getWidth() - (s32)labelSpan.Width - (pad*2) ) / 3;
	rect<s32> labelRect( 0, boxH, labelSpan.Width, boxH*2 ); // for "Translate", "Rotate", and "Scale"
	rect<s32> textRect( 0, 0, boxH, boxH ); // for "X", "Y", and "Z"
	textRect += vector2d<s32>( xyzSpan/2 - 3, boxH/2 - 3 ); // text centering

	// Add labels of X, Y, and Z to the top
	// Note the first is indented so it doesn't hover over the labels
	textRect.move( labelSpan.Width + pad, 0 );
	IGUIStaticText* textElem =
		pEnvironment->addStaticText( L"X", textRect, false, false, this, -1, false ); // no border, wordwrap, nor fill
	textElem->setAlignment( EGUIA_UPPERLEFT, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE );

	textRect.move( xyzSpan, 0 );
	textElem =
		pEnvironment->addStaticText( L"Y", textRect, false, false, this, -1, false ); // no border, wordwrap, nor fill
	textElem->setAlignment( EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE );

	textRect.move( xyzSpan, 0 );
	textElem = pEnvironment->addStaticText( L"Z", textRect, false, false, this, -1, false ); // no border, wordwrap, nor fill
	textElem->setAlignment( EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE );

	// Add the labels of "Translate", "Rotate", and "Scale"
	textElem = pEnvironment->addStaticText( L"Translate", labelRect, false, false, this, -1, false );
	textElem->setAlignment( EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_SCALE, EGUIA_SCALE );

	labelRect.move( 0, boxH + pad );
	textElem = pEnvironment->addStaticText( L"Rotate", labelRect, false, false, this, -1, false );
	textElem->setAlignment( EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_SCALE, EGUIA_SCALE );

	labelRect.move( 0, boxH + pad );
	textElem = pEnvironment->addStaticText( L"Scale", labelRect, false, false, this, -1, false );
	textElem->setAlignment( EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_SCALE, EGUIA_SCALE );

	// Add the vector panels
	rect<s32> vectorPanelRect( labelSpan.Width + pad, boxH + pad, pRect.getWidth(), boxH*2 + pad );

	translationVectorPanel = new GUIVectorPanel( pEnvironment, this, vectorPanelRect, true );
	//translationVectorPanel->setAlignment( EGUIA_UPPERLEFT, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE );

	vectorPanelRect.move( 0, boxH + pad );

	rotationVectorPanel = new GUIVectorPanel( pEnvironment, this, vectorPanelRect, true );
	//rotationVectorPanel->setAlignment( EGUIA_UPPERLEFT, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE );

	vectorPanelRect.move( 0, boxH + pad );

	scaleVectorPanel = new GUIVectorPanel( pEnvironment, this, vectorPanelRect, true );
	//scaleVectorPanel->setAlignment( EGUIA_UPPERLEFT, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE );
}

/*
matrix4 GUIMatrixPanel::getMatrix()
{
	// The problem is that setScale and setRotation conflict, causing one to override the other
	matrix4 m;
	m.setScale( getScale() );
	m.setRotation( getRotation() );
	m.setTranslation( getTranslation() );
	return m;
}
*/

void GUIMatrixPanel::setSpaceMatrix( SpaceMatrix& pMatrix )
{
	//spaceMatrix = pMatrix;
	translationVectorPanel->set3DVectorValue( pMatrix.getTranslation() );
	scaleVectorPanel->set3DVectorValue( pMatrix.getScale() );
	if ( rotInDeg )
		rotationVectorPanel->set3DVectorValue( pMatrix.getRotationDegrees() );
	else
		rotationVectorPanel->set3DVectorValue( pMatrix.getRotationRadians() );
}

void GUIMatrixPanel::setTranslation( vector3d<f32> pTranslation )
{
	//spaceMatrix->setTranslation( pTranslation );
	translationVectorPanel->set3DVectorValue( pTranslation );
}

void GUIMatrixPanel::setRotation( vector3d<f32> pRotation )
{
	rotationVectorPanel->set3DVectorValue( pRotation );
}

void GUIMatrixPanel::setScale( vector3d<f32> pScale )
{
	scaleVectorPanel->set3DVectorValue( pScale );
}

SpaceMatrix GUIMatrixPanel::getSpaceMatrix()
{
	if ( !rotInDeg )
	return SpaceMatrix(
			translationVectorPanel->getVector3D(),
			rotationVectorPanel->getVector3D(),
			scaleVectorPanel->getVector3D()
			);

	// else, must convert from degrees
	SpaceMatrix m = SpaceMatrix(
			translationVectorPanel->getVector3D(),
			vector3d<f32>(),
			scaleVectorPanel->getVector3D()
			);

	m.setRotationDegrees( rotationVectorPanel->getVector3D() );
	return m;
}

vector3d<f32> GUIMatrixPanel::getTranslation()
{
	return translationVectorPanel->getVector3D();
}

vector3d<f32> GUIMatrixPanel::getRotation()
{
	return rotationVectorPanel->getVector3D();
}

vector3d<f32> GUIMatrixPanel::getScale()
{
	return scaleVectorPanel->getVector3D();
}

void GUIMatrixPanel::reset()
{
	translationVectorPanel->reset();
	rotationVectorPanel->reset();
	scaleVectorPanel->set3DValue(1.f, 1.f, 1.f);
}

bool GUIMatrixPanel::OnEvent( const SEvent& event )
{
	if ( !isVisible() || !isEnabled() || event.EventType != EET_GUI_EVENT )
		return false;

	if ( event.GUIEvent.Caller == translationVectorPanel )
	{
		lastAction = EGUIMTXPA_TranslationChanged;
		//spaceMatrix->setTranslation( translationVectorPanel->getVector3D() );
		sendGUIEvent( EGET_EDITBOX_CHANGED, translationVectorPanel );
		return true;
	} else if ( event.GUIEvent.Caller == rotationVectorPanel )
	{
		lastAction = EGUIMTXPA_RotationChanged;
		//spaceMatrix->setRotationDegrees( rotationVectorPanel->getVector3D() );
		sendGUIEvent( EGET_EDITBOX_CHANGED, rotationVectorPanel );
		return true;
	} else if ( event.GUIEvent.Caller == scaleVectorPanel )
	{
		lastAction = EGUIMTXPA_ScaleChanged;
		//spaceMatrix->setScale( scaleVectorPanel->getVector3D() );
		sendGUIEvent( EGET_EDITBOX_CHANGED, scaleVectorPanel );
		return true;
	}
	lastAction = EGUIMTXPA_None;
	return false;
}

void GUIMatrixPanel::sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement )
{
	if ( ! Parent ) return;

	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = pElement;
	event.GUIEvent.EventType = pEventType;

	Parent->OnEvent(event);
}

EGUIMatrixPanelAction GUIMatrixPanel::getLastAction()
{
	return lastAction;
}

bool GUIMatrixPanel::rotatesInDegrees()
{
	return rotInDeg;
}

}}

#endif // #ifndef GUI_MATRIX_PANEL_CPP
