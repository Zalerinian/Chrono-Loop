#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"

namespace Epoch 
{

	struct CCEnterLevel : public CodeComponent 
	{
		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2) 
		{
			SystemLogger::GetLog() << "LOAD LEVEL" << std::endl;
		}
	};

}