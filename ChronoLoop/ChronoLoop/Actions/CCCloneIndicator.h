#pragma once
#include "../Core/TimeManager.h"
#include "../Core/LevelManager.h"

namespace Epoch
{
	struct CCCloneIndicator : public CodeComponent
	{
		unsigned int mId;
		vec3f start, end;
		float totalSnaps = 0, mSnap, ratio;
		float mTempTotal = 0;

		CCCloneIndicator(unsigned int _id)
		{
			mSnap = (float)TimeManager::Instance()->GetCurrentSnapFrame();
			mId = _id;
		}

		virtual void Start()
		{
			ratio = 0;
			mObject->GetTransform().SetMatrix(matrix4::CreateNewScale(.1f, .1f, .1f));
			end = vec4f(mObject->GetTransform().GetMatrix().fourth);
			start = end;
			start.x -= .1f;
		}

		virtual void Update()
		{
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
		
				totalSnaps = (float)TimeManager::Instance()->GetCurrentSnapFrame();
				ratio = mSnap / totalSnaps;
				if (ratio > 1)
					ratio = 1;

				mObject->GetTransform().GetMatrix().fourth = (start + (end - start) * ratio);
			}
		}
	};
}