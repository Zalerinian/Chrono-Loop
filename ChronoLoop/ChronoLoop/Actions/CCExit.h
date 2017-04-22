#pragma once
#include "CodeComponent.hpp"
#include "..\Core\LevelManager.h"

namespace Epoch
{

	struct CCExit : public CodeComponent
	{
		BaseObject* mExitSign;
		Level* cLevel = nullptr;
		CCProgressBar* mPB;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mPB = new CCProgressBar();
			mExitSign = cLevel->FindObjectWithName("mmExitSign");
			mPB->SetCurProgress(0);
			mPB->SetFinalProgress(300);
			mExitSign->AddComponent(mPB);
			mPB->GetProgressBar()->GetTransform().SetMatrix(matrix4::CreateScale(20, 0, 20) * matrix4::CreateTranslation(0, 0.0001f, -2));
			mPB->GetProgressBar()->SetParent(mExitSign);
			mExitSign->AddChild(mPB->GetProgressBar());
			mPB->OnEnable();

		}
		virtual void OnCollision(Collider& _col1, Collider& _col2, float _time)
		{
			if(mPB->GetCurProgress() < mPB->GetFinalProgress())
				mPB->SetCurProgress(mPB->GetCurProgress() + 2);
			if(mPB->GetCurProgress() >= mPB->GetFinalProgress())
				LevelManager::GetInstance().GetCurrentLevel()->ChronoLoop = false;
		}
		virtual void Update()
		{
			if (mPB->GetCurProgress() > 0)
				mPB->SetCurProgress(mPB->GetCurProgress() - 1);
		}
	};

}