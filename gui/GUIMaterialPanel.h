// (c) 2014 Nicolaus Anderson

#include <IGUIElement.h>

#ifndef GUI_MATERIAL_PANEL_H
#define GUI_MATERIAL_PANEL_H

namespace irr {

namespace scene {
	class ISceneManager;
	class IMeshSceneNode;
}

namespace video {
	class SColor;
	class SMaterial;
}

namespace gui {

using core::vector2di;
using core::recti;
using video::SMaterial;
using scene::ISceneManager;
using scene::IMeshSceneNode;

class IGUIButton;
class IGUICheckBox;
class IGUIWindow;
class IGUIEditBox;
class GUIColorEditBox;
class GUIColorSample;
class GUISColorSelect;
class GUIScene;
class GUIWindow2;
class GUIFileSelectPanel;

/* Class : GUI Material Panel

This creates a panel with buttons for opening color-selection windows,
editboxes for editing the colors (ambient, diffuse, emissive, and specular),
and checkboxes for turning on the engine-features of wireframe drawing,
gouraud shading, making the material lit by lights, and having the
engine generate mipmaps for the material textures.
It is not possible to remove any of these items.
In the future, the buttons, editboxes, and checkboxes should be removable,
but this requires grabbing them in the constructor after they have been
created and dropping them in the deconstructor.

The design of this class is such that it can handle all of the material
editing without the programmer having to monitor its events and update
changes to the material accordingly. Pass a pointer to the material
via setMaterial().

Optionally, this panel should have a 3D mesh scene that shows a sphere,
indicating what the material looks like when applied to a scene node.
*/
class GUIMaterialPanel : public IGUIElement
{
protected:
	SMaterial* material;

	GUIScene* meshMaterialScene;
	IMeshSceneNode* materialSceneNode;

	GUIColorEditBox* ambientColorEditBox;
	GUIColorSample* ambientPanelSample;
	IGUIButton* ambientColorButton;
	IGUIWindow* ambientColorSelectWindow;
	GUISColorSelect* ambientColorSelect;
	GUIColorSample* ambientColorSample;

	GUIColorEditBox* diffuseColorEditBox;
	GUIColorSample* diffusePanelSample;
	IGUIButton* diffuseColorButton;
	IGUIWindow* diffuseColorSelectWindow;
	GUISColorSelect* diffuseColorSelect;
	GUIColorSample* diffuseColorSample;

	GUIColorEditBox* emissiveColorEditBox;
	GUIColorSample* emissivePanelSample;
	IGUIButton* emissiveColorButton;
	IGUIWindow* emissiveColorSelectWindow;
	GUISColorSelect* emissiveColorSelect;
	GUIColorSample* emissiveColorSample;

	GUIColorEditBox* specularColorEditBox;
	GUIColorSample* specularPanelSample;
	IGUIButton* specularColorButton;
	IGUIWindow* specularColorSelectWindow;
	GUISColorSelect* specularColorSelect;
	GUIColorSample* specularColorSample;

	IGUICheckBox* wireframeCheckbox;
	IGUICheckBox* gourandShadingCheckbox;
	IGUICheckBox* lightingCheckbox;
	IGUICheckBox* mipmapsCheckbox;

	recti colorSelectWindowRect;

	IGUIEditBox* textureNameEditBox;
	IGUIButton* textureSelectButton;
	IGUIButton* textureRemoveButton;

	GUIWindow2* selectTextureWindow;
	GUIFileSelectPanel* selectTextureFilePanel;


public:
	GUIMaterialPanel( IGUIEnvironment* pEnvironment, ISceneManager* pSceneManager, IGUIElement* pParent, recti pRect, s32 id=-1 );
	~GUIMaterialPanel();

	void setMaterial( SMaterial& pMaterial );
	void closeWindows();
	void setAmbientLight( irr::video::SColor pColor );

	virtual bool OnEvent( const SEvent& event );

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "materialPanel"; }
protected:
	bool OnGuiEvent( const SEvent& event );
	void sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement=0 );

	// Set the mesh material to a copy of the one pointed to by "material".
	void updateMeshScene();

	IGUIWindow* createColorSelectWindow();
	void createSelectTextureDialog();
};

}
}

#endif
