// (c) 2015 Nicolaus Anderson

#ifndef GUI_VECTOR_PANEL_CPP
#define GUI_VECTOR_PANEL_CPP

#include "GUIVectorPanel.h"
#include <IGUISpinBox.h>
#include <IGUIEnvironment.h>

namespace irr {
namespace gui {

GUIVectorPanel::GUIVectorPanel( IGUIEnvironment* pEnvironment, IGUIElement* pParent, rect<s32> pRect, bool pEdit3D, s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, X(0)
	, Y(0)
	, Z(0)
	, edit3D( pEdit3D )
	, dirtyRect( false )
	, xEdit( 0 )
	, yEdit( 0 )
	, zEdit( 0 )
{
	s32 coordBoxWidth;
	s32 pad = 5;
	if ( edit3D )
	{
		coordBoxWidth = (pRect.getWidth() - 2*pad) / 3;
	} else {
		coordBoxWidth = (pRect.getWidth() - pad) / 2;
	}
	rect<s32> valueRect( 0, 0, coordBoxWidth, pRect.getHeight() );
	core::vector2di valueRectShift( coordBoxWidth + pad, 0 );

	xEdit = pEnvironment->addSpinBox( L"0.0", valueRect, true, this );
	xEdit->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT );
	xEdit->setStepSize(0.1f);
	xEdit->setValidateOn(EGUI_SBV_CHANGE); // Requirement for irrlicht 1.9 to update on any change to the spin box

	//valueRect.move( coordBoxWidth + pad, 0 ); // custom Irrlicht
	valueRect.UpperLeftCorner += valueRectShift;
	valueRect.LowerRightCorner += valueRectShift;
	yEdit = pEnvironment->addSpinBox( L"0.0", valueRect, true, this );
	yEdit->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT );
	yEdit->setStepSize(0.1f);
	yEdit->setValidateOn(EGUI_SBV_CHANGE); // Requirement for irrlicht 1.9 to update on any change to the spin box

	//valueRect.move( coordBoxWidth + pad, 0 ); // custom Irrlicht
	valueRect.UpperLeftCorner += valueRectShift;
	valueRect.LowerRightCorner += valueRectShift;
	zEdit = pEnvironment->addSpinBox( L"0.0", valueRect, true, this );
	zEdit->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT );
	zEdit->setStepSize(0.1f);
	zEdit->setValidateOn(EGUI_SBV_CHANGE); // Requirement for irrlicht 1.9 to update on any change to the spin box

	if ( ! edit3D )
	{
		zEdit->setVisible(false);
	}
}

void GUIVectorPanel::set2DValue( f32 pX, f32 pY )
{
	X = pX;
	Y = pY;
	xEdit->setValue( X );
	yEdit->setValue( Y );
}

void GUIVectorPanel::set2DVectorValue( vector2d<f32> pValue )
{
	X = pValue.X;
	Y = pValue.Y;
	xEdit->setValue( X );
	yEdit->setValue( Y );
}

void GUIVectorPanel::set3DValue( f32 pX, f32 pY, f32 pZ )
{
	X = pX;
	Y = pY;
	Z = pZ;
	xEdit->setValue( X );
	yEdit->setValue( Y );
	zEdit->setValue( Z );
}

void GUIVectorPanel::set3DVectorValue( vector3d<f32> pValue )
{
	X = pValue.X;
	Y = pValue.Y;
	Z = pValue.Z;
	xEdit->setValue( X );
	yEdit->setValue( Y );
	zEdit->setValue( Z );
}

void GUIVectorPanel::reset()
{
	X = 0;
	Y = 0;
	Z = 0;
	xEdit->setValue( X );
	yEdit->setValue( Y );
	zEdit->setValue( Z );
}

void GUIVectorPanel::setEdit2DVector() // Force editing a 2D vector
{
	edit3D = false;
	dirtyRect = true;
	updateAbsolutePosition();
}

void GUIVectorPanel::setEdit3DVector() // Force editing a 3D vector
{
	edit3D = true;
	dirtyRect = true;
	updateAbsolutePosition();
}

bool GUIVectorPanel::isEditing3D()
{
	return edit3D;
}

vector2d<f32> GUIVectorPanel::getVector2D()
{
	return vector2d<f32>( X, Y );
}

vector3d<f32> GUIVectorPanel::getVector3D()
{
	return vector3d<f32>( X, Y, Z );
}

void GUIVectorPanel::updateAbsolutePosition()
{
	// Needs to be fixed?
	s32 coordBoxScale;
	f32 fpad = 5.0f / (f32) RelativeRect.getWidth();
	if ( dirtyRect )
	{
		if ( edit3D )
		{
			coordBoxScale = 1.f/3.f - fpad*2;
		} else {
			coordBoxScale = 0.5f - fpad;
		}
		rect<f32> valueRect( 0, 0, coordBoxScale, 1.0f );

		xEdit->setRelativePositionProportional( valueRect );

		core::vector2df valueRectShift( coordBoxScale + fpad, 0.f );

		//valueRect.move( coordBoxScale + fpad, 0.f ); // custom Irrlicht
		valueRect.UpperLeftCorner += valueRectShift;
		valueRect.LowerRightCorner += valueRectShift;
		yEdit->setRelativePositionProportional( valueRect );

		if ( edit3D )
		{
			//valueRect.move( coordBoxScale + fpad, 0.f ); // custom Irrlicht
			valueRect.UpperLeftCorner += valueRectShift;
			valueRect.LowerRightCorner += valueRectShift;
			zEdit->setRelativePositionProportional( valueRect );
		} else {
			zEdit->setVisible(false);
		}
		dirtyRect = false;
	}

	IGUIElement::updateAbsolutePosition();
}

bool GUIVectorPanel::OnEvent( const SEvent& event )
{
	if ( !isVisible() || !isEnabled() || event.EventType != EET_GUI_EVENT )
		return false;

	if ( event.GUIEvent.Caller == xEdit )
	{
		X = xEdit->getValue();
		sendGUIEvent( EGET_EDITBOX_CHANGED, xEdit );
		return true;
	}
	else if ( event.GUIEvent.Caller == yEdit )
	{
		Y = yEdit->getValue();
		sendGUIEvent( EGET_EDITBOX_CHANGED, yEdit );
		return true;
	}
	else if ( event.GUIEvent.Caller == zEdit )
	{
		Z = zEdit->getValue();
		sendGUIEvent( EGET_EDITBOX_CHANGED, zEdit );
		return true;
	}
	return false;
}

void GUIVectorPanel::sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement )
{
	if ( ! Parent ) return;

	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = pElement;
	event.GUIEvent.EventType = pEventType;

	Parent->OnEvent(event);
}

void GUIVectorPanel::serializeAttributes(
	irr::io::IAttributes* out,
	irr::io::SAttributeReadWriteOptions* options
	)
{
	IGUIElement::serializeAttributes(out,options);

	out->addBool("Edit3D", edit3D);
	out->AddFloat("X", X);
	out->AddFloat("Y", X);
	out->AddFloat("Z", X);
}

void GUIVectorPanel::deserializeAttributes(
	irr::io::IAttributes* in,
	irr::io::SAttributeReadWriteOptions* options
	)
{
	IGUIElement::deserializeAttributes(in,options);

	edit3D = in->getBool("Edit3D", edit3D);
	if ( edit3D )
		setEdit3DVector();
	else
		setEdit2DVector();

	X = in->getFloat("X");
	Y = in->getFloat("Y");
	Z = in->getFloat("Z");
}

}}

#endif // #ifndef GUI_VECTOR_PANEL_CPP
