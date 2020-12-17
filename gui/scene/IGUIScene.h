// (c) 2015 Nicolaus Anderson
// Based on code by vitek:
// http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=13220

#ifndef I_GUI_SCENE_H
#define I_GUI_SCENE_H

#include <IGUIElement.h>

namespace irr {

namespace scene {
	class ISceneManager;
}

namespace gui {

using scene::ISceneManager;

class IGUIScene : public IGUIElement
{
protected:
	ISceneManager* sceneManager;

public:
	IGUIScene(IGUIEnvironment* pEnvironment, IGUIElement* pParent, s32 id, core::rect<s32> pRect)
         //: IGUIElement(EGUIET_MESH_VIEWER, pEnvironment, pParent, id, pRect)
	~IGUIScene();

	void setSceneManager( ISceneManager* pSceneManager )
	{
		sceneManager = pSceneManager;
	}

	ISceneManager* getSceneManager()
	{
		return sceneManager;
	}

	virtual void draw();

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() const { return "iScene"; }
};

}}

#endif // #ifndef I_GUI_SCENE_H
