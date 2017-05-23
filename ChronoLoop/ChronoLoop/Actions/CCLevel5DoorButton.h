#pragma once
#include "CodeComponent.hpp"
#include "../Core/LevelManager.h"

namespace Epoch
{

	struct CCLevel5DoorButton : public CodeComponent
	{
		bool colliding = false, mhitting = false, mCanDoorInterp = false, mDoorDoneInterpolating = false, mFlip = false, mSoundOnce = false;;
		bool tempDoor = false;
		BaseObject *Block = nullptr, *Exit = nullptr;
		matrix4 blockend, blockStart;
		Interpolator<matrix4>* blockInterp;
		Level* cLevel;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			Block = cLevel->FindObjectWithName("EnvExitDoor");
			blockInterp = TimeManager::Instance()->GetObjectInterpolator(Block->GetUniqueID());
			blockStart = Block->GetTransform().GetMatrix();
			blockend = blockStart * matrix4::CreateNewTranslation(0, 2.6f, 0);
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "EnvButtonstand")
			{
				colliding = true;

				if (!tempDoor)
				{
					blockInterp->SetActive(true);
					blockInterp->Prepare(.69f, Block->GetTransform().GetMatrix(), blockend, Block->GetTransform().GetMatrix());

					mCanDoorInterp = true;
					mDoorDoneInterpolating = false;
					tempDoor = true;

				}

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
					}
				}
			}
		}

		virtual void Update()
		{
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (colliding)
				{
					//SystemLogger::GetLog() << "Colliding" << std::endl;
					mFlip = true;
					if (mCanDoorInterp && !mDoorDoneInterpolating)
					{
						mDoorDoneInterpolating = (blockInterp->Update(TimeManager::Instance()->GetDeltaTime()));
					}
					else
					{
						mCanDoorInterp = false;
						blockInterp->SetActive(false);
					}
				}
				else
				{
					tempDoor = false;
					//SystemLogger::GetLog() << "Not Colliding" << std::endl;
					if (mFlip)
					{
						mFlip = false;
						blockInterp->SetActive(true);
						blockInterp->Prepare(.69f, Block->GetTransform().GetMatrix(), blockStart, Block->GetTransform().GetMatrix());

						mCanDoorInterp = true;
						mDoorDoneInterpolating = false;
					}

					if (mCanDoorInterp && !mDoorDoneInterpolating)
					{
						mDoorDoneInterpolating = (blockInterp->Update(TimeManager::Instance()->GetDeltaTime()));
					}
					else
					{
						mCanDoorInterp = false;
						blockInterp->SetActive(false);
					}
					mSoundOnce = false;
				}
			}
			colliding = false;
		}
	};

}
