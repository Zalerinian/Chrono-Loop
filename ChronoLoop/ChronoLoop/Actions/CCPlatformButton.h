#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Physics\Physics.h"
#include "..\Common\Logger.h"
#include "../Core/Level.h"
#include "../Core/TimeManager.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"

namespace Epoch
{

	struct CCPlatformButton : public CodeComponent
	{
		bool colliding = false;
		int threshold = 0;

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (_other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "EnvButtonstand")
			{
				colliding = true;
				threshold = 0;
				Settings::GetInstance().SetBool("shouldGoBack", false);
				Settings::GetInstance().SetBool("PlatInterp", true);
				Settings::GetInstance().SetBool("doneInterp", false);
				Settings::GetInstance().SetBool("PrepareInterp", true);
				Settings::GetInstance().SetBool("ButtonHeld", true);

				ButtonCollider* butCol = (ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
				if (fabsf(_other.mVelocity * butCol->mPushNormal) < .1f)
					butCol->mVelocity = vec3f(0, 0, 0);
				else
				{
					vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
					vec3f tForce = norm * (norm * _other.mTotalForce);
					vec3f vel = norm * (norm * _other.mVelocity);
					vec3f accel = norm * (norm * _other.mAcceleration);
					if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0)
					{
						_col.mTotalForce = tForce;
						_col.mVelocity = vel;
						_col.mAcceleration = vel / _time;
						//blockCube->SetPos(blockend);
					}
				}
			}
			else
			{
				Settings::GetInstance().SetBool("shouldGoBack", true);
				colliding = false;
			}
		}

		virtual void Update()
		{
			if(!colliding && threshold == 10)
				Settings::GetInstance().SetBool("ButtonHeld", false);

			threshold++;
			colliding = false;
		}
	};

}
