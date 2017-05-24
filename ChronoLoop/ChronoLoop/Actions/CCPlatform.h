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
		BaseObject* invis = nullptr;
		vec4f startpos = vec4f(), objpos = vec4f();

		virtual void Start() 
		{
			Level* l = LevelManager::GetInstance().GetCurrentLevel();
			invis = l->FindObjectWithName("EnvCantLetYouDoThatStarFox");
			if (invis)
				startpos = invis->GetTransform().GetMatrix().Position;
			platInterp = TimeManager::Instance()->GetObjectInterpolator(mObject->GetUniqueID());
			playerInterp = new Interpolator<matrix4>;
			collider = (CubeCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
			end = mObject->GetTransform().GetMatrix().CreateTranslation(-4, 0, 0);
			objpos = mObject->GetTransform().GetMatrix().Position;
		}

		virtual void OnTriggerEnter(Collider& _col, Collider& _other) 
		{
			if (((Component*)&_other)->GetBaseObject()->GetName() == "StartBound")
			{
				//Start pos
				matrix4 m = invis->GetTransform().GetMatrix();
				m.Position = startpos;
				invis->GetTransform().SetMatrix(m);
			} 
			else if (((Component*)&_other)->GetBaseObject()->GetName() == "EndBound") 
			{
				//Yeah
				
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
					PEnd = VRInputManager::GetInstance().GetPlayerPosition().CreateNewTranslation(-4, 0, 0);
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
			
			if (mObject->GetTransform().GetMatrix().Position != objpos)
			{
				//other pos
				matrix4 m = invis->GetTransform().GetMatrix();
				m.Position = vec4f(2.83, 0, -1.02, 1);
				invis->GetTransform().SetMatrix(m);
			}
		}

		virtual void OnDestroy()
		{
			delete playerInterp;
		}
	};

}
