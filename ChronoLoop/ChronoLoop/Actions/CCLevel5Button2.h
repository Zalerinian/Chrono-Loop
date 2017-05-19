#pragma once
#include "CodeComponent.hpp"
#include "../Core/LevelManager.h"

namespace Epoch
{

	struct CCLevel5Button2 : public CodeComponent
	{
		bool colliding = false, mhitting = false, mCanDoorInterp = false, mDoorDoneInterpolating = false, mFlip = false, mSoundOnce = false;;
		bool tempDoor = false;
		BaseObject *Block = nullptr, *Exit = nullptr;
		CubeCollider* blockCube;
		matrix4 blockend, blockStart;
		Interpolator<matrix4>* blockInterp;
		Level* cLevel;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			Block = cLevel->FindObjectWithName("ExitSideOpening");
			blockInterp = TimeManager::Instance()->GetObjectInterpolator(Block->GetUniqueID());

			blockCube = (CubeCollider*)Block->mComponents[eCOMPONENT_COLLIDER][0];
			blockStart = blockCube->GetTransform().GetMatrix();
			blockend = blockCube->GetTransform().GetMatrix() * blockCube->GetTransform().GetMatrix().CreateTranslation(vec4f(0, 2.6f, 0, 1));
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "EnvButtonstand")
			{
				colliding = true;

				if (!tempDoor)
				{
					blockInterp->SetActive(true);
					blockInterp->Prepare(0.69f, blockCube->GetTransform().GetMatrix(), blockend, blockCube->GetTransform().GetMatrix());

					((Collider*)blockCube->GetBaseObject()->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(blockend.Position);

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
						blockInterp->Prepare(0.69f, blockCube->GetTransform().GetMatrix(), blockStart, blockCube->GetTransform().GetMatrix());
						((Collider*)blockCube->GetBaseObject()->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(blockStart.Position);

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
