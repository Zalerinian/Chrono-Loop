#pragma once
#include <openvr.h>
#include "Controller.h"
#include "../Common/Math.h"
#include <utility>

namespace Epoch {

	enum ControllerType {
		eControllerType_Primary = 0,
		eControllerType_Secondary
	};

	class VIM {
		Controller mLeftController;
		Controller mRightController;
		matrix4 mPlayerPosition;
		vr::TrackedDevicePose_t mPoses[vr::k_unMaxTrackedDeviceCount];
		vr::IVRSystem* mVRSystem;
		bool mIsLeftPrimary = false;
		
		VIM(vr::IVRSystem* _vr);
		~VIM();
	
		friend class VRInputManager;
	public:
		void Update();
		Controller& GetController(ControllerType _t);
		inline unsigned int GetTrackedDeviceCount() { return vr::k_unMaxTrackedDeviceCount; }
		inline vr::TrackedDevicePose_t* GetTrackedPositions() { return mPoses; }
		inline matrix4& GetPlayerPosition() { return mPlayerPosition; }
		inline matrix4 GetPlayerView() { return (matrix4)(mPoses[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking) * mPlayerPosition; }
		inline bool IsVREnabled() const { return mVRSystem != nullptr; }
		inline vr::IVRSystem* GetVRSystem() { return mVRSystem; }
	};
	
	class VRInputManager {
	private:
		static VIM* sInstance;
	
		VRInputManager();
		~VRInputManager();
	public:
		static VIM& GetInstance();
		static void Initialize(vr::IVRSystem* _vr);
		static void DestroyInstance();
	};

}