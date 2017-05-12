#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"

namespace Epoch
{
	
	struct CCPlatform : public CodeComponent
	{
		virtual void OnTriggerEnter(Collider& _col, Collider& _other)
		{
			if(Settings::GetInstance().GetBool("PlatInterp") && !Settings::GetInstance().GetBool("PlayerInterp") &&  _other.mColliderType == Collider::eCOLLIDER_Controller)
			{
				Settings::GetInstance().SetBool("PlayerInterp", true);
			}
			else if(!Settings::GetInstance().GetBool("PlatInterp"))
			{
				Settings::GetInstance().SetBool("PlayerInterp", false);
			}
		}
	};

}
