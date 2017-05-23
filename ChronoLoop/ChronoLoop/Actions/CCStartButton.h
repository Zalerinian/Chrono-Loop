#pragma once
#include "CodeComponent.hpp"
#include "..\Objects\BaseObject.h"
#include "..\Common\Interpolator.h"
#include "..\Core\LevelManager.h"
#include "..\Core\Level.h"
#include "..\Sound\SoundEngine.h"
#include "CCProgressBar.h"

namespace Epoch
{

	struct CCStartButton : public CodeComponent {
		bool mBooped, mBooped2, mBooped3, isComplete;
		bool AudioToggle;

		Interpolator<matrix4>* mChamberInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPlayerInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartSignInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitSignInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mCloseInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPrevButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPrevStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPrevSignInterp = new Interpolator<matrix4>();

		Listener* l;

		BaseObject *mChamberObject, *mExitButton, *mStartStand, *mStartSign, *mExitStand, *mExitSign, *mClosePanel, *mPrevButton, *mPrevStand, *mPrevSign;
		Transform identity;

		Level* cLevel = nullptr;
		//BaseObject *mProgressBar;
		CCProgressBar* mPB;

		virtual void Start()
		{
			AudioToggle = false;
			mBooped = mBooped2 = mBooped3 = isComplete = false;
			mPB = new CCProgressBar();
			cLevel = LevelManager::GetInstance().GetCurrentLevel();

			mChamberObject = cLevel->FindObjectWithName("FloorChamber");
			mStartSign = cLevel->FindObjectWithName("mmStartSign");
			mExitSign = cLevel->FindObjectWithName("mmExitSign");
			mExitButton = cLevel->FindObjectWithName("mmExitButton");
			mStartStand = cLevel->FindObjectWithName("mmStartStand");
			mExitStand = cLevel->FindObjectWithName("mmExitStand");
			mClosePanel = cLevel->FindObjectWithName("mmClosingPanel");
			mPrevButton = cLevel->FindObjectWithName("mmPrevButton");
			mPrevStand = cLevel->FindObjectWithName("mmPrevStand");
			mPrevSign = cLevel->FindObjectWithName("mmPrevSign");
			//mProgressBar = new BaseObject("mmStartProgressBar", identity);
			mPB->SetCurProgress(0);
			mPB->SetFinalProgress(300);
			mStartSign->AddComponent(mPB);
			mPB->GetBackground()->SetParent(mStartSign);
			mStartSign->AddChild(mPB->GetBackground());

			l = new Listener();
			mChamberObject->AddComponent(l);
			AudioWrapper::GetInstance().AddListener(l, "shit");

			mPB->GetBackground()->GetTransform().SetMatrix(matrix4::CreateNewScale(20, 1, 20) * matrix4::CreateNewTranslation(0, 0.0001f, -2));
			//mPB->GetProgressBar()->GetTransform().SetMatrix(matrix4::CreateNewScale(20, 1, 20) * matrix4::CreateNewTranslation(0, 0.0001f, 0));
			((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 2))->CallEvent(Emitter::EventType::ePlay);
			mPB->OnEnable(); 
			//if (Settings::GetInstance().GetBool("mmStartAtBottom"))
			//{
			//	//mPB->OnDisable();
			//Settings::GetInstance().SetInt("mmLevel", 1);
			//}
			//else
			//{
			//	Settings::GetInstance().SetInt("mmLevel", 0);
			//}


			//Settings::GetInstance().SetFloat("StartButton - CurProgress",0);
			//Settings::GetInstance().SetFloat("StartButton - FinalProgress",180);
		
		}

		virtual void OnCollision(Collider& _col1, Collider& _col2, float _time)
		{
			if (mBooped == false && mPB->GetCurProgress() < mPB->GetFinalProgress())
				mPB->SetCurProgress(mPB->GetCurProgress() + 2);
			if (Settings::GetInstance().GetInt("mmLevel") < 2 && mPB->GetCurProgress() >= mPB->GetFinalProgress() && !mBooped && !Settings::GetInstance().GetBool("mmChamberMoving"))
			{
				matrix4 mat = mChamberObject->GetTransform().GetMatrix();
				mChamberInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mChamberObject->GetTransform().GetMatrix());
				mChamberInterp->SetEasingFunction(Easing::ElasticOut);
				mChamberInterp->SetActive(true);

				mat = VRInputManager::GetInstance().GetPlayerPosition();
				mPlayerInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), VRInputManager::GetInstance().GetPlayerPosition());
				mPlayerInterp->SetEasingFunction(Easing::ElasticOut);
				mPlayerInterp->SetActive(true);

