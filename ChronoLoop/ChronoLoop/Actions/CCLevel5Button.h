#pragma once
#include "CodeComponent.hpp"
#include "../Core/LevelManager.h"

namespace Epoch
{
	
	struct CCLevel5Button : public CodeComponent
	{
		bool colliding = false, mhitting = false, mCanDoorInterp = false, mDoorDoneInterpolating = false, mFlip = false, mSoundOnce = false;;
		bool tempDoor = false;
		BaseObject *Block = nullptr, *Exit = nullptr;
		CubeCollider* blockCube;
		MeshComponent* blockMesh;

		//matrix4 blockend, blockStart;
		//Interpolator<matrix4>* blockInterp;
		Level* cLevel;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			Block = cLevel->FindObjectWithName("StartSideOpeningEnergy");

			blockCube = (CubeCollider*)Block->mComponents[eCOMPONENT_COLLIDER][0];
			blockMesh = (MeshComponent*)Block->mComponents[eCOMPONENT_MESH][0];


		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "EnvButtonstand")
			{
				colliding = true;

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
			else
			{
				colliding = false;
			}
		}

		virtual void Update()
		{
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (colliding)
				{
					//SystemLogger::GetLog() << "Colliding" << std::endl;
					blockCube->Disable();
					//blockMesh->SetVisible(false);
					//blockMesh->SetVisible(true);
					//blockMesh->SetVisible(false);
					//blockMesh->SetVisible(true);
					//blockMesh->SetVisible(false);
					//blockMesh->SetVisible(true);
					//blockMesh->SetVisible(false);
					//blockMesh->SetVisible(true);
					blockMesh->SetVisible(false);
				}
				else
				{
					tempDoor = false;
					blockCube->Enable();
					//SystemLogger::GetLog() << "Not Colliding" << std::endl;
					//blockMesh->SetVisible(true);
					//blockMesh->SetVisible(false);
					//blockMesh->SetVisible(true);
					//blockMesh->SetVisible(false);
					//blockMesh->SetVisible(true);
					//blockMesh->SetVisible(false);
					blockMesh->SetVisible(true);

					mSoundOnce = false;
				}
			}
			//colliding = false;
		}
	};

}
