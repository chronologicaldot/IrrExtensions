// Copyright (C) 2013 Matthias Walden
// This file is currently not part of the "Irrlicht Engine".
// A Gui-Scene for rendering inside a IrrlichtGui...
// This is non-restricted usage code.
// Changes by Nic Anderson:
// > Casting variables in some places
// > Set in namespaces
// > Replaced IrrlichtDevice references by using IGUIEnvironment and ISceneManager
// > Changed constructor function header
//		and added rectangle

#ifndef __GUIScene_H_INCLUDED__
#define __GUIScene_H_INCLUDED__

#include "CamController.h"
#include <IGUIElement.h>


// namespaces and using by Nic Anderson
namespace irr {

namespace scene {
	class ISceneManager;
	class ISceneNode;
	class ICameraSceneNode;
	class ILightSceneNode;
	class IAnimatedMesh;
	class IAnimatedMeshSceneNode;
}

namespace video {
	class IVideoDriver;
}

namespace gui {

using core::recti;
using scene::CamController;
using video::IVideoDriver;
using scene::ISceneManager;
using scene::ISceneNode;
using scene::ICameraSceneNode;
using scene::ILightSceneNode;
using scene::IAnimatedMesh;
using scene::IAnimatedMeshSceneNode;

class IGUIWindow;

//---------------------------------------------------------------------------
class GUIScene : public IGUIElement
{
    public:
		//
		GUIScene(IGUIEnvironment* pEnvironment, scene::ISceneManager* pSceneManager, IGUIElement* pParent, recti pRect, s32 id=-1,
			video::E_DRIVER_TYPE pDriverType=EDT_NULL /* added by Nic Anderson */ );
        ~GUIScene(void);

        ISceneManager *GetScene(void);
        ICameraSceneNode *GetCamera(void);
        void SetOptimalCamDistToNode (ISceneNode *aNode);

        IAnimatedMeshSceneNode* setMesh (IAnimatedMesh *aMesh = 0, bool updateOptimalCamDist = true);
        IAnimatedMeshSceneNode* setMesh (stringw aFileName, bool updateOptimalCamDist = true);
        void enableCamControl(bool aEnable); // Nic Anderson renamed
		void setDrawBackground( bool pDrawBack ); // by Nic Anderson
		void setDrawBorder( bool pDrawBorder ); // by Nic Anderson
		void setExtraCamDistance( f32 pExtraDist ); // by Nic Anderson

        virtual void draw (void);
        virtual bool OnEvent (const SEvent &event);

        CamController *getCamCTRL(void){return CamCTRL;};

    private:
        ISceneNode * FocusedNode;
        void CalcOptimalCamDistToNode (ISceneNode *aNode);

        IVideoDriver* IDriver;
        ISceneManager* IScene;
        ICameraSceneNode *Cam;
        ILightSceneNode *Light;

        ISceneNode* Center;
        IAnimatedMeshSceneNode* MeshNode;
        CamController* CamCTRL;

        f32 CurrX;
        f32 CurrY;
        f32 LastX;
        f32 LastY;

	// added by Nic Anderson
	video::E_DRIVER_TYPE driverType;
	bool drawBack; // Indicates the background should be drawn
	bool drawBorder; // Indicates the border should be drawn
	f32 ExtraCamDist;
};

}} // close namespaces

#endif // __GUIScene_H_INCLUDED__
