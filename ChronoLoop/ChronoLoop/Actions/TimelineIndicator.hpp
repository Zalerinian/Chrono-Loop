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
			//if (LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused() || LevelManager::GetInstance().GetCurrentLevel()->GetLeftTimeManipulator()->isTimePaused()) {
				unsigned int CurSnap = TimeManager::Instance()->GetTempCurSnap();
				unsigned int TotalSnap = TimeManager::Instance()->GetCurrentSnapFrame();
				if (TotalSnap > 0) {
					mRatio = (float)(CurSnap) / (float)TotalSnap;
				}
				else {
					mRatio = 1;
				}
				if (mParent)
				{
					//mIndicator->GetTransform().GetMatrix();// = /*(*mParent) * matrix4::CreateNewScale(0.1f, 0.1f, 0.1f) *matrix4::CreateNewTranslation(0, -0.25699f, 0.27277f) *matrix4::CreateNewXRotation((42.237f) * (3.14f / 180.0f)) *matrix4::CreateNewYRotation((mMaxRotation * mRatio) * (3.14f / 180.0f));
					mLine->GetTransform().GetMatrix() = *mParent; //matrix4::CreateNewScale(0.9f, 0.9f, 0.9f);
					GetTransform().GetMatrix() = *mParent;
				}
			//}
		}
	};

}