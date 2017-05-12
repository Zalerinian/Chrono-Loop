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
		bool colliding = false, mhitting = false, mFlip = false, mSoundOnce = false;
		bool interp = false, flip = false, canInterp = false, doneInterp = false, once = false;
		unsigned int missedCount = 0;
		BaseObject* platform;
		CubeCollider* collider;
		Interpolator<matrix4>* platInterp;
		Interpolator<matrix4>* playerInterp;
		matrix4 end, start, PEnd, PStart;
		Level* cLevel;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			platform = cLevel->FindObjectWithName("PlatformFloor");
			collider = (CubeCollider*)platform->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
			platInterp = TimeManager::Instance()->GetObjectInterpolator(platform->GetUniqueID());
			playerInterp = new Interpolator<matrix4>();
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
					interp = true;
					platInterp->SetActive(true);
					platInterp->Prepare(2, platform->GetTransform().GetMatrix(), end, platform->GetTransform().GetMatrix());
				}

				ButtonCollider* butCol = (ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
				if (fabsf(_other.mVelocity * butCol->mPushNormal) < 0)
					butCol->mVelocity = vec3f(0, 0, 0);
				else {
					vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
					vec3f tForce = norm * (norm * _other.mTotalForce);
					vec3f vel = norm * (norm * _other.mVelocity);
					vec3f accel = norm * (norm * _other.mAcceleration);
					if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0) {
						_col.mTotalForce = tForce;
						_col.mVelocity = vel;
						_col.mAcceleration = vel / _time;
						//blockCube->SetPos(blockend);
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
					missedCount = 0;
					flip = true;
					if (canInterp && !doneInterp)
					{
						doneInterp = platInterp->Update(TimeManager::Instance()->GetDeltaTime());
						collider->SetPos(vec3f(*platform->GetTransform().GetPosition()));
						Settings::GetInstance().SetBool("PlatInterp", true);
						if(Settings::GetInstance().GetBool("PlayerInterp"))
						{
							if(!once)
							{
								SystemLogger::Debug() << "moving negative" << std::endl;
								once = true;
								PEnd = matrix4() * matrix4::CreateTranslation(VRInputManager::GetInstance().GetPlayerPosition().Position) * matrix4::CreateTranslation(-4, 0, 0);
								playerInterp->Prepare(2, VRInputManager::GetInstance().GetPlayerPosition(), PEnd, VRInputManager::GetInstance().GetPlayerPosition());
								playerInterp->SetActive(true);
							}
							playerInterp->Update(TimeManager::Instance()->GetDeltaTime());
						}
						else if(!Settings::GetInstance().GetBool("PlayerInterp"))
						{
							playerInterp->SetActive(false);
							Settings::GetInstance().SetBool("PlayerInterp", false);
							once = false;
						}
					}
					else if(doneInterp)
					{
						canInterp = false;
						platInterp->SetActive(false);
						playerInterp->SetActive(false);
						once = false;
						Settings::GetInstance().SetBool("PlatInterp", false);
						Settings::GetInstance().SetBool("PlayerInterp", false);
					}
				}
				else
				{
					interp = false;
					if(++missedCount < 4)
					{
						return;
					}
					if (flip)
					{
						flip = false;
						platInterp->SetActive(true);
						platInterp->Prepare(5, platform->GetTransform().GetMatrix(), start, platform->GetTransform().GetMatrix());
						canInterp = true;
						doneInterp = false;
					}

					if (canInterp && !doneInterp)
					{
						doneInterp = (platInterp->Update(TimeManager::Instance()->GetDeltaTime()));
						collider->SetPos(vec3f(*platform->GetTransform().GetPosition()));
						Settings::GetInstance().SetBool("PlatInterp", true);
						if (Settings::GetInstance().GetBool("PlayerInterp"))
						{
							if (!once) 
							{
								SystemLogger::Debug() << "moving positive" << std::endl;
								once = true;
								PStart = matrix4() * matrix4::CreateTranslation(VRInputManager::GetInstance().GetPlayerPosition().Position) * matrix4::CreateTranslation(4, 0, 0);
								playerInterp->Prepare(2, VRInputManager::GetInstance().GetPlayerPosition(), PStart, VRInputManager::GetInstance().GetPlayerPosition());
								playerInterp->SetActive(true);
							}
							playerInterp->Update(TimeManager::Instance()->GetDeltaTime());
						}
						else if (!Settings::GetInstance().GetBool("PlayerInterp")) 
						{
							playerInterp->SetActive(false);
							Settings::GetInstance().SetBool("PlayerInterp", false);
							once = false;
						}
					} 
					else if(doneInterp)
					{
						canInterp = false;
						platInterp->SetActive(false);
						playerInterp->SetActive(false);
						Settings::GetInstance().SetBool("PlatInterp", false);
						Settings::GetInstance().SetBool("PlayerInterp", false);
						once = false;
					}
				}
			}
			colliding = false;
		}

		virtual void OnDestroy()
		{
			delete playerInterp;
		}
	};

}