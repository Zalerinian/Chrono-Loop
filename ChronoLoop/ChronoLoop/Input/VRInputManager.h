#pragma once
#include <openvr.h>
#include "Controller.h"
#include "../Common/Math.h"

class VRInputManager {
private:
	static VRInputManager* sInstance;
	vr::IVRSystem* mVRSystem;
	Controller mRightController;
	Controller mLeftController;
	VRInputManager();
	~VRInputManager();
	void mInitialize(vr::IVRSystem* _vr);
	
	matrix4 mPlayerPosition;
	vr::TrackedDevicePose_t mPoses[vr::k_unMaxTrackedDeviceCount];

public:
	static VRInputManager& Instance();
	static void Initialize(vr::IVRSystem* _vr);
	static void Shutdown();

	void iUpdate();
	Controller& iGetController(bool left);
	inline bool iIsInitialized() const { return mVRSystem != nullptr; }
	inline matrix4& iGetPlayerPosition() { return mPlayerPosition; }
	inline vr::TrackedDevicePose_t* iGetTrackedPositions() { return mPoses; }
	inline unsigned int iGetTrackedDeviceCount() { return vr::k_unMaxTrackedDeviceCount; }
};