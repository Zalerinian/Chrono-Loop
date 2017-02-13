#pragma once
#include <openvr.h>
#include "Controller.h"

class VRInputManager {
private:
	static VRInputManager* sInstance;
	vr::IVRSystem* mHmd;
	Controller mRightController;
	Controller mLeftController;
	VRInputManager();
	~VRInputManager();
	void mInitialize(vr::IVRSystem* _hmd);
	bool mInitialized = false;
public:
	void update();
	static VRInputManager& Instance();
	static void Initialize(vr::IVRSystem* _hmd);
	static void Shutdown();

	Controller& GetController(bool left);
};

