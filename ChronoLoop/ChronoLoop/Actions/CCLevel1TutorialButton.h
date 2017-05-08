#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Physics\Physics.h"
#include "..\Common\Logger.h"
#include "../Core/Level.h"
#include "../Core/TimeManager.h"
#include "../Common/Settings.h"
#include "../Core/LevelManager.h"

namespace Epoch
{

	struct CCLevel1TutorialButton : public CodeComponent
	{
		bool colliding = false, interp = false;
		BaseObject *tutButton, *tutChamber, *tutDoor;
		Interpolator<matrix4>* doorInterp;
		Interpolator<matrix4>* chamberInterp;
		matrix4 doorStart, doorEnd, chamberStart, chamberEnd;

		virtual void Start()
		{
			Level* clevel = LevelManager::GetInstance().GetCurrentLevel();
			tutButton = clevel->FindObjectWithName("tutButton");
			tutChamber = clevel->FindObjectWithName("tutChamber");
			tutDoor = clevel->FindObjectWithName("TransparentDoor3");

			doorInterp = TimeManager::Instance()->GetObjectInterpolator(tutDoor->GetUniqueID());
			chamberInterp = TimeManager::Instance()->GetObjectInterpolator(tutChamber->GetUniqueID());

			doorStart = tutDoor->GetTransform().GetMatrix();
			chamberStart = tutChamber->GetTransform().GetMatrix();
			doorEnd = doorStart * tutDoor->GetTransform().GetMatrix().CreateTranslation(vec4f(0, 2.6f, 0, 1));
			chamberEnd = chamberStart * tutChamber->GetTransform().GetMatrix().CreateTranslation(vec4f(0, -3.02f, 0, 1));
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!Settings::GetInstance().GetBool("PauseMenuUp"))
			{
				if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName().find("Buttonstand"))
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

						if (!interp && Settings::GetInstance().GetInt("tutStep") == 1)//Teleported
						{
							Settings::GetInstance().SetUInt("tut1ButtonPress", TimeManager::Instance()->GetCurrentSnapFrame());
							float t = Settings::GetInstance().GetUInt("tut1ButtonPress");
							doorInterp->SetActive(true);
							doorInterp->Prepare(0.69f, tutDoor->GetTransform().GetMatrix(), doorEnd, tutDoor->GetTransform().GetMatrix());
							chamberInterp->SetActive(true);
							chamberInterp->Prepare(0.2f, tutChamber->GetTransform().GetMatrix(), chamberEnd, tutChamber->GetTransform().GetMatrix());
							interp = true;

							if (tutDoor->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0)
							{
								if (dynamic_cast<AudioEmitter*>(tutDoor->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
									((AudioEmitter*)tutDoor->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent(Emitter::ePlay);
							}
						}
					}
				}
			}
		}

		virtual void Update()
		{
			if (!interp && Settings::GetInstance().GetInt("tutStep") == 6)
			{
				doorInterp->SetActive(true);
				doorInterp->Prepare(0.69f, tutDoor->GetTransform().GetMatrix(), doorStart * tutDoor->GetTransform().GetMatrix().CreateTranslation(vec4f(0, 2.6f, 0, 1)), tutDoor->GetTransform().GetMatrix());
				interp = true;

				if (tutDoor->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0)
				{
					if (dynamic_cast<AudioEmitter*>(tutDoor->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
						((AudioEmitter*)tutDoor->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent(Emitter::ePlay);
				}
			}

			if (interp && !LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (doorInterp->Update(TimeManager::Instance()->GetDeltaTime()))
					interp = false;
				if(chamberInterp->Update(TimeManager::Instance()->GetDeltaTime()))
					Settings::GetInstance().SetInt("tutStep", 2);//Pause Time
			}
		}
	};
}