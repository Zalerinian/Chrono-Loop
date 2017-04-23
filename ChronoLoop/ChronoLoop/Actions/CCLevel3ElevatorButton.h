#pragma once
#include "../Actions/CodeComponent.hpp"
#include "../Common/Interpolator.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"

namespace Epoch {

	struct CCLevel3ElevatorButton : public CodeComponent {
		bool colliding = false;
		BaseObject* mChamberObject, *mButtonStand;
		Interpolator<matrix4>* mChamberInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPlayerInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartButtonInterp = new Interpolator<matrix4>();
		//vec3f blockend, exitend;
		bool once = false;

		Level* cLevel;

		virtual void Start() {
			mChamberObject = cLevel->FindObjectWithName("L3Chamber");
			mButtonStand = cLevel->FindObjectWithName("L3Stand");
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time) {
			if (!Settings::GetInstance().GetBool("PauseMenuUp")) {
				if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "Buttonstand") {
					colliding = true;

					vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
					vec3f tForce = norm * (norm * _other.mTotalForce);
					vec3f vel = norm * (norm * _other.mVelocity);
					vec3f accel = norm * (norm * _other.mAcceleration);
					if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0) {
						_col.mTotalForce = tForce;
						_col.mVelocity = vel;
						_col.mAcceleration = vel / _time;

						if (!once) {
							//Playsound
							if (_col.GetBaseObject()->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0) {
								if (dynamic_cast<SFXEmitter*>(_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
									((SFXEmitter*)_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();


							}
							matrix4 mat = mChamberObject->GetTransform().GetMatrix();
							mChamberInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -4, 0), mChamberObject->GetTransform().GetMatrix());
							mChamberInterp->SetEasingFunction(Easing::QuadInOut);
							mChamberInterp->SetActive(true);

							mat = VRInputManager::GetInstance().GetPlayerPosition();
							mPlayerInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -4, 0), VRInputManager::GetInstance().GetPlayerPosition());
							mPlayerInterp->SetEasingFunction(Easing::QuadInOut);
							mPlayerInterp->SetActive(true);

							mat = mObject->GetTransform().GetMatrix();
							mStartButtonInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -4, 0), mObject->GetTransform().GetMatrix());
							mStartButtonInterp->SetEasingFunction(Easing::QuadInOut);
							mStartButtonInterp->SetActive(true);

							mat = mButtonStand->GetTransform().GetMatrix();
							mStartStandInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -4, 0), mButtonStand->GetTransform().GetMatrix());
							mStartStandInterp->SetEasingFunction(Easing::QuadInOut);
							mStartStandInterp->SetActive(true);
							once = true;
						}
					}

				} else {
					colliding = false;
				}
			}
		}


		virtual void Update() {
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
				if (once) {
					mChamberInterp->Update(TimeManager::Instance()->GetDeltaTime());
					mPlayerInterp->Update(TimeManager::Instance()->GetDeltaTime());
					mStartButtonInterp->Update(TimeManager::Instance()->GetDeltaTime());
					mStartStandInterp->Update(TimeManager::Instance()->GetDeltaTime());
				}

			}
		}
	};
}