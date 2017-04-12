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
		float totalSnaps, currentSnap, ratio;
		Interpolator<matrix4> interp;
		bool once;

		virtual void Start()
		{
			totalSnaps = currentSnap = 0;
			ratio = 0;
			once = true;
			end = vec3f(mObject->GetTransform().GetMatrix().fourth);
			start = end;
			start.x -= .1f;
		}

		virtual void Update()
		{
			if (LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (once)
				{
					once = false;
					currentSnap = totalSnaps = (float)TimeManager::Instance()->GetCurrentSnapFrame();
				}

				currentSnap = (float)TimeManager::Instance()->GetTempCurSnap();

				ratio = currentSnap / totalSnaps;
				mObject->GetTransform().GetMatrix().fourth = (start + (end - start) * ratio);
			}
			else if (!once)
			{
				once = true;
				mObject->GetTransform().GetMatrix().fourth = end;
			}
		}
	};

}