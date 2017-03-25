#pragma once
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Objects/BaseObject.h"
#include "../Actions/TimeManipulation.h"
#include "../Core/LevelManager.h"

namespace Epoch
{

	struct UICreateToDeleteClone : public CodeComponent
	{
		bool once = false;
		virtual void Update()
		{
			if (LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator(). /*(GetAsyncKeyState(VK_UP))*/)
			{
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
				once = false;
			}
			else if (!once)
			{
				once = true;
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
			}
		}
	};

}