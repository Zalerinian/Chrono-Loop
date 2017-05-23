#pragma once
#pragma once
#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Interpolator.h"
#include "..\Objects\BaseObject.h"
#include "..\Core\TimeManager.h"
#include "../Core/LevelManager.h"

namespace Epoch {
	struct CCBoxSpinRandomSmall : public CodeComponent {
		Interpolator<matrix4> interp;
		matrix4 m1, m2, rot;
		float angle = 0, mIncrease = 0;
		unsigned int mRandDur = 0;
		float mRandDown = 0.0f, mRandUp = 0.0f;
		bool flip;
		TimeManipulation * mManip;

		virtual void Start() {
			flip = true;
			angle = (float)(rand() % 360);
			mIncrease = (.001f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((1.0f) - (.001f))));
			mRandDur = (rand() % 7) + 1;
			mRandDown = (.2f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((1.0f) - (.2f))));
			mRandUp = (.5f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((1.0f) - (.5f))));
			float rad = DirectX::XM_PI / 2;
			float pi = DirectX::XM_PI;
			rot = (matrix4::CreateNewXRotation(angle) * matrix4::CreateNewYRotation(angle) * matrix4::CreateNewZRotation(angle));
			m1 = mObject->GetTransform().GetMatrix() * matrix4::CreateNewTranslation(0, -mRandDown, 0);
			m2 = mObject->GetTransform().GetMatrix() * matrix4::CreateNewTranslation(0, mRandUp, 0);
			//m1 = matrix4::CreateNewScale(.5f, .5f, .5f) *  matrix4::CreateNewTranslation(0, .75f, -3.872531f);
			//m2 = matrix4::CreateNewScale(.5f, .5f, .5f) *  matrix4::CreateNewTranslation(0, 2, -3.872531f);
			mManip = LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator();

			interp.Prepare(2, m1, m2, mObject->GetTransform().GetMatrix());
			interp.SetEasingFunction(Easing::QuadInOut);
			interp.SetActive(true);
		}

		virtual void Update() {
			if (!mManip || (mManip && !mManip->isTimePaused())) {
				if (interp.Update(TimeManager::Instance()->GetDeltaTime())) {
					if (flip) {
						interp.Prepare((float)mRandDur, m2, m1, mObject->GetTransform().GetMatrix());
						flip = false;
					} else {
						interp.Prepare((float)mRandDur, m1, m2, mObject->GetTransform().GetMatrix());
						flip = true;
					}
				}
				mObject->GetTransform().GetMatrix().RotateInPlace(vec3f(1, 1, 1), angle);
				angle += mIncrease * TimeManager::Instance()->GetDeltaTime();
				//rot = matrix4::CreateNewXRotation(angle) * matrix4::CreateNewXRotation(angle) * matrix4::CreateNewXRotation(angle);
			}
		}
	};

}
