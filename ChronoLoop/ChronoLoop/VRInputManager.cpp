#include "stdafx.h"
#include "VrInputManager.h"
#include <iostream>

static VRInputManager* sInstance = nullptr;

VRInputManager::VRInputManager()
{
}

VRInputManager::~VRInputManager()
{
}

void VRInputManager::mInitialize(vr::IVRSystem * _hmd)
{
	mHmd = _hmd;
	int rightID = mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
	int leftID = mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
	std::cout << "Right controller ID: " << rightID << std::endl;
	std::cout << "Left controller ID:  " << leftID << std::endl;
	mRightController.SetUp(rightID, mHmd);
	mLeftController.SetUp(leftID, mHmd);
}

void VRInputManager::update()
{
	if (mRightController.GetPressDown(vr::k_EButton_SteamVR_Trigger))
		std::cout << "Right Trigger Pressed";
	if (mRightController.GetPressDown(vr::k_EButton_SteamVR_Touchpad))
		std::cout << "Right Touchpad Pressed";
	if (mRightController.GetPressDown(vr::k_EButton_ApplicationMenu))
		std::cout << "Right Menu Pressed";
	if (mRightController.GetPressDown(vr::k_EButton_Grip))
		std::cout << "Right Grip Pressed";
	if (mLeftController.GetPressDown(vr::k_EButton_SteamVR_Trigger))
		std::cout << "Left Trigger Pressed";
	if (mLeftController.GetPressDown(vr::k_EButton_SteamVR_Touchpad))
		std::cout << "Left Touchpad Pressed";
	if (mLeftController.GetPressDown(vr::k_EButton_ApplicationMenu))
		std::cout << "Left Menu Pressed";
	if (mLeftController.GetPressDown(vr::k_EButton_Grip))
		std::cout << "Left Grip Pressed";
}

VRInputManager & VRInputManager::Instance()
{
	if (!sInstance)
		sInstance = new VRInputManager();
	return *sInstance;
}

void VRInputManager::Initialize(vr::IVRSystem * _hmd)
{
	if (sInstance)
		sInstance->mInitialize(_hmd);
}

void VRInputManager::Shutdown()
{
	if (sInstance)
		delete sInstance;
}
