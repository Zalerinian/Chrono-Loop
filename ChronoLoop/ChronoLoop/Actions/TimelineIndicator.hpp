#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Core/TimeManager.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/Transform.h"
#include "../Common/Logger.h"
#include <unordered_set>


namespace Epoch {

	struct TimeLineIndicator : public CodeComponent {
		MeshComponent *mIndicator = nullptr;
		float mRatio = 0, mMaxRotation = 113;

		TimeLineIndicator(MeshComponent *_Indicator) {
			mIndicator = _Indicator;
		}

		virtual void Update() override {
			unsigned int CurSnap = TimeManager::Instance()->GetCurrentSnapFrame();
			unsigned int TotalSnap = TimeManager::Instance()->GetTotalSnapsmade();
			if (TotalSnap > 0) {
				mRatio = (float)(CurSnap + 1) / (float)TotalSnap;
			}
			else {
				mRatio = 1;
			}
			mIndicator->GetTransform().Rotate(vec4f(1, 0, 0, 1), (-42.237f) * (3.14f / 180.0f));
			mIndicator->GetTransform().Rotate(vec4f(0, 1, 0, 1), (mMaxRotation * mRatio) * (3.14f / 180.0f));
		}
	};

}