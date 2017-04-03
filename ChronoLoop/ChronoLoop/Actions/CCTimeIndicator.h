#pragma once
#include "CodeComponent.hpp"
#include "..\Core\Timeline.h"
#include "..\Core\TimeManager.h"
#include "..\Objects\BaseObject.h"
#include "..\Core\Level.h"
#include "..\Core\LevelManager.h"

namespace Epoch
{

	struct CCTimeIndicator : public CodeComponent
	{
		vec3f start, end;
		unsigned int totalSnaps, currentSnap;
		Interpolator<matrix4> interp;
		float ratio;
		bool once;

		virtual void Start()
		{
			totalSnaps = currentSnap = 0;
			ratio = 0;
			once = true;
			start = vec3f(mObject->GetTransform().GetMatrix().fourth);
			end = start;
			end.x -= .1f;
		}

		virtual void Update()
		{
			if (LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->isTimePaused())
			{
				if (once)
				{
					once = false;
					currentSnap = totalSnaps = TimeManager::Instance()->GetTotalSnapsmade();
				}

				currentSnap = TimeManager::Instance()->GetCurrentSnapFrame();

				ratio = currentSnap / totalSnaps;
				mObject->GetTransform().GetMatrix().fourth = (start + (end - start) * ratio);
			}
			else if (!once)
				once = true;
		}
	};

}