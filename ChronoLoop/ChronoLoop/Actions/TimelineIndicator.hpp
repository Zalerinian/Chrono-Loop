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
		MeshComponent *mLine = nullptr;
		matrix4 *mParent = nullptr;
		float mRatio = 0, mMaxRotation = 113;

		TimeLineIndicator(MeshComponent *_Indicator, MeshComponent *_Line) {
			mIndicator = _Indicator;
			mLine = _Line;
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
			if (mParent)
			{
				mIndicator->GetTransform().GetMatrix() = *mParent * matrix4::CreateScale(0.5f, 0.5f, 0.5f) * matrix4::CreateTranslation(0, -0.25699f, 0.27277f) * matrix4::CreateXRotation((-42.237f) * (3.14f / 180.0f)) * matrix4::CreateYRotation((mMaxRotation * mRatio) * (3.14f / 180.0f));
				mLine->GetTransform().GetMatrix()= *mParent;
			}
		}
	};

}