#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Physics\Physics.h"
#include "..\Common\Logger.h"
#include "../Core/Level.h"
#include "../Core/TimeManager.h"

namespace Epoch
{

	struct CCButtonPress : public CodeComponent
	{
		bool colliding = false;

		BaseObject *Block = nullptr, *Exit = nullptr;
		CubeCollider* blockCube, *exitCube;
		//matrix4 blockend, exitend;
		vec3f blockend, exitend;

		Level* cLevel;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			Block = cLevel->FindObjectWithName("Door1");
			Exit = cLevel->FindObjectWithName("Door2");

			blockCube = (CubeCollider*)Block->mComponents[eCOMPONENT_COLLIDER][0];
			exitCube = (CubeCollider*)Exit->mComponents[eCOMPONENT_COLLIDER][0];
			blockend = blockCube->GetPos() - vec3f(0, 2.6f, 0);
			exitend = exitCube->GetPos() + vec3f(0, 2.6f, 0);
			//blockend = blockCube->GetTransform().GetMatrix().CreateTranslation(vec3f(0, -2.6f, 0, 1));
			//exitend = exitCube->GetTransform().GetMatrix().CreateTranslation(vec3f(0, 2.6f, 0, 1));
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane)
			{
				colliding = true;

				vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
				vec3f tForce = norm * (norm * _other.mTotalForce);
				vec3f vel = norm * (norm * _other.mVelocity);
				vec3f accel = norm * (norm * _other.mAcceleration);
				if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0)
				{
					_col.mTotalForce = tForce;
					_col.mVelocity = vel;
					_col.mAcceleration = vel / _time;
					blockCube->SetPos(blockend);
					//Interpolator<matrix4>* blockInterp = TimeManager::Instance()->GetObjectInterpolator(Block->GetUniqueID());
					//blockInterp->SetActive(true);
					//blockInterp->Prepare(1.0f, blockCube->GetTransform().GetMatrix(), blockend, blockCube->GetTransform().GetMatrix());
					


					exitCube->SetPos(exitend);
					//Interpolator<matrix4>* exitInterp = TimeManager::Instance()->GetObjectInterpolator(Exit->GetUniqueID());
					//exitInterp->SetActive(true);
					//exitInterp->Prepare(1.0f, exitCube->GetTransform().GetMatrix(), exitend, exitCube->GetTransform().GetMatrix());

					//bool meMooscles = false;
					//while(!meMooscles)
					//{
					//	meMooscles = (blockInterp->Update(TimeManager::Instance()->GetDeltaTime()) && exitInterp->Update(TimeManager::Instance()->GetDeltaTime()));
					//}
					//blockInterp->SetActive(false);
					//exitInterp->SetActive(false);
					exitCube->mShouldMove = false;
					blockCube->mShouldMove = false;
				}
			}
			else
				colliding = false;
		}
	};
}