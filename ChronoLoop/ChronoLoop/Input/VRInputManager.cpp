#include "stdafx.h"
#include "VrInputManager.h"
#include "../Common/Logger.h"
#include "../Core/TimeManager.h"

namespace Epoch {

	VIM* VRInputManager::sInstance = nullptr;
	
	VIM& VRInputManager::GetInstance() {
		return *sInstance;
	}
	
	void VRInputManager::Initialize(vr::IVRSystem * _vr) {
		if (nullptr == sInstance) {
			if (nullptr == _vr) {
				SystemLogger::Debug() << "No valid VR system was found, VR will be disabled." << std::endl;
			}
			sInstance = new VIM(_vr);
		}
	}
	
	void VRInputManager::DestroyInstance() {
		if (nullptr != sInstance) {
			delete sInstance;
			sInstance = nullptr;
		}
	}
	
	VRInputManager::VRInputManager() {}
	
	VRInputManager::~VRInputManager() {}




	VIM::VIM(vr::IVRSystem *_vr) {
		if (nullptr == _vr) {
			SystemLogger::Warn() << "VR Input is disabled." << std::endl;
			return;
		}
		mVRSystem = _vr;
		int rightID = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
		int leftID = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
		SystemLogger::Info() << "Right controller ID: " << rightID << std::endl;
		SystemLogger::Info() << "Left controller ID:  " << leftID << std::endl;
		mRightController.Setup(rightID);
		mLeftController.Setup(leftID);
		mPlayerPosition = matrix4::CreateTranslation(2, -1, 8);
	}

	VIM::~VIM() {}

	void VIM::Update() {
		int GestureCheck = 0;
		if (mRightController.GetIndex() < 0) {
			mRightController.mIndex = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
			if (mRightController.GetIndex() > 0) {
				mRightController.Update();
				GestureCheck = mRightController.CheckGesture();
				TimeManager::Instance()->BrowseTimeline(GestureCheck, 1);
				SystemLogger::Info() << "Right Controller connected at index " << mRightController.GetIndex() << std::endl;
			}
		} else {
			mRightController.Update();
			GestureCheck = mRightController.CheckGesture();
			TimeManager::Instance()->BrowseTimeline(GestureCheck, 1);
		}
		if (mLeftController.GetIndex() < 0) {
			mLeftController.mIndex = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
			if (mLeftController.GetIndex() > 0) {
				mLeftController.Update();
				GestureCheck = mLeftController.CheckGesture();
				TimeManager::Instance()->BrowseTimeline(GestureCheck, 1);
				SystemLogger::Info() << "Left Controller connected at index " << mLeftController.GetIndex() << std::endl;
			}
		} else {
			mLeftController.Update();
			GestureCheck = mLeftController.CheckGesture();
			TimeManager::Instance()->BrowseTimeline(GestureCheck, 1);
		}
		vr::VRCompositor()->WaitGetPoses(mPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
	}

	Controller& VIM::GetController(ControllerType _t) {
		if (mIsLeftPrimary) {
			return _t == eControllerType_Primary ? mLeftController : mRightController;
		} else {
			return _t == eControllerType_Primary ? mRightController : mLeftController;
		}
	}

}