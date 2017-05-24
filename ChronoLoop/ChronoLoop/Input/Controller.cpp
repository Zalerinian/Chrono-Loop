//#include "stdafx.h"
#include "Controller.h"
#include "../Common/Logger.h"
#include "../Core/LevelManager.h"
#include "../Core/TimeManager.h"
#include "../Core/Timeline.h"
#include "VRInputManager.h"

namespace Epoch {

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

	Controller::Controller() {}

	void Controller::Update() {
		//update the contoller pose/state when called. 
		mPrevState = mState;
		mValid = VRInputManager::GetInstance().GetVRSystem()->GetControllerStateWithPose(mTrackingSpace, mIndex, &mState, sizeof(mState), &mPose);
		if (mPrevState.ulButtonPressed != mState.ulButtonPressed) {
			UpdateHairTrigger();
		}
	}

	void Controller::UpdateHairTrigger() {
		mHairTriggerPrevState = mHairTriggerState;
		float value = mState.rAxis[vr::k_eControllerAxis_Trigger].x;
		if (mHairTriggerState)
			if (value < mHairTriggerLimit - mHairTriggerDZ || value <= 0.0f)
				mHairTriggerState = false;
			else
				if (value > mHairTriggerLimit + mHairTriggerDZ || value >= 1.0f)
					mHairTriggerState = true;

		mHairTriggerLimit = mHairTriggerState ? MAX(mHairTriggerLimit, value) : MIN(mHairTriggerLimit, value);
	}

	int Controller::CheckGesture() {
		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
		if (cLevel->GetTimeManipulator() != nullptr) {
			if ((cLevel->GetTimeManipulator()->isTimePaused())) {

				vec2f touch = this->GetAxis();
				if (touch.x == 0 && touch.y == 0)
					return 0;
				if ((InitialPos.x == 0 && InitialPos.y == 0) || InitialPos == touch) {
					InitialPos = touch;
					return 0;
				}
					//vec2f CurPos,line,diff;
					vec2f CurPos = touch;
					vec2f line = InitialPos.Cross();
					vec2f diff = (CurPos - InitialPos);
					if (diff.x >= 0.05f || diff.y >= 0.05f || diff.x <= -0.05f || diff.y <= -0.05f) {
						float slope = (CurPos.y - InitialPos.y) / (CurPos.x - InitialPos.x);
						if ((slope >= 3 || slope <= -3) &&
							(CurPos.x < 0.3f && CurPos.x > -0.3f)) {
							//SystemLogger::GetLog() << "Vertical Wrongness" << std::endl;
							return 0;
						}
						if ((slope <= 0.33f && slope >= -0.33f) &&
							(CurPos.y < 0.3f && CurPos.y > -0.3f)) {
							return 0;
						}
						InitialPos = CurPos;
						if ((powf((CurPos.x), 2) + powf((CurPos.y), 2)) > 0.16f) {
							if (diff * line > 0) {
								SystemLogger::GetLog() << "Gesty: 1" << std::endl;
								return 1;
							}
							if (diff * line < 0) {
								SystemLogger::GetLog() << "Gesty: -1" << std::endl;
								return -1;
							}
						}
					}
			}
		}
		return 0;
	}

	vec2f Controller::GetAxis(vr::EVRButtonId buttonId) {
		Update();
		int axisId = (int)buttonId - (int)vr::k_EButton_Axis0;
		return vec2f(mState.rAxis[axisId].x, mState.rAxis[axisId].y);
	}

	void Controller::TriggerHapticPulse(unsigned short duration_micro_sec, vr::EVRButtonId buttonId) {
			int axisId = (int)buttonId - (int)vr::k_EButton_Axis0;
			VRInputManager::GetInstance().GetVRSystem()->TriggerHapticPulse(mIndex, axisId, duration_micro_sec);
	}

#pragma region Private Functions

	void Controller::Setup(int _index)
	{
		mIndex = _index;
	}

#pragma endregion Private Functions

#pragma region Public Functions

	matrix4 Controller::GetPosition() {
		if (GetValid()) {
			return matrix4(VRInputManager::GetInstance().GetTrackedPositions()[GetIndex()].mDeviceToAbsoluteTracking) * VRInputManager::GetInstance().GetPlayerPosition();
		} else {
			return matrix4();
		}
	}

	vec3f Controller::GetVelocity() {
		vec4f vel(mPose.vVelocity.v[0], mPose.vVelocity.v[1], -mPose.vVelocity.v[2], 0);
		vel = vel * matrix4::CreateNewYawPitchRollRotation(LevelManager::GetInstance().GetCurrentLevel()->GetStartRot());
		return vel;
	}

	vec3f Controller::GetAngularVelocity() {
		return vec3f(-mPose.vAngularVelocity.v[0], -mPose.vAngularVelocity.v[1], mPose.vAngularVelocity.v[2]);
	}

	vr::VRControllerState_t Controller::GetState() {
		return mState;
	}

	vr::VRControllerState_t	Controller::GetPrevState() {
		return mPrevState;
	}

	vr::TrackedDevicePose_t	Controller::GetPose() {
		return mPose;
	}

	int Controller::GetIndex() {
		return mIndex;
	}

	bool Controller::GetValid() {
		return mValid;
	}

	bool Controller::GetPress(vr::EVRButtonId _id) {
		if (!GetValid()) {
			return false;
		}
		return (mState.ulButtonPressed & vr::ButtonMaskFromId(_id)) != 0;
	}

	bool Controller::GetPressDown(vr::EVRButtonId _id) {
		if (!GetValid()) {
			return false;
		}
		return (mState.ulButtonPressed & vr::ButtonMaskFromId(_id)) != 0 &&
			(mPrevState.ulButtonPressed & vr::ButtonMaskFromId(_id)) == 0;
	}

	bool Controller::GetPressUp(vr::EVRButtonId _id) {
		if (!GetValid()) {
			return false;
		}
		return (mState.ulButtonPressed & vr::ButtonMaskFromId(_id)) == 0 &&
			(mPrevState.ulButtonPressed & vr::ButtonMaskFromId(_id)) != 0;
	}

	bool Controller::GetTouch(vr::EVRButtonId _id) {
		if (!GetValid()) {
			return false;
		}
		return (mState.ulButtonTouched & vr::ButtonMaskFromId(_id)) != 0;
	}

	bool Controller::GetTouchDown(vr::EVRButtonId _id) {
		if (!GetValid()) {
			return false;
		}
		return (mState.ulButtonTouched & vr::ButtonMaskFromId(_id)) != 0 &&
			(mPrevState.ulButtonTouched & vr::ButtonMaskFromId(_id)) == 0;
	}

	bool Controller::GetTouchUp(vr::EVRButtonId _id) {
		if (!GetValid()) {
			return false;
		}
		return (mState.ulButtonTouched & vr::ButtonMaskFromId(_id)) == 0 &&
			(mPrevState.ulButtonTouched & vr::ButtonMaskFromId(_id)) != 0;
	}

	bool Controller::GetHairTrigger() {
		if (!GetValid()) {
			return false;
		}
		return mHairTriggerState;
	}

	bool Controller::GetHairTriggerDown() {
		if (!GetValid()) {
			return false;
		}
		return mHairTriggerState && !mHairTriggerPrevState;
	}

	bool Controller::GetHairTriggerUp() {
		if (!GetValid()) {
			return false;
		}
		return !mHairTriggerState && mHairTriggerPrevState;
	}

#pragma endregion Public Functions

}