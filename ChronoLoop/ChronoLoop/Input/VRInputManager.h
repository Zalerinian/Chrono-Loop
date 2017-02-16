#pragma once
#include <openvr.h>
#include "Controller.h"

class VRInputManager {
private:
	static VRInputManager* sInstance;
	vr::IVRSystem* mVRSystem;
	Controller mRightController;
	Controller mLeftController;
	VRInputManager();
	~VRInputManager();
	void mInitialize(vr::IVRSystem* _vr);
	
	vr::TrackedDevicePose_t mPoses[vr::k_unMaxTrackedDeviceCount];

public:
	void update();
	static VRInputManager& Instance();
	static void Initialize(vr::IVRSystem* _vr);
	static void Shutdown();

	Controller& GetController(bool left);
};

