#pragma once
#include "../Actions/CodeComponent.hpp"
#include "../Common/Interpolator.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"



namespace Epoch {

	struct CCLevel3ElevatorButton : public CodeComponent {
		bool colliding = false, mInterpDone = true, mFlip = false, mIsPlayer = false;
		BaseObject* mChamberObject, *mButtonStand, *mWire;
		Interpolator<matrix4>* mChamberInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPlayerInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mClone1Interp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mClone2Interp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mClone3Interp = new Interpolator<matrix4>();
		matrix4 mButStart, mStandStart, mEleStart;
		
		//vec3f blockend, exitend;

		Level* cLevel;

		~CCLevel3ElevatorButton() {
			delete mChamberInterp;
			delete mPlayerInterp;
			delete mStartStandInterp;
			delete mStartButtonInterp;
			delete mClone1Interp;
			delete mClone2Interp;
			delete mClone3Interp;
		}

		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mChamberObject = cLevel->FindObjectWithName("L3ElevatorFloor");
			mButtonStand = cLevel->FindObjectWithName("EnvL3Buttonstand");
			mEleStart = mChamberObject->GetTransform().GetMatrix();
			mButStart = mObject->GetTransform().GetMatrix();
			mStandStart = mButtonStand->GetTransform().GetMatrix();
			mWire = cLevel->FindObjectWithName("D1Wire");
			TimeManager::Instance()->SaveSettingBoolToTimeline("CantPauseTime", false);
			Settings::GetInstance().SetBool("CantTeleport", false);
			TimeManager::Instance()->SaveSettingBoolToTimeline("CantTeleport", false);
		}

