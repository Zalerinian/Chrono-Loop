#pragma once
#include "CodeComponent.hpp"
#include "../Input/Controller.h"
#include "../Input/VRInputManager.h"
#include "../Objects/BaseObject.h"
#include "../Common/Math/matrix4.h"

namespace Epoch
{

	struct UIRewind : public CodeComponent
	{
		bool once = false;
		int gesture;
		virtual void Update()
		{
			gesture = VRInputManager::GetInstance().GetController(ControllerType::eControllerType_Primary).CheckGesture();
			if (gesture == -1 /*GetAsyncKeyState(VK_LEFT) & 1*/)
			{
				if (!once)
				{
					once = true;
					((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/rewind.png", eTEX_DIFFUSE);
				}
				mObject->GetTransform().SetMatrix(mObject->GetTransform().GetMatrix().RotateInPlace(vec3f(0, 1, 0), 0.261799f));
				
			}
			else if (gesture == 1 /*GetAsyncKeyState(VK_RIGHT) & 1*/)
			{
				if (once)
				{
					once = false;
					((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/fastForward.png", eTEX_DIFFUSE);
				}
				mObject->GetTransform().SetMatrix(mObject->GetTransform().GetMatrix().RotateInPlace(vec3f(0, 1, 0), -0.261799f));
			}
		}
	};

}