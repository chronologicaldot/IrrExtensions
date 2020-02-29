// Copyright 2018 Nicolaus Anderson

#ifndef GUI_DROPDOWN_SELECTOR_FACTORY_H
#define GUI_DROPDOWN_SELECTOR_FACTORY_H

#include "GUIDropdownSelector.h"
#include <IGUIElementFactory.h>

namespace irr {
namespace gui {

class IGUIEnvironment;


class GUIDropdownSelectorFactory : public IGUIElementFactory {

	IGUIEnvironment*  Environment;
	const core::stringc Label;

public:
	GUIDropdownSelectorFactory( IGUIEnvironment* environment )
		: Environment(environment)
		, Label("dropdown")
	{}

	IGUIElement* addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent) {
		if ( type != EGUIET_ELEMENT )
			return 0;

		IGUIElement* e = new GUIDropdownSelector(
					Environment,
					parent? parent : Environment->getRootGUIElement(),
					core::recti(0,0,100,100), -1);
		e->drop();
		return e;
	}

	IGUIElement* addGUIElement(const c8* typeName, IGUIElement* parent) {
		if ( Label != typeName )
			return 0;

		IGUIElement* e = new GUIDropdownSelector(
					Environment,
					parent? parent : Environment->getRootGUIElement(),
					core::recti(0,0,100,100), -1);
		e->drop();
		return e;
	}

	s32 getCreatableGUIElementTypeCount() const {
		return 1;
	}

	EGUI_ELEMENT_TYPE getCreateableGUIElementType(s32 idx) const {
		return EGUIET_ELEMENT;
	}

	const c8* getCreateableGUIElementTypeName(s32 idx) const {
		if ( idx == 0 )
			return Label.c_str();

		return 0;
	}

	const c8* getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const {
		if ( type == EGUIET_ELEMENT )
			return Label.c_str();

		return 0;
	}

	EGUI_ELEMENT_TYPE getTypeFromName(const c8* name) const {
		return EGUIET_ELEMENT;
	}
};

}}

#endif