		virtual void OnCollision(Collider& _col, Collider& _other, float _time) {
			if (!Settings::GetInstance().GetBool("PauseMenuUp")) {
				if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane && (_other.GetBaseObject()->GetUniqueID() == cLevel->GetLeftController()->GetUniqueID() || _other.GetBaseObject()->GetUniqueID() == cLevel->GetRightController()->GetUniqueID())) {
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
								
								if (mWire)
								{
									std::vector<Component*>& comps = mWire->GetComponents(eCOMPONENT_CODE);
									for (unsigned int i = 0; i < comps.size(); i++)
									{
										if (dynamic_cast<CCMazeHelper*>(comps[i]))
										{
											((CCMazeHelper*)comps[i])->ResetBoxes();
											break;
										}
									}
								}
								std::vector<Component*>& comps2 = cLevel->FindObjectWithName("WallButton")->GetComponents(eCOMPONENT_CODE);
								for (unsigned int i = 0; i < comps2.size(); i++) {
									if (dynamic_cast<CCButtonPress*>(comps2[i])) {
										((CCButtonPress*)comps2[i])->ResetDoors();
										break;
									}
								}
								tempY = 1;
							}
							else {
								tempY = -1;
							}

							mChamberInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, 3.03f * tempY, 0), mChamberObject->GetTransform().GetMatrix());
							mChamberInterp->SetEasingFunction(Easing::QuadInOut);
							mChamberInterp->SetActive(true);

							if (_other.GetBaseObject()->GetUniqueID() == cLevel->GetLeftController()->GetUniqueID() || _other.GetBaseObject()->GetUniqueID() == cLevel->GetRightController()->GetUniqueID()) {
								mat = VRInputManager::GetInstance().GetPlayerPosition();
								mPlayerInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, 3.03f * tempY, 0), VRInputManager::GetInstance().GetPlayerPosition());
								mPlayerInterp->SetEasingFunction(Easing::QuadInOut);
								mPlayerInterp->SetActive(true);
								mIsPlayer = true;

								Settings::GetInstance().SetBool("CantPauseTime", true);
								Settings::GetInstance().SetBool("CantTeleport", true);
							}
							/*else
							{
								Clonepair* pair = TimeManager::Instance()->GetClonePair(_other.GetBaseObject()->GetUniqueID());
								mat = _other.GetBaseObject()->GetTransform().GetMatrix();
								mClone1Interp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, 3.28f * tempY, 0), _other.GetBaseObject()->GetTransform().GetMatrix());
								mClone1Interp->SetEasingFunction(Easing::QuadInOut);
								mClone1Interp->SetActive(true);

								BaseObject* other2 = cLevel->FindObjectWithID(pair->mOther1);
								mat = other2->GetTransform().GetMatrix();
								mClone2Interp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, 3.28f * tempY, 0), other2->GetTransform().GetMatrix());
								mClone2Interp->SetEasingFunction(Easing::QuadInOut);
								mClone2Interp->SetActive(true);

								BaseObject* other3 = cLevel->FindObjectWithID(pair->mOther1);
								mat = other3->GetTransform().GetMatrix();
								mClone3Interp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, 3.28f * tempY, 0), other3->GetTransform().GetMatrix());
								mClone3Interp->SetEasingFunction(Easing::QuadInOut);
								mClone3Interp->SetActive(true);
								mIsPlayer = false;
							}*/

							mat = mObject->GetTransform().GetMatrix();
							mStartButtonInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, 3.03f * tempY, 0), mObject->GetTransform().GetMatrix());
							mStartButtonInterp->SetEasingFunction(Easing::QuadInOut);
							mStartButtonInterp->SetActive(true);

							mat = mButtonStand->GetTransform().GetMatrix();
							mStartStandInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, 3.03f * tempY, 0), mButtonStand->GetTransform().GetMatrix());
							mStartStandInterp->SetEasingFunction(Easing::QuadInOut);
							mStartStandInterp->SetActive(true);
							mInterpDone = false;
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
					float dt = TimeManager::Instance()->GetDeltaTime();
					if (mChamberInterp->Update(dt)) {
						mInterpDone = true;
						Settings::GetInstance().SetBool("CantPauseTime", false);
						Settings::GetInstance().SetBool("CantTeleport", false);
					}
				//	if(mIsPlayer)
					{
						mPlayerInterp->Update(dt);	
					}
					/*else
					{
						mClone1Interp->Update(dt);
						mClone2Interp->Update(dt);
						mClone3Interp->Update(dt);
					}*/
					mStartButtonInterp->Update(dt);
					mStartStandInterp->Update(dt);
				}
		/*		else
				{
					mIsPlayer = false;
				}*/

				//float tempY = mChamberObject->GetTransform().GetMatrix().Position.y;
				////if elevator is mostly down
				//if (mInterpDone && tempY > -3.27f && tempY < -1.64f) {
				//	matrix4 mat = mChamberObject->GetTransform().GetMatrix();
				//	mChamberInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, -3.28f + (tempY *-1), 0), mChamberObject->GetTransform().GetMatrix());
				//	mChamberInterp->SetEasingFunction(Easing::QuadInOut);
				//	mChamberInterp->SetActive(true);

				//	mat = mObject->GetTransform().GetMatrix();
				//	mStartButtonInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, -3.28f + (tempY *-1), 0), mObject->GetTransform().GetMatrix());
				//	mStartButtonInterp->SetEasingFunction(Easing::QuadInOut);
				//	mStartButtonInterp->SetActive(true);

				//	mat = mButtonStand->GetTransform().GetMatrix();
				//	mStartStandInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, -3.28f + (tempY *-1), 0), mButtonStand->GetTransform().GetMatrix());
				//	mStartStandInterp->SetEasingFunction(Easing::QuadInOut);
				//	mStartStandInterp->SetActive(true);
				//	mInterpDone = false;
				//}
				//else if(mInterpDone && tempY > -1.64 && tempY < -0.116f)
				//{
				//	matrix4 mat = mChamberObject->GetTransform().GetMatrix();
				//	mChamberInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, -0.116f + (tempY *-1), 0), mChamberObject->GetTransform().GetMatrix());
				//	mChamberInterp->SetEasingFunction(Easing::QuadInOut);
				//	mChamberInterp->SetActive(true);

				//	mat = mObject->GetTransform().GetMatrix();
				//	mStartButtonInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, -0.116f + (tempY *-1), 0), mObject->GetTransform().GetMatrix());
				//	mStartButtonInterp->SetEasingFunction(Easing::QuadInOut);
				//	mStartButtonInterp->SetActive(true);

				//	mat = mButtonStand->GetTransform().GetMatrix();
				//	mStartStandInterp->Prepare(6, mat, mat * matrix4::CreateNewTranslation(0, -0.116f + (tempY *-1), 0), mButtonStand->GetTransform().GetMatrix());
				//	mStartStandInterp->SetEasingFunction(Easing::QuadInOut);
				//	mStartStandInterp->SetActive(true);
				//	mInterpDone = false;
				//}

				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mObject->GetTransform().GetMatrix().fourth);
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;
			}

			if(Settings::GetInstance().GetBool("ResetElevator"))
			{
				mChamberObject->GetTransform().SetMatrix(mEleStart);
				mButtonStand->GetTransform().SetMatrix(mStandStart);
				mObject->GetTransform().SetMatrix(mButStart);

				Settings::GetInstance().SetBool("ResetElevator", false);
			}
		}
	};
}