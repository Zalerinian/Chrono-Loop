#include "stdafx.h"
#include "VrInputManager.h"
#include "../Common/Logger.h"

VRInputManager & VRInputManager::Instance() {
	if (!sInstance)
		sInstance = new VRInputManager();
	return *sInstance;
}

void VRInputManager::Initialize(vr::IVRSystem * _vr) {
	if (sInstance)
		sInstance->mInitialize(_vr);
}

void VRInputManager::Shutdown() {
	if (sInstance)
		delete sInstance;
}

VRInputManager* VRInputManager::sInstance = nullptr;

VRInputManager::VRInputManager() {}

VRInputManager::~VRInputManager() {}

void VRInputManager::mInitialize(vr::IVRSystem *_vr) {
	if (nullptr == _vr) {
		SystemLogger::GetLog() << "VR Input is disabled." << std::endl;
		return;
	}
	mVRSystem = _vr;
	int rightID = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
	int leftID = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
	SystemLogger::GetLog() << "Right controller ID: " << rightID << std::endl;
	SystemLogger::GetLog() << "Left controller ID:  " << leftID << std::endl;
	mRightController.SetUp(rightID, mVRSystem);
	mLeftController.SetUp(leftID, mVRSystem);
	mPlayerPosition = Math::MatrixIdentity();
}

void VRInputManager::iUpdate() {
	if (mRightController.GetIndex() < 0) {
		mRightController.SetIndex(mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand));
		if (mRightController.GetIndex() > 0) {
			mRightController.Update();
			SystemLogger::GetLog() << "Right Controller reconnected at " << mRightController.GetIndex() << std::endl;
		}
	} else {
		mRightController.Update();
	}
	if (mLeftController.GetIndex() < 0) {
		mLeftController.SetIndex(mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand));
		if (mLeftController.GetIndex() > 0) {
			mLeftController.Update();
			SystemLogger::GetLog() << "Left Controller reconnected at " << mLeftController.GetIndex() << std::endl;
		}
	} else {
		mLeftController.Update();
	}
	vr::VRCompositor()->WaitGetPoses(mPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
}


Controller& VRInputManager::iGetController(bool left) {
	if (left) {
		return mLeftController;
	} else {
		return mRightController;
	}
}
