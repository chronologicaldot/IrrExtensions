// (C) 2020 Nicolaus Anderson

#ifndef IRR_EXT_GUI_ELEMENT_FACTORY_H
#define IRR_EXT_GUI_ELEMENT_FACTORY_H

#include <IGUIElementFactory.h>

namespace irr {
namespace scene {
	class ISceneManager;
}
namespace gui {

//! IrrEXT GUI Element Factory
/*
	Produces the GUI elements of the IrrExt project by chronologicaldot (Nic Anderson).
	See Irrlicht file IGUIElementFactory.h for documentation.
*/
class IrrExtGUIElementFactory
	: public irr::gui::IGUIElementFactory
{
	IGUIEnvironment* Environment;
	scene::ISceneManager* SceneManager;

public:
	IrrExtGUIElementFactory( IGUIEnvironment*, scene::ISceneManager* );
	virtual IGUIElement* addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent=0);
	virtual IGUIElement* addGUIElement(const c8* typeName, IGUIElement* parent=0);
	virtual s32 getCreatableGUIElementTypeCount() const;
	virtual EGUI_ELEMENT_TYPE getCreateableGUIElementType(s32 idx) const;
	virtual const c8* getCreateableGUIElementTypeName(s32 idx) const;
	virtual const c8* getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const;
};

} // namespace gui
} // namespace irr

#endif // IRR_EXT_GUI_ELEMENT_FACTORY_H
