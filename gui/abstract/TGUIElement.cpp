// Copyright 2018 Nicolaus Anderson

#include "TGUIElement.h"

namespace irr {
namespace gui {

TGUIElement::TGUIElement( EGUI_ELEMENT_TYPE type, IGUIEnvironment* environment, IGUIElement* parent,
		s32 id, const core::rect<s32>& rectangle )
	: IGUIElement( type, environment, parent, id, rectangle )
	, CallbackReceiver(0)
{}

void TGUIElement::setCallbackReceiver( IEventReceiver*  receiver ) {
	CallbackReceiver = receiver;
};

bool TGUIElement::OnEvent( const SEvent&  event ) {

	if ( !IsVisible || !IsEnabled )
		return false;

	switch( event.EventType ) {
	case EET_GUI_EVENT:
		return OnGUIEvent(event.GUIEvent);

	case EET_MOUSE_INPUT_EVENT:
		return OnMouseInput(event.MouseInput);

	case EET_KEY_INPUT_EVENT:
		return OnKeyInput(event.KeyInput);

	case EET_JOYSTICK_INPUT_EVENT:
		return OnJoystickEvent(event.JoystickEvent);

	default:
		break;
	}
	return IGUIElement::OnEvent(event);
}

void makeEvent( EGUI_EVENT_TYPE  eventType ) {
	SEvent e;
	e.EventType = EET_GUI_EVENT;
	e.GUIEvent.EventType = eventType;
	e.GUIEvent.Element = this;
	if ( CallbackReceiver ) {
		CallbackReceiver->OnEvent(e);
	}
	Environment->OnEvent(e);
}

}}
