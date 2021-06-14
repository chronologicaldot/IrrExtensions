// (C) 2020 Nicolaus Anderson

#include "IrrExtGUIElementFactory.h"
#include <ISceneManager.h>
// EXCLUDE: GUIWindow2
#include "SimpleGraph/SGraph2D.h"
#include "GUIColorEditBox.h"
#include "GUIColorSample.h"
#include "GUIDualSection.h"
#include "GUIDropdownSelector.h"
#include "GUIFileSelectPanel.h"
#include "GUIGroupingPanel.h"
#include "GUIMarkedSlider.h"
#include "GUIMaterialPanel.h"
#include "GUIMatrixPanel.h"
#include "GUIPanel.h"
#include "GUISProgressBar.h"
#include "GUISColorSelect.h"
#include "GUIScrollPane.h"
#include "GUITextureView.h"
#include "GUITreeTable.h" // Requires irrTree: util/irrTree/irrTree/irrTree.h
#include "GUIVectorPanel.h"


namespace irr {
namespace gui {

IrrExtGUIElementFactory::IrrExtGUIElementFactory( IGUIEnvironment* environment, scene::ISceneManager* manager )
	: Environment(environment)
	, SceneManager(manager)
{}

IGUIElement*
IrrExtGUIElementFactory::addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent) {
	return 0;
}

IGUIElement*
IrrExtGUIElementFactory::addGUIElement(const c8* typeName, IGUIElement* parent) {
	core::stringc elemname(typeName);

	if ( elemname == SGraph2D::staticTypeName() ) {
		return new SGraph2D( Environment, parent, -1, core::recti(), core::rectf(-1,-1,1,1) );
	}
	else if ( elemname == GUIColorEditBox::staticTypeName() ) {
		return new GUIColorEditBox( Environment, parent, core::recti() );
	}
	else if ( elemname == GUIColorSample::staticTypeName() ) {
		return new GUIColorSample( Environment, parent, core::recti() );
	}
	else if ( elemname == GUIDualSection::staticTypeName() ) {
		// Default: horizontal/left-and-right sections
		return new GUIDualSection( false, 0.5f, Environment, parent, core::recti() );
	}
	else if ( elemname == GUIDropdownSelector::staticTypeName() ) {
		return new GUIDropdownSelector( Environment, parent, core::recti() );
	}
	else if ( elemname == GUIFileSelectPanel::staticTypeName() ) {
		return new GUIFileSelectPanel( Environment, parent, core::recti(), "." );
	}
	else if ( elemname == GUIGroupingPanel::staticTypeName() ) {
		/* Could be saved for elsewhere because we might want access to getClientArea().
		Alternatively, add it to items set by serializeAttributes(). */
		return new GUIGroupingPanel( L"", Environment, parent, core::recti() );
	}
	else if ( elemname == GUIMarkedSlider::staticTypeName() ) {
		// Default: vertical
		return new GUIMarkedSlider( true, 100, core::recti(), Environment, parent );
	}
	else if ( elemname == GUIMaterialPanel::staticTypeName() ) {
		return new GUIMaterialPanel( Environment, SceneManager, parent, core::recti() );
	}
	else if ( elemname == GUIMatrixPanel::staticTypeName() ) {
		// Default: rotate in degrees
		return new GUIMatrixPanel( true, Environment, parent, core::recti() );
	}
	else if ( elemname == GUIPanel::staticTypeName() ) {
		return new GUIPanel( Environment, parent, core::recti() );
	}
	else if ( elemname == GUISProgressBar::staticTypeName() ) {
		return new GUISProgressBar( Environment, parent, core::recti() );
	}
	else if ( elemname == GUISColorSelect::staticTypeName() ) {
		return new GUISColorSelect( Environment, parent, core::recti() );
	}
	else if ( elemname == GUIScrollPane::staticTypeName() ) {
		return new GUIScrollPane( Environment, parent, core::recti() );
	}
	else if ( elemname == GUITextureView::staticTypeName() ) {
		// FIXME: GUITextureView requires a unique ID so it can give a unique name to render targets
		static u32 i = 1; ++i;
		return new GUITextureView(0, Environment, parent, core::recti(), i);
	}
	else if ( elemname == GUITreeTable::staticTypeName() ) {
		return new GUITreeTable( Environment, parent, core::recti() );
	}
	else if ( elemname == GUIVectorPanel::staticTypeName() ) {
		return new GUIVectorPanel( Environment, parent, core::recti() );
	}

	return 0;
}

s32
IrrExtGUIElementFactory::getCreatableGUIElementTypeCount() const {
	return 17;
}

EGUI_ELEMENT_TYPE
IrrExtGUIElementFactory::getCreateableGUIElementType(s32 idx) const {
	return EGUIET_ELEMENT;
}

const c8*
IrrExtGUIElementFactory::getCreateableGUIElementTypeName(s32 idx) const {
	switch(idx)
	{
	case 0: return SGraph2D::staticTypeName();
	case 1: return GUIColorEditBox::staticTypeName();
	case 2: return GUIColorSample::staticTypeName();
	case 3: return GUIDualSection::staticTypeName();
	case 4: return GUIDropdownSelector::staticTypeName();
	case 5: return GUIFileSelectPanel::staticTypeName();
	case 6: return GUIGroupingPanel::staticTypeName();
	case 7: return GUIMarkedSlider::staticTypeName();
	case 8: return GUIMaterialPanel::staticTypeName();
	case 9: return GUIMatrixPanel::staticTypeName();
	case 10: return GUIPanel::staticTypeName();
	case 11: return GUISProgressBar::staticTypeName();
	case 12: return GUISColorSelect::staticTypeName();
	case 13: return GUIScrollPane::staticTypeName();
	case 14: return GUITextureView::staticTypeName();
	case 15: return GUITreeTable::staticTypeName();
	case 16: return GUIVectorPanel::staticTypeName();
	default: return "";
	}
}

const c8*
IrrExtGUIElementFactory::getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const {
	return "";
}

} // namespace gui
} // namespace irr
