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
		BoxSnapToControllerAction* leftBS, *rightBS;
		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mBox = cLevel->FindObjectWithName("Box");
			initialMatrix = mBox->GetTransform().GetMatrix();
			mScaleTipper = 1.0f;
			canBoxShrink = false;
			isBoxShrinking = false;

			std::vector<Component*> codes1 = LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<BoxSnapToControllerAction*>(codes1[x]))
				{
					leftBS = ((BoxSnapToControllerAction*)codes1[x]);
					break;
				}
			}

			codes1 = LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<BoxSnapToControllerAction*>(codes1[x]))
				{
					rightBS = ((BoxSnapToControllerAction*)codes1[x]);
					break;
				}
			}
		}

		virtual void OnTriggerEnter(Collider& _col, Collider& _other)
		{
			if(_other.GetBaseObject()->GetName() == "Box" && _col.mColliderType != Collider::eCOLLIDER_Controller)
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
					leftBS->mHeld = false;
					rightBS->mHeld = false;
					mBox->GetTransform().SetMatrix(initialMatrix);
					vec3f pos = vec3f(*mBox->GetTransform().GetPosition());
					((CubeCollider*)mBox->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(pos);
				}
			}
		}
	};

}