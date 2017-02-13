#pragma once
#include <openvr.h>
#include "../Objects/Transform.h"
#include "../Common/Math.h"

typedef unsigned long ulong;

class Controller {
private:
	vr::IVRSystem* mHmd;
	int mIndex;
	bool mValid;
	vr::VRControllerState_t mState, mPrevState;
	vec3f mPosition;

	vr::ETrackingUniverseOrigin mTrackingSpace = vr::TrackingUniverseStanding;

	float mHairTriggerDelta;
	float mHairTriggerLimit; //trigger dead zone
	bool mHairTriggerState, mHairTriggerPrevState;
	inline void SetIndex(int index);
	inline void SetValid(bool valid);

	friend class VRInputManager;
public:

	vr::TrackedDevicePose_t mPose;
	Controller();
	~Controller() {};
	void Update();
	inline void SetUp(int _index, vr::IVRSystem* _vr);

	//transforms
	inline vec3f GetPosition();
	inline vec3f GetVelocity();
	inline vec3f GetAngularVelocity();

	inline vr::VRControllerState_t GetState();
	inline vr::VRControllerState_t GetPrevState();
	inline vr::TrackedDevicePose_t GetPose();

	//Getters and Setters
	inline int GetIndex();
	inline bool GetValid();

	//controller input
	inline bool GetPress(vr::EVRButtonId buttonId);
	inline bool GetPressDown(vr::EVRButtonId buttonId);
	inline bool GetPressUp(vr::EVRButtonId buttonId);

	inline bool GetTouch(vr::EVRButtonId buttonId);
	inline bool GetTouchDown(vr::EVRButtonId buttonId);
	inline bool GetTouchUp(vr::EVRButtonId buttonId);

	//Trigger
	void UpdateHairTrigger();
	inline bool GetHairTrigger();
	inline bool GetHairTriggerDown();
	inline bool GetHairTriggerUp();

	//mostly for touchpad
	inline vec2f GetAxis(vr::EVRButtonId buttonId = vr::k_EButton_SteamVR_Touchpad);
	inline void TriggerHapticPulse(int duration_micro_sec = 500, vr::EVRButtonId buttonId = vr::k_EButton_SteamVR_Touchpad);
};

