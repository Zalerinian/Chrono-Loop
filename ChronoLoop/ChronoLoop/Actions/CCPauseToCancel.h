#pragma once
#include "CodeComponent.hpp"
#include "../Core/LevelManager.h"
#include "../Core/Level.h"
#include "../Objects/MeshComponent.h"

namespace Epoch
{

	struct CCPauseToCancel : public CodeComponent
	{
		bool once = false;
		virtual void Update()
		{
			if (once && LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->isTimePaused() /*(GetAsyncKeyState(VK_SHIFT))*/)
			{
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/cancel.png", eTEX_DIFFUSE);
				once = false;
			}
			else if(!once && !LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->isTimePaused())
			{
				once = true;
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/pause.png", eTEX_DIFFUSE);
			}

		}

	};

}