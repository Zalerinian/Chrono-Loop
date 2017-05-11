#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Physics\Physics.h"
#include "..\Common\Logger.h"
#include "../Core/Level.h"
#include "../Core/TimeManager.h"
#include "../Core/LevelManager.h"

namespace Epoch
{

	struct CCPlatformButtonHold : public CodeComponent
	{
		bool colliding = false, mhitting = false, mFlip = false, mSoundOnce = false;;
		bool interp = false, flip = false, canInterp = false, doneInterp = false;
		BaseObject* platform;
		Interpolator<matrix4>* platInterp;
		matrix4 end, start;
		Level* cLevel;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			platform = cLevel->FindObjectWithName("Platform");
			platInterp = TimeManager::Instance()->GetObjectInterpolator(platform->GetUniqueID());
			start = platform->GetTransform().GetMatrix();
			end = start * matrix4::CreateTranslation(-4, 0, 0);
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "Buttonstand")
			{
				colliding = true;

				if (!interp)
				{
					interp = false;
					platInterp->SetActive(true);
					platInterp->Prepare(5, platform->GetTransform().GetMatrix(), end, platform->GetTransform().GetMatrix());
				}

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
				ButtonCollider* butCol = (ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
				if (_other.mVelocity * butCol->mPushNormal < .1f)
					butCol->mVelocity = vec3f(0, 0, 0);
			}
		}

		virtual void Update()
		{
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (colliding)
				{
					flip = true;
					if (canInterp && !doneInterp)
						doneInterp = platInterp->Update(TimeManager::Instance()->GetDeltaTime());
					else
					{
						canInterp = false;
						platInterp->SetActive(false);
					}
				}
				else
				{
					interp = false;
					if (flip)
					{
						flip = false;
						platInterp->SetActive(true);
						platInterp->Prepare(5, platform->GetTransform().GetMatrix(), start, platform->GetTransform().GetMatrix());
						canInterp = true;
						doneInterp = false;
					}

					if (canInterp && !doneInterp) {
						doneInterp = (platInterp->Update(TimeManager::Instance()->GetDeltaTime()));
					} else {
						canInterp = false;
						platInterp->SetActive(false);
					}
				}
			}
			colliding = false;
		}
	};

}