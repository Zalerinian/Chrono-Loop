#pragma once
#include <openvr.h>
#include "../Objects/Transform.h"
#include "../Common/Math.h"

typedef unsigned long ulong;

class Controller
{
private:
	vr::IVRSystem* mHmd;
	int mIndex;
	bool mValid, mConnected, mHasTracking, mOutOfRange, mCalibrating, mUninitalized;
	vr::VRControllerState_t mState, mPrevState;
	vec3f mPosition;

	vr::ETrackingUniverseOrigin mTrackingSpace = vr::TrackingUniverseStanding;

	float mHairTriggerDelta;
	float mHairTriggerLimit; //trigger dead zone
	bool mHairTriggerState, mHairTriggerPrevState;
public:
	vr::TrackedDevicePose_t mPose;
	Controller();
	~Controller() {};
	void Update();
	inline void SetUp(int index, vr::IVRSystem* hmd) { mIndex = index; mHmd = hmd; Update(); }

	//transforms
	vec3f GetPosition() { Update();  return mPosition; };
	vec3f GetVelocity() { Update(); return vec3f(mPose.vVelocity.v[0], mPose.vVelocity.v[1], -mPose.vVelocity.v[2]); }
	vec3f GetAngularVelocity() { Update(); return vec3f(-mPose.vAngularVelocity.v[0], -mPose.vAngularVelocity.v[1], mPose.vAngularVelocity.v[2]); }

	vr::VRControllerState_t GetState() { Update(); return mState; }
	vr::VRControllerState_t GetPrevState() { Update(); return mPrevState; }
	vr::TrackedDevicePose_t GetPose() { Update(); return mPose; }

	//Getters and Setters
	inline void SetIndex(int index) { mIndex = index; };
	inline int GetIndex() { return mIndex; };
	inline void SetValid(bool valid) { mValid = valid; };
	inline bool GetValid() { return mValid; };

	//controller input
	bool GetPress(vr::EVRButtonId buttonId) { Update(); return (mState.ulButtonPressed & vr::ButtonMaskFromId(buttonId)) != 0; }
	bool GetPressDown(vr::EVRButtonId buttonId) { Update(); return (mState.ulButtonPressed & vr::ButtonMaskFromId(buttonId)) != 0 && (mPrevState.ulButtonPressed & vr::ButtonMaskFromId(buttonId)) == 0; }
	bool GetPressUp(vr::EVRButtonId buttonId) { Update(); return (mState.ulButtonPressed & vr::ButtonMaskFromId(buttonId)) == 0 && (mPrevState.ulButtonPressed & vr::ButtonMaskFromId(buttonId)) != 0; }

	bool GetTouch(vr::EVRButtonId buttonId) { Update(); return (mState.ulButtonTouched & vr::ButtonMaskFromId(buttonId)) != 0; }
	bool GetTouchDown(vr::EVRButtonId buttonId) { Update(); return (mState.ulButtonTouched & vr::ButtonMaskFromId(buttonId)) != 0 && (mPrevState.ulButtonTouched & vr::ButtonMaskFromId(buttonId)) == 0; }
	bool GetTouchUp(vr::EVRButtonId buttonId) { Update(); return (mState.ulButtonTouched & vr::ButtonMaskFromId(buttonId)) == 0 && (mPrevState.ulButtonTouched & vr::ButtonMaskFromId(buttonId)) != 0; }

	//Trigger
	void UpdateHairTrigger();
	bool GetHairTrigger() { Update(); return mHairTriggerState; }
	bool GetHairTriggerDown() { Update(); return mHairTriggerState && !mHairTriggerPrevState; }
	bool GetHairTriggerUp() { Update(); return !mHairTriggerState && mHairTriggerPrevState; }

	//mostly for touchpad
	vec2f GetAxis(vr::EVRButtonId buttonId = vr::k_EButton_SteamVR_Touchpad);
	void TriggerHapticPulse(int duration_micro_sec = 500, vr::EVRButtonId buttonId = vr::k_EButton_SteamVR_Touchpad);
};

