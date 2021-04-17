// Copyright 2018 Nicolaus Anderson

#include <IGUIElement.h>

#ifndef __TGUIELEMENT_H__
#define __TGUIELEMENT_H__

namespace irr {
namespace gui {

//! Template GUI Element class
/*
	This class provides a base for other classes to have event parents and simplified handling of events such as mouse input.
*/
class TGUIElement {
public:
	TGUIElement( EGUI_ELEMENT_TYPE type, IGUIEnvironment* environment, IGUIElement* parent,
		s32 id, const core::rect<s32>& rectangle );

	virtual ~TGUIElement() {}

	void setCallbackReceiver( IEventReceiver*  receiver );
	virtual bool OnEvent( const SEvent&  event );

protected:
	virtual bool OnMouseInput( const SMouseInput&  event ) { return false; }
	virtual bool OnKeyInput( const SKeyInput&  event ) { return false; }
	virtual bool OnGUIEvent( const SGUIEvent&  event ) { return false; }
	virtual bool OnJoystickEvent( const SJoystickEvent&  event ) { return false; }

	void makeEvent( EGUI_EVENT_TYPE  eventType );

private:
	IEventReceiver*  CallbackReceiver;
};

}}

#endif
