#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Interpolator.h"
#include "..\Objects\BaseObject.h"
#include "..\Core\TimeManager.h"

namespace Epoch
{

	struct CCBoxSpin : public CodeComponent
	{
		Interpolator<matrix4> interp;
		matrix4 m1, m2, rot;
		float angle = 0;
		bool flip;
		TimeManipulation * mManip;

		virtual void Start()
		{
			flip = true;
			float rad = DirectX::XM_PI / 2;
			float pi = DirectX::XM_PI;
			rot = (matrix4::CreateNewXRotation(angle) * matrix4::CreateNewYRotation(angle) * matrix4::CreateNewZRotation(angle));
			m1 = mObject->GetTransform().GetMatrix() * matrix4::CreateNewTranslation(0, -.75f, 0);
			m2 = mObject->GetTransform().GetMatrix() * matrix4::CreateNewTranslation(0, 1, 0);
			//m1 = matrix4::CreateNewScale(.5f, .5f, .5f) *  matrix4::CreateNewTranslation(0, .75f, -3.872531f);
			//m2 = matrix4::CreateNewScale(.5f, .5f, .5f) *  matrix4::CreateNewTranslation(0, 2, -3.872531f);
			mManip = LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator();

			interp.Prepare(2, m1, m2, mObject->GetTransform().GetMatrix());
			interp.SetEasingFunction(Easing::QuadInOut);
			interp.SetActive(true);
		}

		virtual void Update()
		{
			if(!mManip || (mManip && !mManip->isTimePaused()))
			{
				if (interp.Update(TimeManager::Instance()->GetDeltaTime()))
				{
					if (flip)
					{
						interp.Prepare(2, m2, m1, mObject->GetTransform().GetMatrix());
						flip = false;
					}
					else
					{
						interp.Prepare(2, m1, m2, mObject->GetTransform().GetMatrix());
						flip = true;
					}
				}
				mObject->GetTransform().GetMatrix().RotateInPlace(vec3f(1, 1, 1), angle);
				angle += 1.5708f * TimeManager::Instance()->GetDeltaTime();
				//rot = matrix4::CreateNewXRotation(angle) * matrix4::CreateNewXRotation(angle) * matrix4::CreateNewXRotation(angle);
			}
		}
	};

}