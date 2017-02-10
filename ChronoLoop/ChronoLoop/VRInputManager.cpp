#include "stdafx.h"
#include "VrInputManager.h"
#include <iostream>

VRInputManager* VRInputManager::sInstance = nullptr;

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
	if (mRightController.GetIndex() < 0)
	{
		mRightController.SetIndex(mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand));
		std::cout << "Right Controler reconnected." << std::endl;
	}
	if (mLeftController.GetIndex() < 0)
	{
		mLeftController.SetIndex(mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand));
		std::cout << "Left Controler reconnected." << std::endl;
	}
	if (true) {
		if (mRightController.GetPressDown(vr::k_EButton_SteamVR_Trigger))
			std::cout << "Right Trigger Pressed" << std::endl;
		if (mRightController.GetPressDown(vr::k_EButton_SteamVR_Touchpad))
			std::cout << "Right Touchpad Pressed" << std::endl;
		if (mRightController.GetPressDown(vr::k_EButton_ApplicationMenu))
			std::cout << "Right Menu Pressed" << std::endl;
		if (mRightController.GetPressDown(vr::k_EButton_Grip))
			std::cout << "Right Grip Pressed" << std::endl;
	}
	if(true) {
		if (mLeftController.GetPressDown(vr::k_EButton_SteamVR_Trigger))
			std::cout << "Left Trigger Pressed" << std::endl;
		if (mLeftController.GetPressDown(vr::k_EButton_SteamVR_Touchpad))
			std::cout << "Left Touchpad Pressed" << std::endl;
		if (mLeftController.GetPressDown(vr::k_EButton_ApplicationMenu))
			std::cout << "Left Menu Pressed" << std::endl;
		if (mLeftController.GetPressDown(vr::k_EButton_Grip))
			std::cout << "Left Grip Pressed" << std::endl;
	}
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
