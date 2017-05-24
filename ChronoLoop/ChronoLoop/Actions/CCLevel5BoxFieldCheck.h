#pragma once
#include "CodeComponent.hpp"
#include "TeleportAction.hpp"
#include "../Objects/BaseObject.h"
#include "../Core/LevelManager.h"
#include "../Physics/Physics.h"
#include "BoxSnapToControllerAction.hpp"
#include "CCLevel5Fields.h"


namespace Epoch
{
	struct CCLevel5BoxFieldCheck : public CodeComponent
	{
		BaseObject* mBox;
		Level* cLevel;
		ControllerType mControllerRole = eControllerType_Primary;
		TeleportAction* mRTeleportAction, *mLTeleportAction;
		BoxSnapToControllerAction* mRightBS,* mLeftBS;
		BaseObject* mStartField,* mExitField;
		CCLevel5Fields *mStartFScript, *mExitFScript;
		vec3f mStartMin, mStartMax, mExitMin, mExitMax, mHit;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mBox = cLevel->FindObjectWithName("Box");
			mStartField = cLevel->FindObjectWithName("StartSideEnergyWall");
			mExitField = cLevel->FindObjectWithName("ExitSideEnergyWall");
			std::vector<Component*> codes1 = cLevel->GetRightController()->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<TeleportAction*>(codes1[x]))
				{
					mRTeleportAction = ((TeleportAction*)codes1[x]);
					break;
				}
			}
			codes1 = cLevel->GetLeftController()->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<TeleportAction*>(codes1[x]))
				{
					mLTeleportAction = ((TeleportAction*)codes1[x]);
					break;
				}
			}
			codes1 = cLevel->GetRightController()->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<BoxSnapToControllerAction*>(codes1[x]))
				{
					mRightBS = ((BoxSnapToControllerAction*)codes1[x]);
					break;
				}
			}
			codes1 = cLevel->GetLeftController()->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<BoxSnapToControllerAction*>(codes1[x]))
				{
					mLeftBS = ((BoxSnapToControllerAction*)codes1[x]);
					break;
				}
			}
			codes1 = mStartField->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<CCLevel5Fields*>(codes1[x]))
				{
					mStartFScript = ((CCLevel5Fields*)codes1[x]);
					break;
				}
			}			
			codes1 = mExitField->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			for (size_t x = 0; x < codes1.size(); ++x)
			{
				if (dynamic_cast<CCLevel5Fields*>(codes1[x]))
				{
					mExitFScript = ((CCLevel5Fields*)codes1[x]);
					break;
				}
			}
			mStartMin = ((CubeCollider*)mStartField->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mMin;
			mStartMax = ((CubeCollider*)mStartField->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mMax;		
			
			mExitMin = ((CubeCollider*)mStartField->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mMin;
			mExitMax = ((CubeCollider*)mStartField->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mMax;
		}
		virtual void Update()
		{
			if(VRInputManager::GetInstance().GetController(mControllerRole).GetPressUp(vr::EVRButtonId::k_EButton_SteamVR_Touchpad) && !Settings::GetInstance().GetBool("CantTeleport"))
			{
				if (mLeftBS)
				{
					if (mLeftBS->mHeld)
					{
						if (CheckLineBox(mStartMin, mStartMax, mBox->GetTransform().GetMatrix().Position, mLTeleportAction->GetEndPos().Position, mHit))
						{
							mStartFScript->SetIsBoxShrinking(true);
						}
						else if (CheckLineBox(mExitMin, mExitMax, mBox->GetTransform().GetMatrix().Position, mLTeleportAction->GetEndPos().Position, mHit))
						{
							mExitFScript->SetIsBoxShrinking(true);
						}
					}
				}
				if (mRightBS)
				{
					if (mRightBS->mHeld)
					{
						if (CheckLineBox(mStartMin, mStartMax, mBox->GetTransform().GetMatrix().Position, mRTeleportAction->GetEndPos().Position, mHit))
						{
							mStartFScript->SetIsBoxShrinking(true);

						}
						else if (CheckLineBox(mExitMin, mExitMax, mBox->GetTransform().GetMatrix().Position, mRTeleportAction->GetEndPos().Position, mHit))
						{
							mExitFScript->SetIsBoxShrinking(true);
						}
					}
				}
			}
		}
		int inline GetIntersection(float fDst1, float fDst2, vec3f P1, vec3f P2, vec3f &Hit)
		{
			if ((fDst1 * fDst2) >= 0.0f) return 0;
			if (fDst1 == fDst2) return 0;
			Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
			return 1;
		}

		int inline InBox(vec3f Hit, vec3f B1, vec3f B2, const int Axis)
		{
			if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
			if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
			if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
			return 0;
		}

		// returns true if line (L1, L2) intersects with the box (B1, B2)
		// returns intersection point in Hit
		bool CheckLineBox(vec3f B1, vec3f B2, vec3f L1, vec3f L2, vec3f &Hit)
		{
			if (L2.x < B1.x && L1.x < B1.x) return false;
			if (L2.x > B2.x && L1.x > B2.x) return false;
			if (L2.y < B1.y && L1.y < B1.y) return false;
			if (L2.y > B2.y && L1.y > B2.y) return false;
			if (L2.z < B1.z && L1.z < B1.z) return false;
			if (L2.z > B2.z && L1.z > B2.z) return false;
			if (L1.x > B1.x && L1.x < B2.x &&
				L1.y > B1.y && L1.y < B2.y &&
				L1.z > B1.z && L1.z < B2.z)
			{
				Hit = L1;
				return true;
			}
			if ((GetIntersection(L1.x - B1.x, L2.x - B1.x, L1, L2, Hit) && InBox(Hit, B1, B2, 1))
				|| (GetIntersection(L1.y - B1.y, L2.y - B1.y, L1, L2, Hit) && InBox(Hit, B1, B2, 2))
				|| (GetIntersection(L1.z - B1.z, L2.z - B1.z, L1, L2, Hit) && InBox(Hit, B1, B2, 3))
				|| (GetIntersection(L1.x - B2.x, L2.x - B2.x, L1, L2, Hit) && InBox(Hit, B1, B2, 1))
				|| (GetIntersection(L1.y - B2.y, L2.y - B2.y, L1, L2, Hit) && InBox(Hit, B1, B2, 2))
				|| (GetIntersection(L1.z - B2.z, L2.z - B2.z, L1, L2, Hit) && InBox(Hit, B1, B2, 3)))
				return true;

			return false;
		}
	};
}