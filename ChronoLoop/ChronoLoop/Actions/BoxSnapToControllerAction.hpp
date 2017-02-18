#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Core/TimeManager.h"
#include "../Common/Logger.h"

struct BoxSnapToControllerAction : public CodeComponent {
	bool mHeld = false;
	bool mHeldLeft = false;

	virtual void Update() override {
		if (VRInputManager::Instance().iIsInitialized()) {
			if (VRInputManager::Instance().iGetController(true).GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
				SnapToController(true);
				mHeldLeft = true;
			} else if (VRInputManager::Instance().iGetController(false).GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
				SnapToController(false);
				mHeldLeft = false;
			} else if (mHeld) {
				ReleaseCube();
			}
			if (VRInputManager::Instance().iGetController(true).GetPressDown((vr::EVRButtonId::k_EButton_Grip))) {
				TimeManager::Instance()->RewindTimeline();
			} else if (VRInputManager::Instance().iGetController(false).GetPressDown((vr::EVRButtonId::k_EButton_Grip))) {
				TimeManager::Instance()->RewindTimeline();
			}
		}
	}

	virtual void SnapToController(bool left) {
		mHeld = true;
		mHeldLeft = left;
		matrix4 m = Math::FromMatrix(VRInputManager::Instance().iGetController(left).GetPose().mDeviceToAbsoluteTracking);
		((CubeCollider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->SetPos(Math::MatrixTranspose(m).tiers[3]);
		mObject->GetTransform().SetMatrix(m);
		((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mShouldMove = false;
	}

	virtual void ReleaseCube() {
		if (mHeldLeft) {
			vec4f force = VRInputManager::Instance().iGetController(true).GetVelocity();
			((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mVelocity = force;
		} else {
			vec4f force = VRInputManager::Instance().iGetController(false).GetVelocity();
			((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mVelocity = force;
		}
		((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mShouldMove = true;
		mHeldLeft = mHeld = false;
	}
};