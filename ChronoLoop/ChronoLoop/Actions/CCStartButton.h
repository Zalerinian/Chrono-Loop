#pragma once
#include "CodeComponent.hpp"
#include "..\Objects\BaseObject.h"
#include "..\Common\Interpolator.h"
#include "..\Core\LevelManager.h"
#include "..\Core\Level.h"
#include "..\Sound\SoundEngine.h"

namespace Epoch
{

	struct CCStartButton : public CodeComponent {
		int levels;
		bool mBooped, mBooped2;
		bool AudioToggle;

		Interpolator<matrix4>* mChamberInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPlayerInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartSignInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitSignInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mTutButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mTutStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mTutSignInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mCloseInterp = new Interpolator<matrix4>();

		Listener* l;

		BaseObject *mChamberObject, *mExitButton, *mStartStand, *mStartSign, *mExitStand, *mExitSign, *mClosePanel, *mTutButton, *mTutSign, *mTutStand;
		Level* cLevel = nullptr;

		virtual void Start()
		{
			AudioToggle = false;
			mBooped = mBooped2 = false;
			levels = 0;
			cLevel = LevelManager::GetInstance().GetCurrentLevel();

			mChamberObject = cLevel->FindObjectWithName("mmChamber");
			mStartSign = cLevel->FindObjectWithName("mmStartSign");
			mExitSign = cLevel->FindObjectWithName("mmExitSign");
			mExitButton = cLevel->FindObjectWithName("mmExitButton");
			mStartStand = cLevel->FindObjectWithName("mmStartStand");
			mExitStand = cLevel->FindObjectWithName("mmExitStand");
			mTutSign = cLevel->FindObjectWithName("mmTutSign");
			mTutButton = cLevel->FindObjectWithName("mmTutButton");
			mTutStand = cLevel->FindObjectWithName("mmTutStand");
			mClosePanel = cLevel->FindObjectWithName("mmClosingPanel");

			l = new Listener();
			mChamberObject->AddComponent(l);
			AudioWrapper::GetInstance().AddListener(l, "shit");

			((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 2))->CallEvent(Emitter::EventType::ePlay);
		}

		virtual void OnCollision(Collider& _col1, Collider& _col2, float _time)
		{

			if (levels < 1)
			{
				matrix4 mat = mChamberObject->GetTransform().GetMatrix();
				mChamberInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mChamberObject->GetTransform().GetMatrix());
				mChamberInterp->SetEasingFunction(Easing::QuadInOut);
				mChamberInterp->SetActive(true);

				mat = VRInputManager::GetInstance().GetPlayerPosition();
				mPlayerInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), VRInputManager::GetInstance().GetPlayerPosition());
				mPlayerInterp->SetEasingFunction(Easing::QuadInOut);
				mPlayerInterp->SetActive(true);

				mat = mObject->GetTransform().GetMatrix();
				mStartButtonInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mObject->GetTransform().GetMatrix());
				mStartButtonInterp->SetEasingFunction(Easing::QuadInOut);
				mStartButtonInterp->SetActive(true);

				mat = mStartStand->GetTransform().GetMatrix();
				mStartStandInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mStartStand->GetTransform().GetMatrix());
				mStartStandInterp->SetEasingFunction(Easing::QuadInOut);
				mStartStandInterp->SetActive(true);

				mat = mStartSign->GetTransform().GetMatrix();
				mStartSignInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mStartSign->GetTransform().GetMatrix());
				mStartSignInterp->SetEasingFunction(Easing::QuadInOut);
				mStartSignInterp->SetActive(true);

				mat = mExitButton->GetTransform().GetMatrix();
				mExitButtonInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mExitButton->GetTransform().GetMatrix());
				mExitButtonInterp->SetEasingFunction(Easing::QuadInOut);
				mExitButtonInterp->SetActive(true);

				mat = mExitStand->GetTransform().GetMatrix();
				mExitStandInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mExitStand->GetTransform().GetMatrix());
				mExitStandInterp->SetEasingFunction(Easing::QuadInOut);
				mExitStandInterp->SetActive(true);

				mat = mExitSign->GetTransform().GetMatrix();
				mExitSignInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mExitSign->GetTransform().GetMatrix());
				mExitSignInterp->SetEasingFunction(Easing::QuadInOut);
				mExitSignInterp->SetActive(true);

				mat = mTutButton->GetTransform().GetMatrix();
				mTutButtonInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mTutButton->GetTransform().GetMatrix());
				mTutButtonInterp->SetEasingFunction(Easing::QuadInOut);
				mTutButtonInterp->SetActive(true);

				mat = mTutStand->GetTransform().GetMatrix();
				mTutStandInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mTutStand->GetTransform().GetMatrix());
				mTutStandInterp->SetEasingFunction(Easing::QuadInOut);
				mTutStandInterp->SetActive(true);

				mat = mTutSign->GetTransform().GetMatrix();
				mTutSignInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mTutSign->GetTransform().GetMatrix());
				mTutSignInterp->SetEasingFunction(Easing::QuadInOut);
				mTutSignInterp->SetActive(true);

				((SFXEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
				((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::EventType::ePlay);
				mBooped = true;
				levels++;
			}
		}

		virtual void Update()
		{

			//if (!AudioToggle)
			//{
			//	//AudioWrapper::GetInstance().MakeEventAtListener(AK::EVENTS::PLAY_HUB0);
			//	AudioToggle = true;
			//}

			if (mBooped)
			{
				mChamberInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mStartButtonInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mStartStandInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mStartSignInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mExitButtonInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mExitStandInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mExitSignInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mTutButtonInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mTutStandInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mTutSignInterp->Update(TimeManager::Instance()->GetDeltaTime());

				if (mChamberObject->GetTransform().GetMatrix().fourth.y < -3.64f)
				{
					if (mBooped2)
					{
						matrix4 mat;
						mat = mClosePanel->GetTransform().GetMatrix();
						mCloseInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(2, 0, 0), mClosePanel->GetTransform().GetMatrix());
						mCloseInterp->SetActive(true);
						mBooped2 = false;
					}
					mCloseInterp->Update(TimeManager::Instance()->GetDeltaTime());
				}
				
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mObject->GetTransform().GetMatrix().fourth);
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;
				((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mObject->GetTransform().GetMatrix().fourth.y - .2f;

				((ButtonCollider*)mExitButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mExitButton->GetTransform().GetMatrix().fourth);
				((ButtonCollider*)mExitButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mExitButton->GetTransform().GetMatrix().fourth.y - .2f;
				((ButtonCollider*)mExitButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mExitButton->GetTransform().GetMatrix().fourth.y - .2f;

				((ButtonCollider*)mTutButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mTutButton->GetTransform().GetMatrix().fourth);
				((ButtonCollider*)mTutButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mTutButton->GetTransform().GetMatrix().fourth.y - .2f;
				((ButtonCollider*)mTutButton->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mTutButton->GetTransform().GetMatrix().fourth.y - .2f;

				if (!AudioToggle)
				{
					((SFXEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
					((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::EventType::ePlay);
					AudioToggle = true;
				}
				bool complete = mPlayerInterp->Update(TimeManager::Instance()->GetDeltaTime());
				if (complete)
				{
					((AudioEmitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::EventType::eStop);
					mBooped = false;
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
			delete mTutButtonInterp;
			delete mTutStandInterp;
			delete mTutSignInterp;
			delete mCloseInterp;
		}
	};

}