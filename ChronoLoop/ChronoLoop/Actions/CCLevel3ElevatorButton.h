#pragma once
#include "../Actions/CodeComponent.hpp"
#include "../Common/Interpolator.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"


namespace Epoch {

	struct CCLevel3ElevatorButton : public CodeComponent {
		bool colliding = false, mInterpDone = true, mFlip = false;
		BaseObject* mChamberObject, *mButtonStand;
		Interpolator<matrix4>* mChamberInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPlayerInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartButtonInterp = new Interpolator<matrix4>();
		
		//vec3f blockend, exitend;

		Level* cLevel;

		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mChamberObject = cLevel->FindObjectWithName("L3Elevator");
			mButtonStand = cLevel->FindObjectWithName("L3Buttonstand");
			TimeManager::Instance()->SaveSettingBoolToTimeline("CantPauseTime", false);
			Settings::GetInstance().SetBool("CantTeleport", false);
			TimeManager::Instance()->SaveSettingBoolToTimeline("CantTeleport", false);
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

						if (mInterpDone) {
							//Playsound
							if (_col.GetBaseObject()->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0) {
								if (dynamic_cast<SFXEmitter*>(_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
									((SFXEmitter*)_col.GetBaseObject()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
							}
							matrix4 mat = mChamberObject->GetTransform().GetMatrix();
							float tempY = mChamberObject->GetTransform().GetMatrix().Position.y;
							if (tempY < -1.0) {
								tempY = 1;
							}
							else {
								tempY = -1;
							}

							mChamberInterp->Prepare(4, mat, mat * matrix4::CreateTranslation(0, 3.28f * tempY, 0), mChamberObject->GetTransform().GetMatrix());
							mChamberInterp->SetEasingFunction(Easing::QuadInOut);
							mChamberInterp->SetActive(true);

							mat = VRInputManager::GetInstance().GetPlayerPosition();
							mPlayerInterp->Prepare(4, mat, mat * matrix4::CreateTranslation(0, 3.28f * tempY, 0), VRInputManager::GetInstance().GetPlayerPosition());
							mPlayerInterp->SetEasingFunction(Easing::QuadInOut);
							mPlayerInterp->SetActive(true);

							mat = mObject->GetTransform().GetMatrix();
							mStartButtonInterp->Prepare(4, mat, mat * matrix4::CreateTranslation(0, 3.28f * tempY, 0), mObject->GetTransform().GetMatrix());
							mStartButtonInterp->SetEasingFunction(Easing::QuadInOut);
							mStartButtonInterp->SetActive(true);

							mat = mButtonStand->GetTransform().GetMatrix();
							mStartStandInterp->Prepare(4, mat, mat * matrix4::CreateTranslation(0, 3.28f * tempY, 0), mButtonStand->GetTransform().GetMatrix());
							mStartStandInterp->SetEasingFunction(Easing::QuadInOut);
							mStartStandInterp->SetActive(true);
							mInterpDone = false;
							Settings::GetInstance().SetBool("CantPauseTime", true);
							Settings::GetInstance().SetBool("CantTeleport", true);
						}
					}

				} else {
					colliding = false;
				}
			}
		}


		virtual void Update() {
			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
				if (!mInterpDone) {
					if (mChamberInterp->Update(TimeManager::Instance()->GetDeltaTime())) {
						mInterpDone = true;
						Settings::GetInstance().SetBool("CantPauseTime", false);
						Settings::GetInstance().SetBool("CantTeleport", false);
					}
					mPlayerInterp->Update(TimeManager::Instance()->GetDeltaTime());
					mStartButtonInterp->Update(TimeManager::Instance()->GetDeltaTime());
					mStartStandInterp->Update(TimeManager::Instance()->GetDeltaTime());

					((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mObject->GetTransform().GetMatrix().fourth);
					((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;
					((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;
				}
			}
		}
	};
}