#pragma once
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Objects/BaseObject.h"
#include "../Actions/TimeManipulation.h"
#include "../Core/LevelManager.h"

namespace Epoch
{

	struct UIClonePlusToMinus : public CodeComponent
	{
		bool once = false;
		virtual void Update()
		{
			if (once && LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->mIsBeingMade)
			{
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/minus.png", eTEX_DIFFUSE);
				once = false;
			}
			else if (!once && !LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->isTimePaused())
			{
				once = true;
				((MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture("../Resources/plus.png", eTEX_DIFFUSE);
			}
		}
	};

}