#pragma once
#include "CodeComponent.hpp"
#include "../Core/LevelManager.h"


namespace Epoch
{
	
	struct CCLevel5Fields : public CodeComponent
	{
		BaseObject* mBox;
		Level* cLevel;
		matrix4 initialMatrix;
		float mScaleTipper;
		bool canBoxShrink,isBoxShrinking;
		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mBox = cLevel->FindObjectWithName("Box");
			initialMatrix = mBox->GetTransform().GetMatrix();
			mScaleTipper = 1.0f;
			canBoxShrink = false;
			isBoxShrinking = false;
		}

		virtual void OnTriggerEnter(Collider& _col, Collider& _other)
		{
			if(_other.GetBaseObject()->GetName() == "Box")
			{
				canBoxShrink = true;
			}
		}
		virtual void Update()
		{
			if (canBoxShrink)
				isBoxShrinking = true;
			if (isBoxShrinking)
			{
				mScaleTipper -= 0.05f;
				mBox->GetTransform().SetMatrix(matrix4::CreateScale(mScaleTipper, mScaleTipper, mScaleTipper) * mBox->GetTransform().GetMatrix());
				if(mScaleTipper <= 0.0f)
				{
					isBoxShrinking = false;
					canBoxShrink = false;
					mScaleTipper = 1.0f;
					mBox->GetTransform().SetMatrix(initialMatrix);
				}
			}
		}
	};

}