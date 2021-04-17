// Copyright (C) 2013 Matthias Walden
// This file is currently not part of the "Irrlicht Engine".
// CamController.cpp
// This is non-restricted usage code.
// Changes by Nic Anderson:
// > Changed prototype and function heador of LoopProc (return value now "void")

#include "CamController.h"

// namespace by Nic Anderson
namespace irr {
namespace scene {

using core::matrix4;

//---------------------------------------------------------------------------
CamController::CamController(ICameraSceneNode* NewCameraNode, ISceneNode *NewTarget)
{
	// For if cam controller inherits IReferenceCounted
	//#ifdef _DEBUG
	//setDebugName("CamController");
	//#endif

	Target = 0;
	Camera = 0;
	UpdateCam = false;
	FollowTargetDir = true;
	FollowTargetDirX = true;

	DistToTarget = 0;
	Distance = 50.0f;
	DistChanged = false;

	XRotBoundary.X = 60.0f;
	XRotBoundary.Y = 300.0f;

	SetTarget(NewTarget);
	SetCamera(NewCameraNode);

	RotateCamera(vector3df(0));
	Camera->bindTargetAndRotation(true);

	//XRotBoundary
}
//---------------------------------------------------------------------------
//CamController::~CamController(void)
//{
//
//}
//---------------------------------------------------------------------------
void CamController::UpdateCamPos()
{
    CamPos = Camera->getPosition();
}
//---------------------------------------------------------------------------
void CamController::UpdateTargetPos()
{
    LastTargetPos = TargetPos;
    Target->updateAbsolutePosition();
    TargetPos = Target->getAbsolutePosition() + TargetCenter;
}
//---------------------------------------------------------------------------
void CamController::SetTarget (ISceneNode *NewTarget, vector3df NewTargetCenter)
{
    Target = NewTarget;
    TargetCenter = NewTargetCenter;
    UpdateTargetPos();
    LastTargetPos = TargetPos;

    if (Target && Camera)
        UpdateCam = true;
}
//---------------------------------------------------------------------------
void CamController::SetCamera (ICameraSceneNode* NewCameraNode)
{
    Camera = NewCameraNode;
    UpdateCamPos();

    if (Target && Camera)
        UpdateCam = true;
}
//---------------------------------------------------------------------------
void CamController::CalcCameraAlign (void)
{
    //vector3df NewCamPos;

    if(FollowTargetDir)
    {
        if (!FollowTargetDirX)
        {
            CamPos = TargetPos + (CamPos - TargetPos).normalize() * Distance;

            vector3df CurrCamRot;
            CurrCamRot = GetCameraRot();
            CurrCamRot.X = RotX;

            SetCameraRot(CurrCamRot);
        }
        else
        {
            CamPos = TargetPos + (CamPos - TargetPos).normalize() * Distance;
            CheckBoundaries();
        }
    }
    else
    {
        if(DistChanged)
        {
            CamPos = CamPos + (TargetPos - LastTargetPos);

            matrix4 mat;
            mat.setRotationDegrees( (CamPos - TargetPos).getHorizontalAngle());

            CamPos = vector3df(0,0,Distance);
            mat.rotateVect(CamPos);

            CamPos += TargetPos;
            DistChanged = false;
        }
        else
        {
            CamPos += (TargetPos - LastTargetPos);
        }
    }

    Camera->setPosition(CamPos);
    Camera->setTarget(TargetPos);
}
//---------------------------------------------------------------------------
void CamController::CheckBoundaries (void)
{
    //Kontrolle Der X-Grenzwerte
    vector3df CurrCamAngle = GetCameraRot();
    if (CurrCamAngle.X < 90.0f)
    {
        if(CurrCamAngle.X > XRotBoundary.X)
        {
            CurrCamAngle.X = XRotBoundary.X;
            SetCameraRot(CurrCamAngle);
        }
    }
    else
    {
        if(CurrCamAngle.X < XRotBoundary.Y)
        {
            CurrCamAngle.X = XRotBoundary.Y;
            SetCameraRot(CurrCamAngle);
        }
    }
}
//---------------------------------------------------------------------------
vector2df CamController::GetXRotBoundaries (void)
{
    return XRotBoundary;
}
//---------------------------------------------------------------------------
void CamController::SetXRotBoundaries (vector2df aXRotBoundary)
{
    XRotBoundary = aXRotBoundary;
}
//---------------------------------------------------------------------------
vector3df CamController::GetCameraRot(void)
{
    return (CamPos - TargetPos).getHorizontalAngle();
}
//---------------------------------------------------------------------------
vector3df CamController::GetCameraDirection (void)
{
    vector3df CurrCamDir = vector3df (TargetPos - CamPos);
    CurrCamDir.normalize();

    return CurrCamDir;
}
//---------------------------------------------------------------------------
vector3df CamController::GetCameraDirection (vector3df aRotationDelta)
{
    vector3df CurrCamDir = vector3df (TargetPos - CamPos);

    vector3df AdjustedCamDir(0,0,1.f);

    matrix4 m;
    m.setRotationDegrees(CurrCamDir.getHorizontalAngle() + aRotationDelta);
    m.rotateVect(AdjustedCamDir);

    AdjustedCamDir.normalize();

    return AdjustedCamDir;
}
//---------------------------------------------------------------------------
vector3df CamController::GetCameraDirectionH (void)
{
    vector3df CurrCamDir = vector3df (TargetPos - CamPos);
    CurrCamDir.Y=0;
    CurrCamDir.normalize();

    return CurrCamDir;
}
//---------------------------------------------------------------------------
vector3df CamController::GetCameraDirectionH (vector3df aRotationDelta)
{
    vector3df CurrCamDir = vector3df (TargetPos - CamPos);
    CurrCamDir.Y=0;

    vector3df AdjustedCamDir(0,0,1.f);

    matrix4 m;
    m.setRotationDegrees(CurrCamDir.getHorizontalAngle() + aRotationDelta);
    m.rotateVect(AdjustedCamDir);

    AdjustedCamDir.normalize();

    return AdjustedCamDir;
}
//---------------------------------------------------------------------------
void CamController::SetCameraRot(vector3df aRotation)
{
    //Kontrolle Der X-Grenzwerte
    if (aRotation.X < 90.0f)
    {
        if(aRotation.X > XRotBoundary.X)
        {
            aRotation.X = XRotBoundary.X;
        }
    }
    else
    {
        if(aRotation.X < XRotBoundary.Y)
        {
            aRotation.X = XRotBoundary.Y;
        }
    }

    if (!FollowTargetDirX)
    {
        RotX = aRotation.X;
    }

    matrix4 mat;
    mat.setRotationDegrees(aRotation);

    CamPos = vector3df(0,0,Distance);
    mat.rotateVect(CamPos);

    CamPos += TargetPos;
    Camera->setPosition(CamPos);
    UpdateCam = false;
}
//---------------------------------------------------------------------------
void CamController::RotateCamera(vector3df aRotationDelta)
{
    SetCameraRot(GetCameraRot() + aRotationDelta);
}
//---------------------------------------------------------------------------
f32 CamController::GetDistance(void)
{
    return Distance;
}
//---------------------------------------------------------------------------
void CamController::SetDistance(f32 aDistance)
{
    Distance = aDistance;
    UpdateCam = true;
    DistChanged = true;
}
//---------------------------------------------------------------------------
void CamController::ChangeDistance(f32 aDiff)
{
    SetDistance(Distance + aDiff);
}
//---------------------------------------------------------------------------
bool CamController::GetFollowTargetDir (void)
{
    return FollowTargetDir;
}
//---------------------------------------------------------------------------
void CamController::SetFollowTargetDir (bool aFollow)
{
    FollowTargetDir = aFollow;
}
//---------------------------------------------------------------------------
void CamController::SwitchFollowTargetDir(void)
{
    FollowTargetDir = !FollowTargetDir;
}
//---------------------------------------------------------------------------
bool CamController::GetFollowTargetDirX (void)
{
    return FollowTargetDirX;
}
//---------------------------------------------------------------------------
void CamController::SetFollowTargetDirX (bool aFollow)
{
    FollowTargetDirX = aFollow;

    if(!FollowTargetDirX)
    {
        RotX = GetCameraRot().X;
    }
}
//---------------------------------------------------------------------------
void CamController::SwitchFollowTargetDirX(void)
{
    FollowTargetDirX = !FollowTargetDirX;

    if(!FollowTargetDirX)
    {
        RotX = GetCameraRot().X;
    }
}
//---------------------------------------------------------------------------
f32 CamController::CalcDistToTarget(void)
{
    UpdateCamPos();
    UpdateTargetPos();

    DistToTarget = CamPos.getDistanceFrom(TargetPos);
    return DistToTarget;
}
//---------------------------------------------------------------------------
// return set to void by Nic Anderson
void CamController::LoopProc ()
{
	if (Target && Camera)
	{
        CalcDistToTarget();

        if (TargetPos != LastTargetPos)
            UpdateCam = true;

        if (UpdateCam)
        {
            CalcCameraAlign();
            UpdateCam = false;
        }
    }
}

}} // end namespaces
