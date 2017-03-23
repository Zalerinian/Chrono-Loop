#pragma once
#include "CodeComponent.hpp"
#include "../Input/Controller.h"
#include "../Input/VRInputManager.h"
#include "../Objects/BaseObject.h"

namespace Epoch
{

	struct UIRewind : public CodeComponent
	{
		virtual void Update()
		{
			if (VRInputManager::GetInstance().GetController(ControllerType::eControllerType_Primary).CheckGesture() == -1)
				mObject->GetTransform().SetMatrix(mObject->GetTransform().GetMatrix() * matrix4::CreateYRotation(0.0872665f));
		}
	};

}