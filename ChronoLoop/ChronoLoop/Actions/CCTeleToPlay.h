#pragma once
#include "CodeComponent.hpp"
#include "../Core/LevelManager.h"
#include "../Core/Level.h"
#include "../Objects/MeshComponent.h"

namespace Epoch
{

	struct CCTeleToPlay : public CodeComponent
	{
		bool once = false;
		virtual void Update()
		{
			if (/*LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->isTimePaused()*/ (GetAsyncKeyState(VK_SHIFT)))
			{
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/play.png", eTEX_DIFFUSE);
				once = false;
			}
			else if (!once)
			{
				once = true;
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/teleport.png", eTEX_DIFFUSE);
			}

		}

	};

}