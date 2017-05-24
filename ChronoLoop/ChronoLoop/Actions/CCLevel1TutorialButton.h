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
		bool colliding = false, interp = false, once = false;
		BaseObject *tutButton, *tutChamber, *tutDoor;
		Interpolator<matrix4>* doorInterp;
		Interpolator<matrix4>* chamberInterp;
		matrix4 doorStart, doorEnd, chamberStart, chamberEnd;

		virtual void Start()
		{
			((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mPress = true;
			Level* clevel = LevelManager::GetInstance().GetCurrentLevel();
			tutButton = clevel->FindObjectWithName("tutButton");
			tutChamber = clevel->FindObjectWithName("EnvtutChamber");
			tutDoor = clevel->FindObjectWithName("EnvTransparentDoor3");

			doorInterp = TimeManager::Instance()->GetObjectInterpolator(tutDoor->GetUniqueID());
			chamberInterp = TimeManager::Instance()->GetObjectInterpolator(tutChamber->GetUniqueID());

			doorStart = tutDoor->GetTransform().GetMatrix();
			chamberStart = tutChamber->GetTransform().GetMatrix();
			doorEnd = doorStart * tutDoor->GetTransform().GetMatrix().CreateNewTranslation(vec4f(0, 2.6f, 0, 1));
			chamberEnd = chamberStart * tutChamber->GetTransform().GetMatrix().CreateNewTranslation(vec4f(0, -3.02f, 0, 1));
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!Settings::GetInstance().GetBool("PauseMenuUp"))
			{
				if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName().find("Buttonstand")) {
					colliding = true;

					vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
					vec3f tForce = norm * (norm * _other.mTotalForce);
					vec3f vel = norm * (norm * _other.mVelocity);
					vec3f accel = norm * (norm * _other.mAcceleration);
					if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0) {
						_col.mTotalForce = tForce;
						_col.mVelocity = vel;
						_col.mAcceleration = vel / _time;

						doorInterp->SetActive(true);
						doorInterp->Prepare(0.69f, tutDoor->GetTransform().GetMatrix(), doorEnd, tutDoor->GetTransform().GetMatrix());
						interp = true;

						if (Settings::GetInstance().GetInt("tutStep") == 1) {
							Settings::GetInstance().SetUInt("tut1ButtonPress", TimeManager::Instance()->GetCurrentSnapFrame());
							chamberInterp->SetActive(true);
							chamberInterp->Prepare(0.2f, tutChamber->GetTransform().GetMatrix(), chamberEnd, tutChamber->GetTransform().GetMatrix());
						}

						if (tutDoor->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0) {
							if (dynamic_cast<AudioEmitter*>(tutDoor->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
								((AudioEmitter*)tutDoor->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent(Emitter::ePlay);
						}
					}
				} else
					colliding = false;
			}
		}

		virtual void Update()
		{
			if (!once && Settings::GetInstance().GetInt("tutStep") == 6)
			{
				doorInterp->SetActive(true);
				doorInterp->Prepare(0.69f, tutDoor->GetTransform().GetMatrix(), doorStart * tutDoor->GetTransform().GetMatrix().CreateNewTranslation(vec4f(0, 2.6f, 0, 1)), tutDoor->GetTransform().GetMatrix());
				interp = true;
				once = true;

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
				
				if(Settings::GetInstance().GetInt("tutStep") == 1 && chamberInterp->Update(TimeManager::Instance()->GetDeltaTime()))
					Settings::GetInstance().SetInt("tutStep", 2);//Pause Time
			}
		}
	};
}