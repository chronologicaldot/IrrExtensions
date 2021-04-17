// Copyright (C) 2013 Matthias Walden
// This file is currently not part of the "Irrlicht Engine".
// A Gui-Scene for rendering inside a IrrlichtGui...
// This is non-restricted usage code.
// Changes by Nic Anderson:
// > Casting variables in some places
// > Replaced IrrlichtDevice references by using IGUIEnvironment and ISceneManager
// > Changed constructor function header


#include "GUIScene.h"
// includes changed by Nic Anderson
#include <irrArray.h>
#include <IGUIEnvironment.h>
#include <ISceneManager.h>
#include <SViewFrustum.h>
#include <IAnimatedMeshSceneNode.h>
#include <ICameraSceneNode.h>
#include <ILightSceneNode.h>
#include <IGUIWindow.h>

// namespace by Nic Anderson
namespace irr {
namespace gui {

//---------------------------------------------------------------------------
GUIScene::GUIScene(IGUIEnvironment* pEnvironment, scene::ISceneManager* pSceneManager, IGUIElement* pParent, recti pRect, s32 id, video::E_DRIVER_TYPE pDriverType)
	: IGUIElement(EGUIET_ELEMENT, pEnvironment, pParent, id, pRect)
		// initializations added by Nic Anderson
	, FocusedNode(0)
	, IDriver(0)
	, IScene(0)
	, Cam(0)
	, Light(0)
	, Center(0)
	, MeshNode(0)
	, CamCTRL(0)
	, CurrX(0)
	, CurrY(0)
	, LastX(0)
	, LastY(0)
	, driverType(pDriverType)
	, drawBack(false)
	, drawBorder(false)
	, ExtraCamDist(0) // not added by Nic Anderson
{
    //setRelativePositionProportional(rectf(0,0,1,1));
    //recti Pos = getRelativePosition();
    //Pos.UpperLeftCorner.X +=1;
    //Pos.UpperLeftCorner.Y +=1;
    //Pos.LowerRightCorner.X -=1;
    //Pos.LowerRightCorner.Y -=1;
    //setRelativePosition(Pos);

	// changes by Nic Anderson
    IDriver = pEnvironment->getVideoDriver();
    IScene = pSceneManager->createNewSceneManager(false);

    Cam = IScene->addCameraSceneNode(0, vector3df(0,0,-100.f), vector3df(0,0,0));
   // Cam->setFOV(30 * DEGTORAD);

    Light = IScene->addLightSceneNode(Cam);
    Light->setRadius(1000.f);

    Center = IScene->addEmptySceneNode();

    IScene->getParameters()->setAttribute(ALLOW_ZWRITE_ON_TRANSPARENT, true);
}
//---------------------------------------------------------------------------
GUIScene::~GUIScene(void)
{
    IScene->drop();
}
//---------------------------------------------------------------------------
ISceneManager *GUIScene::GetScene(void)
{
    return IScene;
}
//---------------------------------------------------------------------------
ICameraSceneNode *GUIScene::GetCamera(void)
{
    return Cam;
}
//---------------------------------------------------------------------------
void GUIScene::SetOptimalCamDistToNode (ISceneNode *aNode)
{
    FocusedNode = aNode;
}
//---------------------------------------------------------------------------
void GUIScene::CalcOptimalCamDistToNode (ISceneNode *aNode)
{
    //Node
    aabbox3df NodeBBox = aNode->getBoundingBox();
    f32 NodeWidth = NodeBBox.MaxEdge.X - NodeBBox.MinEdge.X;
    f32 NodeHeight = NodeBBox.MaxEdge.Y - NodeBBox.MinEdge.Y;
    f32 NodeDepth = NodeBBox.MaxEdge.Z - NodeBBox.MinEdge.Z;

    f32 MaxWidth;

    if(NodeWidth > NodeDepth)
    {
        MaxWidth = NodeWidth;
    }
    else
    {
        MaxWidth = NodeDepth;
    }


    //Far
    const SViewFrustum* f = Cam->getViewFrustum();
    f32 Far = Cam->getFarValue();
    f32 FarWidth = line3df(f->getFarLeftUp(), f->getFarRightUp()).getLength();
    f32 FarHeight = line3df(f->getFarLeftUp(), f->getFarLeftDown()).getLength();

    //GuiElement
    recti CurrRect = getAbsolutePosition();

    //Zoom
    f32 ZoomX = CurrRect.getWidth() / MaxWidth;
    f32 ZoomY = CurrRect.getHeight() / NodeHeight;

    f32 CamDist;

    if (ZoomX < ZoomY)
    {
        CamDist =1.0f * ((MaxWidth / (FarWidth / Far)) + (0.5f * MaxWidth) );
    }
    else
    {
        CamDist =1.0f * ((NodeHeight / (FarHeight / Far)) + (0.5f * MaxWidth) );
    }

    if(!CamCTRL)
    {
        Cam->setPosition(vector3df(0,0,CamDist*-1));
    }
    else
    {
        CamCTRL->SetDistance(CamDist + ExtraCamDist); // modified by Nic Anderson for extra cam distance
    }

    /*cout << "Far: " << Far << endl;
    cout << "Far Width: " << FarWidth << endl;
    cout << "Far Height: " << FarHeight << endl;
    cout << "Node Width: " << NodeWidth << endl;
    cout << "Cam Dist: " << CamDist << endl;
    cout << "FOV: " << Cam->getFOV() << endl;
    cout << "NodeBBox.MinEdge.Z :" << NodeBBox.MinEdge.Z << endl;*/
}
//---------------------------------------------------------------------------
IAnimatedMeshSceneNode* GUIScene::setMesh (IAnimatedMesh *aMesh, bool updateOptimalCamDist)
{
    if(!aMesh)
    {
        if(MeshNode)
        {
            MeshNode->remove();
            MeshNode = 0;
        }
        return MeshNode;
    }
    else
    if(MeshNode)
    {
        MeshNode->remove();
        MeshNode = 0;
    }

    //if(!MeshNode)
    //{
        MeshNode = IScene->addAnimatedMeshSceneNode(aMesh);
        MeshNode->setPosition(MeshNode->getBoundingBox().getCenter()*-1);

        if(updateOptimalCamDist)
        {
            SetOptimalCamDistToNode(MeshNode);
        }
    //}

    return MeshNode;
}
//---------------------------------------------------------------------------
IAnimatedMeshSceneNode* GUIScene::setMesh (stringw aFileName, bool updateOptimalCamDist)
{
    return setMesh(IScene->getMesh(aFileName), updateOptimalCamDist);
}
//---------------------------------------------------------------------------
void GUIScene::enableCamControl (bool aEnable)  // Nic Anderson renamed
{
    if(!CamCTRL)
    {
        if(aEnable)
        {
            CamCTRL = new CamController(Cam, Center);
        }
    }
    else
    {
        if(!aEnable)
        {
            delete CamCTRL;
            CamCTRL = 0;
        }
    }
}
//---------------------------------------------------------------------------
void GUIScene::setDrawBackground( bool pDrawBack ) // by Nic Anderson
{
	drawBack = pDrawBack;
}

void GUIScene::setDrawBorder( bool pDrawBorder ) // by Nic Anderson
{
	drawBorder = pDrawBorder;
}

void GUIScene::setExtraCamDistance( f32 pExtraDist ) // by Nic Anderson
{
	ExtraCamDist = pExtraDist;
}
//---------------------------------------------------------------------------
void GUIScene::draw (void)
{
    rect<s32> OldViewPort = IDriver->getViewPort();

    IDriver->setViewPort(getAbsoluteClippingRect());

    //IDriver->clearZBuffer(); // function depreciated, but I'm not sure why

	// border setting added by Nic Anderson
	recti borderRect;
	if ( driverType == video::EDT_BURNINGSVIDEO ) // Burning's doesn't set the viewport for 2D drawing
	{
		borderRect = getAbsoluteClippingRect();
		borderRect.UpperLeftCorner.X -= 2;
		borderRect.UpperLeftCorner.Y -= 2;
		borderRect.LowerRightCorner.X += 2;
		borderRect.LowerRightCorner.Y += 2;
	} else {
		borderRect = Environment->getRootGUIElement()->getAbsoluteClippingRect();
	}
	if ( drawBorder )
	{
		Environment->getSkin()->draw3DSunkenPane(
									this,
									drawBack? 0xff000000:0, // black
									false,
									true,
									borderRect,
									0 );
	}

    dimension2d<s32> Size = getAbsoluteClippingRect().getSize();
    IScene->getActiveCamera()->setAspectRatio((f32)Size.Width / (f32)Size.Height);

    if(CamCTRL)
        CamCTRL->LoopProc();

    IScene->drawAll();

    if(FocusedNode)
    {
        CalcOptimalCamDistToNode(FocusedNode);
        FocusedNode = 0;
    }

    IDriver->setViewPort(OldViewPort);
}
//---------------------------------------------------------------------------
bool GUIScene::OnEvent (const SEvent &event)
{
    if(CamCTRL)
    {
        switch(event.EventType)
        {
            case EET_MOUSE_INPUT_EVENT:
            {
                if(event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
                {
                    LastX = (f32)event.MouseInput.X;
                    LastY = (f32)event.MouseInput.Y;
                    return true;
                }
                else
                if(event.MouseInput.Event==EMIE_MOUSE_MOVED)
                {
                    if(event.MouseInput.isLeftPressed())
                    {
                        CurrX = (f32)event.MouseInput.X;
                        CurrY = (f32)event.MouseInput.Y;

                        CamCTRL->RotateCamera(vector3df(LastY-CurrY,CurrX-LastX,0));

                        LastX = CurrX;
                        LastY = CurrY;
                        return true;
                    }
                }
            }
            break;

		// added by Nic Anderson (to remove -Wall error)
		default: break;
        }
    }

    return IGUIElement::OnEvent(event);
}

}} // end namespaces
