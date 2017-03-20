#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"

namespace Epoch 
{

	struct CCEnterLevel : public CodeComponent 
	{
		bool once = true;
		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2) 
		{
				once = false;
		}

		virtual void Update() {
			if (!once) {
				SystemLogger::GetLog() << "CCEnterLevel: LOAD LEVEL" << std::endl;
				Level* next;
				LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (status == LM::LevelStatus::Success) {
					LevelManager::GetInstance().RequestLevelChange(next);
				}
			}
		}
	};

}