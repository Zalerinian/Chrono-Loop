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
		bool colliding = false, mCanDoorInterp = false, mDoorDoneInterpolating = false;
		BaseObject *Block = nullptr, *Exit = nullptr;
		CubeCollider* blockCube, *exitCube;
		matrix4 blockend, exitend;
		Interpolator<matrix4>* blockInterp;
		Interpolator<matrix4>* exitInterp;
		//vec3f blockend, exitend;
		bool once = false;

		Level* cLevel;
		void SetDoorInterpBool(bool _set) { mCanDoorInterp = _set; };
		bool GetDoorInterpBool() { return mCanDoorInterp; };
		virtual void Start()
		{

			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			Block = cLevel->FindObjectWithName("TransparentDoor1");
			Exit = cLevel->FindObjectWithName("TransparentDoor2");
			blockInterp = TimeManager::Instance()->GetObjectInterpolator(Block->GetUniqueID());
			exitInterp = TimeManager::Instance()->GetObjectInterpolator(Exit->GetUniqueID());

			blockCube = (CubeCollider*)Block->mComponents[eCOMPONENT_COLLIDER][0];
			exitCube = (CubeCollider*)Exit->mComponents[eCOMPONENT_COLLIDER][0];
			//blockend = blockCube->GetPos() - vec3f(0, 2.6f, 0);
			//exitend = exitCube->GetPos() + vec3f(0, 2.6f, 0);
			blockend = blockCube->GetTransform().GetMatrix() * blockCube->GetTransform().GetMatrix().CreateTranslation(vec4f(0, -2.6f, 0, 1));
			exitend = exitCube->GetTransform().GetMatrix() * exitCube->GetTransform().GetMatrix().CreateTranslation(vec4f(0, 2.6f, 0, 1));
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!Settings::GetInstance().GetBool("PauseMenuUp")) {
				if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane) {
					colliding = true;

					vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
					vec3f tForce = norm * (norm * _other.mTotalForce);
					vec3f vel = norm * (norm * _other.mVelocity);
					vec3f accel = norm * (norm * _other.mAcceleration);
					if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0) {
						_col.mTotalForce = tForce;
						_col.mVelocity = vel;
						_col.mAcceleration = vel / _time;
						//blockCube->SetPos(blockend);
						blockInterp->SetActive(true);
						blockInterp->Prepare(0.69f, blockCube->GetTransform().GetMatrix(), blockend, blockCube->GetTransform().GetMatrix());

						//exitCube->SetPos(exitend);
						exitInterp->SetActive(true);
						exitInterp->Prepare(0.69f, exitCube->GetTransform().GetMatrix(), exitend, exitCube->GetTransform().GetMatrix());

					if (!once)
					{
						if (_col.GetBaseObject()->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0)
						{
							if (dynamic_cast<SFXEmitter*>(_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
								((SFXEmitter*)_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
						}

						if (Block->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0)
						{
							if (dynamic_cast<SFXEmitter*>(Block->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
								((SFXEmitter*)Block->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
						}
						if (Exit->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0)
						{
							if (dynamic_cast<SFXEmitter*>(Exit->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
								((SFXEmitter*)Exit->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
						}
						once = true;
					}

					mCanDoorInterp = true;
					mDoorDoneInterpolating = false;
				}
			}
			else
			{
				once = false;
				colliding = false;
			}
		}
		virtual void Update()
		{
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
				if (mCanDoorInterp && !mDoorDoneInterpolating)
				{
					mDoorDoneInterpolating = (blockInterp->Update(TimeManager::Instance()->GetDeltaTime()) || exitInterp->Update(TimeManager::Instance()->GetDeltaTime()));
				}
				else
				{


					mCanDoorInterp = false;
					blockInterp->SetActive(false);
					exitInterp->SetActive(false);
					exitCube->mShouldMove = false;
					blockCube->mShouldMove = false;
				}
			}
		}
	};
}