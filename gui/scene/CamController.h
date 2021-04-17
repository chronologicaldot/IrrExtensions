// Copyright (C) 2013 Matthias Walden
// This file is currently not part of the "Irrlicht Engine".
// CamController.h
// This is non-restricted usage code.
// Changes by Nic Anderson:
// > Changed prototype and function heador of LoopProc (return value now "void")

#ifndef __CAMCONTROLLER_H_INCLUDED__
#define __CAMCONTROLLER_H_INCLUDED__

#include "irrArray.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// namespace by Nic Anderson
namespace irr {
namespace scene {

using core::vector3df;

class CamController
{
	public:

		CamController(ICameraSceneNode* NewCameraNode, ISceneNode *NewTarget);
		//~CamController(void);

	    void SetTarget (ISceneNode *NewTarget, vector3df NewTargetCenter = vector3df (0.f));
	    void SetCamera (ICameraSceneNode* NewCameraNode);

	    void CalcCameraAlign (void);

	    vector2df GetXRotBoundaries (void);
	    void SetXRotBoundaries (vector2df aXRotBoundary);

	    vector3df GetCameraRot(void);
	    vector3df GetCameraDirection (void);
	    vector3df GetCameraDirection (vector3df aRotationDelta);
	    vector3df GetCameraDirectionH (void); //H = horizontal!
	    vector3df GetCameraDirectionH (vector3df aRotationDelta); //H = horizontal!
	    void SetCameraRot(vector3df aRotation);
	    void RotateCamera(vector3df aRotationDelta);

	    f32  GetDistance(void);
	    void SetDistance(f32 aDistance);
	    void ChangeDistance(f32 aDiff);

	    bool GetFollowTargetDir (void);
	    void SetFollowTargetDir (bool aFollow);
	    void SwitchFollowTargetDir (void);

	    bool GetFollowTargetDirX (void);
	    void SetFollowTargetDirX (bool aFollow);
	    void SwitchFollowTargetDirX (void);

		// return set to void by Nic Anderson
    	void LoopProc ();

	private:

        f32 CalcDistToTarget(void);
		void UpdateCamPos();
		void UpdateTargetPos();

		void CheckBoundaries (void);

		ISceneNode *Target;
		ICameraSceneNode* Camera;

		vector3df CamPos;
		vector3df TargetPos;
		vector3df TargetCenter;
		vector3df LastTargetPos;

		vector2df XRotBoundary;
		f32 RotX;

		f32 DistToTarget;
		bool UpdateCam;
		bool FollowTargetDir;
		bool FollowTargetDirX;

		f32 Distance;
		bool DistChanged;
};

}} // end namespaces

#endif // __CAMCONTROLLER_H_INCLUDED__
