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

	struct CCPlatform : public CodeComponent
	{
		bool playerCanInterp = false, tempDoor = false, mFlip = false;
		matrix4 PlatEnd, PlayerEnd, PlatStart, PlayerStart;
		CubeCollider* collider;
		Interpolator<matrix4>* platInterp; //(block)
		Interpolator<matrix4>* playerInterp;//(exit)

		virtual void Start()
		{
			platInterp = TimeManager::Instance()->GetObjectInterpolator(mObject->GetUniqueID());
			playerInterp = new Interpolator<matrix4>;
			collider = (CubeCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
			PlatEnd = mObject->GetTransform().GetMatrix() * matrix4::CreateTranslation(-4, 0, 0);
			PlatStart = mObject->GetTransform().GetMatrix();
		}

		virtual void OnTriggerEnter(Collider& _col, Collider& _other)
		{
			if (_other.mColliderType == Collider::eCOLLIDER_Controller && Settings::GetInstance().GetBool("PlatInterp"))
			{
				playerCanInterp = true;
			}
		}
		virtual void Update()
		{
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (Settings::GetInstance().GetBool("ButtonHeld"))
				{
					//SystemLogger::GetLog() << "Colliding" << std::endl;

					if (!mFlip && Settings::GetInstance().GetBool("PrepareInterp"))
					{
						mFlip = true;
						Settings::GetInstance().SetBool("PrepareInterp", false);
						platInterp->SetActive(true);
						platInterp->Prepare(5, mObject->GetTransform().GetMatrix(), PlatEnd, mObject->GetTransform().GetMatrix());
					
						if(playerCanInterp)
						{
							PlayerEnd = matrix4() * matrix4::CreateTranslation(VRInputManager::GetInstance().GetPlayerPosition().Position) * matrix4::CreateTranslation(-4, 0, 0);
							playerInterp->Prepare(5, VRInputManager::GetInstance().GetPlayerPosition(), PlayerEnd, VRInputManager::GetInstance().GetPlayerPosition());
							playerInterp->SetActive(true);
						}
					}

					if (Settings::GetInstance().GetBool("PlatInterp") && !Settings::GetInstance().GetBool("doneInterp"))
					{
						if (Settings::GetInstance().GetBool("shouldGoBack"))
						{
							platInterp->Prepare(0.3f, mObject->GetTransform().GetMatrix(), PlatStart, mObject->GetTransform().GetMatrix());
						}
						Settings::GetInstance().SetBool("doneInterp", (platInterp->Update(TimeManager::Instance()->GetDeltaTime())));
						collider->SetPos(vec3f(*mObject->GetTransform().GetPosition()));
						if (playerCanInterp)
							playerInterp->Update(TimeManager::Instance()->GetDeltaTime());
					}
					
					if(Settings::GetInstance().GetBool("doneInterp"))
					{
						Settings::GetInstance().SetBool("PlatInterp", false);
						platInterp->SetActive(false);
						playerInterp->SetActive(false);
						playerCanInterp = false;
					}
				}
				else if (!Settings::GetInstance().GetBool("ButtonHeld"))
				{
					if (mFlip)
					{
						Settings::GetInstance().SetBool("PrepareInterp", false);
						mFlip = false;
						platInterp->SetActive(true);
						platInterp->Prepare(5, mObject->GetTransform().GetMatrix(), PlatStart, mObject->GetTransform().GetMatrix());

						if(playerCanInterp)
						{
							PlayerEnd = matrix4() * matrix4::CreateTranslation(VRInputManager::GetInstance().GetPlayerPosition().Position) * matrix4::CreateTranslation(4, 0, 0);
							playerInterp->Prepare(5, VRInputManager::GetInstance().GetPlayerPosition(), PlayerStart, VRInputManager::GetInstance().GetPlayerPosition());
							playerInterp->SetActive(true);
						}


						Settings::GetInstance().SetBool("PlatInterp", true);
						Settings::GetInstance().SetBool("doneInterp", false);
					}

					if (Settings::GetInstance().GetBool("PlatInterp") && !Settings::GetInstance().GetBool("doneInterp"))
					{
						if (Settings::GetInstance().GetBool("shouldGoBack"))
						{
							platInterp->Prepare(0.3f, mObject->GetTransform().GetMatrix(), PlatStart, mObject->GetTransform().GetMatrix());
						}
						Settings::GetInstance().SetBool("doneInterp", (platInterp->Update(TimeManager::Instance()->GetDeltaTime())));
						collider->SetPos(vec3f(*mObject->GetTransform().GetPosition()));
						if (playerCanInterp)
							playerInterp->Update(TimeManager::Instance()->GetDeltaTime());
					}
					else if (Settings::GetInstance().GetBool("doneInterp"))
					{
						Settings::GetInstance().SetBool("PlatInterp", false);
						platInterp->SetActive(false);
						playerInterp->SetActive(false);
						playerCanInterp = false;
					}
					else if (Settings::GetInstance().GetBool("shouldGoBack") && !Settings::GetInstance().GetBool("doneInterp"))
					{
						platInterp->SetActive(true);
						Settings::GetInstance().SetBool("PlatInterp", true);
						platInterp->Prepare(0.3f, mObject->GetTransform().GetMatrix(), PlatStart, mObject->GetTransform().GetMatrix());
					}
				}
			}
			playerCanInterp = false;
		}

		virtual void OnDestroy()
		{
			delete playerInterp;
		}
	};
}


