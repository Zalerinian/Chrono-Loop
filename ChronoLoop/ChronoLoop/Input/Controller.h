#pragma once
#include <openvr.h>
#include "../Objects/Transform.h"
#include "../Common/Math.h"

namespace Epoch {

	typedef unsigned long ulong;

	class Controller {
	private:
		int mIndex = -1;
		bool mValid = false;
		vr::VRControllerState_t mState, mPrevState;
		vec3f mPosition;

		vr::ETrackingUniverseOrigin mTrackingSpace = vr::TrackingUniverseStanding;

		float mHairTriggerDZ = 0.1f;
		float mHairTriggerLimit;
		bool mHairTriggerState, mHairTriggerPrevState;
		void Setup(int _index);

		friend class VIM;
	public:

		vr::TrackedDevicePose_t mPose;
		Controller();
		~Controller() {};
		void Update();

		//transforms
		matrix4 GetPosition();
		vec3f GetVelocity();
		vec3f GetAngularVelocity();

		vr::VRControllerState_t GetState();
		vr::VRControllerState_t GetPrevState();
		vr::TrackedDevicePose_t GetPose();

		//Getters and Setters
		int GetIndex();
		bool GetValid();

		//controller input
		bool GetPress(vr::EVRButtonId buttonId);
		bool GetPressDown(vr::EVRButtonId buttonId);
		bool GetPressUp(vr::EVRButtonId buttonId);

		bool GetTouch(vr::EVRButtonId buttonId);
		bool GetTouchDown(vr::EVRButtonId buttonId);
		bool GetTouchUp(vr::EVRButtonId buttonId);

		//Trigger
		void UpdateHairTrigger();
		bool GetHairTrigger();
		bool GetHairTriggerDown();
		bool GetHairTriggerUp();

		//mostly for touchpad
		vec2f GetAxis(vr::EVRButtonId buttonId = vr::k_EButton_SteamVR_Touchpad);
		void TriggerHapticPulse(int duration_micro_sec = 500, vr::EVRButtonId buttonId = vr::k_EButton_SteamVR_Touchpad);
	};

}