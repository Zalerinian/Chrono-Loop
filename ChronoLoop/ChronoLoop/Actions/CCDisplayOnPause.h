#pragma once
#include "CodeComponent.hpp"
#include "../Core/LevelManager.h"
#include "../Core/Level.h"
#include "../Objects/MeshComponent.h"

namespace Epoch
{

	struct CCDisplayOnPause : public CodeComponent
	{
		bool once = false;
		virtual void Update()
		{
			if (/*LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->isTimePaused()*/ (GetAsyncKeyState(VK_SHIFT)))
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