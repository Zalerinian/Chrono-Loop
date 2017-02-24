#pragma once
#include "Actions/CodeComponent.hpp"
#include "Objects/MeshComponent.h"
#include "Objects/BaseObject.h"
#include "Input/VRInputManager.h"

class HeadsetFollow : public CodeComponent
{
private:
	
public:

virtual void Update() {
	if (!VRInputManager::Instance().iIsInitialized()) {
		return;
	}

		matrix4 mat = Math::FromMatrix(VRInputManager::Instance().iGetTrackedPositions()[0].mDeviceToAbsoluteTracking) * VRInputManager::Instance().iGetPlayerPosition();
			mObject->GetTransform().SetMatrix(mat);
		}

};
