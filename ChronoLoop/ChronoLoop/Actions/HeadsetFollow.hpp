#pragma once
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Objects/BaseObject.h"
#include "../Input/VRInputManager.h"

namespace Epoch {

	class HeadsetFollow : public CodeComponent
	{
		Collider* Col = nullptr;
	private:

	public:
		virtual void Start()
		{
			Col = (Collider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
		}
		virtual void Update()
		{
			if (!VRInputManager::GetInstance().IsVREnabled())
			{
				return;
			}

			matrix4 mat = (matrix4)(VRInputManager::GetInstance().GetTrackedPositions()[0].mDeviceToAbsoluteTracking) * VRInputManager::GetInstance().GetPlayerPosition();
			mObject->GetTransform().SetMatrix(mat);
			if(Col)
			Col->SetPos(mat.Position);

		}

	};

}