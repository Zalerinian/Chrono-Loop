#pragma once
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Objects/BaseObject.h"
#include "../Input/VRInputManager.h"

namespace Epoch
{

	class HeadsetFollow : public CodeComponent
	{
	private:

	public:

		virtual void Update()
		{
			if (!VRInputManager::GetInstance().IsVREnabled())
			{
				return;
			}

			matrix4 mat = (matrix4)(VRInputManager::GetInstance().GetTrackedPositions()[0].mDeviceToAbsoluteTracking) * VRInputManager::GetInstance().GetPlayerPosition();
			mObject->GetTransform().SetMatrix(mat);
		}

	};

}