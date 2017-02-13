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
	if (nullptr == _hmd) {
		std::cout << "VR Input is disabled." << std::endl;
		return;
	}
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
	if (!mInitialized) {
		return;
	}
	if (mRightController.GetIndex() < 0)
	{
		mRightController.SetIndex(mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand));
		if (mRightController.GetIndex() > 0)
			std::cout << "Right Controller reconnected." << std::endl;
	}
	if (mLeftController.GetIndex() < 0)
	{
		mLeftController.SetIndex(mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand));
		if (mLeftController.GetIndex() > 0)
			std::cout << "Left Controller reconnected." << std::endl;
	}
	if (mRightController.GetIndex() > 0) {
		if (mRightController.GetPress(vr::k_EButton_SteamVR_Trigger))
			std::cout << "Right Trigger Pressed" << std::endl;
		if (mRightController.GetPress(vr::k_EButton_SteamVR_Touchpad))
			std::cout << "Right Touchpad Pressed" << std::endl;
		if (mRightController.GetPress(vr::k_EButton_ApplicationMenu))
			std::cout << "Right Menu Pressed" << std::endl;
		if (mRightController.GetPress(vr::k_EButton_Grip))
			std::cout << "Right Grip Pressed" << std::endl;
	}
	if(mLeftController.GetIndex() > 0) {
		if (mLeftController.GetPress(vr::k_EButton_SteamVR_Trigger))
			std::cout << "Left Trigger Pressed" << std::endl;
		if (mLeftController.GetPress(vr::k_EButton_SteamVR_Touchpad))
			std::cout << "Left Touchpad Pressed" << std::endl;
		if (mLeftController.GetPress(vr::k_EButton_ApplicationMenu))
			std::cout << "Left Menu Pressed" << std::endl;
		if (mLeftController.GetPress(vr::k_EButton_Grip))
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

Controller& VRInputManager::GetController(bool left) {
	if (left) {
		return mLeftController;
	} else {
		return mRightController;
	}
}
