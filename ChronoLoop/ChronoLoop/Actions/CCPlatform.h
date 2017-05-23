#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"

namespace Epoch {

	struct CCPlatform : public CodeComponent 
	{
		bool playerCanInterp = false;
		Interpolator<matrix4>* platInterp;
		Interpolator<matrix4>* playerInterp;
		matrix4 end, PEnd;
		CubeCollider* collider;

		virtual void Start() 
		{
			platInterp = TimeManager::Instance()->GetObjectInterpolator(mObject->GetUniqueID());
			playerInterp = new Interpolator<matrix4>;
			collider = (CubeCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
			end = mObject->GetTransform().GetMatrix() * matrix4::CreateNewTranslation(-4, 0, 0);
		}

		virtual void OnTriggerEnter(Collider& _col, Collider& _other) 
		{
			if (((Component*)&_other)->GetBaseObject()->GetName() == "StartBound") 
			{
				
			} 
			else if (((Component*)&_other)->GetBaseObject()->GetName() == "EndBound") 
			{
				
			}

			if(_other.mColliderType == Collider::eCOLLIDER_Controller && Settings::GetInstance().GetBool("PlatInterp"))
			{
				playerCanInterp = true;
			}

			//if(_other.mColliderType == Collider::eCOLLIDER_Controller && Settings::GetInstance().GetBool("PlatInterp"))
			//{
			//	
			//}
		}

		virtual void Update() 
		{
			if(Settings::GetInstance().GetBool("PrepareInterp"))
			{
				Settings::GetInstance().SetBool("PrepareInterp", false);
				platInterp->SetActive(true);
				platInterp->Prepare(2, mObject->GetTransform().GetMatrix(), end, mObject->GetTransform().GetMatrix());

				if(playerCanInterp)
				{
					PEnd = matrix4() * matrix4::CreateNewTranslation(VRInputManager::GetInstance().GetPlayerPosition().Position) * matrix4::CreateNewTranslation(-4, 0, 0);
					playerInterp->Prepare(2, VRInputManager::GetInstance().GetPlayerPosition(), PEnd, VRInputManager::GetInstance().GetPlayerPosition());
					playerInterp->SetActive(true);
				}
			}

			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (Settings::GetInstance().GetBool("PlatInterp") && !Settings::GetInstance().GetBool("doneInterp"))
				{
					Settings::GetInstance().SetBool("doneInterp", platInterp->Update(TimeManager::Instance()->GetDeltaTime()));
					collider->SetPos(vec3f(*mObject->GetTransform().GetPosition()));

					if (playerCanInterp)
						playerInterp->Update(TimeManager::Instance()->GetDeltaTime());
				}
				else
				{
					Settings::GetInstance().SetBool("PlatInterp", false);
					platInterp->SetActive(false);
					playerCanInterp = false;
				}
			}
		}

		virtual void OnDestroy()
		{
			delete playerInterp;
		}
	};

}
