#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Core/TimeManager.h"
#include "../Common/Logger.h"
#include <unordered_set>
#include "../Common/Settings.h"

namespace Epoch {

	struct BoxSnapToControllerAction : public CodeComponent {
		ControllerType mControllerRole = eControllerType_Primary;
		bool mHeld = false;
		ControllerCollider* mCollider;
		Collider* mPickUp = nullptr;
		InputTimeline::InputNode* mInput = nullptr;

		virtual void Start() {
			mCollider = (ControllerCollider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0));
		}

		virtual void Update() override {
			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();

			bool paused = false;

			if (cLevel->GetTimeManipulator()!= nullptr ) {
				paused= cLevel->GetTimeManipulator()->isTimePaused();
			}
			if (VRInputManager::GetInstance().IsVREnabled() && mCollider) {
				InputTimeline::InputNode*temp;
				if (paused) {
					temp = VRInputManager::GetInstance().FindLastInput(mCollider->GetBaseObject()->GetUniqueID(), true);
				}
				else {
					temp = VRInputManager::GetInstance().FindLastInput(mCollider->GetBaseObject()->GetUniqueID(), false);
				}
				if (mInput && temp && mPickUp &&
					(temp->mData.mLastFrame < mInput->mData.mLastFrame ||
					(temp->mData.mLastFrame == mInput->mData.mLastFrame && temp->mData.mTime < mInput->mData.mTime))) {
					mHeld = false;
					mPickUp->mShouldMove = true;
				}
				mInput = temp;
			}
			if (VRInputManager::GetInstance().IsVREnabled() && mInput) {
				if (mHeld && mPickUp != nullptr) {
					matrix4 m = mObject->GetTransform().GetMatrix();
					mPickUp->SetPos(m.Position);
					if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == 1 && mHeld || (paused)) {
						SystemLogger::GetLog() << "Id: " << mCollider->GetBaseObject()->GetUniqueId() << " Released object" << std::endl;
						if (mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueId() || mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueId()) {
							VRInputManager::GetInstance().GetController((mInput->mData.mPrimary) ? eControllerType_Primary : eControllerType_Secondary).TriggerHapticPulse(1000, mInput->mData.mButton);
						}
						ReleaseObject();
					}
				}
				else if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == -1 && !mCollider->mHitting.empty() && ((!paused) || !Settings::GetInstance().GetBool("PauseMenuUp"))) {
					SomethingtoController();
					//TODO PAT: FIX THIS HARDCODED haptic feedback
					if (mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueId()  
						|| mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueId()) {
						VRInputManager::GetInstance().GetController((mInput->mData.mPrimary) ? eControllerType_Primary : eControllerType_Secondary).TriggerHapticPulse(1000, mInput->mData.mButton);
					}
				}
			}

		
		}

		void CheckIfBoxAlreadyHeld()
		{
			std::vector<BaseObject*>clones = TimeManager::Instance()->GetClonesVec();
			for (unsigned int i = 0; i < clones.size(); i++) {
				//you would usally loop to find correct code component but i know that this is the only code components clones will have in our game
				Component* temp = clones[i]->GetComponentIndexed(eCOMPONENT_CODE, 0);
				if(temp && dynamic_cast<BoxSnapToControllerAction*>(temp))
				{
					//If my mPickup is the same as the other code component, make it release theirs so new code component can pick it up
					if(((BoxSnapToControllerAction*)temp)->mHeld == true && ((BoxSnapToControllerAction*)temp)->mPickUp == mPickUp)
					{
						((BoxSnapToControllerAction*)temp)->ReleaseObject();
						break;
					}
				}
			}
		}
		

		virtual void SomethingtoController() {
			mHeld = true;
			//matrix4 m = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
			matrix4 m = mObject->GetTransform().GetMatrix();

			vec4f pos, setPos;
			vec4f controllerPos = mCollider->GetPos();
			bool unset = true;
			for (auto iter = mCollider->mHitting.begin(); iter != mCollider->mHitting.end(); ++iter) {
				pos = (*iter)->GetPos();
				if (unset) {
					setPos = pos;
					mPickUp = (*iter);
					unset = false;
				} else {
					float tx = pos.x - mCollider->GetPos().x;
					float ty = pos.y - mCollider->GetPos().y;
					float tz = pos.z - mCollider->GetPos().z;
					float sx = setPos.x - mCollider->GetPos().x;
					float sy = setPos.y - mCollider->GetPos().y;
					float sz = setPos.z - mCollider->GetPos().z;

					if (tx < sx || ty < sy || tz < sz) {
						setPos = pos;
						mPickUp = (*iter);
					}
				}
			}

			CheckIfBoxAlreadyHeld();

			if (mPickUp) {
				if (!mPickUp->mShouldMove) {
					mCollider->mHitting.erase(mPickUp);
					return;
				}
				mHeld = true;
				mPickUp->SetPos((m).tiers[3]);
				//mObject->GetTransform().SetMatrix(m);
				mPickUp->mShouldMove = false;
				mPickUp->mForces = { 0,0,0 };
				mPickUp->mVelocity = { 0,0,0 };
				mPickUp->mAcceleration = { 0,0,0 };
			}
		}

		virtual void ReleaseObject() {
			//vec4f force = VRInputManager::GetInstance().GetController(mControllerRole).GetVelocity();
			vec4f force = mInput->mData.mVelocity;
			force[2] *= -1; // SteamVR seems to Assume +Z goes into the screen.
			mPickUp->mVelocity = force;
			mPickUp->mShouldMove = true;
			mPickUp = nullptr;
			mHeld = false;
		}
	};
} // Epoch Namespace