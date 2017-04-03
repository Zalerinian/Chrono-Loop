#pragma once
#include "CodeComponent.hpp"
#include "..\Core\LevelManager.h"

namespace Epoch
{

	struct CCExit : public CodeComponent
	{
		virtual void OnCollision(Collider& _col1, Collider& _col2, float _time)
		{
			LevelManager::GetInstance().GetCurrentLevel()->ChronoLoop = false;
		}
	};

}