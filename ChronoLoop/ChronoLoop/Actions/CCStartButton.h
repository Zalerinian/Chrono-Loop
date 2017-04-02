#pragma once
#include "CodeComponent.hpp"
#include "..\Objects\BaseObject.h"
#include "..\Common\Interpolator.h"
#include "..\Core\LevelManager.h"
#include "..\Core\Level.h"
#include "..\Sound\SoundEngine.h"

namespace Epoch
{

	struct CCStartButton : public CodeComponent
	{
		int levels;
		bool mBooped;
		bool AudioToggle;

		Interpolator<matrix4>* mChamberInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mPlayerInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mStartSignInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitButtonInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitStandInterp = new Interpolator<matrix4>();
		Interpolator<matrix4>* mExitSignInterp = new Interpolator<matrix4>();

		BaseObject *mChamberObject, *mStartButton, *mStartStand, *mStartSign, *mExitStand, *mExitSign;
		Level* cLevel = nullptr;

		virtual void Start()
		{
			AudioToggle = false;
			mBooped = false;
			levels = 0;
			cLevel = LevelManager::GetInstance().GetCurrentLevel();

			mChamberObject = cLevel->FindObjectWithName("mmChamber");
			mStartSign = cLevel->FindObjectWithName("mmStart");
			mExitSign = cLevel->FindObjectWithName("mmExit");
			mStartButton = cLevel->FindObjectWithName("mmStartButton");
			mStartStand = cLevel->FindObjectWithName("mmStartStand");
			mExitStand = cLevel->FindObjectWithName("mmExitStand");

			AudioWrapper::GetInstance().AddListener(l, "shit");
		}

		virtual void OnCollision(Collider* _col1, Collider* _col2)
		{
			if (levels < 1)
			{
				matrix4 mat = mChamberObject->GetTransform().GetMatrix();
				mChamberInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mChamberObject->GetTransform().GetMatrix());
				mChamberInterp->SetActive(true);

				mat = VRInputManager::GetInstance().GetPlayerPosition();
				mPlayerInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), VRInputManager::GetInstance().GetPlayerPosition());
				mPlayerInterp->SetActive(true);

				mat = mStartButton->GetTransform().GetMatrix();
				mStartButtonInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mStartButton->GetTransform().GetMatrix());
				mStartButtonInterp->SetActive(true);

				mat = mStartStand->GetTransform().GetMatrix();
				mStartStandInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mStartStand->GetTransform().GetMatrix());
				mStartStandInterp->SetActive(true);

				mat = mStartSign->GetTransform().GetMatrix();
				mStartSignInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mStartSign->GetTransform().GetMatrix());
				mStartSignInterp->SetActive(true);

				mat = mObject->GetTransform().GetMatrix();
				mExitButtonInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mObject->GetTransform().GetMatrix());
				mExitButtonInterp->SetActive(true);

				mat = mExitStand->GetTransform().GetMatrix();
				mExitStandInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mExitStand->GetTransform().GetMatrix());
				mExitStandInterp->SetActive(true);

				mat = mExitSign->GetTransform().GetMatrix();
				mExitSignInterp->Prepare(15, mat, mat * matrix4::CreateTranslation(0, -10, 0), mExitSign->GetTransform().GetMatrix());
				mExitSignInterp->SetActive(true);


				((Emitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->PlaySFX(0);
				((Emitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->Play(0);

				mBooped = true;
				levels++;
			}
		}

		virtual void Update()
		{

			if (!AudioToggle)
			{
				AudioWrapper::GetInstance().MakeEventAtListener(AK::EVENTS::PLAY_TEST1);
				AudioToggle = true;
			}

			if (mBooped)
			{
				mChamberInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mStartButtonInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mStartStandInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mStartSignInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mExitButtonInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mExitStandInterp->Update(TimeManager::Instance()->GetDeltaTime());
				mExitSignInterp->Update(TimeManager::Instance()->GetDeltaTime());


				bool complete = mPlayerInterp->Update(TimeManager::Instance()->GetDeltaTime());
				if (complete)
				{
					((Emitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->Stop(0);
					mBooped = false;
				}
			}
		}

		virtual void OnDestroy()
		{
			AudioWrapper::GetInstance().MakeEventAtListener(AK::EVENTS::STOP_TEST1);
			delete mChamberInterp;
			delete mPlayerInterp;
		}
	};

}