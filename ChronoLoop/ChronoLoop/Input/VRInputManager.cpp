#include "stdafx.h"
#include "VrInputManager.h"
#include "../Common/Logger.h"

VRInputManager* VRInputManager::sInstance = nullptr;

VRInputManager::VRInputManager() {}

VRInputManager::~VRInputManager() {}

void VRInputManager::mInitialize(vr::IVRSystem * _hmd) {
	if (nullptr == _hmd) {
		SystemLogger::GetLog() << "VR Input is disabled." << std::endl;
		return;
	}
	mHmd = _hmd;
	int rightID = mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
	int leftID = mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
	SystemLogger::GetLog() << "Right controller ID: " << rightID << std::endl;
	SystemLogger::GetLog() << "Left controller ID:  " << leftID << std::endl;
	mRightController.SetUp(rightID, mHmd);
	mLeftController.SetUp(leftID, mHmd);
}

void VRInputManager::update() {
	if (mRightController.GetIndex() < 0) {
		mRightController.SetIndex(mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand));
		mRightController.Update();
		if (mRightController.GetIndex() > 0) {
			SystemLogger::GetLog() << "Right Controller reconnected." << std::endl;
		}
	} else {
		mRightController.Update();
	}
	if (mLeftController.GetIndex() < 0) {
		mLeftController.SetIndex(mHmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand));
		mLeftController.Update();
		if (mLeftController.GetIndex() > 0) {
			SystemLogger::GetLog() << "Left Controller reconnected." << std::endl;
		}
	} else {
		mLeftController.Update();
	}


	//if (mRightController.GetIndex() > 0) {
	//	if (mRightController.GetPress(vr::k_EButton_SteamVR_Trigger)) {
	//		SystemLogger::GetLog() << "Right Trigger Pressed" << std::endl;
	//	}
	//	if (mRightController.GetPress(vr::k_EButton_SteamVR_Touchpad)) {
	//		SystemLogger::GetLog() << "Right Touchpad Pressed" << std::endl;
	//	}
	//	if (mRightController.GetPress(vr::k_EButton_ApplicationMenu)) {
	//		SystemLogger::GetLog() << "Right Menu Pressed" << std::endl;
	//	}
	//	if (mRightController.GetPress(vr::k_EButton_Grip)) {
	//		SystemLogger::GetLog() << "Right Grip Pressed" << std::endl;
	//	}
	//}
	//if (mLeftController.GetIndex() > 0) {
	//	if (mLeftController.GetPress(vr::k_EButton_SteamVR_Trigger)) {
	//		SystemLogger::GetLog() << "Left Trigger Pressed" << std::endl;
	//	}
	//	if (mLeftController.GetPress(vr::k_EButton_SteamVR_Touchpad)) {
	//		SystemLogger::GetLog() << "Left Touchpad Pressed" << std::endl;
	//	}
	//	if (mLeftController.GetPress(vr::k_EButton_ApplicationMenu)) {
	//		SystemLogger::GetLog() << "Left Menu Pressed" << std::endl;
	//	}
	//	if (mLeftController.GetPress(vr::k_EButton_Grip)) {
	//		SystemLogger::GetLog() << "Left Grip Pressed" << std::endl;
	//	}
	//}
}

VRInputManager & VRInputManager::Instance() {
	if (!sInstance)
		sInstance = new VRInputManager();
	return *sInstance;
}

void VRInputManager::Initialize(vr::IVRSystem * _hmd) {
	if (sInstance)
		sInstance->mInitialize(_hmd);
}

void VRInputManager::Shutdown() {
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