				mat = mObject->GetTransform().GetMatrix();
				mStartButtonInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mObject->GetTransform().GetMatrix());
				mStartButtonInterp->SetEasingFunction(Easing::ElasticOut);
				mStartButtonInterp->SetActive(true);

				mat = mStartStand->GetTransform().GetMatrix();
				mStartStandInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mStartStand->GetTransform().GetMatrix());
				mStartStandInterp->SetEasingFunction(Easing::ElasticOut);
				mStartStandInterp->SetActive(true);

				mat = mStartSign->GetTransform().GetMatrix();
				mStartSignInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mStartSign->GetTransform().GetMatrix());
				mStartSignInterp->SetEasingFunction(Easing::ElasticOut);
				mStartSignInterp->SetActive(true);

				mat = mExitButton->GetTransform().GetMatrix();
				mExitButtonInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mExitButton->GetTransform().GetMatrix());
				mExitButtonInterp->SetEasingFunction(Easing::ElasticOut);
				mExitButtonInterp->SetActive(true);

				mat = mExitStand->GetTransform().GetMatrix();
				mExitStandInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mExitStand->GetTransform().GetMatrix());
				mExitStandInterp->SetEasingFunction(Easing::ElasticOut);
				mExitStandInterp->SetActive(true);

				mat = mExitSign->GetTransform().GetMatrix();
				mExitSignInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mExitSign->GetTransform().GetMatrix());
				mExitSignInterp->SetEasingFunction(Easing::ElasticOut);
				mExitSignInterp->SetActive(true);

				mat = mPrevButton->GetTransform().GetMatrix();
				mPrevButtonInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mPrevButton->GetTransform().GetMatrix());
				mPrevButtonInterp->SetEasingFunction(Easing::ElasticOut);
				mPrevButtonInterp->SetActive(true);

				mat = mPrevStand->GetTransform().GetMatrix();
				mPrevStandInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mPrevStand->GetTransform().GetMatrix());
				mPrevStandInterp->SetEasingFunction(Easing::ElasticOut);
				mPrevStandInterp->SetActive(true);

				mat = mPrevSign->GetTransform().GetMatrix();
				mPrevSignInterp->Prepare(1, mat, mat * matrix4::CreateNewTranslation(0, -.05f, 0), mPrevSign->GetTransform().GetMatrix());
				mPrevSignInterp->SetEasingFunction(Easing::ElasticOut);
				mPrevSignInterp->SetActive(true);

				((SFXEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
				((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::EventType::ePlay);
				mBooped = true;
				mBooped3 = true;
				Settings::GetInstance().SetInt("mmLevel", Settings::GetInstance().GetInt("mmLevel") + 1);
				Settings::GetInstance().SetBool("mmChamberMoving", true);
				Settings::GetInstance().SetBool("CantTeleport", true);
				//mPB->OnDisable();
			}
		}

		virtual void Update()
		{
			//if (!AudioToggle)
			//{
			//	//AudioWrapper::GetInstance().MakeEventAtListener(AK::EVENTS::PLAY_HUB0);
			//	AudioToggle = true;
			//}
			if(mPB->GetCurProgress() > 0)
				mPB->SetCurProgress(mPB->GetCurProgress() - 1);
			if (mBooped)
			{
				float dt = TimeManager::Instance()->GetDeltaTime();
				mChamberInterp->Update(dt);
				mStartButtonInterp->Update(dt);
				mStartStandInterp->Update(dt);
				mStartSignInterp->Update(dt);
				mExitButtonInterp->Update(dt);
				mExitStandInterp->Update(dt);
				mExitSignInterp->Update(dt);
				mPrevButtonInterp->Update(dt);
				mPrevStandInterp->Update(dt);
				mPrevSignInterp->Update(dt);

				if (mChamberObject->GetTransform().GetMatrix().fourth.y < -3.64f)
				{
					if (!mBooped2)
					{
						matrix4 mat;
						mat = mClosePanel->GetTransform().GetMatrix();
						mCloseInterp->Prepare(5, mat, mat * matrix4::CreateNewTranslation(2, 0, 0), mClosePanel->GetTransform().GetMatrix());
						mCloseInterp->SetActive(true);
						mBooped2 = true;
					}
					mCloseInterp->Update(TimeManager::Instance()->GetDeltaTime());
				}
				
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mObject->GetTransform().GetMatrix().fourth);
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;

				((ButtonCollider*)mExitButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mExitButton->GetTransform().GetMatrix().fourth);
				((ButtonCollider*)mExitButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mExitButton->GetTransform().GetMatrix().fourth.y - .2f;
				((ButtonCollider*)mExitButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mExitButton->GetTransform().GetMatrix().fourth.y - .2f;

				((ButtonCollider*)mPrevButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mPrevButton->GetTransform().GetMatrix().fourth);
				((ButtonCollider*)mPrevButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mPrevButton->GetTransform().GetMatrix().fourth.y - .2f;
				((ButtonCollider*)mPrevButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mPrevButton->GetTransform().GetMatrix().fourth.y - .2f;

				if (!AudioToggle)
				{
					((SFXEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
					((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::EventType::ePlay);
					AudioToggle = true;
				}
				bool complete = mPlayerInterp->Update(TimeManager::Instance()->GetDeltaTime());
				if (complete && mBooped3)
				{
					matrix4 mat = mChamberObject->GetTransform().GetMatrix();
					mChamberInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mChamberObject->GetTransform().GetMatrix());
					mChamberInterp->SetEasingFunction(Easing::QuadInOut);
					mChamberInterp->SetActive(true);

					mat = VRInputManager::GetInstance().GetPlayerPosition();
					mPlayerInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), VRInputManager::GetInstance().GetPlayerPosition());
					mPlayerInterp->SetEasingFunction(Easing::QuadInOut);
					mPlayerInterp->SetActive(true);

					mat = mObject->GetTransform().GetMatrix();
					mStartButtonInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mObject->GetTransform().GetMatrix());
					mStartButtonInterp->SetEasingFunction(Easing::QuadInOut);
					mStartButtonInterp->SetActive(true);

					mat = mStartStand->GetTransform().GetMatrix();
					mStartStandInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mStartStand->GetTransform().GetMatrix());
					mStartStandInterp->SetEasingFunction(Easing::QuadInOut);
					mStartStandInterp->SetActive(true);

					mat = mStartSign->GetTransform().GetMatrix();
					mStartSignInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mStartSign->GetTransform().GetMatrix());
					mStartSignInterp->SetEasingFunction(Easing::QuadInOut);
					mStartSignInterp->SetActive(true);

					mat = mExitButton->GetTransform().GetMatrix();
					mExitButtonInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mExitButton->GetTransform().GetMatrix());
					mExitButtonInterp->SetEasingFunction(Easing::QuadInOut);
					mExitButtonInterp->SetActive(true);

					mat = mExitStand->GetTransform().GetMatrix();
					mExitStandInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mExitStand->GetTransform().GetMatrix());
					mExitStandInterp->SetEasingFunction(Easing::QuadInOut);
					mExitStandInterp->SetActive(true);

					mat = mExitSign->GetTransform().GetMatrix();
					mExitSignInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0,- 9.95f, 0), mExitSign->GetTransform().GetMatrix());
					mExitSignInterp->SetEasingFunction(Easing::QuadInOut);
					mExitSignInterp->SetActive(true);

					mat = mPrevButton->GetTransform().GetMatrix();
					mPrevButtonInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mPrevButton->GetTransform().GetMatrix());
					mPrevButtonInterp->SetEasingFunction(Easing::QuadInOut);
					mPrevButtonInterp->SetActive(true);

					mat = mPrevStand->GetTransform().GetMatrix();
					mPrevStandInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mPrevStand->GetTransform().GetMatrix());
					mPrevStandInterp->SetEasingFunction(Easing::QuadInOut);
					mPrevStandInterp->SetActive(true);

					mat = mPrevSign->GetTransform().GetMatrix();
					mPrevSignInterp->Prepare(16, mat, mat * matrix4::CreateNewTranslation(0, -9.95f, 0), mPrevSign->GetTransform().GetMatrix());
					mPrevSignInterp->SetEasingFunction(Easing::QuadInOut);
					mPrevSignInterp->SetActive(true);

					mBooped3 = false;
				}
				else if(complete && !mBooped3)
				{
					((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::EventType::eStop);
					mBooped = false;
					Settings::GetInstance().SetBool("mmChamberMoving", false);
					Settings::GetInstance().SetBool("CantTeleport", false);
					complete = false;
				}
			}
		}

		virtual void OnDestroy()
		{
			AudioWrapper::GetInstance().MakeEventAtListener(AK::EVENTS::STOP_TEST1);
			delete mChamberInterp;
			delete mPlayerInterp;
			delete mStartStandInterp;
			delete mStartButtonInterp;
			delete mStartSignInterp;
			delete mExitButtonInterp;
			delete mExitStandInterp;
			delete mExitSignInterp;
			delete mCloseInterp;
			delete mPrevButtonInterp;
			delete mPrevStandInterp;
			delete mPrevSignInterp;
		}
	};

}